set(Extern.Dir.INCLUDE source/extern/include/)
set(Extern.Dir.SRC source/extern/src/)

file(GLOB_RECURSE Extern.File.SRC ${Extern.Dir.SRC}*.cpp ${Extern.Dir.SRC}*.c)

add_library(mana-extern STATIC ${Extern.File.SRC})

target_include_directories(mana-extern PUBLIC ${Extern.Dir.INCLUDE})
target_include_directories(mana-extern PUBLIC ${Extern.Dir.SRC})