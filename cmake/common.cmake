set(Common.Dir.INCLUDE source/common/include/)
set(Common.Dir.SRC source/common/src/)

file(GLOB_RECURSE Common.File.SRC ${Common.Dir.SRC}*.cpp ${Common.Dir.SRC}*.c)

add_library(mana-common STATIC ${Common.File.SRC})

target_include_directories(mana-common PUBLIC ${Common.Dir.INCLUDE})
target_include_directories(mana-common PUBLIC ${Common.Dir.SRC})

target_link_libraries(mana-common mana-extern)