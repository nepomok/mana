# Mana
A 3D+2D game engine written in C++.

## Directories
### /engine
The engine provides the api for rendering, sound, input, etc.

### /editor
The editor provides a gui for handling scenes in json format.

### /runtime
The runtime defines a packaged scene format for the json scenes and referenced assets 
and defines the code which executes the packaged format. 
The packaged format includes all asset data in compressed and optionally encrypted form.
