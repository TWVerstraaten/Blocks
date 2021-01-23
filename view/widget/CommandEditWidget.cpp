#include "CommandEditWidget.h"

#include "../../misc/defines.h"
#include "../../model/Model.h"
#include "../FontManager.h"
#include "../color.h"
#include "CentralWidget.h"
#include "CommandScrollArea.h"
#include "TextEdit.h"
#include "TextEditCommentWidget.h"

#include <QLabel>

namespace view {
    CommandEditWidget::CommandEditWidget(CommandScrollArea* parent, model::Cluster& cluster)
        : QWidget(parent), m_index(cluster.index()), m_name(cluster.name()), m_commandScrollArea(parent),
          m_lineWidget(new TextEditCommentWidget(this)), m_commentWidget(new TextEditCommentWidget(this)) {
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
        m_lineWidget->setBackgroundColor(view::color::WIDGET_LIGHT.lighter(115));
        m_lineWidget->setLineHeight(m_textEdit->lineHeight());
        m_lineWidget->setTopMargin(m_textEdit->topMargin());
        m_lineWidget->fillLineNumbers(m_textEdit->document()->blockCount());

        connect(m_textEdit, &TextEdit::textChanged, [this]() { m_lineWidget->fillLineNumbers(m_textEdit->document()->blockCount()); });

        m_commentWidget->setWidth(30);
        m_commentWidget->setLineHeight(m_textEdit->lineHeight());
        m_commentWidget->setBackgroundColor(view::color::WIDGET_LIGHT.lighter(115));
        m_commentWidget->setTopMargin(m_textEdit->topMargin());

        l->addWidget(m_lineWidget, 1, 0);
        l->addWidget(m_textEdit, 1, 1);
        l->addWidget(m_commentWidget, 1, 2);
        l->setMargin(4);

        l->setHorizontalSpacing(0);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, view::color::WIDGET_LIGHT);
        setAutoFillBackground(true);
        setPalette(pal);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        connect(m_textEdit, &TextEdit::textChanged, this, &CommandEditWidget::updateCommandVector);
    }

    CommandEditWidget::~CommandEditWidget() {
        disconnect();
    }

    TextEdit* CommandEditWidget::textEdit() {
        return m_textEdit;
    }

    size_t CommandEditWidget::index() const {
        return m_index;
    }

    CommandScrollArea* CommandEditWidget::commandScrollArea() const {
        return m_commandScrollArea;
    }

    void CommandEditWidget::updateCommandVector() {
        commandVector().set(m_textEdit->contents());
    }

    model::CommandVector& CommandEditWidget::commandVector() {
        return m_commandScrollArea->centralWidget()->mainView()->model()->clusterWithIndex(m_index)->commandVector();
    }

    void CommandEditWidget::updateSelection() {
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

    void CommandEditWidget::disconnectCommandVectorUpdate() {
        m_textEdit->blockSignals(true);
    }

} // namespace view