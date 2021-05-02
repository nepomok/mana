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

#ifndef MANA_MONOCPPASSEMBLY_HPP
#define MANA_MONOCPPASSEMBLY_HPP

#include <string>

#include "engine/script/mono/monocppobject.hpp"

namespace mana {
    class MonoCppAssembly {
    public:
        explicit MonoCppAssembly(void *domainPointer, void *assemblyPointer);

        ~MonoCppAssembly();

        void invokeStaticMethod(const std::string &nameSpace, const std::string &className, const std::string &functionName) const;

        void invokeStaticMethod(const std::string &nameSpace, const std::string &className, const std::string &functionName, double arg) const;

        void setStaticField(const std::string &nameSpace,
                            const std::string &className,
                            const std::string &fieldName,
                            float value) const;

        MonoCppObject *createObject(const std::string &nameSpace,
                                    const std::string &className);

        void *domainPointer;
        void *assemblyPointer;
        void *imagePointer;
    };
}

#endif //MANA_MONOCPPASSEMBLY_HPP
