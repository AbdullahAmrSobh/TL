#pragma once

#include "TL/Assert.hpp"
#include "TL/Span.hpp"

namespace TL
{
    struct TL_NODISCARD Block
    {
        template<typename T>
        static Block create(const T& obj)
        {
            return {(void*)&obj, sizeof(obj)};
        }

        template<typename T>
        static Block create(TL::Span<T> t)
        {
            return {(void*)t.data(), t.size_bytes()};
        }

        template<typename T>
        static Block create(TL::Span<const T> t)
        {
            return {(void*)t.data(), t.size_bytes()};
        }

        template<typename T, typename Allocator>
        static Block create(std::vector<T, Allocator>& t)
        {
            return {(void*)t.data(), t.size() * sizeof(T)};
        }

        template<typename T>
        static Block create(T* value, size_t count)
        {
            return {(void*)value, count * sizeof(T)};
        }

        friend void swap(Block& a, Block& b)
        {
            std::swap(a.ptr, b.ptr);
            std::swap(a.size, b.size);
        }

        void*  ptr;
        size_t size;
    };
} // namespace TL