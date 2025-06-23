#include <TL/Allocator/Arena.hpp>
#include <TL/Allocator/MemPlumber.hpp>
#include <TL/Serialization/Binary.hpp>

#include <TL/Serialization/Binary.hpp>
#include <TL/Containers.hpp>
#include <TL/FileSystem/File.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>
#include <TL/Stacktrace.hpp>
#include <TL/FileSystem/FileWatcher.hpp>
#include <TL/DynamicLibrary.hpp>

extern "C" __declspec(dllexport) void __stdcall
TL_DllEntry(TL::LibraryLoaderAction action, void* state)
{
    TL_LOG_INFO("Hello, from DLL");
    // TL_LOG_INFO("Hello, from DLL");
    TL_LOG_INFO("Hello, from DLL");
    TL_LOG_INFO("Hello, from DLL");
    TL_LOG_INFO("Hello, from DLL");
    TL_LOG_INFO("Hello, from DLL After some --more update");
    TL_LOG_INFO("Hello, from DLL After some --even more update");
}