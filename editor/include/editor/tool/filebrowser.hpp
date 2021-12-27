#ifndef MANA_FILEBROWSER_HPP
#define MANA_FILEBROWSER_HPP

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>

class FileBrowser : public QWidget {
Q_OBJECT
public:
    //TODO: ListWidget file display, Drag and Drop, Context menu
    explicit FileBrowser(QWidget *parent)
            : QWidget(parent) {
        setLayout(new QVBoxLayout());
        listWidget = new QListWidget(this);
        layout()->addWidget(listWidget);
        layout()->setMargin(0);
    }

    void setCurrentPath(const std::string &value) {
        currentPath = value;
    }

    const std::string &getCurrentPath() {
        return currentPath;
    }

private:
    std::string currentPath;
    QListWidget *listWidget;
};

#endif //MANA_FILEBROWSER_HPP
