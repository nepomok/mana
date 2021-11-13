# Dependencies

### Engine
- OpenGL 4.6
- [GLM](https://github.com/g-truc/glm)
- [GLFW](https://github.com/glfw/glfw)
- [AssImp](https://github.com/assimp/assimp)
- [OpenAL-Soft](https://github.com/kcat/openal-soft)
- [libsndfile](https://github.com/libsndfile/libsndfile)
- [Freetype2](https://freetype.org/)
- [Box2D](https://box2d.org/)
- [shaderc](https://github.com/google/shaderc)
- [SPIRV-Cross](https://github.com/KhronosGroup/SPIRV-Cross)
- [Mono](https://github.com/mono/mono)

### Editor
- Qt 5

# How to build
### Engine
1. Install the dependencies ([install_dependencies.sh](install_dependencies.sh))
2. Initialize the ImGui git submodule
3. Build the "mana" cmake target

### Sample
1. Build the "sample-application" cmake target