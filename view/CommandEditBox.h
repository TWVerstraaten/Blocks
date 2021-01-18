#ifndef COMMANDEDITBOX_H
#define COMMANDEDITBOX_H

#include "TextEdit.h"

#include <QWidget>

namespace model {
    class Cluster;
    class CommandVector;
} // namespace model

namespace view {

    class CommandScrollArea;

    class CommandEditBox : public QWidget {
        Q_OBJECT

      public:
        CommandEditBox(CommandScrollArea* parent, model::Cluster& cluster);
        ~CommandEditBox() override;

        void setCommandVectorPointer();

        [[nodiscard]] TextEdit*          textEdit();
        [[nodiscard]] size_t             index() const;
        [[nodiscard]] CommandScrollArea* commandScrollArea() const;

      private slots:
        void setHeight();
        void updateCommandVector();

      private:
        size_t                m_index;
        std::string           m_name;
        TextEdit*             m_textEdit;
        model::CommandVector* m_commandVector;
        CommandScrollArea*    m_commandScrollArea;
    };
} // namespace view
#endif // COMMANDEDITBOX_H
