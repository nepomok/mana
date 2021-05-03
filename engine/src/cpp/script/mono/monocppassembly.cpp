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

#include <stdexcept>

#include <mono/jit/jit.h>
#include <mono/metadata/loader.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/object.h>

#include "engine/script/mono/monocppassembly.hpp"

namespace mana {
    MonoCppAssembly::MonoCppAssembly(void *domainPointer, void *assemblyPointer)
            : domainPointer(domainPointer),
              assemblyPointer(assemblyPointer) {}

    MonoCppAssembly::~MonoCppAssembly() = default; //TODO: Unload assembly in destructor

    MonoCppObject MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace,
                                                      const std::string &className,
                                                      const std::string &functionName,
                                                      MonoCppArguments &args) const {
        auto *imagePointer = mono_assembly_get_image(static_cast<MonoAssembly *>(assemblyPointer));
        auto *c = mono_class_from_name(imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 0);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);

        void *a[args.size()];
        for (int i = 0; i < args.size(); i++) {
            a[i] = args.data()[i];
        }
        return std::move(MonoCppObject(mono_runtime_invoke(m, nullptr, a, nullptr)));
    }

    void MonoCppAssembly::setStaticField(const std::string &nameSpace,
                                         const std::string &className,
                                         const std::string &fieldName,
                                         MonoCppValue value) const {
        auto *imagePointer = mono_assembly_get_image(static_cast<MonoAssembly *>(assemblyPointer));
        auto *c = mono_class_from_name(imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *f = mono_class_get_field_from_name(c, fieldName.c_str());
        if (f == nullptr)
            throw std::runtime_error("Failed to find field " + fieldName);
        mono_field_static_set_value(mono_class_vtable(static_cast<MonoDomain *>(domainPointer), c), f, value.ptr);
    }

    MonoCppObject MonoCppAssembly::getStaticField(const std::string &nameSpace,
                                                  const std::string &className,
                                                  const std::string &fieldName) const {
        auto *imagePointer = mono_assembly_get_image((MonoAssembly *) assemblyPointer);
        auto *c = mono_class_from_name(imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *f = mono_class_get_field_from_name(c, fieldName.c_str());
        if (f == nullptr)
            throw std::runtime_error("Failed to find field " + fieldName);
        void *p = nullptr;
        mono_field_static_get_value(mono_class_vtable(static_cast<MonoDomain *>(domainPointer), c), f, &p);
        return std::move(MonoCppObject(p));
    }

    MonoCppObject MonoCppAssembly::createObject(const std::string &nameSpace,
                                                const std::string &className,
                                                bool pinned) const {
        auto *imagePointer = mono_assembly_get_image(static_cast<MonoAssembly *>(assemblyPointer));
        auto *monoClass = mono_class_from_name(imagePointer, nameSpace.c_str(), className.c_str());
        if (monoClass == nullptr)
            throw std::runtime_error("Class not found " + nameSpace + "." + className);
        auto *o = mono_object_new(static_cast<MonoDomain *>(domainPointer), monoClass);
        mono_runtime_object_init(o);
        return std::move(MonoCppObject(o, pinned));
    }
}