#include "TL/Context.hpp"
#include "TL/FileSystem/FileWatcher.hpp"

#include <filesystem>
#include <vector>
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
            HANDLE                   dirHandle   = INVALID_HANDLE_VALUE;
            OVERLAPPED               overlapped  = {};
            HANDLE                   eventHandle = nullptr; // signalled when operation completes
            std::vector<uint8_t>     buffer;                // separate buffer
            TL::String               path;
            TL::Flags<FileEventType> flags;
            bool                     watchSubtree = false;
            bool                     armed        = false; // have we issued an async ReadDirectoryChangesW?
        };

        TL::Map<TL::String, std::unique_ptr<WatchHandle>> watchList;

        ~Impl()
        {
            for (auto& [_, handle] : watchList)
            {
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

    FileWatcher::~FileWatcher() = default;

    void FileWatcher::watch(StringView path, Flags<FileEventType> eventTypes, bool watchSubtree)
    {
        if (m_impl->watchList.contains(path.data()))
            return;

        auto handle          = std::make_unique<Impl::WatchHandle>();
        handle->path         = path.data();
        handle->flags        = eventTypes;
        handle->watchSubtree = watchSubtree;

        auto absolutePath = std::filesystem::weakly_canonical(path.data()).generic_wstring();
        handle->dirHandle = ::CreateFileW(
            absolutePath.data(),
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
                TL_LOG_ERROR("Failed to watch '{}'. Error: (TODO-insertt error report here).", path);
                // log error
            }
            return;
        }

        m_impl->watchList[TL::String(path.data(), path.size())] = std::move(handle);
    }

    void FileWatcher::unwatch(StringView path)
    {
        auto it = m_impl->watchList.find(path.data());
        if (it != m_impl->watchList.end())
        {
            if (it->second->dirHandle != INVALID_HANDLE_VALUE)
                CloseHandle(it->second->dirHandle);
            m_impl->watchList.erase(it);
        }
    }

    void FileWatcher::poll()
    {
        const DWORD mask = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE;

        for (auto& [_, h] : m_impl->watchList)
        {
            if (!h->armed)
                continue;

            DWORD bytesTransferred = 0;
            BOOL  completed        = ::GetOverlappedResult(h->dirHandle, &h->overlapped, &bytesTransferred, /*bWait=*/FALSE);

            if (!completed)
            {
                DWORD err = GetLastError();
                // Not finished yet — still pending
                if (err == ERROR_IO_INCOMPLETE || err == ERROR_IO_PENDING)
                    continue;
                // Other error: consider handling/logging and re-arming or disabling this watch
                continue;
            }

            // We have results in h->buffer with bytesTransferred bytes. Parse them.
            uint8_t* ptr = h->buffer.data();
            uint8_t* end = ptr + bytesTransferred;
            while (ptr < end)
            {
                auto*         info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(ptr);
                FileEventType type = mapActionToEvent(info->Action);

                if (h->flags & type)
                {
                    FileEvent event;
                    event.type = type;

                    std::filesystem::path basePath = h->path;
                    std::wstring          fileName(info->FileName, info->FileNameLength / sizeof(WCHAR));
                    std::filesystem::path fullPath = basePath / fileName;
                    event.path                     = TL::String(fullPath.string().c_str());

                    broadcast(event);
                }

                if (info->NextEntryOffset == 0)
                    break;
                ptr += info->NextEntryOffset;
            }

            // Reset event and re-arm another async ReadDirectoryChangesW
            ResetEvent(h->eventHandle);
            ::memset(&h->overlapped, 0, sizeof(OVERLAPPED));
            h->overlapped.hEvent = h->eventHandle;

            BOOL ok = ::ReadDirectoryChangesW(
                h->dirHandle,
                h->buffer.data(),
                static_cast<DWORD>(h->buffer.size()),
                h->watchSubtree,
                mask,
                /*lpBytesReturned*/ nullptr,
                &h->overlapped,
                /*lpCompletionRoutine*/ nullptr);

            if (!ok)
            {
                DWORD err = GetLastError();
                if (err != ERROR_IO_PENDING)
                {
                    // failed to re-arm; mark not armed or cleanup
                    h->armed = false;
                    CloseHandle(h->dirHandle);
                    CloseHandle(h->eventHandle);
                }
            }
        }
    }

} // namespace TL
