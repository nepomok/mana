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
    MonoCppAssembly::MonoCppAssembly(void *domainPointer, void *imagePointer)
            : domainPointer(domainPointer),
              imagePointer(imagePointer) {}

    MonoCppAssembly::~MonoCppAssembly() = default; //TODO: Unload assembly in destructor

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace,
                                             const std::string &className,
                                             const std::string &functionName) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 0);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        mono_runtime_invoke(m, nullptr, nullptr, nullptr);
    }

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace, const std::string &className,
                                             const std::string &functionName, void **args) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 1);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        mono_runtime_invoke(m, nullptr, args, nullptr);
    }

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace,
                                             const std::string &className,
                                             const std::string &functionName,
                                             int arg) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 1);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        auto *s = mono_method_signature(m);
        void *iter = nullptr;
        auto *params = mono_signature_get_params(s, &iter);
        if (mono_type_get_type(params) != MONO_TYPE_I4)
            throw std::runtime_error("Method argument type is not int");
        void *args[1];
        args[0] = &arg;
        mono_runtime_invoke(m, nullptr, args, nullptr);
    }

    void MonoCppAssembly::invokeStaticMethod(const std::string &nameSpace,
                                             const std::string &className,
                                             const std::string &functionName,
                                             float arg) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *m = mono_class_get_method_from_name(c, functionName.c_str(), 1);
        if (m == nullptr)
            throw std::runtime_error("Failed to find method " + functionName);
        auto *s = mono_method_signature(m);
        void *iter = nullptr;
        auto *params = mono_signature_get_params(s, &iter);
        if (mono_type_get_type(params) != MONO_TYPE_R4)
            throw std::runtime_error("Method argument type is not float");
        void *args[1];
        args[0] = &arg;
        mono_runtime_invoke(m, nullptr, args, nullptr);
    }

    void MonoCppAssembly::setStaticField(const std::string &nameSpace, const std::string &className,
                                         const std::string &fieldName, void *value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *f = mono_class_get_field_from_name(c, fieldName.c_str());
        if (f == nullptr)
            throw std::runtime_error("Failed to find field " + fieldName);
        mono_field_static_set_value(mono_class_vtable((MonoDomain *) domainPointer, c), f, value);
    }

    void MonoCppAssembly::setStaticField(const std::string &nameSpace,
                                         const std::string &className,
                                         const std::string &fieldName,
                                         int value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        if (c == nullptr)
            throw std::runtime_error("Failed to find class " + className);
        auto *f = mono_class_get_field_from_name(c, fieldName.c_str());
        if (f == nullptr)
            throw std::runtime_error("Failed to find field " + fieldName);

        if (mono_type_get_type(mono_field_get_type(f)) != MONO_TYPE_I4) {
            throw std::runtime_error("Field type is not integer");
        }
        mono_field_static_set_value(mono_class_vtable((MonoDomain *) domainPointer, c), f, &value);
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

        if (mono_type_get_type(mono_field_get_type(f)) != MONO_TYPE_R4) {
            throw std::runtime_error("Field type is not float");
        }
        mono_field_static_set_value(mono_class_vtable((MonoDomain *) domainPointer, c), f, &value);
    }

    MonoCppObject *MonoCppAssembly::createObject(const std::string &nameSpace, const std::string &className) const {
        MonoClass *monoClass = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        MonoObject *classObject = mono_object_new((MonoDomain *) domainPointer, monoClass);
        mono_runtime_object_init(classObject);
        return new MonoCppObject((MonoImage *) imagePointer, classObject);
    }
}