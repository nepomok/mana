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

#include "engine/script/mono/monocpparguments.hpp"
#include "engine/script/mono/monocppvalue.hpp"

namespace mana {
    class MonoCppObject {
    public:
        void *objectPointer;

        explicit MonoCppObject(void *objectPointer);

        ~MonoCppObject();

        MonoCppObject invokeMethod(const std::string &name, MonoCppArguments args = {}) const;

        void setField(const std::string &name, MonoCppValue &value) const;

        MonoCppValue getField(const std::string &name) const;

        bool isNull() const;
    };
}

#endif //MANA_MONOCPPOBJECT_HPP
