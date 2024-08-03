#include "TL/Log.hpp"
#include "TL/Assert.hpp"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace TL
{
    namespace Logger
    {
        class Stream
        {
        public:
            // spdlog::logger logger;
        };

        Stream& GetStdCoutStream()
        {
            // spdlog::error("Logging throw custom stream is not implemented yet");
            // TL_UNREACHABLE();
            static Stream stream = Stream();
            return stream;
        }

        void Log([[maybe_unused]] Stream& stream, Level level, const char* message)
        {
            switch (level)
            {
            case Level::None:     TL_UNREACHABLE(); break;
            case Level::Debug:    spdlog::debug(message); break;
            case Level::Info:     spdlog::info(message); break;
            case Level::Warnning: spdlog::warn(message); break;
            case Level::Error:    spdlog::error(message); break;
            }
        }
    } // namespace Logger
} // namespace TL