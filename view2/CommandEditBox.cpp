#include "CommandEditBox.h"

#include "../model/Cluster.h"

#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <utility>

namespace view2 {

    CommandEditBox::CommandEditBox(QWidget* parent, model::Cluster& cluster)
        : QWidget(parent), m_index(cluster.index()), m_name(cluster.name()), m_commandVector(&cluster.commandVector()) {
        m_textEdit = new TextEdit(this);
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
    }

    TextEdit* CommandEditBox::textEdit() {
        return m_textEdit;
    }

    size_t CommandEditBox::index() const {
        return m_index;
    }

    void CommandEditBox::resizeEvent(QResizeEvent* event) {
        m_textEdit->setHeight();
        QWidget::resizeEvent(event);
    }

} // namespace view2