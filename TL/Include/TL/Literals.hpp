#pragma once

#include <chrono>

// Time

// Seconds
constexpr std::chrono::seconds operator""_s(size_t s)
{
    return std::chrono::seconds(s);
}

// Milliseconds
constexpr std::chrono::milliseconds operator""_ms(size_t ms)
{
    return std::chrono::milliseconds(ms);
}

// Microseconds
constexpr std::chrono::microseconds operator""_us(size_t us)
{
    return std::chrono::microseconds(us);
}

// Nanoseconds
constexpr std::chrono::nanoseconds operator""_ns(size_t ns)
{
    return std::chrono::nanoseconds(ns);
}

// Minutes
constexpr std::chrono::minutes operator""_min(size_t min)
{
    return std::chrono::minutes(min);
}

// Hours
constexpr std::chrono::hours operator""_h(size_t h)
{
    return std::chrono::hours(h);
}

/// sizes

// kilobytes
constexpr size_t operator""_kb(size_t kb)
{
    return 1000 * kb;
}

// megabytes
constexpr size_t operator""_mb(size_t mb)
{
    return 1000 * 1000 * mb;
}

// gigabytes
constexpr size_t operator""_gb(size_t gb)
{
    return 1000 * 1000 * 1000 * gb;
}
