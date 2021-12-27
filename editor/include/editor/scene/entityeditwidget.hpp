#ifndef MANA_ENTITYEDITWIDGET_HPP
#define MANA_ENTITYEDITWIDGET_HPP

#include <QWidget>

#include "engine/ecs/componentmanager.hpp"

class EntityEditWidget : public QWidget {
Q_OBJECT
public:
    explicit EntityEditWidget(QWidget *parent)
            : QWidget(parent) {}

    void setEntity(engine::Entity value) {
        entity = value;
    }

    void setComponentManager(engine::ComponentManager *value) {
        componentManager = value;
    }

private:
    engine::Entity entity;
    engine::ComponentManager *componentManager = nullptr;
};

#endif //MANA_ENTITYEDITWIDGET_HPP
