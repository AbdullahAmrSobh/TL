#include <TL/Allocator/Arena.hpp>
#include <TL/Allocator/MemPlumber.hpp>
#include <TL/Serialization/Binary.hpp>

#include <TL/Serialization/Binary.hpp>
#include <TL/Containers/String.hpp>
#include <TL/Containers/Vector.hpp>
#include <TL/File/File.hpp>
#include <TL/Log.hpp>
#include <TL/Assert.hpp>
#include <TL/FileSystem/FileSystemWatcher.hpp>
// #include <TL/DynamicLibrary.hpp>

// extern "C" __declspec(dllexport) void __stdcall
// TL_DllEntry(TL::LibraryLoaderAction action, void* state)
// {
//     TL::LogInfo("Hello, from DLL");
//     // TL::LogInfo("Hello, from DLL");
//     TL::LogInfo("Hello, from DLL");
//     TL::LogInfo("Hello, from DLL");
//     TL::LogInfo("Hello, from DLL");
//     TL::LogInfo("Hello, from DLL After some --more update");
//     TL::LogInfo("Hello, from DLL After some --even more update");
// }