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
#include <mono/metadata/mono-config.h>

#include "engine/script/mono/monocppruntime.hpp"

namespace mana {
    MonoCppRuntime::MonoCppRuntime() : msCorLib(nullptr, nullptr) {
        domainPointer = mono_jit_init("DefaultDomain");
        mono_config_parse(nullptr);
        msCorLib = MonoCppAssembly(domainPointer, mono_get_corlib());
    }

    MonoCppRuntime::MonoCppRuntime(const std::string &domainName) : msCorLib(nullptr, nullptr) {
        domainPointer = mono_jit_init(domainName.c_str());
        mono_config_parse(nullptr);
        msCorLib = MonoCppAssembly(domainPointer, mono_get_corlib());
    }

    MonoCppRuntime::~MonoCppRuntime() {
        mono_jit_cleanup((MonoDomain *) domainPointer);
    }

    MonoCppAssembly &MonoCppRuntime::getMsCorLibAssembly() {
        return msCorLib;
    }

    MonoCppAssembly *MonoCppRuntime::loadAssembly(const std::string &filePath) {
        return new mana::MonoCppAssembly(domainPointer, mono_assembly_get_image(mono_domain_assembly_open(
                (MonoDomain *) domainPointer, filePath.c_str())));
    }
}