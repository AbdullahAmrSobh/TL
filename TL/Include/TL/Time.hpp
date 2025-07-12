#pragma once

#include <chrono>

// User-defined literals for time durations

// Seconds
constexpr std::chrono::seconds operator"" _tl_s(size_t s)
{
    return std::chrono::seconds(s);
}

// Milliseconds
constexpr std::chrono::milliseconds operator"" _tl_ms(size_t ms)
{
    return std::chrono::milliseconds(ms);
}

// Microseconds
constexpr std::chrono::microseconds operator"" _tl_us(size_t us)
{
    return std::chrono::microseconds(us);
}

// Nanoseconds
constexpr std::chrono::nanoseconds operator"" _tl_ns(size_t ns)
{
    return std::chrono::nanoseconds(ns);
}

// Minutes
constexpr std::chrono::minutes operator"" _tl_min(size_t min)
{
    return std::chrono::minutes(min);
}

// Hours
constexpr std::chrono::hours operator"" _tl_h(size_t h)
{
    return std::chrono::hours(h);
}

/// sizes

// kilobytes
constexpr size_t operator"" _tl_kb(size_t kb)
{
    return 1000 * kb;
}

// megabytes
constexpr size_t operator"" _tl_mb(size_t mb)
{
    return 1000 * 1000 * mb;
}

// gigabytes
constexpr size_t operator"" _tl_gb(size_t gb)
{
    return 1000 * 1000 * 1000 * gb;
}
