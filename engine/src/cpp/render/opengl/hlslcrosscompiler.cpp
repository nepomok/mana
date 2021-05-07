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

#include "engine/render/opengl/hlslcrosscompiler.hpp"

#include <stdexcept>

#include <ShaderConductor/ShaderConductor.hpp>

struct FunctionWrapper {
    std::function<std::string(const char *)> func;

    ShaderConductor::Blob getBlob(const char *name) const {
        std::string s = func(name);
        return ShaderConductor::Blob(s.data(), s.size());
    }
};

std::function<ShaderConductor::Blob(const char *)> wrap(FunctionWrapper &func) {
    return std::bind(&FunctionWrapper::getBlob, &func, std::placeholders::_1);
}

namespace mana {

    std::string HlslCrossCompiler::compileVertexShader(const std::string &source, const std::string &entryPoint) {
        ::ShaderConductor::Compiler::SourceDesc sourceDesc{};
        ::ShaderConductor::Compiler::Options options{};
        ::ShaderConductor::Compiler::TargetDesc targetDesc{};

        auto *macro = new ::ShaderConductor::MacroDefine[macros.size()];

        int i = 0;
        for (auto &m : macros) {
            macro[i].name = m.first.c_str();
            macro[i].value = m.second.c_str();
            i++;
        }

        options.disableOptimizations = true;

        sourceDesc.entryPoint = entryPoint.c_str();
        sourceDesc.defines = macro;
        sourceDesc.numDefines = macros.size();

        FunctionWrapper w;
        w.func = std::bind(&HlslCrossCompiler::includeHandler, this, std::placeholders::_1);
        sourceDesc.loadIncludeCallback = wrap(w);

        sourceDesc.source = source.c_str();
        sourceDesc.stage = ::ShaderConductor::ShaderStage::VertexShader;

        targetDesc.language = ::ShaderConductor::ShadingLanguage::Glsl;
        targetDesc.version = "130"; //Use glsl version 130 to avoid the cross compiler creating uniform block globals.

        ::ShaderConductor::Compiler::ResultDesc result = ::ShaderConductor::Compiler::Compile(sourceDesc,
                                                                                              options,
                                                                                              targetDesc);
        delete[] macro;
        if (result.hasError) {
            throw std::runtime_error(std::string("Compile failed: ") + (const char *) result.errorWarningMsg.Data());
        }
        std::string s;
        for (i = 0; i < result.target.Size(); i++) {
            s += ((char) ((const char *) result.target.Data())[i]);
        }

        //Patch version manually back to 330 because otherwise the glsl compiler throws an error.
        std::string v = "#version 330";

        s.replace(0, v.size(), v);

        return s;
    }

    std::string HlslCrossCompiler::compileFragmentShader(const std::string &source, const std::string &entryPoint) {
        ::ShaderConductor::Compiler::SourceDesc sourceDesc{};
        ::ShaderConductor::Compiler::Options options{};
        ::ShaderConductor::Compiler::TargetDesc targetDesc{};

        auto *macro = new ::ShaderConductor::MacroDefine[macros.size()];

        int i = 0;
        for (auto &m : macros) {
            macro[i].name = m.first.c_str();
            macro[i].value = m.second.c_str();
            i++;
        }

        options.disableOptimizations = true;

        sourceDesc.entryPoint = entryPoint.c_str();
        sourceDesc.defines = macro;
        sourceDesc.numDefines = macros.size();

        FunctionWrapper w;
        w.func = std::bind(&HlslCrossCompiler::includeHandler, this, std::placeholders::_1);
        sourceDesc.loadIncludeCallback = wrap(w);

        sourceDesc.source = source.c_str();
        sourceDesc.stage = ::ShaderConductor::ShaderStage::PixelShader;

        targetDesc.language = ::ShaderConductor::ShadingLanguage::Glsl;
        targetDesc.version = "130";

        ::ShaderConductor::Compiler::ResultDesc result = ::ShaderConductor::Compiler::Compile(sourceDesc,
                                                                                              options,
                                                                                              targetDesc);
        delete[] macro;
        if (result.hasError) {
            throw std::runtime_error(std::string("Compile failed: ") + (const char *) result.errorWarningMsg.Data());
        }

        std::string s;
        for (i = 0; i < result.target.Size(); i++) {
            s += ((char) ((const char *) result.target.Data())[i]);
        }

        std::string v = "#version 330";

        s.replace(0, v.size(), v);

        return s;
    }

    void HlslCrossCompiler::setMacros(const std::map<std::string, std::string> &m) {
        macros = m;
    }

    const std::map<std::string, std::string> &HlslCrossCompiler::getMacros() {
        return macros;
    }

    void HlslCrossCompiler::setInclude(const std::map<std::string, std::string> &inc) {
        include = inc;
    }

    const std::map<std::string, std::string> &HlslCrossCompiler::getInclude() {
        return include;
    }

    void HlslCrossCompiler::setIncludeCallback(const std::function<std::string(const char *)> &func) {
        userCallback = func;
    }

    std::string HlslCrossCompiler::includeHandler(const char *name) {
        if (userCallback) {
            return userCallback(name);
        } else {
            if (include.find(name) != include.end()) {
                return include.at(name);
            } else {
                throw std::runtime_error("Include not found: " + std::string(name));
            }
        }
    }
}