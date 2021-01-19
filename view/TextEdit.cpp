#include "TextEdit.h"

#include "CentralWidget.h"
#include "action/TextEditAction.h"
#include "view/color.h"

#include <QApplication>
#include <QDebug>

namespace view {
    TextEdit::TextEdit(CommandEditBox* commandEditBox, const QString& string)
        : QTextEdit(commandEditBox), m_commandEditBox(commandEditBox), m_syntaxHighlighter(new SyntaxHighlighter(document())) {
        assert(m_commandEditBox);
        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);

        setMaximumWidth(200);
        document()->adjustSize();
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        const int id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Regular.ttf");
        assert(id >= 0);
        const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
        QFont         font(family, 10);
        setFont(font);
        append(string);

        connect(document(), &QTextDocument::undoCommandAdded, this, &TextEdit::sendUndo);

        QTextEdit::setStyleSheet(QString(" selection-background-color : rgb(%1, %2, %3)")
                                     .arg(view::color::WIDGET_LIGHT.red())
                                     .arg(view::color::WIDGET_LIGHT.green())
                                     .arg(view::color::WIDGET_LIGHT.blue()));
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

    TextEdit::~TextEdit() {
        delete m_syntaxHighlighter;
    }

} // namespace view