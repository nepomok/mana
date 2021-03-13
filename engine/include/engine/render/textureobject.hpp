#ifndef MANA_TEXTUREOBJECT_HPP
#define MANA_TEXTUREOBJECT_HPP

namespace mana {
    class TextureObject {
    protected:
        TextureObject() = default;

        TextureObject(const TextureObject &copy) = default;

        TextureObject &operator=(const TextureObject &) = default;

    public:
        virtual ~TextureObject() = default;
    };
}

#endif //MANA_TEXTUREOBJECT_HPP
