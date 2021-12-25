#ifndef MANA_SHADERLANGUAGE_HPP
#define MANA_SHADERLANGUAGE_HPP

namespace engine {
    enum ShaderLanguage {
        HLSL_SHADER_MODEL_4,
        GLSL_460, //GLSL 460 with OpenGL semantics
        GLSL_460_VK, //GLSL 460 with Vulkan semantics
        GLSL_ES_320 //GLSL 320 ES with OpenGL semantics
    };
}

#endif //MANA_SHADERLANGUAGE_HPP
