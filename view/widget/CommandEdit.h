#ifndef COMMANDEDITBOX_H
#define COMMANDEDITBOX_H

#include "CentralWidget_enums.h"

#include <QWidget>

namespace model {
    class Cluster;
    class CommandVector;
} // namespace model

namespace view::widget {

    class TextEditSideBar;
    class TextEdit;
    class CommandScroll;

    class CommandEdit : public QWidget {
        Q_OBJECT

      public:
        CommandEdit(CommandScroll* parent, model::Cluster& cluster);
        ~CommandEdit() override;

        void updateSelection();
        void disconnectCommandVectorUpdate();
        void connectSignals();

        [[nodiscard]] TextEdit*      textEdit();
        [[nodiscard]] size_t         index() const;
        [[nodiscard]] CommandScroll* commandScrollArea() const;

      private slots:
        void updateCommandVector();

      private:
        model::CommandVector& commandVector();

        size_t           m_index;
        std::string      m_name;
        TextEdit*        m_textEdit;
        CommandScroll*   m_commandScrollArea;
        TextEditSideBar* m_lineWidget;
        TextEditSideBar* m_commentWidget;
    };
} // namespace view
#endif // COMMANDEDITBOX_H
