#ifndef MANA_RESOURCE_HPP
#define MANA_RESOURCE_HPP

#include "engine/common/buffer.hpp"

namespace mana {
    /**
     * Interface for resource types.
     *
     * A resource holds a object of type T.
     *
     * A resource is loaded when calling get and stays
     * in memory until free is called or the resource object is destroyed.
     *
     * A resource can be loaded again by calling get after free has been called.
     */
    template<typename T>
    class Resource {
    public:
        virtual ~Resource() = default;

        virtual T &get() = 0;

        virtual void free() = 0;
    };
}

#endif //MANA_RESOURCE_HPP
