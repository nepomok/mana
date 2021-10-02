/**
 *  Mana - 3D Game Engine
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef MANA_SHADERCOMPILER_HPP
#define MANA_SHADERCOMPILER_HPP

#include <map>
#include <string>
#include <functional>
#include <stdexcept>

namespace engine {
    namespace ShaderCompiler {
        enum ShaderStage {
            VERTEX,
            GEOMETRY,
            FRAGMENT
        };

        enum ShaderLanguage {
            HLSL, //HLSL Shader Model 3
            GLSL_460, //GLSL 460 with OpenGL semantics
            GLSL_460_VK, //GLSL 460 with Vulkan semantics
            GLSL_ES_320 //GLSL 320 ES with OpenGL semantics
        };

        /**
         * Compile the given source to spirv.
         *
         * @param source
         * @param entryPoint
         * @param stage
         * @param language
         * @return
         */
        std::vector<uint32_t> compileToSPIRV(const std::string &source,
                                             const std::string &entryPoint,
                                             ShaderStage stage,
                                             ShaderLanguage language);

        std::string decompileSPIRV(const std::vector<uint32_t> &source, ShaderLanguage targetLanguage);

        std::string preprocess(const std::string &source,
                               ShaderStage stage,
                               ShaderLanguage language,
                               const std::function<std::string(const char *)> &include = {},
                               const std::map<std::string, std::string> &macros = {});

        /**
         * Cross compile the source by using spirv as an intermediate.
         *
         * When compiling hlsl to glsl globals defined in the hlsl source are stored in a struct which has an instance
         * with the name "Globals". When using the render allocator to create a shader program the
         * implementation will append the prefix automatically when needed.
         *
         * @param source
         * @param entryPoint
         * @param stage
         * @param language
         * @param targetLanguage
         * @return
         */
        std::string crossCompile(const std::string &source,
                                 const std::string &entryPoint,
                                 ShaderStage stage,
                                 ShaderLanguage language,
                                 ShaderLanguage targetLanguage);
    }
}

#endif //MANA_SHADERCOMPILER_HPP
