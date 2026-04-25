#include "TL/FileSystem/FileSystemWatcher.hpp"

#include "WindowsCommon.inl"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <mutex>

namespace TL
{
    using Mutex     = std::mutex;
    using LockGuard = std::lock_guard<Mutex>;

    struct FileWatcher::Impl
    {
        struct Watch
        {
            HANDLE               handle = INVALID_HANDLE_VALUE;
            TL::String           path;
            Flags<FileEventType> eventTypes;
            bool                 watchSubtree = false;
            OVERLAPPED           overlapped   = {};
            TL::Vector<uint8_t>  buffer;
            TL::String           pendingOldPath; // for rename pairing
            bool                 readPending = false;
        };

        TL::Vector<Watch*> watches; // heap-allocated so OVERLAPPED ptr stays stable
        TL::Mutex          mutex;

        static bool        issueRead(Watch* w);
    };

    bool FileWatcher::Impl::issueRead(FileWatcher::Impl::Watch* w)
    {
        DWORD notifyFilter = FILE_NOTIFY_CHANGE_FILE_NAME |
                             FILE_NOTIFY_CHANGE_DIR_NAME |
                             FILE_NOTIFY_CHANGE_ATTRIBUTES |
                             FILE_NOTIFY_CHANGE_SIZE |
                             FILE_NOTIFY_CHANGE_LAST_WRITE;
        DWORD bytes = 0;
        BOOL  ok    = ::ReadDirectoryChangesW(
            w->handle,
            w->buffer.data(),
            static_cast<DWORD>(w->buffer.size()),
            w->watchSubtree ? TRUE : FALSE,
            notifyFilter,
            &bytes,
            &w->overlapped,
            nullptr);

        if (!ok)
        {
            DWORD code = ::GetLastError();
            if (code != ERROR_IO_PENDING)
            {
                TL::windows::logError(code);
                w->readPending = false;
                return false;
            }
        }
        w->readPending = true;
        return true;
    }

    FileWatcher::FileWatcher()
    {
        m_impl = TL::construct<Impl>();
    }

    FileWatcher::~FileWatcher()
    {
        for (auto* w : m_impl->watches)
        {
            if (w->handle != INVALID_HANDLE_VALUE)
            {
                ::CancelIoEx(w->handle, &w->overlapped);
                ::CloseHandle(w->handle);
            }
            if (w->overlapped.hEvent)
                ::CloseHandle(w->overlapped.hEvent);
            TL::destruct(w);
        }
        TL::destruct(m_impl);
    }

    void FileWatcher::watch(StringView path, Flags<FileEventType> eventTypes, bool watchSubtree)
    {
        auto* w         = TL::construct<Impl::Watch>();
        w->path         = path;
        w->eventTypes   = eventTypes;
        w->watchSubtree = watchSubtree;
        w->buffer.resize(64 * 1024);

        w->handle = ::CreateFileA(
            TL::String(path).c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr);

        if (w->handle == INVALID_HANDLE_VALUE)
        {
            TL::windows::logError(::GetLastError());
            TL::destruct(w);
            return;
        }

        w->overlapped.hEvent = ::CreateEventA(nullptr, TRUE, FALSE, nullptr);
        if (!w->overlapped.hEvent)
        {
            TL::windows::logError(::GetLastError());
            ::CloseHandle(w->handle);
            TL::destruct(w);
            return;
        }

        if (!Impl::issueRead(w))
        {
            ::CloseHandle(w->overlapped.hEvent);
            ::CloseHandle(w->handle);
            TL::destruct(w);
            return;
        }

        {
            TL::LockGuard lock(m_impl->mutex);
            m_impl->watches.push_back(w);
        }
    }

    void FileWatcher::unwatch(StringView path)
    {
        TL::LockGuard lock(m_impl->mutex);

        for (size_t i = 0; i < m_impl->watches.size(); ++i)
        {
            if (m_impl->watches[i]->path == path)
            {
                auto* w = m_impl->watches[i];
                if (w->handle != INVALID_HANDLE_VALUE)
                {
                    ::CancelIoEx(w->handle, &w->overlapped);
                    ::CloseHandle(w->handle);
                }
                if (w->overlapped.hEvent)
                    ::CloseHandle(w->overlapped.hEvent);
                TL::destruct(w);
                m_impl->watches.erase(m_impl->watches.begin() + i);
                break;
            }
        }
    }

    static TL::String utf16ToUtf8(const WCHAR* data, size_t wlen)
    {
        if (wlen == 0)
            return {};
        int        needed = ::WideCharToMultiByte(CP_UTF8, 0, data, static_cast<int>(wlen), nullptr, 0, nullptr, nullptr);
        TL::String out;
        out.resize(needed);
        ::WideCharToMultiByte(CP_UTF8, 0, data, static_cast<int>(wlen), out.data(), needed, nullptr, nullptr);
        return out;
    }

    void FileWatcher::poll()
    {
        TL::LockGuard lock(m_impl->mutex);

        for (auto* w : m_impl->watches)
        {
            if (!w->readPending)
                continue;

            DWORD bytesReturned = 0;
            BOOL  ready         = ::GetOverlappedResult(w->handle, &w->overlapped, &bytesReturned, FALSE);
            if (!ready)
            {
                DWORD code = ::GetLastError();
                if (code == ERROR_IO_INCOMPLETE)
                    continue;
                TL::windows::logError(code);
                w->readPending = false;
                continue;
            }

            w->readPending = false;
            ::ResetEvent(w->overlapped.hEvent);

            if (bytesReturned == 0)
            {
                // Buffer overflow: events lost. Re-arm.
                Impl::issueRead(w);
                continue;
            }

            BYTE* ptr = w->buffer.data();
            while (true)
            {
                auto*      info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);

                TL::String fileName = utf16ToUtf8(info->FileName, info->FileNameLength / sizeof(WCHAR));
                TL::String fullPath = w->path + "/" + fileName;

                FileEvent  evt;
                evt.path   = fullPath;
                evt.target = FileEventTarget::Unknown;
                evt.type   = FileEventType::None;

                switch (info->Action)
                {
                case FILE_ACTION_ADDED:    evt.type = FileEventType::Added; break;
                case FILE_ACTION_REMOVED:  evt.type = FileEventType::Removed; break;
                case FILE_ACTION_MODIFIED: evt.type = FileEventType::Modified; break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    w->pendingOldPath = fullPath;
                    evt.type          = FileEventType::None; // suppress until NEW_NAME
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    evt.type    = FileEventType::Renamed;
                    evt.oldPath = w->pendingOldPath;
                    w->pendingOldPath.clear();
                    break;
                default:
                    break;
                }

                if (evt.type != FileEventType::None && (w->eventTypes & Flags<FileEventType>(evt.type)))
                    broadcast(evt);

                if (info->NextEntryOffset == 0)
                    break;
                ptr += info->NextEntryOffset;
            }

            Impl::issueRead(w);
        }
    }
} // namespace TL
