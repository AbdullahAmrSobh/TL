#include "TL/Context.hpp"
#include "TL/FileSystem/FileWatcher.hpp"

#include <filesystem>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <condition_variable>

#include "WindowsCommon.inl"

namespace TL
{
    struct WatchEntry
    {
        TL::String               path;
        TL::Flags<FileEventType> flags;
        bool                     watchSubtree;
    };

    struct FileWatcher::Impl
    {
        struct WatchHandle
        {
            HANDLE                   dirHandle  = INVALID_HANDLE_VALUE;
            OVERLAPPED               overlapped = {};
            size_t                   size;
            uint8_t                  buffer[256];
            TL::String               path;
            TL::Flags<FileEventType> flags;
            bool                     watchSubtree = false;
            std::atomic<bool>        active{true};
            std::thread              thread;
        };

        TL::Map<TL::String, std::unique_ptr<WatchHandle>> watchList;
        std::mutex                                        mutex;
        std::condition_variable                           cv;
        std::atomic<bool>                                 running{true};

        ~Impl()
        {
            running = false;
            cv.notify_all();
            std::lock_guard<std::mutex> lock(mutex);
            for (auto& [_, handle] : watchList)
            {
                handle->active = false;
                if (handle->dirHandle != INVALID_HANDLE_VALUE)
                    CancelIoEx(handle->dirHandle, nullptr);
                if (handle->thread.joinable())
                    handle->thread.join();
                if (handle->dirHandle != INVALID_HANDLE_VALUE)
                    CloseHandle(handle->dirHandle);
            }
        }
    };

    inline static FileEventType mapActionToEvent(DWORD action)
    {
        switch (action)
        {
        case FILE_ACTION_ADDED:            return FileEventType::Added;
        case FILE_ACTION_REMOVED:          return FileEventType::Removed;
        case FILE_ACTION_MODIFIED:         return FileEventType::Modified;
        case FILE_ACTION_RENAMED_OLD_NAME: return FileEventType::Renamed;
        case FILE_ACTION_RENAMED_NEW_NAME: return FileEventType::Renamed;
        default:                           return FileEventType::None;
        }
    }

    FileWatcher::FileWatcher()
        : FileEventQueue()
        , m_impl(Construct<Impl>())
    {
    }

    FileWatcher::~FileWatcher()
    {
        // The Impl destructor ensures all watcher threads are joined and directory handles are closed.
        // Impl destructor handles cleanup
    }

    void FileWatcher::watch(StringView path, Flags<FileEventType> eventTypes, bool watchSubtree)
    {
        std::lock_guard<std::mutex> lock(m_impl->mutex);
        if (m_impl->watchList.contains(path.data()))
            return;

        auto handle          = std::make_unique<Impl::WatchHandle>();
        handle->path         = path.data();
        handle->flags        = eventTypes;
        handle->watchSubtree = watchSubtree;

        auto absolutePath = std::filesystem::absolute(path.data());
        handle->dirHandle = ::CreateFileW(
            absolutePath.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr);

        if (handle->dirHandle == INVALID_HANDLE_VALUE)
        {
            auto error = getlastError();
            if (error != IOResultCode::Success)
            {
                // LoglastError();
            }
            return;
        }

        auto watchThread = [this, h = handle.get()]()
        {
            while (h->active)
            {
                DWORD bytesReturned = 0;
                memset(&h->overlapped, 0, sizeof(OVERLAPPED));
                BOOL success = ::ReadDirectoryChangesW(
                    h->dirHandle,
                    h->buffer,
                    sizeof(h->buffer),
                    TRUE,
                    FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
                    nullptr,
                    &h->overlapped,
                    nullptr);
                if (!success && GetLastError() != ERROR_IO_PENDING)
                {
                    // LoglastError();
                    continue;
                }

                DWORD wait = WaitForSingleObjectEx(h->dirHandle, INFINITE, TRUE);
                if (!h->active)
                    break;

                DWORD bytes;
                if (!GetOverlappedResult(h->dirHandle, &h->overlapped, &bytes, TRUE))
                    continue;

                while (bytes > 0)
                {
                    auto* info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(h->buffer);
                    FileEventType type = mapActionToEvent(info->Action);
                    // switch ()
                    // {

                    // }
                    if (h->flags & type)
                    {
                        FileEvent event;
                        event.type                     = type;
                        std::filesystem::path basePath = h->path;
                        std::wstring          fileName(info->FileName, info->FileNameLength / sizeof(WCHAR));
                        std::filesystem::path fullPath = basePath / fileName;
                        event.path                     = TL::String(fullPath.string().c_str());
                        broadcast(event);
                    }
                    if (info->NextEntryOffset == 0)
                        break;
                    // ptr += info->NextEntryOffset;
                    h->size = bytes;
                    bytes -= info->NextEntryOffset;
                }
            }
        };

        static std::thread t(watchThread);

        m_impl->watchList[TL::String(path.data(), path.size())] = std::move(handle);
        m_impl->watchList[path.data()]                          = std::move(handle);
    }

    void FileWatcher::unwatch(StringView path)
    {
        std::lock_guard<std::mutex> lock(m_impl->mutex);
        auto                        it = m_impl->watchList.find(path.data());
        if (it != m_impl->watchList.end())
        {
            it->second->active = false;
            if (it->second->dirHandle != INVALID_HANDLE_VALUE)
                CancelIoEx(it->second->dirHandle, nullptr);
            if (it->second->thread.joinable())
                it->second->thread.join();
            if (it->second->dirHandle != INVALID_HANDLE_VALUE)
                CloseHandle(it->second->dirHandle);
            m_impl->watchList.erase(it);
        }
    }

    void FileWatcher::poll()
    {
        // No-op: events are pushed from threads, poll is not needed for Win32 async
    }

} // namespace TL