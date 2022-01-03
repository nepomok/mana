#ifndef MANA_PROJECTBUILDDIALOG_HPP
#define MANA_PROJECTBUILDDIALOG_HPP

#include <string>

#include <QDialog>

#include "compiler/projectcompiler.hpp"

class ProjectBuildDialog : public QDialog {
Q_OBJECT
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
