#ifndef AUDIOTRACK_HPP
#define AUDIOTRACK_HPP

#include <string>

namespace mana {
    class AudioBuffer {
    public:
        AudioBuffer();

        ~AudioBuffer();

        void loadData();

        void unloadData();

        long getDuration();
    };
}

#endif // AUDIOTRACK_HPP