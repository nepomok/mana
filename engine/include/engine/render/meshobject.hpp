#ifndef MANA_MESHOBJECT_HPP
#define MANA_MESHOBJECT_HPP

namespace mana {
    class MeshObject {
    protected:
        MeshObject() = default;

        MeshObject(const MeshObject &copy) = default;

        MeshObject &operator=(const MeshObject &) = default;

    public:
        virtual ~MeshObject() = default;
    };
}

#endif //MANA_MESHOBJECT_HPP
