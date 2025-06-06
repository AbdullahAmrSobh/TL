#pragma once

#include "TL/Block.hpp"
#include "TL/Containers.hpp"
#include "TL/Context.hpp"

#include <fstream>

namespace TL
{
    // Temp file operations
    inline static void WriteBinaryFile(const char* filename, TL::Block data)
    {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs)
        {
            TL_LOG_ERROR("Failed to open write to file: {}", filename);
            return;
        }

        ofs.write(reinterpret_cast<const char*>(data.ptr), data.size);
        ofs.flush();
        ofs.close();
    }

    inline static TL::Block ReadBinaryFile(const char* filename)
    {
        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs)
        {
            TL_LOG_ERROR("Failed to open read from file: {}", filename);
            return {};
        }

        ifs.seekg(0, std::ios::end);
        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        // Allocate memory for the data and read it
        auto data = TL::Allocate(size, alignof(char));
        if (ifs.read(reinterpret_cast<char*>(data.ptr), size))
        {
            return data;
        }

        TL_LOG_ERROR("Failed to read from file: {}", filename);
        return {};
    }

    inline static TL::String ReadTextFile(const char* filename)
    {
        std::ifstream ifs(filename);
        if (!ifs)
        {
            TL_LOG_ERROR("Failed to open read from file: {}", filename);
            return {};
        }

        std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
        return TL::String(content.c_str());
    }

    enum class FileEventType
    {
        None,
        Rename,
        Added,
        Removed,
        Modified,
    };

    typedef TL::Function<void(const char* path, FileEventType event)> FileEventCB;

    // class FileWatcher
    // {
    // public:
    //     void (const char* path, FileEventCB);
    // };

} // namespace TL