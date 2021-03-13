cmake_minimum_required(VERSION 2.8)

project(mana)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EDITOR_INCLUDE editor/include/)
set(ENGINE_INCLUDE engine/include/)
set(RUNTIME_INCLUDE runtime/include/)

file(GLOB_RECURSE EDITOR_SRC editor/src/*.cpp editor/src/*.c)
file(GLOB_RECURSE ENGINE_SRC engine/src/*.cpp engine/src/*.c)
file(GLOB_RECURSE RUNTIME_SRC runtime/src/*.cpp runtime/src/*.c)
file(GLOB_RECURSE SAMPLE_SRC sample/src/*.cpp sample/src/*.c)

add_library(mana-engine ${ENGINE_SRC})
target_include_directories(mana-engine PUBLIC ${ENGINE_INCLUDE})
target_link_libraries(mana-engine glfw GL dl)

add_executable(mana-editor ${EDITOR_SRC} ${ENGINE_SRC})
target_include_directories(mana-editor PUBLIC ${EDITOR_INCLUDE} ${ENGINE_INCLUDE})
target_link_libraries(mana-editor glfw GL dl)

add_executable(mana-runtime ${RUNTIME_SRC} ${EDITOR_SRC} ${ENGINE_SRC})
target_include_directories(mana-runtime PUBLIC ${RUNTIME_INCLUDE} ${EDITOR_INCLUDE} ${ENGINE_INCLUDE})
target_link_libraries(mana-runtime glfw GL dl)

add_executable(mana-sample ${RUNTIME_SRC} ${EDITOR_SRC} ${ENGINE_SRC} ${SAMPLE_SRC})
target_include_directories(mana-sample PUBLIC ${RUNTIME_INCLUDE} ${EDITOR_INCLUDE} ${ENGINE_INCLUDE})
target_link_libraries(mana-sample glfw GL dl)