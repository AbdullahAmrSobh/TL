#pragma once

namespace TL::FS
{
    struct FileSystem
    {
        bool   preciseErrorMessages = false;

        Result init(StringSpan initialDirectory);

        Result changeDirectory(StringSpan newDirectory);

        using CopyFlags = FileSystemCopyFlags;

        struct CopyOperation
        {
            StringSpan source;
            StringSpan destination;
            CopyFlags  copyFlags;
        };


        Result copyFiles(Span<const CopyOperation> sourceDestination);


        Result copyFile(StringSpan source, StringSpan destination, CopyFlags copyFlags = CopyFlags())
        {
            return copyFiles(CopyOperation{source, destination, copyFlags});
        }

        Result copyDirectories(Span<const CopyOperation> sourceDestination);


        Result copyDirectory(StringSpan source, StringSpan destination, CopyFlags copyFlags = CopyFlags())
        {
            return copyDirectories(CopyOperation{source, destination, copyFlags});
        }


        Result             rename(StringSpan path, StringSpan newPath);

        Result             removeFiles(Span<const StringSpan> files);

        Result             removeFile(StringSpan source) { return removeFiles({source}); }

        Result             removeFileIfExists(StringSpan source);

        Result             removeLinkIfExists(StringSpan source);

        Result             removeDirectoriesRecursive(Span<const StringSpan> directories);


        Result             removeDirectoryRecursive(StringSpan directory) { return removeDirectoriesRecursive({directory}); }

        Result             removeEmptyDirectories(Span<const StringSpan> directories);

        Result             removeEmptyDirectory(StringSpan directory) { return removeEmptyDirectories({directory}); }

        Result             makeDirectories(Span<const StringSpan> directories);

        Result             makeDirectory(StringSpan directory) { return makeDirectories({directory}); }

        Result             makeDirectoriesIfNotExists(Span<const StringSpan> directories);

        Result             makeDirectoryIfNotExists(StringSpan directory) { return makeDirectoriesIfNotExists({directory}); }

        Result             makeDirectoriesRecursive(Span<const StringSpan> directories);

        Result             makeDirectoryRecursive(StringSpan directory) { return makeDirectoriesRecursive({directory}); }

        Result             createSymbolicLink(StringSpan sourceFileOrDirectory, StringSpan linkFile);

        [[nodiscard]] bool exists(StringSpan fileOrDirectory);

        [[nodiscard]] bool existsAndIsDirectory(StringSpan directory);


        [[nodiscard]] bool existsAndIsFile(StringSpan file);

        [[nodiscard]] bool existsAndIsLink(StringSpan file);

        [[nodiscard]] bool moveDirectory(StringSpan sourceDirectory, StringSpan destinationDirectory);


        Result             write(StringSpan file, Span<const char> data);
        Result             write(StringSpan file, Span<const uint8_t> data);

        Result             writeString(StringSpan file, StringSpan text);

        Result             writeStringAppend(StringSpan file, StringSpan text);

        template<typename T>
        Result read(StringSpan file, T& data)
        {
            FileDescriptor fd;
            StringSpan     encodedPath;
            SC_TRY(convert(file, fileFormatBuffer1, &encodedPath))
            SC_TRY(fd.open(encodedPath, FileOpen::Read));
            return fd.readUntilEOF(data);
        }

        using FileStat = FileSystemStat;

        Result getFileStat(StringSpan file, FileStat& fileStat);

        Result setLastModifiedTime(StringSpan file, Time::Realtime time);

        struct SC_COMPILER_EXPORT Operations
        {
            static Result     createSymbolicLink(StringSpan sourceFileOrDirectory, StringSpan linkFile);
            static Result     makeDirectory(StringSpan dir);
            static Result     exists(StringSpan path);
            static Result     existsAndIsDirectory(StringSpan path);
            static Result     existsAndIsFile(StringSpan path);
            static Result     existsAndIsLink(StringSpan path);
            static Result     makeDirectoryRecursive(StringSpan path);
            static Result     removeEmptyDirectory(StringSpan path);
            static Result     moveDirectory(StringSpan source, StringSpan destination);
            static Result     removeFile(StringSpan path);
            static Result     copyFile(StringSpan srcPath, StringSpan destPath, FileSystemCopyFlags flags);
            static Result     rename(StringSpan path, StringSpan newPath);
            static Result     copyDirectory(StringSpan srcPath, StringSpan destPath, FileSystemCopyFlags flags);
            static Result     removeDirectoryRecursive(StringSpan directory);
            static Result     getFileStat(StringSpan path, FileSystemStat& fileStat);
            static Result     setLastModifiedTime(StringSpan path, Time::Realtime time);

            static StringSpan getExecutablePath(StringPath& executablePath);
            static StringSpan getApplicationRootDirectory(StringPath& applicationRootDirectory);

        private:
            struct Internal;
        };

    private:
        [[nodiscard]] bool convert(const StringSpan file, StringPath& destination, StringSpan* encodedPath = nullptr);

        StringPath         fileFormatBuffer1;
        StringPath         fileFormatBuffer2;
        StringPath         currentDirectory;

        char               errorMessageBuffer[256] = {0};

        Result             formatError(int errorNumber, StringSpan item, bool isWindowsNativeError);
        struct Internal;
    };
} // namespace TL::FS