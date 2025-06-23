#pragma once

#include "TL/Event.hpp"
#include "TL/String.hpp"
#include "TL/Flags.hpp"

namespace TL
{
    enum class FileEventType
    {
        None     = 0 << 0,
        Added    = 1 << 0,
        Removed  = 1 << 1,
        Modified = 1 << 2,
        Renamed  = 1 << 3,
    };

    enum class FileEventTarget
    {
        Unknown,
        File,
        Directory,
        FileAndDirectory,
    };

    struct FileEvent
    {
        TL::String      path;    // Path affected
        FileEventType   type;    // What happened (add, remove, etc.)
        FileEventTarget target;  // File or directory
        TL::String      oldPath; // For rename events, previous path (empty otherwise)
    };

    class FileWatcher final : public EventQueue<FileEvent>
    {
    public:
        using FileEventQueue = EventQueue<FileEvent>;

        FileWatcher();
        ~FileWatcher();

        // add a path to watch list
        void watch(StringView path, Flags<FileEventType> eventTypes, bool watchSubtree);

        // remove path from watch list
        void unwatch(StringView path);

        // will broadcast
        void poll();

    private:
        struct Impl;
        Impl* m_impl;
    };
} // namespace TL