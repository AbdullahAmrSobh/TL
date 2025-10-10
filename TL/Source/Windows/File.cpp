#include "TL/FileSystem/File.hpp"

#include "WindowsCommon.inl"

namespace TL
{
    File::File() = default;

    File::File(StringView path, IOMode mode)
    {
        auto err = open(path, mode);
    }

    File::~File()
    {
        close();
    }

    IOResultCode File::open(StringView path, IOMode mode)
    {
        DWORD dwDesiredAccess       = 0;
        DWORD dwShareMode           = 0;
        DWORD dwCreationDisposition = 0;
        switch (mode)
        {
        case IOMode::Read:
            dwDesiredAccess       = GENERIC_READ;
            dwShareMode           = FILE_SHARE_READ;
            dwCreationDisposition = OPEN_EXISTING;
            break;
        case IOMode::Write:
            dwDesiredAccess       = GENERIC_WRITE;
            dwShareMode           = 0;
            dwCreationDisposition = CREATE_ALWAYS;
            break;
        case IOMode::Append:
            dwDesiredAccess       = FILE_APPEND_DATA;
            dwShareMode           = 0;
            dwCreationDisposition = OPEN_ALWAYS;
            break;
        case IOMode::Overwrite:
            dwDesiredAccess       = GENERIC_WRITE;
            dwShareMode           = 0;
            dwCreationDisposition = TRUNCATE_EXISTING;
            break;
        default:
            // Handle invalid mode if necessary
            break;
        }

        m_handle = ::CreateFile((LPCSTR)path.data(), dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_handle)
            getlastError();
        return IOResultCode::Success;
    }

    void File::close()
    {
        if (m_handle)
        {
            ::CloseHandle(m_handle);
            m_handle = nullptr;
        }
    }

    size_t File::size() const
    {
        LARGE_INTEGER fileSize;
        if (!::GetFileSizeEx(m_handle, &fileSize))
            return 0;
        return static_cast<size_t>(fileSize.QuadPart);
    }

    size_t File::position() const
    {
        LARGE_INTEGER zero = {0};
        LARGE_INTEGER pos;
        if (!::SetFilePointerEx(m_handle, zero, &pos, FILE_CURRENT))
            return 0;
        return static_cast<size_t>(pos.QuadPart);
    }

    IOResult File::read(Block block, uint64_t offset)
    {
        if (!block.ptr || block.size == 0)
            return 0;

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesRead = 0;
        BOOL  ok        = ::ReadFile(m_handle, block.ptr, static_cast<DWORD>(block.size), &bytesRead, &ov);
        if (!ok)
            getlastError();
        return static_cast<size_t>(bytesRead);
    }

    IOResult File::read(String& string, uint64_t offset)
    {
        // TL_ASSERT(string.size() >= size() - offset);
        if (string.empty())
            return 0;

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesRead = 0;
        BOOL  ok        = ::ReadFile(m_handle, &string[0], static_cast<DWORD>(string.size()), &bytesRead, &ov);
        if (!ok)
            LoglastError();
        return static_cast<size_t>(bytesRead);
    }

    IOResult File::write(Block block, uint64_t offset)
    {
        if (!block.ptr || block.size == 0)
            return 0;

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesWritten = 0;
        BOOL  ok           = ::WriteFile(m_handle, block.ptr, static_cast<DWORD>(block.size), &bytesWritten, &ov);
        if (!ok)
            getlastError();
        return static_cast<size_t>(bytesWritten);
    }

    IOResult File::write(String string, uint64_t offset)
    {
        if (string.empty())
            return 0;

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesWritten = 0;
        BOOL  ok           = ::WriteFile(m_handle, string.data(), static_cast<DWORD>(string.size()), &bytesWritten, &ov);
        if (!ok)
            getlastError();
        return static_cast<size_t>(bytesWritten);
    }
} // namespace TL