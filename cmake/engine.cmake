include(cmake/engine-platform.cmake)

option(BUILD_ENGINE_COMPAT_IMGUI "Build imgui and the imgui compat implementation" ON)

add_compile_definitions(BOX2D_VERSION=${BOX2D_VERSION})

set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/engine/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

set(Engine.Dir.SRC engine/src/engine/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c engine/src/common/*.cpp engine/src/common/*.c)

add_library(mana-engine SHARED ${Engine.File.SRC})

target_include_directories(mana-engine PUBLIC engine/include/)
target_include_directories(mana-engine PUBLIC engine/src/)

if (BUILD_ENGINE_COMPAT_IMGUI)
    include(cmake/imgui.cmake)
    add_compile_definitions(BUILD_ENGINE_IM_GUI)
    target_link_libraries(mana-engine imgui) # imgui target provided by imgui.cmake
endif ()

target_link_libraries(mana-engine
        Threads::Threads
        dl
        assimp
        sndfile
        freetype
        shaderc_shared
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl)

if (BUILD_ENGINE_COMPAT_IMGUI)
    add_compile_definitions(BUILD_ENGINE_COMPAT_IMGUI)
endif ()

target_link_libraries(mana-engine mana-engine-platform)