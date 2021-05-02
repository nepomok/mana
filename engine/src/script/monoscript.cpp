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

#include <engine/script/mono/monoscript.hpp>

namespace mana {
    MonoScript::MonoScript() : object(nullptr),
                               assembly(nullptr),
                               scriptClassNameSpace(),
                               scriptClassName() {}

    MonoScript::MonoScript(MonoCppAssembly *assembly,
                           std::string scriptClassNameSpace,
                           std::string scriptClass)
            : object(nullptr),
              assembly(assembly),
              scriptClassNameSpace(std::move(scriptClassNameSpace)),
              scriptClassName(std::move(scriptClass)) {
        object = assembly->createObject(scriptClassNameSpace, scriptClassName);
    }

    MonoScript::~MonoScript() {
        delete assembly;
        delete object;
    }

    void MonoScript::onEnable() {
        object->invokeMethod(scriptClassNameSpace + "." + scriptClassName + ":OnEnable()");
    }

    void MonoScript::onDisable() {
        object->invokeMethod(scriptClassNameSpace + "." + scriptClassName + ":OnDisable()");
    }

    void MonoScript::onUpdate() {
        object->invokeMethod(scriptClassNameSpace + "." + scriptClassName + ":OnUpdate()");
    }
}
