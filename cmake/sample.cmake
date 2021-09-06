file(GLOB_RECURSE Sample.File.SRC sample/src/*.cpp sample/src/*.c)

add_executable(mana-sample ${Sample.File.SRC})

target_link_libraries(mana-sample mana-engine mana-runtime)

# Copy mana dll and assets folder to binary dir
file(COPY runtime/csharp/mana.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY runtime/csharp/lib/Newtonsoft.Json/Newtonsoft.Json.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sample/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})