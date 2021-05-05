set(DIR_ENGINE_INCLUDE engine/include/)
set(DIR_ENGINE_SRC engine/src/cpp/)

file(GLOB_RECURSE ENGINE_SRC ${DIR_ENGINE_SRC}*.cpp ${DIR_ENGINE_SRC}*.c)

add_library(mana STATIC SHARED ${ENGINE_SRC})
target_include_directories(mana PUBLIC ${DIR_ENGINE_INCLUDE} ${DIR_ENGINE_SRC})
target_link_libraries(mana glfw GL dl ShaderConductor dxcompiler assimp openal sndfile mono-2.0)