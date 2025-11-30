#ifndef LOGGERAPI_H
#define LOGGERAPI_H

#include "logger/src/Logger.h"

#define LOG_INFO    logger::root::Logger( spdlog::level::info,      { __FILE__, __LINE__, SPDLOG_FUNCTION } )
#define LOG_WARN    logger::root::Logger( spdlog::level::warn,      { __FILE__, __LINE__, SPDLOG_FUNCTION } )
#define LOG_ERROR   logger::root::Logger( spdlog::level::err,       { __FILE__, __LINE__, SPDLOG_FUNCTION } )
#define LOG_DEBUG   logger::root::Logger( spdlog::level::debug,     { __FILE__, __LINE__, SPDLOG_FUNCTION } )
#define LOG_TRACE   logger::root::Logger( spdlog::level::trace,     { __FILE__, __LINE__, SPDLOG_FUNCTION } )
#define LOG_CRIT    logger::root::Logger( spdlog::level::critical,  { __FILE__, __LINE__, SPDLOG_FUNCTION } )

#define LOG_INFO_MT logger::root::Logger( spdlog::level::info,      { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )
#define LOG_WARN_MT logger::root::Logger( spdlog::level::warn,      { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )
#define LOG_ERROR_MT logger::root::Logger( spdlog::level::err,      { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )
#define LOG_DEBUG_MT logger::root::Logger( spdlog::level::debug,    { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )
#define LOG_TRACE_MT logger::root::Logger( spdlog::level::trace,    { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )
#define LOG_CRIT_MT  logger::root::Logger( spdlog::level::critical, { __FILE__, __LINE__, SPDLOG_FUNCTION }, true )


namespace logger {
namespace setting {

}
}

#endif // LOGGERAPI_H
