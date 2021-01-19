#include "CommandEditBox.h"

#include "../model/command/CommandParser.h"
#include "CentralWidget.h"
#include "MainView.h"
#include "color.h"
#include "global/defines.h"

#include <QLabel>

namespace view {

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

        const int id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Bold.ttf");
        assert(id >= 0);
        const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont         font(family, 11);
        setFont(font);

        auto* label = new QLabel(m_name.c_str(), this);
        label->setFont(font);

        l->addWidget(label);
        l->addWidget(m_textEdit);
        l->addStretch();
        l->setMargin(4);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, view::color::WIDGET_LIGHT);
        setAutoFillBackground(true);
        setPalette(pal);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        connect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::updateCommandVector);
        connect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::setHeight);
        m_textEdit->setHeight();
        update();
    }

    CommandEditBox::~CommandEditBox() {
        disconnect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::updateCommandVector);
        disconnect(m_textEdit, &QTextEdit::textChanged, this, &CommandEditBox::setHeight);
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

    void CommandEditBox::updateCommandVector() {
        m_commandVector->set(m_textEdit->contents());
    }

    model::CommandVector* CommandEditBox::commandVector() {
        return m_commandVector;
    }

    void CommandEditBox::updateSelection() {
        assert(m_commandVector);
        auto index = m_commandVector->commandIndex();

        QTextCharFormat textFormat = m_textEdit->currentCharFormat();
        textFormat.setBackground(Qt::yellow);

        QTextCharFormat f;
        QTextCursor     c(m_textEdit->document());
        c.setPosition(m_textEdit->document()->characterCount() - 1, QTextCursor::KeepAnchor);
        c.setCharFormat(f);

        QString data = m_textEdit->toPlainText();
#ifdef Q_OS_WIN
        QStringList stringList = data.split(QRegExp("[\n]"), Qt::SplitBehaviorFlags::KeepEmptyParts);
#else
        QStringList stringList = data.split(QRegExp("[\n]"), QString::KeepEmptyParts);
#endif

        if (stringList.empty()) {
            return;
        }

        size_t blockIndex = 0;
        while (true) {
            while (model::CommandParser::isCommentOrEmpty(stringList[blockIndex].toStdString())) {
                ++blockIndex;
            }
            if (index == 0) {
                break;
            }
            --index;
            ++blockIndex;
        }
        const auto block    = m_textEdit->document()->findBlockByLineNumber(blockIndex);
        const auto blockPos = block.position();

        auto cursor = QTextCursor(m_textEdit->document());
        cursor.setPosition(blockPos);
        cursor.select(cursor.LineUnderCursor);
        cursor.setCharFormat(textFormat);
    }

    void CommandEditBox::disconnectCommandVectorUpdate() {
        m_textEdit->blockSignals(true);
    }

} // namespace view