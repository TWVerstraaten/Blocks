#ifndef BLOCKS_COMMANDEDIT_H
#define BLOCKS_COMMANDEDIT_H

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
        TextEdit*        m_textEdit          = nullptr;
        CommandScroll*   m_commandScrollArea = nullptr;
        TextEditSideBar* m_lineWidget        = nullptr;
        TextEditSideBar* m_commentWidget     = nullptr;
    };
} // namespace view::widget
#endif // BLOCKS_COMMANDEDIT_H
