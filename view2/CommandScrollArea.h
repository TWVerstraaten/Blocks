#ifndef COMMANDSCROLLAREA_H
#define COMMANDSCROLLAREA_H

#include "../model/Cluster.h"
#include "CommandEditBox.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <memory>
#include <vector>

namespace view2 {
    class CentralWidget;

    class CommandScrollArea : public QScrollArea {
        Q_OBJECT
      public:
        explicit CommandScrollArea(CentralWidget* centralWidget);
        ~CommandScrollArea() override;

        void add(model::Cluster& cluster);
        void addNeeded(std::list<model::Cluster>& clusters);
        void removeUnnecessary(const std::list<model::Cluster>& clusters);
        void addToLayout(std::unique_ptr<CommandEditBox>&& commandEditBox);

        [[nodiscard]] CentralWidget*    centralWidget() const;
        std::unique_ptr<CommandEditBox> removeFromLayout(size_t index);

      private slots:

        void moveFocusToNext();
        void moveFocusToPrevious();

      private:
        std::vector<std::unique_ptr<CommandEditBox>> m_commandEditBoxes;
        QVBoxLayout*                                 m_layout;
        CentralWidget*                               m_centralWidget;
    };
} // namespace view2

#endif // COMMANDSCROLLAREA_H
