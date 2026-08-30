#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>

// Time

// Seconds
constexpr std::chrono::seconds operator""_s(unsigned long long s)
{
    return std::chrono::seconds(s);
}

// Milliseconds
constexpr std::chrono::milliseconds operator""_ms(unsigned long long ms)
{
    return std::chrono::milliseconds(ms);
}

// Microseconds
constexpr std::chrono::microseconds operator""_us(unsigned long long us)
{
    return std::chrono::microseconds(us);
}

// Nanoseconds
constexpr std::chrono::nanoseconds operator""_ns(unsigned long long ns)
{
    return std::chrono::nanoseconds(ns);
}

// Minutes
constexpr std::chrono::minutes operator""_min(unsigned long long min)
{
    return std::chrono::minutes(min);
}

// Hours
constexpr std::chrono::hours operator""_h(unsigned long long h)
{
    return std::chrono::hours(h);
}

/// sizes

// kilobytes
constexpr size_t operator""_kb(unsigned long long kb)
{
    return 1000 * kb;
}

// megabytes
constexpr size_t operator""_mb(unsigned long long mb)
{
    return 1000 * 1000 * mb;
}

// gigabytes
constexpr size_t operator""_gb(unsigned long long gb)
{
    return 1000 * 1000 * 1000 * gb;
}
