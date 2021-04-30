cmake_minimum_required(VERSION 3.3)

project(BackdoorApi CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif ()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(inc ${Boost_INCLUDE_DIRS})

add_library(api STATIC
    src/api.cc
)

add_executable(cli
    src/main.cc
    src/cli.cc
)

target_link_libraries(cli api ${Boost_LIBRARIES})

add_custom_target(CopyCompileCommands ALL
    ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
)
