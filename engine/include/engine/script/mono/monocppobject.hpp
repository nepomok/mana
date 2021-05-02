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

#ifndef MANA_MONOCPPOBJECT_HPP
#define MANA_MONOCPPOBJECT_HPP

#include <string>

namespace mana {
    class MonoCppObject {
    public:
        explicit MonoCppObject(void *imagePointer, void *objectPointer);

        ~MonoCppObject();

        const void *getAssemblyPointer() const { return imagePointer; }

        const void *getObjectPointer() const { return objectPointer; }

        void invokeMethod(const std::string &name, void **args) const;

        void invokeMethod(const std::string &name) const;

        void setField(const std::string &name, MonoCppObject *object) const;

        void setField(const std::string &name, void *value) const;

        void setField(const std::string &name, float value) const;

        void setField(const std::string &name, int value) const;

        MonoCppObject *getFieldObject(const std::string &name) const;

        void *getFieldValue(const std::string &name) const;

        float getFieldFloat(const std::string &name) const;

        int getFieldInt(const std::string &name) const;

        std::string nameSpace;
        std::string className;
        void *imagePointer;
        void *objectPointer;
    };
}

#endif //MANA_MONOCPPOBJECT_HPP
