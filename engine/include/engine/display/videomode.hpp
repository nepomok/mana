#ifndef MANA_MONITORMODE_HPP
#define MANA_MONITORMODE_HPP

namespace mana {
    struct VideoMode {
        int width;
        int height;
        int redBits;
        int greenBits;
        int blueBits;
        int refreshRate;
    };
}

#endif //MANA_MONITORMODE_HPP
