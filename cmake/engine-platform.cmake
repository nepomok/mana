option(BUILD_ENGINE_DISPLAY_GLFW "Build the glfw display implementation" ON)
option(BUILD_ENGINE_RENDERER_OPENGL "Build the OpenGL render support (Window and Render implementations" ON)
option(BUILD_ENGINE_RENDERER_QT "Build the Qt render support (Window and Render implementations)" ON)
option(BUILD_ENGINE_SCRIPT_MONO "Build the mono script implementation" ON)
option(BUILD_ENGINE_PHYSICS_BOX2D "Build the box2d physics implementation" ON)
option(BUILD_ENGINE_AUDIO_OPENAL "Build the OpenAL audio implementation" ON)

set(Engine.Platform.Dir.INCLUDE engine/include/)
set(Engine.Platform.Dir.SRC engine/src/platform/)

file(GLOB_RECURSE Engine.Platform.File.SRC ${Engine.Platform.Dir.SRC}*.cpp ${Engine.Platform.Dir.SRC}*.c engine/src/common/*.cpp engine/src/common/*.c)

add_library(mana-engine-platform STATIC ${Engine.Platform.File.SRC})

target_include_directories(mana-engine-platform PUBLIC engine/include/)
target_include_directories(mana-engine-platform PUBLIC engine/src/)

if (BUILD_ENGINE_SCRIPT_MONO)
    add_compile_definitions(BUILD_ENGINE_SCRIPT_MONO)
    target_link_libraries(mana-engine-platform mono-2.0)
endif ()

if (BUILD_ENGINE_PHYSICS_BOX2D)
    add_compile_definitions(BUILD_ENGINE_PHYSICS_BOX2D)
    target_link_libraries(mana-engine-platform ${BOX2D_LIB})
endif ()

if (BUILD_ENGINE_AUDIO_OPENAL)
    add_compile_definitions(BUILD_ENGINE_AUDIO_OPENAL)
    target_link_libraries(mana-engine-platform openal)
endif ()

if (BUILD_ENGINE_DISPLAY_GLFW)
    add_compile_definitions(BUILD_ENGINE_DISPLAY_GLFW)
    target_link_libraries(mana-engine-platform glfw)
endif ()

if (BUILD_ENGINE_RENDERER_OPENGL)
    add_compile_definitions(BUILD_ENGINE_RENDERER_OPENGL)
    target_link_libraries(mana-engine-platform GL)
endif ()

if (BUILD_ENGINE_RENDERER_QT)
    add_compile_definitions(BUILD_ENGINE_RENDERER_QT)
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Widgets REQUIRED)
    target_link_libraries(mana-engine-platform Qt5::Core Qt5::Widgets)
endif ()