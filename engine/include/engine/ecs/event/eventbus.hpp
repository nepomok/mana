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

#ifndef MANA_EVENTBUS_HPP
#define MANA_EVENTBUS_HPP

#include <functional>
#include <set>
#include <typeindex>

#include "engine/ecs/event/event.hpp"
#include "engine/ecs/event/eventreceiver.hpp"

namespace engine {
    /**
     * Any type which inherits from the Event class can be used as event type.
     *
     * Member methods should only be invoked from a single thread
     */
    class EventBus {
    public:
        template<typename T>
        void invoke(const T &event) {
            auto id = std::type_index(typeid(T));
            for (auto *callback : receivers[id]) {
                callback->onEvent(dynamic_cast<const Event &>(*event));
            }
        }

        void subscribe(EventReceiver *receiver,
                       const std::set<std::type_index> &eventTypes) {
            for (auto &type : eventTypes) {
                receivers[type].insert(receiver);
            }
        }

        void unsubscribe(EventReceiver *receiver) {
            for (auto &pair : receivers) {
                pair.second.erase(receiver);
            }
        }

    private:
        std::map<std::type_index, std::set<EventReceiver *>> receivers;
    };
}

#endif //MANA_EVENTBUS_HPP
