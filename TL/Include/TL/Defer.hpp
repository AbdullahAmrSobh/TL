#pragma once

#include <functional>
#include <utility>

namespace TL
{
    class Defer
    {
    public:
        template<typename Func>
        Defer(Func&& func)
            : func_(std::forward<Func>(func))
        {
        }

        ~Defer()
        {
            if (func_)
                func_();
        }

        // Disallow copy and assignment
        Defer(const Defer&)            = delete;
        Defer& operator=(const Defer&) = delete;

        // Allow move semantics
        Defer(Defer&& other) noexcept
            : func_(std::move(other.func_))
        {
            other.func_ = nullptr;
        }

    private:
        std::function<void()> func_;
    };
} // namespace TL

// Macro to generate a unique variable name
#define TL_CONCATENATE_DETAIL(x, y) x##y
#define TL_CONCATENATE(x, y)        TL_CONCATENATE_DETAIL(x, y)

// Defer macro to use a lambda without additional parentheses
#define TL_defer                    [[maybe_unused]] auto TL_CONCATENATE(_tl_defer_, TL_CONCATENATE(__COUNTER__, __LINE__)) = [&]()
