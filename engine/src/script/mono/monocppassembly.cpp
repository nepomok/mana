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

#include "engine/script/mono/monocppassembly.hpp"

namespace mana {
    MonoCppAssembly::MonoCppAssembly(void *domainPointer, void *assemblyPointer)
            : domainPointer(domainPointer),
              assemblyPointer(assemblyPointer),
              imagePointer(mono_assembly_get_image((MonoAssembly *) assemblyPointer)) {}

    MonoCppAssembly::~MonoCppAssembly() {
    }

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace, const std::string &className,
                                             const std::string &functionName) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 0);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        mono_runtime_invoke(m, nullptr, nullptr, nullptr);
    }

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace,
                                             const std::string &className,
                                             const std::string &functionName,
                                             double arg) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 1);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        double *p;
        p = &arg;
        mono_runtime_invoke(m, nullptr, (void**)&p, nullptr);
    }

    void MonoCppAssembly::setStaticField(const std::string &nameSpace,
                                         const std::string &className,
                                         const std::string &fieldName,
                                         float value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *f = mono_class_get_field_from_name(c, fieldName.c_str());
        if (f == nullptr)
            throw std::runtime_error("Failed to find field " + fieldName);
        mono_field_set_value(nullptr, f, &value);
    }

    MonoCppObject *MonoCppAssembly::createObject(const std::string &nameSpace, const std::string &className) {
        MonoImage * image = mono_assembly_get_image((::MonoAssembly *) assemblyPointer);
        MonoClass *monoClass = mono_class_from_name(image, nameSpace.c_str(), className.c_str());
        MonoObject * classObject = mono_object_new((MonoDomain *) domainPointer, monoClass);
        mono_runtime_object_init(classObject);
        return new MonoCppObject(image, classObject);
    }
}