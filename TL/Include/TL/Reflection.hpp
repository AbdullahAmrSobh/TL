#pragma once

namespace TL::Reflect
{
    struct Member
    {
        uint32_t index;
        std::string name;
    };

    struct Reflector
    {
        template<typename T>
        static void visit(T&& type)
        {
            if constexpr (requires { std::forward<T>(type).TLReflect(); })
            {
                std::forward<T>(type).TLReflect();
            }
            else if constexpr (requires { TL::reflect(std::forward<T>(type)); })
            {
                TL::reflect(std::forward<T>(type));
            }
            else
            {
                static_assert(false, "Type T must have either TLReflect() member function or TL::reflect() free function for reflection support");
            }
        }
    };
} // namespace TL::Reflect