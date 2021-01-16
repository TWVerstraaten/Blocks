#ifndef COMMANDSCROLLAREA_H
#define COMMANDSCROLLAREA_H

#include "../model/Cluster.h"
#include "CommandEditBox.h"

#include <QScrollArea>
#include <QUndoGroup>
#include <QVBoxLayout>
#include <QWidget>
#include <vector>

namespace view2 {

    class CommandScrollArea : public QScrollArea {
        Q_OBJECT
      public:
        explicit CommandScrollArea(QWidget* parent = nullptr);

        void add(model::Cluster& cluster);
        void removeUnnecessary(const std::list<model::Cluster>& clusters);
        void addNeeded(std::list<model::Cluster>& clusters);

      private slots:
        void moveFocusToNext();
        void moveFocusToPrevious();

      private:
        QUndoGroup                   m_undoGroup;
        std::vector<CommandEditBox*> m_commandEditBoxes;
        QVBoxLayout*                 m_layout;
    };
} // namespace view2

#endif // COMMANDSCROLLAREA_H
