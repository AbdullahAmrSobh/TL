#pragma once

#include <TL/Export.hpp>
#include <TL/Result.hpp>

#include <TL/Containers/StringView.hpp>

namespace TL
{
    enum class LibraryOpenFlags
    {
        None,
    };

    class Library
    {
    public:
        static Result<Library> open(TL::StringView path, LibraryOpenFlags flags = LibraryOpenFlags::None);

        static void            close(Library library);

        void*                  getProc(TL::StringView procName);

        template<typename Proc>
        Proc getProc(TL::StringView procName)
        {
            return reinterpret_cast<Proc>(getProc(procName));
        }

    private:
        void* m_impl = nullptr;
    };
} // namespace TL