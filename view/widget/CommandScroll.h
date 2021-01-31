#ifndef COMMANDSCROLLAREA_H
#define COMMANDSCROLLAREA_H

#include "CentralWidget_enums.h"
#include "CommandEdit.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <memory>
#include <vector>

namespace view {
    class CentralWidget;

    class CommandScroll : public QScrollArea {
        Q_OBJECT

      public:
        explicit CommandScroll(CentralWidget* centralWidget);
        ~CommandScroll() override;

        void add(model::Cluster& cluster);
        void addNeeded(std::vector<model::Cluster>& clusters);
        void removeUnneeded(std::vector<model::Cluster>& clusters);
        void setShouldStashCommandEditBoxes(bool shouldStashCommandEditBoxes);
        void updateSelection();
        void disable();
        void removeFromLayout(size_t index);

        [[nodiscard]] CentralWidget* centralWidget() const;

      private slots:
        void moveFocusToNext();
        void moveFocusToPrevious();

      private:
        void stash(std::unique_ptr<CommandEdit>&& commandEditWidget);

        bool                                      m_shouldStashCommandEditBoxes = false;
        std::vector<std::unique_ptr<CommandEdit>> m_commandEditBoxes;
        std::vector<std::unique_ptr<CommandEdit>> m_stashedCommandEditBoxes;
        QVBoxLayout*                              m_layout;
        CentralWidget*                            m_centralWidget;
    };
} // namespace view

#endif // COMMANDSCROLLAREA_H
