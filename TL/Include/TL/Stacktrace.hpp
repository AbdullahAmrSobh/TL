#pragma once

#include "TL/Export.hpp"

#include <array>
#include <string>

#define TL_STACKTRACE_DEPTH 20

namespace TL
{
    using Stacktrace = std::array<void*, TL_STACKTRACE_DEPTH>;

    // Capture the current Stacktrace
    TL_EXPORT Stacktrace CaptureStacktrace(uint32_t skipFramesCount = 1);

    // Report the Stacktrace to the debug callbacks
    TL_EXPORT std::string ReportStacktrace(const Stacktrace& Stacktrace);

    // Return the name of the symbol address
    TL_EXPORT std::string GetSymbolName(void* address);

    // return the file and line of the symbol address
    TL_EXPORT std::string GetSymbolFileAndLine(void* address);
} // namespace RHI