file(GLOB_RECURSE Sample.File.SRC sample/src/*.cpp sample/src/*.c)

add_executable(mana-sample ${Sample.File.SRC})
target_link_libraries(mana-sample mana)

# Copy mana dll and assets folder to binary dir
file(COPY engine/src/csharp/mana.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sample/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})