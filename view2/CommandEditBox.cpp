#include "CommandEditBox.h"

#include "../model/Cluster.h"

#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>

namespace view2 {

    CommandEditBox::CommandEditBox(QWidget* parent, model::Cluster& cluster)
        : QWidget(parent), m_index(cluster.index()), m_name(cluster.name()), m_commandVector(&cluster.commandVector()) {

        setContentsMargins(0, 0, 0, 0);
        setMaximumWidth(200);

        const auto& strings = cluster.commandVector().strings();
        QString     text;
        if (not strings.empty()) {
            text.push_back(strings.front().c_str());
            for (size_t i = 1; i != strings.size(); ++i) {
                text += '\n';
                text += strings[i].c_str();
            }
        }

        m_textEdit = new TextEdit(this, text, nullptr);
        auto* l    = new QVBoxLayout(this);
        l->addWidget(new QLabel(m_name.c_str()));
        l->addWidget(m_textEdit);
        l->addStretch();
        l->setMargin(2);
        QPalette pal = palette();

        pal.setColor(QPalette::Window, Qt::yellow);
        setAutoFillBackground(true);
        setPalette(pal);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        connect(m_textEdit, &QTextEdit::textChanged, [this]() { m_commandVector->set(m_textEdit->contents()); });
        connect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::setHeight);
        m_textEdit->setHeight();
        update();
    }

    TextEdit* CommandEditBox::textEdit() {
        return m_textEdit;
    }

    size_t CommandEditBox::index() const {
        return m_index;
    }

    void CommandEditBox::setHeight() {
        m_textEdit->setHeight();
    }

    //    void CommandEditBox::resizeEvent(QResizeEvent* event) {
    //        m_textEdit->setHeight();
    //        QWidget::resizeEvent(event);
    //    }

} // namespace view2