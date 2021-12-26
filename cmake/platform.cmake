option(BUILD_ENGINE_DISPLAY_GLFW "Build the glfw display implementation" ON)
option(BUILD_ENGINE_RENDERER_OPENGL "Build the OpenGL render support (Window and Render implementations" ON)
option(BUILD_ENGINE_RENDERER_QT "Build the Qt render support (Window and Render implementations)" ON)
option(BUILD_ENGINE_SCRIPT_MONO "Build the mono script implementation" ON)
option(BUILD_ENGINE_PHYSICS_BOX2D "Build the box2d physics implementation" ON)
option(BUILD_ENGINE_AUDIO_OPENAL "Build the OpenAL audio implementation" ON)

set(Platform.Dir.INCLUDE platform/include/)
set(Platform.Dir.SRC platform/src/)

file(GLOB_RECURSE Platform.File.SRC ${Platform.Dir.SRC}*.cpp ${Platform.Dir.SRC}*.c)

add_library(mana-platform STATIC ${Platform.File.SRC})

target_include_directories(mana-platform PUBLIC platform/include/)
target_include_directories(mana-platform PUBLIC platform/src/)

target_link_libraries(mana-platform
        mana-common
        Threads::Threads
        freetype
        shaderc_shared
        spirv-cross-core
        spirv-cross-glsl
        spirv-cross-hlsl
        imgui
        implot)

if (BUILD_ENGINE_SCRIPT_MONO)
    add_compile_definitions(BUILD_ENGINE_SCRIPT_MONO)
    target_link_libraries(mana-platform mono-2.0)
endif ()

if (BUILD_ENGINE_PHYSICS_BOX2D)
    add_compile_definitions(BOX2D_VERSION=${BOX2D_VERSION})
    add_compile_definitions(BUILD_ENGINE_PHYSICS_BOX2D)
    target_link_libraries(mana-platform ${BOX2D_LIB})
endif ()

if (BUILD_ENGINE_AUDIO_OPENAL)
    add_compile_definitions(BUILD_ENGINE_AUDIO_OPENAL)
    target_link_libraries(mana-platform openal)
endif ()

if (BUILD_ENGINE_DISPLAY_GLFW)
    add_compile_definitions(BUILD_ENGINE_DISPLAY_GLFW)
    target_link_libraries(mana-platform glfw)
endif ()

if (BUILD_ENGINE_RENDERER_OPENGL)
    add_compile_definitions(BUILD_ENGINE_RENDERER_OPENGL)
    target_link_libraries(mana-platform GL)
endif ()

if (BUILD_ENGINE_RENDERER_QT)
    add_compile_definitions(BUILD_ENGINE_RENDERER_QT)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    target_link_libraries(mana-platform Qt5::Core Qt5::Widgets)
endif ()