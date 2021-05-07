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

#ifndef MANA_HLSLCROSSCOMPILER_HPP
#define MANA_HLSLCROSSCOMPILER_HPP

#include <map>
#include <string>

namespace mana {
    namespace HlslCrossCompiler {
        /**
         * Compile the hlsl vertex shader text into glsl.
         *
         * @param source The hlsl vertex shader source
         * @param entryPoint The name of the entry point function in the source
         * @param macros A mapping of macro names to macro values which are applied in the processing of the source string.
         * @return The cross compiled glsl source code
         */
        std::string compileVertexShader(const std::string &source,
                                        const std::string &entryPoint,
                                        const std::map<std::string, std::string> &macros);

        /**
         * Compile the hlsl fragment shader text into glsl.
         *
         * @param source The hlsl fragment shader source
         * @param entryPoint The name of the entry point function in the source
         * @param macros A mapping of macro names to macro values which are applied in the processing of the source string.
         * @return The cross compiled glsl source code
         */
        std::string compileFragmentShader(const std::string &source,
                                          const std::string &entryPoint,
                                          const std::map<std::string, std::string> &macros);
    }
}

#endif //MANA_HLSLCROSSCOMPILER_HPP
