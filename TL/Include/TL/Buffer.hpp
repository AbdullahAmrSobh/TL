#pragma once

#include "TL/Block.hpp"

#include <cstddef>
#include <cstring>
#include <utility>

namespace TL
{
    class TL_EXPORT Buffer
    {
    public:
        // Constructor
        explicit Buffer(size_t size)
            : m_size(size)
            , m_data(new char[size])
        {
        }

        // Move Constructor
        Buffer(Buffer&& other) noexcept
            : m_size(other.m_size)
            , m_data(other.m_data)
        {
            other.m_size = 0;
            other.m_data = nullptr;
        }

        // Move Assignment Operator
        Buffer& operator=(Buffer&& other) noexcept
        {
            if (this != &other)
            {
                delete[] m_data; // Release the current resource

                m_size = other.m_size;
                m_data = other.m_data;

                other.m_size = 0;
                other.m_data = nullptr;
            }
            return *this;
        }

        // Destructor
        ~Buffer()
        {
            delete[] m_data;
        }

        // Disabled Copy Constructor
        Buffer(const Buffer&) = delete;

        // Disabled Copy Assignment Operator
        Buffer& operator=(const Buffer&) = delete;

        // Get the data as a Block
        Block Data() const
        {
            return Block{ m_data, m_size };
        }

        // Get the size of the buffer
        size_t Size() const
        {
            return m_size;
        }

        // Clone function to create a copy of the buffer
        Buffer Clone() const
        {
            Buffer clone(m_size);
            std::memcpy(clone.m_data, m_data, m_size);
            return clone;
        }

    private:
        size_t m_size;
        char* m_data;
    };
} // namespace TL
