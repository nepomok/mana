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

#ifndef MANA_HLSLCROSSCOMPILER_HPP
#define MANA_HLSLCROSSCOMPILER_HPP

#include <map>
#include <string>
#include <functional>

namespace engine {
    class HlslCrossCompiler {
    public:
        /**
         * Compile the hlsl vertex shader text into glsl.
         *
         * @param source The hlsl vertex shader source
         * @param entryPoint The name of the entry point function in the source
         * @return The cross compiled glsl source code
         */
        std::string compileVertexShader(const std::string &source,
                                        const std::string &entryPoint);

        /**
         * Compile the hlsl fragment shader text into glsl.
         *
         * @param source The hlsl fragment shader source
         * @param entryPoint The name of the entry point function in the source
         * @return The cross compiled glsl source code
         */
        std::string compileFragmentShader(const std::string &source,
                                          const std::string &entryPoint);

        void setMacros(const std::map<std::string, std::string> &macros);

        const std::map<std::string, std::string> &getMacros();

        void setInclude(const std::map<std::string, std::string> &include);

        const std::map<std::string, std::string> &getInclude();

        void setIncludeCallback(const std::function<std::string(const char *)> &func);

    private:
        std::string includeHandler(const char *name);

        std::map<std::string, std::string> macros;
        std::map<std::string, std::string> include;

        std::function<std::string(const char *)> userCallback;
    };
}

#endif //MANA_HLSLCROSSCOMPILER_HPP
