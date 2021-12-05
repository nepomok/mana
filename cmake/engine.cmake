option(BUILD_ENGINE_DISPLAY_GLFW "Build the glfw display implementation" ON)
option(BUILD_ENGINE_RENDERER_OPENGL "Build the OpenGL render support (Window and Render implementations" ON)
option(BUILD_ENGINE_RENDERER_QT "Build the Qt render support (Window and Render implementations)" ON)
option(BUILD_ENGINE_COMPAT_IMGUI "Build the imgui compatibility and render pass implementations" ON)
option(BUILD_ENGINE_SCRIPT_MONO "Build the mono script implementation" ON)
option(BUILD_ENGINE_PHYSICS_BOX2D "Build the box2d physics implementation" ON)
option(BUILD_ENGINE_AUDIO_OPENAL "Build the OpenAL audio implementation" ON)

add_compile_definitions(BOX2D_VERSION=${BOX2D_VERSION})

set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

add_library(mana SHARED ${Engine.File.SRC})

target_include_directories(mana PUBLIC ${Engine.Dir.INCLUDE})
target_include_directories(mana PUBLIC ${Engine.Dir.SRC})

if (BUILD_ENGINE_COMPAT_IMGUI)
    include(cmake/imgui.cmake)
    add_compile_definitions(BUILD_ENGINE_IM_GUI)
    target_link_libraries(mana imgui) # imgui target provided by lib/imgui.cmake
endif ()

target_link_libraries(mana
        Threads::Threads
        dl
        assimp
        sndfile
        freetype
        shaderc_shared
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl)

if (BUILD_ENGINE_AUDIO_OPENAL)
    add_compile_definitions(BUILD_ENGINE_AUDIO_OPENAL)
    target_link_libraries(mana openal)
endif ()

if (BUILD_ENGINE_DISPLAY_GLFW)
    add_compile_definitions(BUILD_ENGINE_DISPLAY_GLFW)
    target_link_libraries(mana glfw)
endif ()

if (BUILD_ENGINE_RENDERER_OPENGL)
    add_compile_definitions(BUILD_ENGINE_RENDERER_OPENGL)
    target_link_libraries(mana GL)
endif ()

if (BUILD_ENGINE_RENDERER_QT)
    add_compile_definitions(BUILD_ENGINE_RENDERER_QT)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    target_link_libraries(mana Qt5::Core Qt5::Widgets)
endif ()

if (BUILD_ENGINE_COMPAT_IMGUI)
    add_compile_definitions(BUILD_ENGINE_COMPAT_IMGUI)
endif ()

if (BUILD_ENGINE_SCRIPT_MONO)
    add_compile_definitions(BUILD_ENGINE_SCRIPT_MONO)
    target_link_libraries(mana mono-2.0)
endif ()

if (BUILD_ENGINE_PHYSICS_BOX2D)
    add_compile_definitions(BUILD_ENGINE_PHYSICS_BOX2D)
    target_link_libraries(mana ${BOX2D_LIB})
endif ()