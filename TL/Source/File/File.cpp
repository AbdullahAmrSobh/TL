#include "TL/File/File.hpp"
#include "WindowsCommon.inl"

namespace TL
{
    File::File() = default;

    File::File(StringView path, IOMode mode)
    {
        open(path, mode);
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
            return IOResultCode::InvalidParameter;
        }

        m_handle = ::CreateFileA(
            path.data(),
            dwDesiredAccess,
            dwShareMode,
            nullptr,
            dwCreationDisposition,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (m_handle == INVALID_HANDLE_VALUE)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return TL::windows::getlastError(code);
        }

        return IOResultCode::Success;
    }

    void File::close()
    {
        if (m_handle && m_handle != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(m_handle);
            m_handle = nullptr;
        }
    }

    size_t File::size() const
    {
        LARGE_INTEGER fileSize;
        if (!::GetFileSizeEx(m_handle, &fileSize))
        {
            TL::windows::logError(::GetLastError());
            return 0;
        }
        return static_cast<size_t>(fileSize.QuadPart);
    }

    size_t File::position() const
    {
        LARGE_INTEGER zero = {0};
        LARGE_INTEGER pos;
        if (!::SetFilePointerEx(m_handle, zero, &pos, FILE_CURRENT))
        {
            TL::windows::logError(::GetLastError());
            return 0;
        }
        return static_cast<size_t>(pos.QuadPart);
    }

    IOResult File::read(Block block, uint64_t offset)
    {
        if (!block.ptr || block.size == 0)
            return {0, IOResultCode::InvalidParameter};

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesRead = 0;
        BOOL  ok        = ::ReadFile(m_handle, block.ptr, static_cast<DWORD>(block.size), &bytesRead, &ov);

        if (!ok)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return {0, TL::windows::getlastError(code)};
        }

        return {bytesRead, IOResultCode::Success};
    }

    IOResult File::read(String& string, uint64_t offset)
    {
        if (string.empty())
            return {0, IOResultCode::InvalidParameter};

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesRead = 0;
        BOOL  ok        = ::ReadFile(m_handle, string.data(), static_cast<DWORD>(string.size()), &bytesRead, &ov);

        if (!ok)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return {0, TL::windows::getlastError(code)};
        }

        return {bytesRead, IOResultCode::Success};
    }

    IOResult File::write(Block block, uint64_t offset)
    {
        if (!block.ptr || block.size == 0)
            return {0, IOResultCode::InvalidParameter};

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesWritten = 0;
        BOOL  ok           = ::WriteFile(m_handle, block.ptr, static_cast<DWORD>(block.size), &bytesWritten, &ov);

        if (!ok)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return {0, TL::windows::getlastError(code)};
        }

        return {bytesWritten, IOResultCode::Success};
    }

    IOResult File::write(String string, uint64_t offset)
    {
        if (string.empty())
            return {0, IOResultCode::InvalidParameter};

        OVERLAPPED ov = {};
        ov.Offset     = static_cast<DWORD>(offset & 0xFFFFFFFF);
        ov.OffsetHigh = static_cast<DWORD>((offset >> 32) & 0xFFFFFFFF);

        DWORD bytesWritten = 0;
        BOOL  ok           = ::WriteFile(m_handle, string.data(), static_cast<DWORD>(string.size()), &bytesWritten, &ov);

        if (!ok)
        {
            DWORD code = ::GetLastError();
            TL::windows::logError(code);
            return {0, TL::windows::getlastError(code)};
        }

        return {bytesWritten, IOResultCode::Success};
    }
} // namespace TL
