#include "CommandEdit.h"

#include "../../model/Model.h"
#include "../FontManager.h"
#include "CentralWidget.h"
#include "CommandScroll.h"
#include "MainView.h"
#include "TextEdit.h"
#include "TextEditSideBar.h"

namespace view::widget {
    CommandEdit::CommandEdit(CommandScroll* parent, model::Cluster& cluster)
        : QWidget(parent), m_index(cluster.index()), m_name(cluster.name()), m_commandScrollArea(parent), m_lineWidget(new TextEditSideBar(this)),
          m_commentWidget(new TextEditSideBar(this)) {

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
        auto* l    = new QGridLayout(this);

        auto* label = new QLabel(m_name.c_str(), this);
        label->setFont(FontManager ::font(FONT_ENUM::ANON_PRO_BOLD, 11));

        l->addWidget(label, 0, 0, 1, 3);
        m_lineWidget->setWidth(30);
        m_lineWidget->setLineHeight(m_textEdit->lineHeight());
        m_lineWidget->setTopMargin(m_textEdit->topMargin());
        m_lineWidget->fillLineNumbers(static_cast<size_t>(m_textEdit->document()->blockCount()));

        connectSignals();

        m_commentWidget->setWidth(30);
        m_commentWidget->setLineHeight(m_textEdit->lineHeight());
        //        m_commentWidget->setBackgroundColor(view::color::WIDGET_LIGHT.lighter(115));
        m_commentWidget->setTopMargin(m_textEdit->topMargin());

        l->addWidget(m_lineWidget, 1, 0);
        l->addWidget(m_textEdit, 1, 1);
        l->addWidget(m_commentWidget, 1, 2);
        l->setMargin(4);

        l->setHorizontalSpacing(0);
        l->setVerticalSpacing(0);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    }

    CommandEdit::~CommandEdit() {
        disconnect();
    }

    TextEdit* CommandEdit::textEdit() {
        return m_textEdit;
    }

    size_t CommandEdit::index() const {
        return m_index;
    }

    CommandScroll* CommandEdit::commandScrollArea() const {
        return m_commandScrollArea;
    }

    void CommandEdit::updateCommandVector() {
        commandVector().set(m_textEdit->contents());
    }

    model::CommandVector& CommandEdit::commandVector() {
        return m_commandScrollArea->centralWidget()->mainView()->model()->clusterWithIndex(m_index)->commandVector();
    }

    void CommandEdit::updateSelection() {
        const auto& cmdVec = commandVector();
        if (cmdVec.isEmpty()) {
            return;
        }
        m_textEdit->setSelection(cmdVec.commandIndex());
        m_commentWidget->clearComments();
        m_commentWidget->update();
        if (cmdVec.currentIsRepeat()) {
            m_commentWidget->addComment(m_textEdit->nThOpaqueLine(cmdVec.commandIndex()), QString("%1").arg(cmdVec.repeatCount() + 1));
        }
        update();
    }

    void CommandEdit::disconnectCommandVectorUpdate() {
        m_textEdit->blockSignals(true);
    }

    void CommandEdit::connectSignals() {
        connect(m_textEdit, &TextEdit::textChanged, [this]() {
            m_lineWidget->fillLineNumbers(static_cast<size_t>(m_textEdit->document()->blockCount()));
        });
        m_textEdit->connectSignals();
        connect(m_textEdit, &TextEdit::textChanged, this, &CommandEdit::updateCommandVector);
    }

} // namespace view::widget