#include "TextEdit.h"

#include "../../action/TextEditAction.h"
#include "../../model/cmd/CommandParser.h"
#include "../../view/FontManager.h"
#include "../../view/color.h"
#include "CentralWidget.h"
#include "CommandScroll.h"
#include "SyntaxHighlighter.h"
#include "Window.h"

#include <QApplication>

namespace view::widget {

    TextEdit::TextEdit(CommandEdit* commandEditBox, const QString& string)
        : QPlainTextEdit(commandEditBox), m_commandEditBox(commandEditBox), m_syntaxHighlighter(new SyntaxHighlighter(document())) {
        setContextMenuPolicy(Qt::NoContextMenu);

        installEventFilter(m_commandEditBox->commandScrollArea()->centralWidget()->mainWindow());

        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        const int fontSize = 12;
        setFont(FontManager::font(FONT_ENUM::UBUNTU_MONO_BOLD, fontSize));
        m_lineHeight = static_cast<size_t>(fontMetrics().height());

        document()->adjustSize();
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

        appendPlainText(string);
        setHeight();
    }

    TextEdit::~TextEdit() {
        disconnect();
        delete m_syntaxHighlighter;
    }

    void TextEdit::keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
            case Qt::Key_Escape:
                clearFocus();
                m_commandEditBox->commandScrollArea()->centralWidget()->setFocus();
                break;
            case Qt::Key_Tab:
                emit tabPressed();
                break;
            case Qt::Key_Backtab:
                emit backTabPressed();
                break;
            default:
                QPlainTextEdit::keyPressEvent(event);
        }
    }

    void TextEdit::setHeight() {
        updateGeometry();
        QSize size = document()->size().toSize();
        setFixedHeight(static_cast<int>((size.height() + 1) * m_lineHeight + 1));
    }

    std::vector<std::string> TextEdit::contents() const {
        QString data = toPlainText();
#ifdef Q_OS_WIN
        QStringList stringList = data.split(QRegExp("[\n]"), Qt::SplitBehaviorFlags::KeepEmptyParts);
#else
        QStringList stringList = data.split(QRegExp("[\n]"), QString::KeepEmptyParts);
#endif
        std::vector<std::string> result;
        for (const auto& it : stringList) {
            result.emplace_back(it.toStdString());
        }
        return result;
    }

    QSize TextEdit::sizeHint() const {
        QSize size = document()->size().toSize();
        return size;
    }

    void TextEdit::sendUndo() {
        m_commandEditBox->commandScrollArea()->centralWidget()->addAction(new action::TextEditAction(this));
    }

    void TextEdit::setSelection(size_t actionIndex) {
        QTextCursor  cursor     = QTextCursor(document());
        const size_t lineNumber = nThOpaqueLine(actionIndex);
        for (size_t i = 0; i != lineNumber; ++i) {
            cursor.movePosition(QTextCursor::Down);
        }
        highlightLine(cursor);
    }

    void TextEdit::highlightLine(const QTextCursor& cursor) {
        QList<QTextEdit::ExtraSelection> extraSelections;
        QTextEdit::ExtraSelection        selection;
        selection.cursor = cursor;

        selection.format.setBackground(view::color::WIDGET_LIGHT.lighter(110));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        extraSelections.append(selection);
        setExtraSelections(extraSelections);
    }

    size_t TextEdit::nThOpaqueLine(size_t n) const {
        int i = 0;
        while (model::CommandParser::isCommentOrEmpty(document()->findBlockByLineNumber(i).text().toStdString())) {
            ++i;
        }
        while (n != 0) {
            ++i;
            --n;
            while (model::CommandParser::isCommentOrEmpty(document()->findBlockByLineNumber(i).text().toStdString())) {
                ++i;
            }
        }
        return static_cast<size_t>(i);
    }

    size_t TextEdit::lineHeight() const {
        return m_lineHeight;
    }

    size_t TextEdit::topMargin() const {
        return static_cast<size_t>(contentsMargins().top());
    }

    void TextEdit::connectSignals() {
        connect(this, &TextEdit::textChanged, this, &TextEdit::setHeight);
        connect(document(), &QTextDocument::undoCommandAdded, this, &TextEdit::sendUndo);
        connect(this, &TextEdit::cursorPositionChanged, [this]() { highlightLine(textCursor()); });
    }

} // namespace view::widget