file(GLOB_RECURSE Sample.File.SRC sample/src/*.cpp sample/src/*.c)

add_executable(sample-application ${Sample.File.SRC})

target_include_directories(sample-application PRIVATE sample/src/)
target_link_libraries(sample-application mana)

# Copy mana dll and assets folder to binary dir
file(COPY engine/csharp/mana.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY engine/csharp/lib/Newtonsoft.Json/Newtonsoft.Json.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY sample/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})