# /engine
The high level engine api which uses the platform module.

## platform
A set of interfaces which abstract platform specific functionality.

When porting the engine to other platforms only the implementations of this set of interfaces should need to be edited.

Currently, a target platform has to support c++17 and build support for the 3rd party libraries. 