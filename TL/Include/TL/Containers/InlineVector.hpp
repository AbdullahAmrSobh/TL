#pragma once

#include "TL/Assert.hpp"
#include "TL/Span.hpp"

#include <cstddef>
#include <cassert>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <utility>

namespace TL
{
    template<typename T, std::size_t Capacity>
    class InlineVector
    {
    public:
        using value_type      = T;
        using size_type       = std::size_t;
        using reference       = T&;
        using const_reference = const T&;
        using pointer         = T*;
        using const_pointer   = const T*;
        using iterator        = T*;
        using const_iterator  = const T*;

        InlineVector() noexcept = default;

        InlineVector(std::initializer_list<T> init)
        {
            for (const auto& value : init)
                push_back(value);
        }

        InlineVector(const InlineVector& other)
        {
            for (size_type i = 0; i < other.m_size; ++i)
                push_back(other[i]);
        }

        InlineVector(InlineVector&& other) noexcept
        {
            for (size_type i = 0; i < other.m_size; ++i)
                push_back(std::move(other[i]));
            other.clear();
        }

        InlineVector& operator=(const InlineVector& other)
        {
            if (this != &other)
            {
                clear();
                for (size_type i = 0; i < other.m_size; ++i)
                    push_back(other[i]);
            }
            return *this;
        }

        InlineVector& operator=(InlineVector&& other) noexcept
        {
            if (this != &other)
            {
                clear();
                for (size_type i = 0; i < other.m_size; ++i)
                    push_back(std::move(other[i]));
                other.clear();
            }
            return *this;
        }

        ~InlineVector()
        {
            clear();
        }

        reference push_back(const T& value)
        {
            assert(m_size < Capacity && "InlineVector capacity exceeded");
            return *::new (slot(m_size++)) T(value);
        }

        reference push_back(T&& value)
        {
            assert(m_size < Capacity && "InlineVector capacity exceeded");
            return *::new (slot(m_size++)) T(std::move(value));
        }

        template<typename... Args>
        reference emplace_back(Args&&... args)
        {
            assert(m_size < Capacity && "InlineVector capacity exceeded");
            return *::new (slot(m_size++)) T(std::forward<Args>(args)...);
        }

        void pop_back()
        {
            TL_ASSERT(m_size > 0, "InlineVector is empty");
            data()[--m_size].~T();
        }

        // No-op other than a capacity sanity check: storage is fixed and inline.
        void reserve(size_type count) const noexcept
        {
            TL_ASSERT(count <= Capacity, "InlineVector reserve exceeds capacity");
        }

        void resize(size_type count)
        {
            TL_ASSERT(count <= Capacity, "InlineVector resize exceeds capacity");
            while (m_size > count)
                pop_back();
            while (m_size < count)
                emplace_back();
        }

        void clear() noexcept
        {
            for (size_type i = 0; i < m_size; ++i)
                data()[i].~T();
            m_size = 0;
        }

        size_type size() const noexcept
        {
            return m_size;
        }

        size_type size_bytes() const noexcept
        {
            return m_size * sizeof(T);
        }

        bool empty() const noexcept
        {
            return m_size == 0;
        }

        static constexpr size_type capacity() noexcept
        {
            return Capacity;
        }

        pointer data() noexcept
        {
            return reinterpret_cast<pointer>(m_storage);
        }

        const_pointer data() const noexcept
        {
            return reinterpret_cast<const_pointer>(m_storage);
        }

        reference operator[](size_type index) noexcept
        {
            TL_ASSERT(index < m_size, "InlineVector index out of range");
            return data()[index];
        }

        const_reference operator[](size_type index) const noexcept
        {
            TL_ASSERT(index < m_size, "InlineVector index out of range");
            return data()[index];
        }

        reference front() noexcept
        {
            return data()[0];
        }

        const_reference front() const noexcept
        {
            return data()[0];
        }

        reference back() noexcept
        {
            return data()[m_size - 1];
        }

        const_reference back() const noexcept
        {
            return data()[m_size - 1];
        }

        iterator begin() noexcept
        {
            return data();
        }

        const_iterator begin() const noexcept
        {
            return data();
        }

        iterator end() noexcept
        {
            return data() + m_size;
        }

        const_iterator end() const noexcept
        {
            return data() + m_size;
        }

        operator Span<T>()
        {
            return {};
        }

        Span<T> toSpan() const
        {
            return {};
        }

    private:
        void* slot(size_type index) noexcept { return &m_storage[index * sizeof(T)]; }

        alignas(T) std::byte m_storage[Capacity * sizeof(T)];
        size_type m_size = 0;
    };
} // namespace TL
