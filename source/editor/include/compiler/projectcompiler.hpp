#ifndef MANA_PROJECTCOMPILER_HPP
#define MANA_PROJECTCOMPILER_HPP

#include <string>

#include "compiler/buildsettings.hpp"

class ProjectCompiler {
public:
    ProjectCompiler &setSettings(const BuildSettings &settings);

    void compile();

private:
    std::string getCMakeSource();

    BuildSettings buildSettings;
};

#endif //MANA_PROJECTCOMPILER_HPP
