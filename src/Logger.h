#ifndef LOGGER_H
#define LOGGER_H

#include <type_traits>
#include <string_view>
#include <sstream>
#include <memory>
#include <cstdio>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

#ifdef QT_CORE_LIB
#include <QString>
#endif

namespace logger {
namespace root {

template< typename T >
concept StreamableValueContainer = requires ( std::ostream & ostream, T & t )
{
    { t.begin() } -> std::input_or_output_iterator;
    { t.end() }   -> std::input_or_output_iterator;
    { ostream << *( t.begin() ) } -> std::convertible_to< std::ostream & >;
    requires std::same_as< decltype( t.end() ), decltype( t.begin() ) > ||
                 std::sentinel_for< decltype( t.end() ), decltype( t.begin() ) >;
} ||
requires ( std::ostream & ostream, const T & t )
{
    { t.begin() } -> std::input_or_output_iterator;
    { t.end() };
    { ostream << *( t.begin() ) } -> std::convertible_to< std::ostream & >;
    requires std::same_as< decltype( t.end() ), decltype( t.begin() ) > ||
                 std::sentinel_for< decltype( t.end() ), decltype( t.begin() ) >;
};

class Logger
{
public:

    static void init()
    {
        if( initialized_ ) return;

        constexpr std::string_view defaultPattern = "[%H:%M:%S.%e %s:%#][%^%l%$] %v";
        constexpr std::string_view defaultLogFile = "logs/app.log";
        try
        {
            auto consoleSink = std::make_shared< spdlog::sinks::stdout_color_sink_mt >();
            consoleSink->set_pattern( defaultPattern.data() );

            const std::string logFile = logFile_.empty() ? std::string( defaultLogFile ) : logFile_;
            auto fileSink = std::make_shared< spdlog::sinks::basic_file_sink_mt >( logFile, true );
            fileSink->set_pattern( defaultPattern.data() );

            spdlog::sinks_init_list sinkList{ consoleSink, fileSink };

            stLogger_ = std::make_shared< spdlog::logger >( "st_logger", sinkList.begin(), sinkList.end());
            stLogger_->set_level( spdlog::level::trace );

            mtLogger_ = std::make_shared< spdlog::logger >( "mt_logger", sinkList.begin(), sinkList.end() );
            mtLogger_->set_level( spdlog::level::trace );
            initialized_ = true;
        }
        catch( const std::exception & e )
        {
            std::fprintf( stderr, "Exception during logger initialization: %s\n", e.what() );
        }
        catch (...)
        {
            std::fprintf( stderr, "Unknown exception during logger initialization\n" );
        }
    }

    static void setLogFile( const std::string & file )
    {
        if( initialized_ )
        {
            std::fprintf( stderr, "Attempt to change log file after logger initialization\n" );
            return;
        }
        logFile_ = file;
    }

    template < typename T >
    Logger & operator<<( T && value )
    {
        using U = std::remove_cvref_t< T >;

        if constexpr ( std::is_same_v< U, bool > )
        {
            stream_ << ( value ? "true" : "false" );
            return *this;
        }

        if constexpr ( requires( std::ostream & os, const U & x ) { os << x; } )
        {
            stream_ << value;
            return *this;
        }

        if constexpr ( StreamableValueContainer< U > )
        {
            stream_ << "Container: [";

            auto it = value.begin();
            const auto end = value.end();

            if ( it != end )
            {
                stream_ << *it;
                ++it;
                for ( ; it != end; ++it )
                {
                    stream_ << ", " << *it;
                }
            }

            stream_ << "]";
            return *this;
        }

#ifdef QT_CORE_LIB
        if constexpr ( std::is_same_v< U, QString > )
        {
            stream_ << value.toStdString();
            return *this;
        }
#endif
        stream_ << "[unsupported type]";
        return *this;
    }

    explicit Logger( spdlog::level::level_enum lvl, spdlog::source_loc loc, bool useMt = false ) : level_( lvl ), loc_( loc ), useMt_( useMt ) {}

    virtual ~Logger()
    {
        try
        {
            if( !initialized_ )
            {
                Logger::init();
            }

            std::shared_ptr< spdlog::logger > & targetLogger = useMt_ ? mtLogger_ : stLogger_;
            if ( !targetLogger )
            {
                std::fprintf( stderr, "Logger not initialized\n" );
                return;
            }

            targetLogger->log( loc_, level_, stream_.str() );
            targetLogger->flush();
        }
        catch( const std::exception & e )
        {
            std::fprintf( stderr, "Exception during logging: %s\n", e.what() );
        }
        catch( ... )
        {
            std::fprintf( stderr, "Unknown exception during logging\n" );
        }
    }

private:

    inline static bool initialized_ = false;
    inline static std::shared_ptr< spdlog::logger > stLogger_;
    inline static std::shared_ptr< spdlog::logger > mtLogger_;
    inline static std::string logFile_;

    spdlog::level::level_enum level_;
    spdlog::source_loc loc_;
    std::ostringstream stream_;
    bool useMt_;
};

} // root
} // logger

#endif // LOGGER_H
