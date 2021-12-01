# The platform specific global build settings, users can edit this to build on the target platform

# Defaults
set(USER_INCLUDE ${PROJECT_SOURCE_DIR}/lib/include) # Include directories which are appended to target private include directories
set(USER_LINK ${PROJECT_SOURCE_DIR}/lib/bin) # Link directories which are appended to target link directories
set(BOX2D_VERSION 0) # The Box2d version number
set(BOX2D_LIB "box2d") # The Box2d library name

# Debian 11 bullseye
set(USER_INCLUDE ${USER_INCLUDE} /usr/include/freetype2/ /usr/include/mono-2.0/)
set(BOX2D_VERSION 231)
set(BOX2D_LIB "Box2D")
