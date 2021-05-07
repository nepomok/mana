set(Runtime.Dir.INCLUDE runtime/include/)
set(Runtime.Dir.SRC runtime/src/)

file(GLOB_RECURSE Runtime.File.SRC ${Runtime.Dir.SRC}*.cpp ${Runtime.Dir.SRC}*.c)

add_executable(mana-runtime ${Runtime.File.SRC})
target_link_libraries(mana-runtime mana)