set(Engine.Dir.INCLUDE engine/include/)
set(Engine.Dir.SRC engine/src/)

file(GLOB_RECURSE Engine.File.SRC ${Engine.Dir.SRC}*.cpp ${Engine.Dir.SRC}*.c)

add_library(mana-engine SHARED ${Engine.File.SRC})

target_include_directories(mana-engine PUBLIC engine/include/)
target_include_directories(mana-engine PUBLIC engine/src/)

target_link_libraries(mana-engine
        mana-platform
        assimp
        sndfile)