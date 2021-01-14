#ifndef COMMANDSCROLLAREA_H
#define COMMANDSCROLLAREA_H

#include <QWidget>

#include <QScrollArea>
#include <QVBoxLayout>
#include <vector>

#include "CommandEditBox.h"

namespace view2 {

    class CommandScrollArea : public QScrollArea {
    Q_OBJECT
    public:
        explicit CommandScrollArea(QWidget *parent = nullptr);

        void add();

        void remove();

    private slots:

        void moveFocusToNext();

        void moveFocusToPrevious();

    signals:

    private:

        std::vector<CommandEditBox *> m_commandEditBoxes;
        QVBoxLayout *m_layout;


    };
}

#endif // COMMANDSCROLLAREA_H
