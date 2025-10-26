cmake_minimum_required( VERSION 3.16 )
project( logger LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_subdirectory( spdlog EXCLUDE_FROM_ALL )

add_library( logger INTERFACE )
add_library( Logger::logger ALIAS logger )

target_include_directories( logger
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/api>
        $<INSTALL_INTERFACE:api>
)

target_link_libraries( logger
    INTERFACE
        spdlog::spdlog_header_only
)
