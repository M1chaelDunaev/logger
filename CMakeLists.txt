cmake_minimum_required( VERSION 3.16 )
project( logger LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_library( logger INTERFACE )
add_library( Logger::logger ALIAS logger )

target_include_directories( logger
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/api>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/spdlog>
        $<INSTALL_INTERFACE:api>
)