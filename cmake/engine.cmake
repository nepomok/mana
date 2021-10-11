add_compile_definitions(BOX2D_VERSION=${BOX2D_VERSION})

set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

add_library(mana SHARED ${Engine.File.SRC})

target_include_directories(mana PUBLIC ${Engine.Dir.INCLUDE})
target_include_directories(mana PRIVATE ${Engine.Dir.SRC})

target_link_libraries(mana
        glfw
        GL
        dl
        assimp
        openal
        sndfile
        Threads::Threads
        freetype
        ${BOX2D_LIB}
        shaderc_shared
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        mono-2.0)
