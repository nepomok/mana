file(GLOB_RECURSE Sample.File.SRC sample/src/*.cpp sample/src/*.c)

add_executable(mana-sample ${Sample.File.SRC})
target_link_libraries(mana-sample mana)