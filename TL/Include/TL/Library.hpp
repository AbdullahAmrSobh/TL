#pragma once

#include <TL/Export.hpp>
#include <TL/Result.hpp>

namespace TL
{
    enum class LibraryOpenFlags
    {
        None,
    };

    class Library
    {
    public:
        static Result<Library> Open(const char* path, LibraryOpenFlags flags = LibraryOpenFlags::None);
        static void            Close(Library library);

        void*                  GetProc(const char* procName);

        template<typename Proc>
        Proc GetProc(const char* procName)
        {
            return reinterpret_cast<Proc>(GetProc(procName));
        }

    private:
        void* m_impl;
    };
} // namespace TL