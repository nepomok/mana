cmake_minimum_required(VERSION 2.8)

project(mana)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DIR_ENGINE_INCLUDE engine/include/)
set(DIR_ENGINE_SRC engine/src/)

set(DIR_EDITOR_INCLUDE editor/include/)
set(DIR_EDITOR_SRC editor/src/)

set(DIR_RUNTIME_INCLUDE runtime/include/)
set(DIR_RUNTIME_SRC runtime/src/)

file(GLOB_RECURSE ENGINE_SRC ${DIR_ENGINE_SRC}*.cpp ${DIR_ENGINE_SRC}*.c)
file(GLOB_RECURSE EDITOR_SRC ${DIR_EDITOR_SRC}*.cpp ${DIR_EDITOR_SRC}.c)
file(GLOB_RECURSE RUNTIME_SRC ${DIR_RUNTIME_SRC}*.cpp ${DIR_RUNTIME_SRC}*.c)

file(GLOB_RECURSE SAMPLE_SRC sample/src/*.cpp sample/src/*.c)

include_directories(${PROJECT_SOURCE_DIR}/lib/include)
link_directories(${PROJECT_SOURCE_DIR}/lib/bin)

add_library(mana-engine ${ENGINE_SRC})
target_include_directories(mana-engine PUBLIC ${DIR_ENGINE_INCLUDE} ${DIR_ENGINE_SRC})
target_link_libraries(mana-engine glfw GL dl ShaderConductor dxcompiler)

add_executable(mana-editor ${ENGINE_SRC} ${EDITOR_SRC})
target_include_directories(mana-editor PUBLIC ${DIR_ENGINE_INCLUDE} ${DIR_ENGINE_SRC} ${DIR_EDITOR_INCLUDE} )
target_link_libraries(mana-editor glfw GL dl ShaderConductor dxcompiler)

add_executable(mana-runtime ${ENGINE_SRC} ${EDITOR_SRC} ${RUNTIME_SRC})
target_include_directories(mana-runtime PUBLIC ${DIR_ENGINE_INCLUDE} ${DIR_ENGINE_SRC} ${DIR_EDITOR_INCLUDE} ${DIR_RUNTIME_INCLUDE})
target_link_libraries(mana-runtime glfw GL dl ShaderConductor dxcompiler)

add_executable(mana-sample ${ENGINE_SRC} ${SAMPLE_SRC})
target_include_directories(mana-sample PUBLIC ${DIR_ENGINE_INCLUDE} ${DIR_ENGINE_SRC})
target_link_libraries(mana-sample glfw GL dl ShaderConductor dxcompiler)