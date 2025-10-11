#pragma once

#include "TL/Export.hpp"

#include <array>
#include <stacktrace>

#define TL_STACKTRACE_DEPTH 30

namespace TL
{
    using Stacktrace = std::array<std::stacktrace_entry, TL_STACKTRACE_DEPTH>;

    // Capture the current Stacktrace
    TL_EXPORT Stacktrace CaptureStacktrace(uint32_t skipFramesCount = 1);

    // Report the Stacktrace to the debug callbacks
    TL_EXPORT std::string ReportStacktrace(const Stacktrace& Stacktrace);
} // namespace RHI