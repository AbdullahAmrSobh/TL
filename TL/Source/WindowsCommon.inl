#pragma once

#include "TL/Stream.hpp"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace TL::windows
{
    inline static IOResultCode getlastError(DWORD code)
    {
        switch (code)
        {
        case ERROR_FILE_NOT_FOUND: return IOResultCode::NotFound;
        case ERROR_PATH_NOT_FOUND: return IOResultCode::NotFound;
        case ERROR_ACCESS_DENIED:  return IOResultCode::Denied;
        case ERROR_HANDLE_EOF:     return IOResultCode::Eof;
        case ERROR_DISK_FULL:      return IOResultCode::Full;
        // case ERROR_INVALID_HANDLE:
        //     return IOResultCode::InvalidHandle;
        // case ERROR_INVALID_PARAMETER:
        //     return IOResultCode::InvalidParameter;
        // case ERROR_SHARING_VIOLATION:
        //     return IOResultCode::SharingViolation;
        // case ERROR_LOCK_VIOLATION:
        //     return IOResultCode::LockViolation;
        // case ERROR_ALREADY_EXISTS:
        // case ERROR_FILE_EXISTS:
        //     return IOResultCode::AlreadyExists;
        case ERROR_NOT_ENOUGH_MEMORY:
        case ERROR_OUTOFMEMORY:
        //     return IOResultCode::OutOfMemory;
        // case ERROR_OPERATION_ABORTED:
        //     return IOResultCode::Aborted;
        // case ERROR_IO_DEVICE:
        //     return IOResultCode::DeviceError;
        // case ERROR_WRITE_PROTECT:
        //     return IOResultCode::WriteProtected;
        case ERROR_READ_FAULT:
        case ERROR_WRITE_FAULT:
        //     return IOResultCode::IOResultCode;
        // case ERROR_BROKEN_PIPE:
        //     return IOResultCode::BrokenPipe;
        // case ERROR_NO_MORE_FILES:
        //     return IOResultCode::NoMoreFiles;
        // case ERROR_CRC:
        //     return IOResultCode::Corrupt;
        // case ERROR_NOT_SUPPORTED:
        //     return IOResultCode::NotSupported;
        case ERROR_TIMEOUT:
            return IOResultCode::Timeout;
        // case ERROR_FILE_CORRUPT:
        //     return IOResultCode::Corrupt;
        // case ERROR_FILE_TOO_LARGE:
        //     return IOResultCode::FileTooLarge;
        default:
            return IOResultCode::Unknown;
        }
    }

    inline static TL::StringView errorToString(DWORD code)
    {
        switch (code)
        {
        case ERROR_FILE_NOT_FOUND:    return "ERROR_FILE_NOT_FOUND";
        case ERROR_PATH_NOT_FOUND:    return "ERROR_PATH_NOT_FOUND";
        case ERROR_ACCESS_DENIED:     return "ERROR_ACCESS_DENIED";
        case ERROR_HANDLE_EOF:        return "ERROR_HANDLE_EOF";
        case ERROR_DISK_FULL:         return "ERROR_DISK_FULL";
        case ERROR_INVALID_HANDLE:    return "ERROR_INVALID_HANDLE";
        case ERROR_INVALID_PARAMETER: return "ERROR_INVALID_PARAMETER";
        case ERROR_SHARING_VIOLATION: return "ERROR_SHARING_VIOLATION";
        case ERROR_LOCK_VIOLATION:    return "ERROR_LOCK_VIOLATION";
        case ERROR_ALREADY_EXISTS:    return "ERROR_ALREADY_EXISTS";
        case ERROR_FILE_EXISTS:       return "ERROR_FILE_EXISTS";
        case ERROR_NOT_ENOUGH_MEMORY: return "ERROR_NOT_ENOUGH_MEMORY";
        case ERROR_OUTOFMEMORY:       return "ERROR_OUTOFMEMORY";
        case ERROR_OPERATION_ABORTED: return "ERROR_OPERATION_ABORTED";
        case ERROR_IO_DEVICE:         return "ERROR_IO_DEVICE";
        case ERROR_WRITE_PROTECT:     return "ERROR_WRITE_PROTECT";
        case ERROR_READ_FAULT:        return "ERROR_READ_FAULT";
        case ERROR_WRITE_FAULT:       return "ERROR_WRITE_FAULT";
        case ERROR_BROKEN_PIPE:       return "ERROR_BROKEN_PIPE";
        case ERROR_NO_MORE_FILES:     return "ERROR_NO_MORE_FILES";
        case ERROR_CRC:               return "ERROR_CRC";
        case ERROR_NOT_SUPPORTED:     return "ERROR_NOT_SUPPORTED";
        case ERROR_TIMEOUT:           return "ERROR_TIMEOUT";
        case ERROR_FILE_CORRUPT:      return "ERROR_FILE_CORRUPT";
        case ERROR_FILE_TOO_LARGE:    return "ERROR_FILE_TOO_LARGE";
        default:                      return "<unknown error code>";
        }
    }

    inline static void logError(DWORD code)
    {
        switch (code)
        {
        case ERROR_FILE_NOT_FOUND:    TL_LOG_ERROR("Error: ERROR_FILE_NOT_FOUND"); break;
        case ERROR_PATH_NOT_FOUND:    TL_LOG_ERROR("Error: ERROR_PATH_NOT_FOUND"); break;
        case ERROR_ACCESS_DENIED:     TL_LOG_ERROR("Error: ERROR_ACCESS_DENIED"); break;
        case ERROR_HANDLE_EOF:        TL_LOG_ERROR("Error: ERROR_HANDLE_EOF"); break;
        case ERROR_DISK_FULL:         TL_LOG_ERROR("Error: ERROR_DISK_FULL"); break;
        case ERROR_INVALID_HANDLE:    TL_LOG_ERROR("Error: ERROR_INVALID_HANDLE"); break;
        case ERROR_INVALID_PARAMETER: TL_LOG_ERROR("Error: ERROR_INVALID_PARAMETER"); break;
        case ERROR_SHARING_VIOLATION: TL_LOG_ERROR("Error: ERROR_SHARING_VIOLATION"); break;
        case ERROR_LOCK_VIOLATION:    TL_LOG_ERROR("Error: ERROR_LOCK_VIOLATION"); break;
        case ERROR_ALREADY_EXISTS:    TL_LOG_ERROR("Error: ERROR_ALREADY_EXISTS"); break;
        case ERROR_FILE_EXISTS:       TL_LOG_ERROR("Error: ERROR_FILE_EXISTS"); break;
        case ERROR_NOT_ENOUGH_MEMORY: TL_LOG_ERROR("Error: ERROR_NOT_ENOUGH_MEMORY"); break;
        case ERROR_OUTOFMEMORY:       TL_LOG_ERROR("Error: ERROR_OUTOFMEMORY"); break;
        case ERROR_OPERATION_ABORTED: TL_LOG_ERROR("Error: ERROR_OPERATION_ABORTED"); break;
        case ERROR_IO_DEVICE:         TL_LOG_ERROR("Error: ERROR_IO_DEVICE"); break;
        case ERROR_WRITE_PROTECT:     TL_LOG_ERROR("Error: ERROR_WRITE_PROTECT"); break;
        case ERROR_READ_FAULT:        TL_LOG_ERROR("Error: ERROR_READ_FAULT"); break;
        case ERROR_WRITE_FAULT:       TL_LOG_ERROR("Error: ERROR_WRITE_FAULT"); break;
        case ERROR_BROKEN_PIPE:       TL_LOG_ERROR("Error: ERROR_BROKEN_PIPE"); break;
        case ERROR_NO_MORE_FILES:     TL_LOG_ERROR("Error: ERROR_NO_MORE_FILES"); break;
        case ERROR_CRC:               TL_LOG_ERROR("Error: ERROR_CRC"); break;
        case ERROR_NOT_SUPPORTED:     TL_LOG_ERROR("Error: ERROR_NOT_SUPPORTED"); break;
        case ERROR_TIMEOUT:           TL_LOG_ERROR("Error: ERROR_TIMEOUT"); break;
        case ERROR_FILE_CORRUPT:      TL_LOG_ERROR("Error: ERROR_FILE_CORRUPT"); break;
        case ERROR_FILE_TOO_LARGE:    TL_LOG_ERROR("Error: ERROR_FILE_TOO_LARGE"); break;
        default:                      TL_LOG_ERROR("Error: {}", ::GetLastError()); break;
        }
    }

} // namespace TL::windows
