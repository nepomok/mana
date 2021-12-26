set(Common.Dir.INCLUDE common/include/)
set(Common.Dir.SRC common/src/)

file(GLOB_RECURSE Common.File.SRC ${Common.Dir.SRC}*.cpp ${Common.Dir.SRC}*.c)

add_library(mana-common STATIC ${Common.File.SRC})

target_include_directories(mana-common PUBLIC common/include/)
target_include_directories(mana-common PUBLIC common/src/)

target_link_libraries(mana-common mana-extern)