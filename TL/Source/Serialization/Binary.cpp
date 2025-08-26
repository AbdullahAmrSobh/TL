#include <TL/Serialization/Binary.hpp>

#include <stdlib.h>

#include <cstdint>

namespace TL
{
    void BinaryArchive::StreamWrite(Block block)
    {
        try
        {
            m_stream->write((const char*)(block.ptr), block.size);
        }
        catch (const std::exception& e)
        {
            TL_ASSERT(m_stream->good(), "Serialization error: {}", e.what());
        }
    }

    void BinaryArchive::StreamRead(Block block)
    {
        try
        {
            m_stream->read((char*)block.ptr, block.size);
        }
        catch (const std::exception& e)
        {
            TL_ASSERT(m_stream->good(), "Deserialization error: {}", e.what());
        }
    }
} // namespace TL