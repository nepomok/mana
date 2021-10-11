# The platform specific build settings, users can edit this to build on the target platform

# Defaults
set(USER_INCLUDE ${PROJECT_SOURCE_DIR}/lib/include)
set(USER_LINK ${PROJECT_SOURCE_DIR}/lib/bin)
set(BOX2D_VERSION 0)
set(BOX2D_LIB "box2d")

# Debian 11 bullseye
set(USER_INCLUDE ${USER_INCLUDE} /usr/include/freetype2/ /usr/include/mono-2.0/)
set(BOX2D_VERSION 231)
set(BOX2D_LIB "Box2D")
