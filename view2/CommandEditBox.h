#ifndef COMMANDEDITBOX_H
#define COMMANDEDITBOX_H

#include <QWidget>

#include "TextEdit.h"
#include <QDebug>

namespace view2 {

    class CommandEditBox : public QWidget {
    Q_OBJECT
    public:
        explicit CommandEditBox(QWidget *parent = nullptr);

        TextEdit *textEdit() { return m_textEdit; }


    private:
        void setHeight();

    signals:

        void moveFocusToNext();

        void textChanged();

    private:
        TextEdit *m_textEdit;
    };
}
#endif // COMMANDEDITBOX_H
