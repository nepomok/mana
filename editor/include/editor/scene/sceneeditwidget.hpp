#ifndef MANA_SCENEEDITWIDGET_HPP
#define MANA_SCENEEDITWIDGET_HPP

#include <QWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QSplitter>

#include "editor/scene/sceneeditwidget.hpp"
#include "editor/scene/entityeditwidget.hpp"
#include "engine/ecs/entitymanager.hpp"

class SceneEditWidget : public QWidget {
Q_OBJECT
public:
    //TODO: QTreeWidget entities display
    explicit SceneEditWidget(QWidget *parent)
            : QWidget(parent) {
        auto layout = new QHBoxLayout(this);
        setLayout(layout);
        splitter = new QSplitter(this);
        sceneTree = new QTreeWidget(this);
        entityEditWidget = new EntityEditWidget(layout->widget());
        splitter->addWidget(sceneTree);
        splitter->addWidget(entityEditWidget);
        layout->addWidget(splitter);
        sceneTree->headerItem()->setHidden(true);
    }

    void setEntityManager(engine::EntityManager *value) {
        entityManager = value;
    }

private:
    QSplitter *splitter;
    QTreeWidget *sceneTree;
    EntityEditWidget *entityEditWidget;

    engine::EntityManager *entityManager = nullptr;
};

#endif //MANA_SCENEEDITWIDGET_HPP
