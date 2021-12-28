#include "editor/compiler/projectcompiler.hpp"

#include <fstream>

ProjectCompiler &ProjectCompiler::setSettings(const BuildSettings &settings) {
    buildSettings = settings;
    return *this;
}

void ProjectCompiler::compile() {
    // Package assets

    // Copy Packaged assets to output dir

    // Create the CMakeLists.txt file
    auto cmSrc = getCMakeSource();
    std::ofstream fs(buildSettings.outputDir + "/CMakeLists.txt");
    fs << cmSrc;
    fs.close();

    // Run Cmake

    // Copy outputs from cmake

    // Copy library binaries
}

std::string ProjectCompiler::getCMakeSource() {
    throw std::runtime_error("Not Implemented");
}
