#include "CommandEditBox.h"

#include <QVBoxLayout>

#include <QLabel>
#include <QKeyEvent>

namespace view2 {

    CommandEditBox::CommandEditBox(QWidget *parent) : QWidget(parent) {

        m_textEdit = new TextEdit(this);
        auto *l = new QVBoxLayout(this);
        l->addWidget(new QLabel("Title"));
        l->addWidget(m_textEdit);
        l->addStretch();
        l->setMargin(2);

        QPalette pal = palette();

        pal.setColor(QPalette::Background, Qt::yellow);
        setAutoFillBackground(true);
        setPalette(pal);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        connect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::setHeight);
        setHeight();
    }


    void CommandEditBox::setHeight() {
        updateGeometry();
    }

}