#pragma once

#include "TL/Span.hpp"
#include "TL/Containers.hpp"

namespace TL
{
    class StringView : public TL::Span<const char>
    {
    public:
        using Span::Span;

        // Default constructor
        StringView()
            : Span()
        {
        }

        // Constructor from string literal (deduces size at compile time, excludes null terminator)
        template<size_t N>
        constexpr StringView(const char (&str)[N])
            : Span(str, N - 1)
        {
        }

        // Constructor from const char* and length
        StringView(const char* str, size_t len)
            : Span(str, len)
        {
        }

        // Constructor from null-terminated C string
        StringView(const char* str)
            : Span(str, str ? strlen(str) : 0)
        {
        }

        // TODO: Move to cpp file
// #if TL_PLATFORM_WINDOWS
//         // Constructor from LPCWSTR (wide string, converts to UTF-8)
//         StringView(void* wstr)
//         {
//             if (!wstr)
//             {
//                 m_data  = nullptr;
//                 m_count = 0;
//                 return;
//             }
//             int len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
//             if (len <= 1)
//             {
//                 m_data  = nullptr;
//                 m_count = 0;
//                 return;
//             }
//             // Allocate temporary buffer (not owning, for demonstration)
//             static thread_local std::string temp;
//             temp.resize(len - 1);
//             WideCharToMultiByte(CP_UTF8, 0, wstr, -1, temp.data(), len, nullptr, nullptr);
//             m_data  = temp.data();
//             m_count = len - 1;
//         }
// #endif

// #if TL_PLATFORM_WINDOWS
//         void* AsWindowsCStr() const
//         {
//             // Ensure null-terminated for Windows APIs
//             if (!m_data || m_count == 0)
//                 return "";
//             if (m_data[m_count] == '\0')
//                 return m_data;
//             // Allocate temporary buffer (not owning, for demonstration)
//             static thread_local std::string temp;
//             temp.assign(m_data, m_count);
//             temp.push_back('\0');
//             return temp.c_str();
//         }
// #endif
    };
} // namespace TL