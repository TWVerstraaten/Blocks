#include "TextEdit.h"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QTime>
#include <QTimer>

namespace view2 {
    TextEdit::TextEdit(QWidget* parent, const QString& string, CommandEditBox* commandEditBox) : QTextEdit(parent), m_commandEditBox(commandEditBox) {

        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);

        setMaximumWidth(200);
        document()->adjustSize();

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        QFont font("UbuntuMono-Regular", 10, QFont::Normal);
        setFont(font);
        append(string);

        connect(document(), &QTextDocument::undoCommandAdded, [this]() { qDebug() << QTime::currentTime(); });
    }

    void TextEdit::keyPressEvent(QKeyEvent* event) {
        if ((QApplication::keyboardModifiers() & Qt::ControlModifier) && event->key() == Qt::Key_Z) {
            if (QApplication::keyboardModifiers() & Qt::ShiftModifier) {
                qDebug() << "Redo intercepted";
            } else {
                qDebug() << "Undo intercepted";
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
        QString                  data       = toPlainText();
        QStringList              stringList = data.split(QRegExp("[\n]"), QString::KeepEmptyParts);
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

} // namespace view2