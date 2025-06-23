#include "TL/DynamicLibrary.hpp"

#include <filesystem>

namespace TL
{
    inline static Result<Library> LoadLibrary(const char* path)
    {
        // Check if the file exists
        if (!std::filesystem::exists(path))
        {
            TL_LOG_ERROR("Library file '{}' does not exist", path);
            return Error(std::format("Library file '{}' does not exist", path));
        }

        // make temp copy of the library file
        std::filesystem::path tempPath = std::filesystem::path(path).replace_extension("loaded.tmp");
        std::filesystem::copy_file(path, tempPath, std::filesystem::copy_options::overwrite_existing);

        auto [library, error] = Library::Open(tempPath.string().c_str());
        if (error.IsError())
        {
            TL_LOG_ERROR("Failed to open library at '{}': {}", path, error.GetMessage());
            return error;
        }

        auto pfnDllEntry = library.GetProc<PFN_DllEntry>("TL_DllEntry");
        if (pfnDllEntry)
        {
            pfnDllEntry(LibraryLoaderAction::Load, nullptr);
        }
        else
        {
            TL_LOG_ERROR("Failed to find 'TL_DllEntry' in library '{}'", path);
            Library::Close(library);
            return Error(std::format("Failed to find 'TL_DllEntry' in library '{}'", path));
        }

        TL_LOG_INFO("Library '{}' loaded successfully", path);
        return library;
    }

    LibraryWatcher::LibraryWatcher(TL::StringView libraryPath)
    {
        auto [library, error] = LoadLibrary(libraryPath.data());

        // Check for file events
        auto handler = [this](const FileEvent& event)
        {
            if (event.type == FileEventType::Modified)
            {
                auto [library, error] = LoadLibrary(event.path.c_str());
                if (error.IsError())
                {
                    TL_LOG_ERROR("Failed to reload library '{}': {}", event.path, error.GetMessage());
                    return false; // Stop processing further events
                }
                onUpdate(LibraryLoaderAction::Reload, nullptr);
            }
            return true; // Continue processing other events
        };

        m_watcher.subscribe(handler);
        m_watcher.watch(libraryPath, FileEventType::Modified, false);
    }

    LibraryWatcher::~LibraryWatcher()
    {
        Library::Close(m_library);
    }

    void LibraryWatcher::poll()
    {
        m_watcher.poll();
    }
} // namespace TL