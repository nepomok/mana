/**
 *  Mana - 3D Engine
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

#include "render/opengl/hlslcrosscompiler.hpp"

#include <stdexcept>

#include <ShaderConductor/ShaderConductor.hpp>

std::string mana::HlslCrossCompiler::compileVertexShader(const std::string &source,
                                                         const std::string &entryPoint,
                                                         const std::map<std::string, std::string> &macros) {
    ShaderConductor::Compiler::SourceDesc sourceDesc{};
    ShaderConductor::Compiler::Options options{};
    ShaderConductor::Compiler::TargetDesc targetDesc{};

    auto *macro = new ShaderConductor::MacroDefine[macros.size()];

    int i = 0;
    for (auto &m : macros) {
        macro[i].name = m.first.c_str();
        macro[i].value = m.second.c_str();
        i++;
    }

    sourceDesc.entryPoint = entryPoint.c_str();
    sourceDesc.defines = macro;
    sourceDesc.numDefines = macros.size();

    sourceDesc.source = source.c_str();
    sourceDesc.stage = ShaderConductor::ShaderStage::VertexShader;

    targetDesc.language = ShaderConductor::ShadingLanguage::Glsl;

    ShaderConductor::Compiler::ResultDesc result = ShaderConductor::Compiler::Compile(sourceDesc,
                                                                                      options,
                                                                                      targetDesc);
    delete[] macro;
    if (result.hasError) {
        throw std::runtime_error(std::string("Compile failed: ") + (const char *) result.errorWarningMsg.Data());
    }
    std::string s;
    for (i = 0; i < result.target.Size(); i++) {
        s += ((char)((const char *)result.target.Data())[i]);
    }

    return s;
}

std::string mana::HlslCrossCompiler::compileFragmentShader(const std::string &source,
                                                           const std::string &entryPoint,
                                                           const std::map<std::string, std::string> &macros) {
    ShaderConductor::Compiler::SourceDesc sourceDesc{};
    ShaderConductor::Compiler::Options options{};
    ShaderConductor::Compiler::TargetDesc targetDesc{};

    auto *macro = new ShaderConductor::MacroDefine[macros.size()];

    int i = 0;
    for (auto &m : macros) {
        macro[i].name = m.first.c_str();
        macro[i].value = m.second.c_str();
        i++;
    }

    sourceDesc.entryPoint = entryPoint.c_str();
    sourceDesc.defines = macro;

    sourceDesc.source = source.c_str();
    sourceDesc.stage = ShaderConductor::ShaderStage::PixelShader;

    targetDesc.language = ShaderConductor::ShadingLanguage::Glsl;

    ShaderConductor::Compiler::ResultDesc result = ShaderConductor::Compiler::Compile(sourceDesc,
                                                                                      options,
                                                                                      targetDesc);
    delete[] macro;
    if (result.hasError) {
        throw std::runtime_error(std::string("Compile failed: ") + (const char *) result.errorWarningMsg.Data());
    }

    std::string s;
    for (i = 0; i < result.target.Size(); i++) {
        s += ((char)((const char *)result.target.Data())[i]);
    }

    return s;
}
