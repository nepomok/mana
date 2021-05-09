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

#ifndef MANA_MONOSCRIPTRESOURCE_HPP
#define MANA_MONOSCRIPTRESOURCE_HPP

#include "engine/resource/resource.hpp"

#include "engine/script/mono/monoscript.hpp"

namespace mana {
    class MonoScriptResource : public Resource<Script> {
    public:
        MonoScriptResource(MonoCppDomain &monoRuntime,
                           std::string assemblyFileName,
                           std::string nameSpace,
                           std::string className);

        ~MonoScriptResource() override;

        void load() override;

        void free() override;

        const Script &get() override;

    private:
        MonoCppDomain *monoCppRuntime;
        std::string assemblyFileName;
        std::string nameSpace;
        std::string className;
        MonoScript *script;
        bool isLoaded = false;
    };
}

#endif //MANA_MONOSCRIPTRESOURCE_HPP
