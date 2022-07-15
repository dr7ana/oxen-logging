#pragma once

#include <spdlog/spdlog.h>
#include "type.hpp"
#include "source_location.hpp"
#include "level.hpp"

namespace oxen::log {

using logger_ptr = std::shared_ptr<spdlog::logger>;

}

namespace oxen::log::detail {

/** internal */

spdlog::sink_ptr make_sink(Type type, const std::string& file);

bool is_ansicolor_sink(const spdlog::sink_ptr& sink);

template <typename... More>
inline static constexpr std::string_view strip_prefixes(
        std::string_view str, std::string_view prefix, More&&... more_prefixes) {
    if (str.substr(0, prefix.size()) == prefix)
        str.remove_prefix(prefix.size());
    if constexpr (sizeof...(More) == 0)
        return str;
    else
        return strip_prefixes(str, std::forward<More>(more_prefixes)...);
}

inline auto as_spdlog_source_loc(const slns::source_location& loc) {
    std::string_view filename{loc.file_name()};
#ifdef OXEN_LOG_SOURCE_ROOT
    filename = strip_prefixes(filename, #OXEN_LOG_SOURCE_ROOT, "..", "/");
#endif

    return spdlog::source_loc{filename.data(), static_cast<int>(loc.line()), loc.function_name()};
}

template <Level level, typename... T>
struct LeveledLogger {
    LeveledLogger(
            const logger_ptr& cat_logger,
            fmt::format_string<T...> fmt,
            T&&... args,
            const slns::source_location& location = slns::source_location::current()) {
#if defined(NDEBUG) && !defined(OXEN_LOGGING_RELEASE_TRACE)
        if constexpr (level == Level::trace)
            return;
        else
#endif
                if (cat_logger)
            cat_logger->log(as_spdlog_source_loc(location), level, fmt, std::forward<T>(args)...);
    }
};

}  // namespace oxen::log::detail
