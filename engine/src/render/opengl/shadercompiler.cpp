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

#include "render/opengl/shadercompiler.hpp"

#include <stdexcept>

#include "shaderc/shaderc.hpp"

#include "spirv_glsl.hpp"
#include "spirv_hlsl.hpp"

namespace engine {
    class IncludeHandler : public shaderc::CompileOptions::IncluderInterface {
    public:
        std::function<std::string(const char *)> callback;

        IncludeHandler() = default;

        explicit IncludeHandler(std::function<std::string(const char *)> callback)
                : callback(std::move(callback)) {}

        shaderc_include_result *GetInclude(const char *requested_source,
                                           shaderc_include_type type,
                                           const char *requesting_source,
                                           size_t include_depth) override {
            auto *ret = new shaderc_include_result();
            auto userData = new std::pair<std::string, std::string>(std::string(callback(requested_source)),
                                                                    std::string(requested_source));
            ret->user_data = userData;
            ret->content = userData->first.c_str();
            ret->content_length = userData->first.length();
            ret->source_name = userData->second.c_str();
            ret->source_name_length = userData->second.length();

            return ret;
        }

        void ReleaseInclude(shaderc_include_result *data) override {
            auto *pair = static_cast<std::pair<std::string, std::string> *>(data->user_data);
            delete pair;
            delete data;
        }
    };

    std::vector<uint32_t> compileToSPIRV(const std::string &source,
                                         const std::string &entryPoint,
                                         ShaderCompiler::ShaderStage stage,
                                         ShaderCompiler::ShaderLanguage sourceLanguage,
                                         const std::function<std::string(const char *)> &includeCallback,
                                         const std::map<std::string, std::string> &macros) {
        auto shaderStage = stage == ShaderCompiler::VERTEX
                           ? shaderc_vertex_shader
                           : shaderc_fragment_shader;
        auto shaderLang = sourceLanguage == ShaderCompiler::GLSL
                          ? shaderc_source_language_glsl
                          : shaderc_source_language_hlsl;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;

        for (auto &p : macros)
            options.AddMacroDefinition(p.first, p.second);

        options.SetIncluder(std::make_unique<IncludeHandler>(includeCallback));
        options.SetSourceLanguage(shaderLang);
        options.SetAutoBindUniforms(true);
        options.SetAutoSampledTextures(true);
        options.SetAutoMapLocations(true);
        options.SetOptimizationLevel(shaderc_optimization_level_zero);

        auto preProcessResult = compiler.PreprocessGlsl(source,
                                                        shaderStage,
                                                        "",
                                                        options);

        if (preProcessResult.GetCompilationStatus() != shaderc_compilation_status_success) {
            throw std::runtime_error("Failed to preprocess glsl: " + preProcessResult.GetErrorMessage());
        }

        std::string processed{preProcessResult.cbegin(), preProcessResult.cend()};

        auto compileResult = compiler.CompileGlslToSpv(processed,
                                                       shaderStage,
                                                       "",
                                                       entryPoint.c_str(),
                                                       options);

        if (compileResult.GetCompilationStatus() != shaderc_compilation_status_success) {
            throw std::runtime_error("Failed to compile glsl " + compileResult.GetErrorMessage());
        }

        return {compileResult.cbegin(), compileResult.cend()};
    }

    std::string crossCompile(const std::string &source,
                             const std::string &entryPoint,
                             ShaderCompiler::ShaderLanguage sourceLanguage,
                             ShaderCompiler::ShaderLanguage targetLanguage,
                             ShaderCompiler::ShaderStage stage,
                             const std::function<std::string(const char *)> &includeCallback,
                             const std::map<std::string, std::string> &macros) {
        auto spirv = compileToSPIRV(source, entryPoint, stage, sourceLanguage, includeCallback, macros);

        if (targetLanguage == ShaderCompiler::GLSL) {
            spirv_cross::CompilerGLSL sCompiler(spirv);

            spirv_cross::ShaderResources resources = sCompiler.get_shader_resources();

            // Set the first uniform buffer name to "Globals" because shaderc
            // merges all uniform variables declared in the source into a single uniform buffer in the SPIRV.
            if (!resources.uniform_buffers.empty())
                sCompiler.set_name(resources.uniform_buffers[0].id, "Globals");

            spirv_cross::CompilerGLSL::Options sOptions;
            sOptions.version = 320;

            //Dont generate glsl which uses the uniform buffer api.
            sOptions.emit_uniform_buffer_as_plain_uniforms = true;

            //Either setting separate shader objects or OpenGL ES causes spirv to generate the required
            //layout specifiers. For portability all shaders will be required to be OpenGL ES supported.
            sOptions.es = true;
            sOptions.separate_shader_objects = false;

            sCompiler.set_common_options(sOptions);

            return sCompiler.compile();
        } else if (targetLanguage == ShaderCompiler::HLSL) {
            spirv_cross::CompilerHLSL sCompiler(spirv);

            spirv_cross::ShaderResources resources = sCompiler.get_shader_resources();

            // Set the first uniform buffer name to "Globals" because shaderc
            // merges all uniform variables declared in the source into a single uniform buffer in the SPIRV.
            if (!resources.uniform_buffers.empty())
                sCompiler.set_name(resources.uniform_buffers[0].id, "Globals");

            spirv_cross::CompilerGLSL::Options sOptions;
            sCompiler.set_common_options(sOptions);

            return sCompiler.compile();
        } else {
            throw std::runtime_error("");
        }
    }

    std::string ShaderCompiler::compileGlslToHlsl(const std::string &source,
                                                  ShaderCompiler::ShaderStage stage,
                                                  const std::function<std::string(const char *)> &includeCallback,
                                                  const std::map<std::string, std::string> &macros) {
        return crossCompile(source, "", GLSL, HLSL, stage, includeCallback, macros);
    }

    std::string ShaderCompiler::compileHlslToGlsl(const std::string &source,
                                                  const std::string &entryPoint,
                                                  ShaderCompiler::ShaderStage stage,
                                                  const std::function<std::string(const char *)> &includeCallback,
                                                  const std::map<std::string, std::string> &macros) {
        return crossCompile(source, entryPoint, HLSL, GLSL, stage, includeCallback, macros);
    }

    std::vector<uint32_t> ShaderCompiler::compileGlslToSPIRV(const std::string &source,
                                                             ShaderCompiler::ShaderStage stage,
                                                             const std::function<std::string(const char *)> &include,
                                                             const std::map<std::string, std::string> &macros) {
        return compileToSPIRV(source, "", stage, GLSL, include, macros);
    }

    std::vector<uint32_t> ShaderCompiler::compileHlslToSPIRV(const std::string &source,
                                                             const std::string &entryPoint,
                                                             ShaderCompiler::ShaderStage stage,
                                                             const std::function<std::string(const char *)> &include,
                                                             const std::map<std::string, std::string> &macros) {
        return compileToSPIRV(source, entryPoint, stage, HLSL, include, macros);
    }
}