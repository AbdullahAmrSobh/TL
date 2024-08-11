#pragma once

#include "TL/Block.hpp"
#include "TL/Allocator.hpp"
#include "TL/Export.hpp"

#include <lz4.h>

/// @todo: this is a temp implementation for quick prototyping

namespace TL
{

    inline static Block CompressBlock(const Block& block)
    {
        int maxCompressedSize = LZ4_compressBound(static_cast<int>(block.size));
        Block compressedBlock = Allocator::Allocate(maxCompressedSize + sizeof(size_t));

        // Store the original size at the beginning of the compressed block
        *reinterpret_cast<size_t*>(compressedBlock.ptr) = block.size;

        // Perform compression, starting after the size storage
        int compressedSize = LZ4_compress_default(
            static_cast<const char*>(block.ptr),
            static_cast<char*>(compressedBlock.ptr) + sizeof(size_t),
            static_cast<int>(block.size),
            maxCompressedSize);

        if (compressedSize <= 0)
        {
            Allocator::Release(compressedBlock);
            TL_LOG_ERROR("Compression failed");
            return {};
        }

        compressedBlock.size = compressedSize + sizeof(size_t);
        return compressedBlock;
    }

    inline static Block DecompressBlock(const Block& compressedBlock)
    {
        // Retrieve the original size stored at the beginning of the compressed block
        size_t originalSize = *reinterpret_cast<size_t*>(compressedBlock.ptr);
        Block decompressedBlock = Allocator::Allocate(originalSize);

        // Perform decompression, starting after the size storage
        int decompressedSize = LZ4_decompress_safe(
            static_cast<const char*>(compressedBlock.ptr) + sizeof(size_t),
            static_cast<char*>(decompressedBlock.ptr),
            static_cast<int>(compressedBlock.size - sizeof(size_t)),
            static_cast<int>(originalSize));

        if (decompressedSize < 0)
        {
            Allocator::Release(decompressedBlock);
            TL_LOG_ERROR("Decompression failed");
            return {};
        }

        decompressedBlock.size = originalSize;
        return decompressedBlock;
    }

} // namespace TL