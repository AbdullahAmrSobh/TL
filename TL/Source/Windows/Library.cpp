#include "TL/Library.hpp"

#include "WindowsCommon.inl"

namespace TL
{
    Result<Library> Library::Open(const char* path, LibraryOpenFlags flags)
    {
        Library library;
        library.m_impl = LoadLibraryEx(path, nullptr, 0);
        if (library.m_impl == nullptr)
        {
            LoglastError();
            return Error(std::format("Failed to open library at '{}': {}", path, ::GetLastError()));

        }
        return library;
    }

    void Library::Close(Library library)
    {
        FreeLibrary((HMODULE)library.m_impl);
    }

    void* Library::GetProc(const char* procName)
    {
        auto ptr = (void*)GetProcAddress((HMODULE)m_impl, procName);

        if (ptr == nullptr)
        {
            LoglastError();

            return nullptr;
        }

        return ptr;
    }
} // namespace TL