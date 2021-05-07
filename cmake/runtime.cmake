set(Dir.Runtime.INCLUDE runtime/include/)
set(Dir.Runtime.SRC runtime/src/)

file(GLOB_RECURSE File.Runtime.SRC ${Dir.Runtime.SRC}*.cpp ${Dir.Runtime.SRC}*.c)

add_executable(mana-runtime ${File.Runtime.SRC})
target_link_libraries(mana-runtime mana)