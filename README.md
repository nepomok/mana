# Mana
A 3D+2D game engine written in C++.

## Directories
### /engine
The engine provides the low level api for rendering, sound, input, etc.

It can be used independently of the runtime and editor.

### /runtime
The runtime extends the engine api with a higher level interface in the form of an ecs. It also defines abstractions around scripting and defines the io schema.

### /editor
The editor provides a toolset for handling projects using the the runtime interface.