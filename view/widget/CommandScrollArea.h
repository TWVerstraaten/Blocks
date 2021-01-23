#ifndef COMMANDSCROLLAREA_H
#define COMMANDSCROLLAREA_H

#include "CentralWidget_enums.h"
#include "CommandEditWidget.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <memory>
#include <vector>

namespace view {
    class CentralWidget;

    class CommandScrollArea : public QScrollArea {
        Q_OBJECT

      public:
        explicit CommandScrollArea(CentralWidget* centralWidget);
        ~CommandScrollArea() override;

        void add(model::Cluster& cluster);
        void addToLayout(std::unique_ptr<CommandEditWidget>&& commandEditBox);
        void addNeeded(std::vector<model::Cluster>& clusters);
        void removeUnneeded(std::vector<model::Cluster>& clusters);
        void updateSelection();
        void disable();

        [[nodiscard]] CommandEditWidget*                 withIndex(size_t index);
        [[nodiscard]] CentralWidget*                     centralWidget() const;
        [[nodiscard]] std::unique_ptr<CommandEditWidget> removeFromLayout(size_t index);

      private slots:

        void moveFocusToNext();
        void moveFocusToPrevious();

      private:
        std::vector<std::unique_ptr<CommandEditWidget>> m_commandEditBoxes;
        QVBoxLayout*                                    m_layout;
        CentralWidget*                                  m_centralWidget;
    };
} // namespace view

#endif // COMMANDSCROLLAREA_H
