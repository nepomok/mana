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

#include "engine/script/mono/monocppdomain.hpp"

namespace mana {
    MonoCppDomain::MonoCppDomain()
            : msCorLib(nullptr, nullptr) {
        domainPointer = mono_jit_init("DefaultDomain");
        if (domainPointer == nullptr)
            throw std::runtime_error("Failed to create mono domain DefaultDomain");
        mono_config_parse(nullptr);
        msCorLib = MonoCppAssembly(domainPointer, mono_image_get_assembly(mono_get_corlib()));
    }

    MonoCppDomain::MonoCppDomain(const std::string &domainName)
            : msCorLib(nullptr, nullptr) {
        domainPointer = mono_jit_init(domainName.c_str());
        if (domainPointer == nullptr)
            throw std::runtime_error("Failed to create mono domain " + domainName);
        mono_config_parse(nullptr);
        msCorLib = MonoCppAssembly(domainPointer, mono_image_get_assembly(mono_get_corlib()));
    }

    MonoCppDomain::~MonoCppDomain() {
        mono_jit_cleanup(static_cast<MonoDomain *>(domainPointer));
    }

    MonoCppAssembly &MonoCppDomain::getMsCorLibAssembly() {
        return msCorLib;
    }

    MonoCppAssembly *MonoCppDomain::loadAssembly(const std::string &filePath) {
        auto *ap = mono_domain_assembly_open(static_cast<MonoDomain *>(domainPointer), filePath.c_str());
        if (ap == nullptr)
            throw std::runtime_error("Failed to load assembly " + filePath);
        return new mana::MonoCppAssembly(domainPointer, ap);
    }

    MonoCppObject MonoCppDomain::stringFromUtf8(const std::string &str, bool pinned) {
        auto *p = mono_string_new(static_cast<MonoDomain *>(domainPointer), str.c_str());
        return std::move(MonoCppObject(p, pinned));
    }

    std::string MonoCppDomain::stringToUtf8(const MonoCppObject &strObject) {
        return mono_string_to_utf8(static_cast<MonoString *>(strObject.getObjectPointer()));
    }
}