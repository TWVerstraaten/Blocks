#ifndef COMMANDEDITBOX_H
#define COMMANDEDITBOX_H

#include "TextEdit.h"

#include <QDebug>
#include <QWidget>

namespace model {
    class Cluster;
    class CommandVector;
} // namespace model

namespace view2 {

    class CommandEditBox : public QWidget {
        Q_OBJECT

      public:
        CommandEditBox(QWidget* parent, model::Cluster& cluster);

        [[nodiscard]] TextEdit* textEdit();
        [[nodiscard]] size_t    index() const;

      private slots:
        void setHeight();

      private:
        size_t                m_index;
        std::string           m_name;
        TextEdit*             m_textEdit;
        model::CommandVector* m_commandVector = nullptr;
    };
} // namespace view2
#endif // COMMANDEDITBOX_H
