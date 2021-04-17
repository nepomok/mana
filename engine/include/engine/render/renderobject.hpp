#ifndef MANA_OBJECT_HPP
#define MANA_OBJECT_HPP

/**
 * The common ancestor of all object interfaces which abstract graphics api dependent objects such as framebuffers,
 * mesh objects and shader programs.
 *
 * All render objects have a destructor which cleans up the graphics api dependent resources associated with it.
 */
class RenderObject
{
public:
    virtual ~RenderObject() = default;
};

#endif //MANA_OBJECT_HPP
