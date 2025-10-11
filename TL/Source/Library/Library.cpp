#include "TL/Library.hpp"

#include "WindowsCommon.inl"

namespace TL
{
    Result<Library> Library::open(TL::StringView path, LibraryOpenFlags flags)
    {
        Library library;

        DWORD   loadFlags = 0;
        // if (flags & LibraryOpenFlags::DontResolveReferences)
        //     loadFlags |= DONT_RESOLVE_DLL_REFERENCES;
        // if (flags & LibraryOpenFlags::AlterSearchPath)
        //     loadFlags |= LOAD_WITH_ALTERED_SEARCH_PATH;

        library.m_impl = ::LoadLibraryExA(path.data(), nullptr, loadFlags);

        if (library.m_impl == nullptr)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);

            return Error(std::format("Failed to open library '{}': {} (code={})",
                         path,
                         TL::windows::errorToString(code),
                         code));
        }

        return library;
    }

    void Library::close(Library library)
    {
        if (library.m_impl)
        {
            ::FreeLibrary(static_cast<HMODULE>(library.m_impl));
            library.m_impl = nullptr;
        }
    }

    void* Library::getProc(TL::StringView procName)
    {
        if (!m_impl)
            return nullptr;

        void* ptr = reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(m_impl), procName.data()));

        if (ptr == nullptr)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);

            TL_LOG_ERROR("Failed to get procedure '{}' from library: {} (code={})",
                         procName,
                         TL::windows::errorToString(code),
                         code);
            return nullptr;
        }

        return ptr;
    }
} // namespace TL
