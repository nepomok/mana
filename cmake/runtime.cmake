set(Runtime.Dir.INCLUDE runtime/include/)
set(Runtime.Dir.SRC runtime/src/)

file(GLOB_RECURSE Runtime.File.SRC ${Runtime.Dir.SRC}*.cpp ${Runtime.Dir.SRC}*.c)

add_library(mana-runtime SHARED ${Runtime.File.SRC})

target_include_directories(mana-runtime PUBLIC ${Runtime.Dir.INCLUDE})
target_include_directories(mana-runtime PRIVATE ${Runtime.Dir.SRC})

target_link_libraries(mana-runtime mana-engine mono-2.0)