#include "Tl/Serialization/Serialization.hpp"
#include "Tl/Allocator.hpp"

namespace TL
{
    Archive::Archive() = default;

    Archive::Archive(Block block)
        : m_stream(std::string((char*)block.ptr, block.size), std::ios::in | std::ios::binary)
    {
    }

    Block Archive::GetBlock() const
    {
        auto memory = m_stream.str();
        auto buffer = Allocator::Allocate(memory.size());
        std::memcpy(buffer.ptr, memory.data(), memory.size());
        return buffer;
    }
} // namespace TL
