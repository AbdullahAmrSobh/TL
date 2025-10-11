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
            TL::Vector<uint8_t>   buffer;
        };

        TL::Vector<Watch> watches;
        TL::Mutex         mutex;
    };

    FileWatcher::FileWatcher()
    {
        m_impl = new Impl();
    }

    FileWatcher::~FileWatcher()
    {
        // Close all handles
        for (auto& watch : m_impl->watches)
        {
            if (watch.handle != INVALID_HANDLE_VALUE)
            {
                ::CancelIoEx(watch.handle, nullptr);
                ::CloseHandle(watch.handle);
            }
        }

        delete m_impl;
    }

    void FileWatcher::watch(StringView path, Flags<FileEventType> eventTypes, bool watchSubtree)
    {
        Impl::Watch watch;
        watch.path         = path;
        watch.eventTypes   = eventTypes;
        watch.watchSubtree = watchSubtree;
        watch.buffer.resize(64 * 1024); // 64KB buffer per watch

        watch.handle = ::CreateFileA(
            path.data(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr);

        if (watch.handle == INVALID_HANDLE_VALUE)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return;
        }

        {
            TL::LockGuard lock(m_impl->mutex);
            m_impl->watches.push_back(std::move(watch));
        }
    }

    void FileWatcher::unwatch(StringView path)
    {
        TL::LockGuard lock(m_impl->mutex);

        for (size_t i = 0; i < m_impl->watches.size(); ++i)
        {
            if (m_impl->watches[i].path == path)
            {
                auto& w = m_impl->watches[i];
                if (w.handle != INVALID_HANDLE_VALUE)
                {
                    ::CancelIoEx(w.handle, nullptr);
                    ::CloseHandle(w.handle);
                }
                m_impl->watches.erase(m_impl->watches.begin() + i);
                break;
            }
        }
    }

    void FileWatcher::poll()
    {
        TL::LockGuard lock(m_impl->mutex);

        for (auto& watch : m_impl->watches)
        {
            TL::WString fullPath;
            fullPath.resize(watch.path.size());
            ::MultiByteToWideChar(CP_UTF8, 0, watch.path.data(), static_cast<int>(watch.path.size()), fullPath.data(), static_cast<int>(fullPath.size()));

            DWORD bytesReturned = 0;
            BOOL  success       = ::ReadDirectoryChangesW(
                watch.handle,
                fullPath.data(),
                static_cast<DWORD>(fullPath.size()),
                watch.watchSubtree ? TRUE : FALSE,
                FILE_NOTIFY_CHANGE_FILE_NAME |
                    FILE_NOTIFY_CHANGE_DIR_NAME |
                    FILE_NOTIFY_CHANGE_ATTRIBUTES |
                    FILE_NOTIFY_CHANGE_SIZE |
                    FILE_NOTIFY_CHANGE_LAST_WRITE,
                &bytesReturned,
                nullptr,
                nullptr);

            if (!success)
            {
                DWORD code = ::GetLastError();
                TL::windows::logError(code);
                continue;
            }

            // Parse events
            BYTE* ptr = watch.buffer.data();
            while (true)
            {
                FILE_NOTIFY_INFORMATION* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);

                // Convert UTF-16 to UTF-8
                TL::String               fileName;
                fileName.resize(info->FileNameLength / sizeof(WCHAR));
                ::WideCharToMultiByte(CP_UTF8, 0, info->FileName, info->FileNameLength / sizeof(WCHAR), fileName.data(), static_cast<int>(fileName.size()), nullptr, nullptr);

                FileEvent evt;
                evt.path   = watch.path + "\\" + fileName;
                evt.target = FileEventTarget::Unknown;

                switch (info->Action)
                {
                case FILE_ACTION_ADDED:    evt.type = FileEventType::Added; break;
                case FILE_ACTION_REMOVED:  evt.type = FileEventType::Removed; break;
                case FILE_ACTION_MODIFIED: evt.type = FileEventType::Modified; break;
                case FILE_ACTION_RENAMED_OLD_NAME:
                    evt.type    = FileEventType::Renamed;
                    evt.oldPath = watch.path + "\\" + fileName;
                    break;
                case FILE_ACTION_RENAMED_NEW_NAME:
                    evt.type = FileEventType::Renamed;
                    evt.path = watch.path + "\\" + fileName;
                    break;
                default:
                    evt.type = FileEventType::None;
                    break;
                }

                if (evt.type != FileEventType::None)
                    broadcast(evt);

                if (info->NextEntryOffset == 0)
                    break;

                ptr += info->NextEntryOffset;
            }
        }
    }
} // namespace TL
