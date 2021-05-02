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

#include "engine/script/mono/monocppobject.hpp"

namespace mana {
    MonoCppObject::MonoCppObject(void *imagePointer, void *objectPointer) : imagePointer(imagePointer),
                                                                            objectPointer(objectPointer) {}

    MonoCppObject::~MonoCppObject() = default; //TODO: Deallocate object in destructor

    void MonoCppObject::invokeMethod(const std::string &name, void **args) const {
        auto desc = mono_method_desc_new(name.c_str(), true);
        auto method = mono_method_desc_search_in_image(desc, (MonoImage *) imagePointer);
        mono_runtime_invoke(method, objectPointer, args, nullptr);
    }

    void MonoCppObject::invokeMethod(const std::string &name) const {
        auto desc = mono_method_desc_new(name.c_str(), true);
        auto method = mono_method_desc_search_in_image(desc, (MonoImage *) imagePointer);
        mono_runtime_invoke(method, objectPointer, nullptr, nullptr);
    }

    void MonoCppObject::setField(const std::string &name, MonoCppObject *object) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        mono_field_set_value((MonoObject *) objectPointer, f, object->objectPointer);
    }

    void MonoCppObject::setField(const std::string &name, void *value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        mono_field_set_value((MonoObject *) objectPointer, f, value);
    }

    void MonoCppObject::setField(const std::string &name, float value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        mono_field_set_value((MonoObject *) objectPointer, f, &value);
    }

    void MonoCppObject::setField(const std::string &name, int value) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        mono_field_set_value((MonoObject *) objectPointer, f, &value);
    }

    MonoCppObject *MonoCppObject::getFieldObject(const std::string &name) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        void *ret = nullptr;
        mono_field_get_value((MonoObject *) objectPointer, f, ret);
        return new MonoCppObject(imagePointer, objectPointer);
    }

    void *MonoCppObject::getFieldValue(const std::string &name) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        void *ret = nullptr;
        mono_field_get_value((MonoObject *) objectPointer, f, ret);
        return ret;
    }

    float MonoCppObject::getFieldFloat(const std::string &name) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        float ret;
        mono_field_get_value((MonoObject *) objectPointer, f, &ret);
        return ret;
    }

    int MonoCppObject::getFieldInt(const std::string &name) const {
        auto *c = mono_class_from_name((MonoImage *) imagePointer, nameSpace.c_str(), className.c_str());
        auto *f = mono_class_get_field_from_name(c, name.c_str());
        int ret;
        mono_field_get_value((MonoObject *) objectPointer, f, &ret);
        return ret;
    }
}
