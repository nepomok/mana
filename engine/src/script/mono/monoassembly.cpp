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

#include "engine/script/mono/monoassembly.hpp"

namespace mana {
    MonoAssembly::MonoAssembly(void *assemblyPointer) : assemblyPointer(assemblyPointer) {}

    MonoAssembly::~MonoAssembly() {
        mono_assembly_close((::MonoAssembly *) assemblyPointer);
    }

    void MonoAssembly::invokeStaticMethod(const std::string &name) const {
        auto desc = mono_method_desc_new(name.c_str(), true);
        auto method = mono_method_desc_search_in_image(desc,
                                                       mono_assembly_get_image((::MonoAssembly *) assemblyPointer));
        mono_runtime_invoke(method, nullptr, nullptr, nullptr);
    }

    void MonoAssembly::invokeMethod(const MonoObject &object, const std::string &name) const {
        auto desc = mono_method_desc_new(name.c_str(), true);
        auto method = mono_method_desc_search_in_image(desc,
                                                       mono_assembly_get_image((::MonoAssembly *) assemblyPointer));
        mono_runtime_invoke(method, object.objectPointer, nullptr, nullptr);
    }
}