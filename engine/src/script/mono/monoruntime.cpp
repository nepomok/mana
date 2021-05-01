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

#include <mono/jit/jit.h>
#include <mono/metadata/loader.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "engine/script/mono/monoruntime.hpp"

namespace mana {
    MonoRuntime::MonoRuntime() {
        domainPointer = mono_jit_init("DefaultDomain");
    }

    MonoRuntime::MonoRuntime(const std::string &domainName) {
        domainPointer = mono_jit_init(domainName.c_str());
    }

    MonoRuntime::~MonoRuntime() {
        mono_jit_cleanup((MonoDomain *) domainPointer);
    }

    MonoAssembly *MonoRuntime::loadAssembly(const std::string &filePath) {
        return new mana::MonoAssembly(mono_domain_assembly_open((MonoDomain *) domainPointer, filePath.c_str()));
    }

    MonoObject *MonoRuntime::createObject(const MonoAssembly &assembly,
                                          const std::string &nameSpace,
                                          const std::string &className) {
        MonoImage *image = mono_assembly_get_image((::MonoAssembly *) assembly.assemblyPointer);
        MonoClass *my_class = mono_class_from_name(image, nameSpace.c_str(), className.c_str());
        ::MonoObject *my_class_instance = mono_object_new((MonoDomain *) domainPointer, my_class);
        mono_runtime_object_init(my_class_instance);
        return new MonoObject(my_class_instance);
    }
}