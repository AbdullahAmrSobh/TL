#include "TL/Compiler.hpp"
#include "TL/Fmt.hpp"
#include "TL/Library.hpp"

#if TL_PLATFORM_WINDOWS
    #include "WindowsCommon.inl"
#endif

namespace TL
{
    Result<Library> Library::open(TL::StringView path, LibraryOpenFlags flags)
    {
#if TL_PLATFORM_WINDOWS
        Library library;

        DWORD loadFlags = 0;
        // if (flags & LibraryOpenFlags::DontResolveReferences)
        //     loadFlags |= DONT_RESOLVE_DLL_REFERENCES;
        // if (flags & LibraryOpenFlags::AlterSearchPath)
        //     loadFlags |= LOAD_WITH_ALTERED_SEARCH_PATH;

        library.m_impl = ::LoadLibraryExA(path.data(), nullptr, loadFlags);

        if (library.m_impl == nullptr)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);

            return Error(TL::fmt("Failed to open library '{}': {} (code={})",
                                 path,
                                 TL::windows::errorToString(code),
                                 code));
        }

        return library;
#else
        static_cast<void>(path);
        static_cast<void>(flags);
        return Error("Dynamic libraries are not supported on this platform");
#endif
    }

    void Library::close(Library library)
    {
#if TL_PLATFORM_WINDOWS
        if (library.m_impl)
        {
            ::FreeLibrary(static_cast<HMODULE>(library.m_impl));
            library.m_impl = nullptr;
        }
#else
        static_cast<void>(library);
#endif
    }

    void* Library::getProc(TL::StringView procName)
    {
#if TL_PLATFORM_WINDOWS
        if (!m_impl)
            return nullptr;

        void* ptr = reinterpret_cast<void*>(::GetProcAddress(static_cast<HMODULE>(m_impl), procName.data()));

        if (ptr == nullptr)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);

            ::TL::LogError("Failed to get procedure '{}' from library: {} (code={})",
                           procName,
                           TL::windows::errorToString(code),
                           code);
            return nullptr;
        }

        return ptr;
#else
        static_cast<void>(procName);
        return nullptr;
#endif
    }
} // namespace TL
