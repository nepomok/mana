set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/)

# Freetype include path has to be set manually because for some reason freetype in the common repositories is in a
# freetype2 subfolder which breaks everything. On unix we assume the location is /usr/include/freetype2 if your distro
# uses a different path you will have to set FREETYPE_INCLUDE to the correct path here.
if (UNIX)
    set(FREETYPE_INCLUDE /usr/include/freetype2/)
elseif (WIN32)
    message(WARNING "Freetype include path has to be set manually on win32, please set \"FREETYPE_INCLUDE\"")
endif ()

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

add_library(mana SHARED ${Engine.File.SRC})

target_include_directories(mana PUBLIC ${Engine.Dir.INCLUDE})
target_include_directories(mana PRIVATE ${Engine.Dir.SRC} ${FREETYPE_INCLUDE})

target_link_libraries(mana
        glfw
        GL
        dl
        assimp
        openal
        sndfile
        Threads::Threads
        freetype
        box2d
        shaderc_shared
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        mono-2.0)
