#ifndef MANA_TEXTUREATTRIBUTES_HPP
#define MANA_TEXTUREATTRIBUTES_HPP

namespace mana {
    enum TextureWrapping {
        REPEAT,
        MIRRORED_REPEAT,
        CLAMP_TO_EDGE,
        CLAMP_TO_BORDER
    };

    enum TextureFiltering {
        NEAREST,
        LINEAR
    };

    enum MipMapFiltering {
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR
    };

    struct TextureAttributes {
        TextureWrapping texWrapping = REPEAT;
        TextureFiltering texFilterMin = NEAREST;
        TextureFiltering texFilterMag = NEAREST;

        bool generateMipmap = false;
        MipMapFiltering mipmapFilter = NEAREST_MIPMAP_NEAREST;
    };
}

#endif //MANA_TEXTUREATTRIBUTES_HPP
