#pragma once

#include "TL/Stream.hpp"
#include "TL/Containers/StringView.hpp"

namespace TL
{
    enum class IOMode
    {
        Read,
        Write,
        Append,
        Overwrite,
    };

    class File : public IStream
    {
    public:

        // remove ctor/dtor (make them default)
        File();
        File(StringView path, IOMode mode);
        ~File();

        IOResultCode open(StringView path, IOMode mode);

        void         close();

        size_t       size() const override;

        size_t       position() const override;

        IOResult     read(Block block, uint64_t offset = 0) override;

        IOResult     read(String& string, uint64_t offset = 0) override;

        IOResult     write(Block block, uint64_t offset = 0) override;

        IOResult     write(String string, uint64_t offset = 0) override;

    private:
        void* m_handle = nullptr;
    };

    class MMFile : public File
    {
    public:
        static Result<MMFile, IOResult> open(TL::StringView path);
        void                            close(MMFile file);

        size_t       size() const override;
        size_t       position() const override;

        IOResult     read(Block block, uint64_t offset = 0) override;

        IOResult     read(String& string, uint64_t offset = 0) override;

        IOResult     write(Block block, uint64_t offset = 0) override;

        IOResult     write(String string, uint64_t offset = 0) override;

    private:
        void* m_impl;
    };

} // namespace TL