#pragma once

#include "TL/Block.hpp"
#include "TL/Allocator.hpp"

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

        // Get the file size
        ifs.seekg(0, std::ios::end);
        std::streamsize size = ifs.tellg();
        ifs.seekg(0, std::ios::beg);

        // Allocate memory for the data and read it
        auto data = TL::Allocator::Allocate(size);
        if (ifs.read(reinterpret_cast<char*>(data.ptr), size))
        {
            return data;
        }
        else
        {
            TL_LOG_ERROR("Failed to read from file: {}", filename);
            return {};
        }
    }
} // namespace TL