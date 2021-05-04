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

#include "engine/resource/script/monoscriptresource.hpp"

#include <utility>

namespace mana {
    MonoScriptResource::MonoScriptResource(MonoCppRuntime &monoCppRuntime,
                                           std::string assemblyFileName,
                                           std::string nameSpace,
                                           std::string className)
            : monoCppRuntime(&monoCppRuntime),
              assemblyFileName(std::move(assemblyFileName)),
              nameSpace(std::move(nameSpace)),
              className(std::move(className)),
              script(nullptr) {

    }

    MonoScriptResource::~MonoScriptResource() {
        if (isLoaded)
            delete script;
    }

    void MonoScriptResource::load() {
        if (isLoaded)
            return;
        script = new MonoScript(monoCppRuntime->loadAssembly(assemblyFileName), nameSpace, className);
        isLoaded = true;
    }

    void MonoScriptResource::free() {
        if (!isLoaded)
            return;
        delete script;
        script = nullptr;
        isLoaded = false;
    }

    Script *MonoScriptResource::getScript() {
        if (!isLoaded)
            load();
        return script;
    }
}