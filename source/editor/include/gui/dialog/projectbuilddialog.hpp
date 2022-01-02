#ifndef MANA_PROJECTBUILDDIALOG_HPP
#define MANA_PROJECTBUILDDIALOG_HPP

#include <string>

#include "compiler/projectcompiler.hpp"

class ProjectBuildDialog {
public:
    void setBuildSettings(const BuildSettings &settings) {
        buildSettings = settings;
    }

    const BuildSettings &getBuildSettings() {
        return buildSettings;
    }

private:
    BuildSettings buildSettings;
};

#endif //MANA_PROJECTBUILDDIALOG_HPP
