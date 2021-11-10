cmake_minimum_required(VERSION 2.8)

project(Mana)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS_RELEASE -O3)
endif()

find_package (Threads REQUIRED)

include(cmake/config.cmake)
include_directories(${USER_INCLUDE})
link_directories(${USER_LINK})

include(cmake/imgui.cmake)

include(cmake/engine.cmake)
include(cmake/editor.cmake)
include(cmake/sample.cmake)