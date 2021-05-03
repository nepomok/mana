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

#include <cstring>

#include "engine/script/mono/monocppobject.hpp"

namespace mana {
    MonoCppObject::MonoCppObject(void *objectPointer) : objectPointer(objectPointer) {}

    MonoCppObject::~MonoCppObject() = default; //TODO: Deallocate object in destructor

    MonoCppObject MonoCppObject::invokeMethod(const std::string &name, MonoCppArguments args) const {
        if (objectPointer == nullptr)
            throw std::runtime_error("Null object");
        auto *classPointer = mono_object_get_class((MonoObject *) objectPointer);
        auto *method = mono_class_get_method_from_name((MonoClass *) classPointer, name.c_str(), args.args.size());
        if (method == nullptr)
            throw std::runtime_error("Failed to find method " + name);
        void *a[args.args.size()];
        for (int i = 0; i < args.args.size(); i++) {
            a[i] = args.args[i];
        }
        auto *o = mono_runtime_invoke(method, objectPointer, a, nullptr);
        return MonoCppObject(o);
    }

    void MonoCppObject::setField(const std::string &name, MonoCppValue &value) const {
        if (objectPointer == nullptr)
            throw std::runtime_error("Null object");
        auto *classPointer = mono_object_get_class((MonoObject *) objectPointer);
        auto *f = mono_class_get_field_from_name((MonoClass *) classPointer, name.c_str());
        if (f == nullptr)
            throw std::runtime_error("Field not found " + name);
        mono_field_set_value((MonoObject *) objectPointer, f, value.ptr);
    }

    MonoCppObject MonoCppObject::getField(const std::string &name) const {
        if (objectPointer == nullptr)
            throw std::runtime_error("Null object");
        auto *classPointer = mono_object_get_class((MonoObject *) objectPointer);
        auto *f = mono_class_get_field_from_name((MonoClass *) classPointer, name.c_str());
        if (f == nullptr)
            throw std::runtime_error("Field not found " + name);
        MonoCppObject ret(nullptr);
        mono_field_get_value((MonoObject *) objectPointer, f, &ret.objectPointer);
        return ret;
    }

    MonoCppValue MonoCppObject::getFieldValue(const std::string &name, size_t valueSize) const {
        if (objectPointer == nullptr)
            throw std::runtime_error("Null object");
        auto *classPointer = mono_object_get_class((MonoObject *) objectPointer);
        auto *f = mono_class_get_field_from_name((MonoClass *) classPointer, name.c_str());
        if (f == nullptr)
            throw std::runtime_error("Field not found " + name);
        MonoCppValue ret;
        ret.ptr = new void *[valueSize];
        mono_field_get_value((MonoObject *) objectPointer, f, ret.ptr);
        return ret;
    }

    bool MonoCppObject::isNull() const {
        return objectPointer == nullptr;
    }
}
