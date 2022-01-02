#ifndef MANA_SCENEEDITWIDGET_HPP
#define MANA_SCENEEDITWIDGET_HPP

#include <QWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QHeaderView>

#include "gui/scene/entityeditwidget.hpp"

#include "ecs/entitymanager.hpp"
#include "ecs/components/transformcomponent.hpp"

class SceneEditWidget : public QWidget, engine::ComponentPool<engine::TransformComponent>::Listener {
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
        layout->setMargin(0);
    }

    ~SceneEditWidget() override {
        if (entityManager)
            entityManager->getComponentManager().getPool<engine::TransformComponent>().removeListener(this);
    }

    void setEntityManager(engine::EntityManager &value) {
        if (entityManager)
            entityManager->getComponentManager().getPool<engine::TransformComponent>().removeListener(this);
        entityManager = &value;
        entityManager->getComponentManager().getPool<engine::TransformComponent>().addListener(this);
    }

    QByteArray saveSplitterState() const {
        return splitter->saveState();
    }

    void restoreSplitterState(const QByteArray &state) const {
        splitter->restoreState(state);
    }

signals:

    void currentEntityChanged(engine::Entity entity);

private:
    void onComponentCreate(const engine::Entity &entity, const engine::TransformComponent &component) override {}

    void onComponentDestroy(const engine::Entity &entity, const engine::TransformComponent &component) override {}

    void onComponentUpdate(const engine::Entity &entity,
                           const engine::TransformComponent &oldValue,
                           const engine::TransformComponent &newValue) override {}

    QSplitter *splitter;
    QTreeWidget *sceneTree;
    EntityEditWidget *entityEditWidget;

    engine::EntityManager *entityManager = nullptr;

    std::map<engine::Entity, QTreeWidgetItem *> entityItems;
};

#endif //MANA_SCENEEDITWIDGET_HPP
