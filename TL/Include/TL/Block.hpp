#pragma once
#include "TL/Export.hpp"
#include "TL/Assert.hpp"

#include "TL/Span.hpp"

namespace TL
{
    struct TL_NODISCARD Block
    {
        template<typename T>
        static Block Create(T& obj)
        {
            return { (void*)&obj, sizeof(obj) };
        }

        template<typename T>
        static Block Create(TL::Span<T> t)
        {
            return { (void*)t.data(), t.size_bytes() };
        }

        template<typename T>
        static Block Create(TL::Span<const T> t)
        {
            return { (void*)t.data(), t.size_bytes() };
        }

        template<typename T, typename Allocator>
        static Block Create(std::vector<T, Allocator>& t)
        {
            return { (void*)t.data(), t.size() * sizeof(T) };
        }

        template<typename T>
        static Block Create(T* value, size_t count)
        {
            return { (void*)value, count * sizeof(T) };
        }

        void* ptr;
        size_t size;
    };

    class TL_EXPORT Buffer
    {
        Block m_block;
        class Allocator* m_allocator;

    public:
        Buffer();
        Buffer(TL::Block block);
        Buffer(const Buffer& other) = delete;
        Buffer(Buffer&& other) = default;
        ~Buffer();

        Buffer& operator=(const Buffer& other) = delete;
        Buffer& operator=(Buffer&& other) = default;

        TL::Block Release();

        size_t GetSize() const;

        const void* GetData() const;

        void* GetData();

        operator const Block() const;
        operator Block();
    };
} // namespace TL