#include "TL/Stacktrace.hpp"

#include <Windows.h>
#include <DbgHelp.h>

#include <format>
#include <string>
#include <vector>

namespace TL
{
#if _WIN32
    static bool InitializeSymbolHandler()
    {
        if (!::SymInitialize(::GetCurrentProcess(), nullptr, TRUE))
        {
            // Consider logging or handling error
            return false;
        }
        return true;
    }
#endif

    Stacktrace CaptureStacktrace(uint32_t skipFramesCount)
    {
#if _WIN32
        Stacktrace stacktrace;
        if (!InitializeSymbolHandler())
        {
            // Handle initialization failure
            return stacktrace; // Return an empty stacktrace or handle it as needed
        }
        ::CaptureStackBackTrace(skipFramesCount, TL_STACKTRACE_DEPTH, stacktrace.data(), nullptr);
        return stacktrace;
#else
    #error "This function is not implemented for the target platform"
#endif
    }

    std::string ReportStacktrace(const Stacktrace& stacktrace)
    {
        std::string message;
        message.reserve(1024); // Reserve space to avoid multiple allocations

        message.append("Stacktrace Report\n");

        for (void* address : stacktrace)
        {
            if (address == nullptr)
            {
                break;
            }

            std::string symbolName = GetSymbolName(address);
            std::string symbolFileAndLine = GetSymbolFileAndLine(address);

            message.append(std::format("\t{} {} \"{}\"\n", address, symbolName, symbolFileAndLine));
        }

        return message;
    }

    std::string GetSymbolName(void* address)
    {
#if _WIN32
        constexpr DWORD64 MAX_NAME_LENGTH = 256;
        char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_NAME_LENGTH - 1];
        SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
        symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        symbol->MaxNameLen = MAX_NAME_LENGTH;

        DWORD64 displacement;
        if (::SymFromAddr(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, symbol))
        {
            return symbol->Name;
        }
        return "Unknown Symbol";
#else
    #error "This function is not implemented for the target platform"
#endif
    }

    std::string GetSymbolFileAndLine(void* address)
    {
#if _WIN32
        IMAGEHLP_LINE64 lineInfo;
        lineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        DWORD displacement;

        if (::SymGetLineFromAddr64(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, &lineInfo))
        {
            return std::format("{}:{}", lineInfo.FileName, lineInfo.LineNumber);
        }
        return "Unknown Line Info";
#else
    #error "This function is not implemented for the target platform"
#endif
    }

} // namespace TL
