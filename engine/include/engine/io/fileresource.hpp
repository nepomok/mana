#ifndef MANA_IMAGERESOURCE_HPP
#define MANA_IMAGERESOURCE_HPP

#include "engine/io/file.hpp"
#include "resource.hpp"

namespace mana {
    /**
     * A resource backed by a file.
     */
    template<typename T>
    class FileResource : public Resource<T> {
    public:
        FileResource(const File &file);

        ~FileResource() override = default;

        T &getData() override;

        void free() override;

    private:
        File file;
    };
}

#endif //MANA_IMAGERESOURCE_HPP
