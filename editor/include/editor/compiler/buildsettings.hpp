#ifndef MANA_BUILDSETTINGS_HPP
#define MANA_BUILDSETTINGS_HPP

#include "editor/compiler/buildoptimization.hpp"
#include "editor/compiler/buildplatform.hpp"

struct BuildSettings {
    std::string includeDir{}; // User include path
    std::string sourceDir{}; // User source files
    std::string assetDir{}; // Assets directory which is packed into dirName.pack
    std::string linkDir{}; // Directory containing the library binaries (Engine and dependencies) for the target platform.
    std::string outputDir{}; // The output directory

    BuildPlatform targetPlatform;
    BuildOptimization optimization{};
};

#endif //MANA_BUILDSETTINGS_HPP
