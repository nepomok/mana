#ifndef MANA_WINDOWATTRIBUTES_HPP
#define MANA_WINDOWATTRIBUTES_HPP

namespace mana {
    /**
     * The attributes of a window which are required for creation.
     * Some attributes may be changed for an existing window, for which
     * individual member functions are added to the window class.
     */
    struct WindowAttributes {
        bool resizable = true;
        bool visible = true;
        bool decorated = true;
        bool focused = true;
        bool autoMinimize = true;
        bool alwaysOnTop = false;
        bool maximized = false;
        bool centerCursor = true;
        bool transparentFramebuffer = false;
        bool focusOnShow = true;
        bool scaleToMonitor = false;

        int bitsRed = 8;
        int bitsGreen = 8;
        int bitsBlue = 8;
        int bitsAlpha = 8;
        int bitsDepth = 24;
        int bitsStencil = 8;

        int msaaSamples = 0;

        bool sRGBCapable = false;

        bool doubleBuffer = true;
    };
}

#endif //MANA_WINDOWATTRIBUTES_HPP
