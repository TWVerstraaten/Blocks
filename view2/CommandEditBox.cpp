#include "CommandEditBox.h"

#include "../model/Cluster.h"
#include "../view/color.h"
#include "CentralWidget.h"
#include "MainView.h"

#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <global/defines.h>

namespace view2 {

    CommandEditBox::CommandEditBox(CommandScrollArea* parent, model::Cluster& cluster)
        : QWidget(parent), m_index(cluster.index()), m_name(cluster.name()), m_commandVector(&cluster.commandVector()), m_commandScrollArea(parent) {
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

        m_textEdit = new TextEdit(this, text);
        auto* l    = new QVBoxLayout(this);

        QFont font("UbuntuMono-Regular", 12, QFont::Normal);
        setFont(font);
        auto* label = new QLabel(m_name.c_str(), this);
        label->setFont(font);

        l->addWidget(label);
        l->addWidget(m_textEdit);
        l->addStretch();
        l->setMargin(4);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, view::color::COMMAND_EDIT_BACKGROUND);
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

    CommandScrollArea* CommandEditBox::commandScrollArea() const {
        return m_commandScrollArea;
    }

    void CommandEditBox::setCommandVectorPointer() {
        auto it =
            std::find_if(D_IT(m_commandScrollArea->centralWidget()->mainView()->model()->clusters()), D_FUNC(cluster, cluster.index() == m_index));
        assert(it != m_commandScrollArea->centralWidget()->mainView()->model()->clusters().end());
        m_commandVector = &it->commandVector();
    }

} // namespace view2