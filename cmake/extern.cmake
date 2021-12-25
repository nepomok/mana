set(Extern.Dir.INCLUDE extern/include/)
set(Extern.Dir.SRC extern/src/)

file(GLOB_RECURSE Extern.File.SRC ${Extern.Dir.SRC}*.cpp ${Extern.Dir.SRC}*.c)

add_library(mana-extern STATIC ${Extern.File.SRC})

target_include_directories(mana-extern PUBLIC extern/include/)
target_include_directories(mana-extern PUBLIC extern/src/)