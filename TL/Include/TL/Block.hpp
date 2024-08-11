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

        void* ptr;
        size_t size;
    };
} // namespace TL