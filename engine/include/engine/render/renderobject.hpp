#ifndef MANA_OBJECT_HPP
#define MANA_OBJECT_HPP

/**
 * A render object is a dynamically allocated graphics api dependent object which can be deallocated by
 * calling the destructor.
 */
class RenderObject {
public:
    virtual ~RenderObject() = default;
};

#endif //MANA_OBJECT_HPP
