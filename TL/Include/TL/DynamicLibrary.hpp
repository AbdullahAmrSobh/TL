#pragma once

#include <TL/Export.hpp>
#include <TL/Result.hpp>
#include <TL/Library.hpp>
#include <TL/FileSystem/FileWatcher.hpp>

namespace TL
{
    enum class LibraryLoaderAction
    {
        None,
        Load,
        Unload,
        Reload,
    };

    typedef void (*PFN_DllEntry)(LibraryLoaderAction action, void* state);

    class LibraryWatcher
    {
    public:
        LibraryWatcher(TL::StringView libraryPath);
        ~LibraryWatcher();

        // Returns the library handle
        Library&     getLibrary() { return m_library; }

        // Polls the file watcher and reloads the library if modified
        void         poll();

        // Called when the library is updated (loaded, unloaded, or reloaded)
        virtual void onUpdate(LibraryLoaderAction action, void* state) = 0;

    private:
        Library     m_library;
        FileWatcher m_watcher;
    };
} // namespace TL