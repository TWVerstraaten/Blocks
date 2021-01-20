#include "TextEdit.h"

#include "../model/command/CommandParser.h"
#include "CentralWidget.h"
#include "action/TextEditAction.h"
#include "view/color.h"

#include <QApplication>
#include <QDebug>

namespace view {
    TextEdit::TextEdit(CommandEditWidget* commandEditBox, const QString& string)
        : QTextEdit(commandEditBox), m_commandEditBox(commandEditBox), m_syntaxHighlighter(new SyntaxHighlighter(document())) {
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        const int id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Regular.ttf");
        assert(id >= 0);
        const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont         font(family, 10);
        setFont(font);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Highlight, QColor(view::color::WIDGET_LIGHT.lighter(110)));
        palette.setColor(QPalette::HighlightedText, QColor(Qt::black));
        setPalette(palette);

        setMaximumWidth(200);
        document()->adjustSize();
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);

        append(string);

        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);
        connect(document(), &QTextDocument::undoCommandAdded, this, &TextEdit::sendUndo);
        connect(this, &QTextEdit::cursorPositionChanged, [this]() { highlightLine(textCursor()); });
    }

    TextEdit::~TextEdit() {
        delete m_syntaxHighlighter;
    }

    void TextEdit::keyPressEvent(QKeyEvent* event) {
        if ((QApplication::keyboardModifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Z) {
            if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
                qDebug() << "Redo";
                m_commandEditBox->commandScrollArea()->centralWidget()->redo();
            } else {
                qDebug() << "Undo";
                m_commandEditBox->commandScrollArea()->centralWidget()->undo();
            }
            return;
        }

        switch (event->key()) {
            case Qt::Key_Escape:
                clearFocus();
                m_commandEditBox->commandScrollArea()->centralWidget()->setFocus();
                update();
                break;
            case Qt::Key_Tab:
                emit tabPressed();
                update();
                break;
            case Qt::Key_Backtab:
                emit backTabPressed();
                update();
                break;
            default:
                QTextEdit::keyPressEvent(event);
        }
    }

    void TextEdit::setHeight() {
        updateGeometry();
        QSize size = document()->size().toSize();
        setMaximumHeight(size.height());
        setMinimumHeight(size.height());
        update();
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
        assert(m_commandEditBox);
        assert(m_commandEditBox->commandScrollArea());
        assert(m_commandEditBox->commandScrollArea()->centralWidget());
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

    size_t TextEdit::nThOpaqueLine(size_t n) {
        size_t i = 0;
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
        return i;
    }

    size_t TextEdit::lineHeight() const {
        return QFontMetrics(font()).height();
    }

    size_t TextEdit::topMargin() const {
        return document()->documentMargin();
    }

} // namespace view