#include <format>
#include <stacktrace>
#include <string>
#include <vector>

#include "TL/Stacktrace.hpp"

namespace TL
{
    Stacktrace CaptureStacktrace(uint32_t skipFramesCount)
    {
        // Capture the current stacktrace, skipping the requested number of frames + this function
        std::stacktrace st = std::stacktrace::current(skipFramesCount + 1);
        Stacktrace result{};
        size_t     count = std::min(st.size(), result.size());
        for (size_t i = 0; i < count; ++i)
            result[i] = st[i] ? st[i] : std::stacktrace_entry{};
        return result;
    }

    std::string ReportStacktrace(const Stacktrace& stacktrace)
    {
        std::string report;
        uint32_t    i = 0;
        for (const auto& e : stacktrace)
        {
            if (!e)
                break;

            report += std::format("#{} {} [{}:{}]\n",
                                  i++,
                                  e.description(),
                                  e.source_file(),
                                  e.source_line());
        }
        return report;
    }

} // namespace TL
