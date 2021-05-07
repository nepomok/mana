file(GLOB_RECURSE File.Sample.SRC sample/src/*.cpp sample/src/*.c)

add_executable(mana-sample ${File.Sample.SRC})
target_link_libraries(mana-sample mana)