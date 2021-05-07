set(Dir.Engine.INCLUDE engine/include/)
set(Dir.Engine.SRC engine/src/cpp/)

file(GLOB_RECURSE File.Engine.SRC ${Dir.Engine.SRC}*.cpp ${Dir.Engine.SRC}*.c)

add_library(mana STATIC SHARED ${File.Engine.SRC})
target_include_directories(mana PUBLIC ${Dir.Engine.INCLUDE} ${Dir.Engine.SRC})
target_link_libraries(mana glfw GL dl ShaderConductor dxcompiler assimp openal sndfile mono-2.0)