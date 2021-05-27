set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/cpp/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

add_library(mana STATIC SHARED ${Engine.File.SRC})
target_include_directories(mana PUBLIC ${Engine.Dir.INCLUDE} ${Engine.Dir.SRC})
target_link_libraries(mana glfw GL dl ShaderConductor dxcompiler assimp openal sndfile mono-2.0 Threads::Threads)