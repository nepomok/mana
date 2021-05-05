file(GLOB_RECURSE SAMPLE_SRC sample/src/*.cpp sample/src/*.c)

add_executable(mana-sample ${SAMPLE_SRC})
target_link_libraries(mana-sample mana)