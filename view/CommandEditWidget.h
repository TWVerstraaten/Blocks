#ifndef COMMANDEDITBOX_H
#define COMMANDEDITBOX_H

#include "CentralWidget_enums.h"

#include <QWidget>

namespace model {
    class Cluster;
    class CommandVector;
} // namespace model

namespace view {

    class TextEditCommentWidget;
    class TextEdit;
    class CommandScrollArea;

    class CommandEditWidget : public QWidget {
        Q_OBJECT

      public:
        CommandEditWidget(CommandScrollArea* parent, model::Cluster& cluster);
        ~CommandEditWidget() override;

        void setCommandVectorPointer();
        void updateSelection();
        void disconnectCommandVectorUpdate();

        [[nodiscard]] TextEdit*             textEdit();
        [[nodiscard]] size_t                index() const;
        [[nodiscard]] CommandScrollArea*    commandScrollArea() const;
        [[nodiscard]] model::CommandVector* commandVector();

      private slots:
        void updateCommandVector();

      private:
        size_t                 m_index;
        std::string            m_name;
        TextEdit*              m_textEdit;
        model::CommandVector*  m_commandVector;
        CommandScrollArea*     m_commandScrollArea;
        TextEditCommentWidget* m_lineWidget;
        TextEditCommentWidget* m_commentWidget;
    };
} // namespace view
#endif // COMMANDEDITBOX_H
