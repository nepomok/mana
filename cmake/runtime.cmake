set(DIR_RUNTIME_INCLUDE runtime/include/)
set(DIR_RUNTIME_SRC runtime/src/)

file(GLOB_RECURSE RUNTIME_SRC ${DIR_RUNTIME_SRC}*.cpp ${DIR_RUNTIME_SRC}*.c)

add_executable(mana-runtime ${RUNTIME_SRC})
target_link_libraries(mana-runtime mana)