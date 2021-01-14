#include "TextEdit.h"

#include <QDebug>
#include <QFontDatabase>
#include <QKeyEvent>

namespace view2 {
    TextEdit::TextEdit(QWidget* parent) : QTextEdit(parent) {
        QFont font("UbuntuMono-Regular", 10, QFont::Normal);
        setFont(font);
        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        setFixedHeight(QFontMetrics(font).height() + 3);
        connect(this, &QTextEdit::textChanged, [this]() {
            const auto l = contents();
            for (const auto& s : l) {
                qDebug() << s;
            }
        });
    }

    void TextEdit::keyPressEvent(QKeyEvent* e) {
        switch (e->key()) {
            case Qt::Key_Escape:
                clearFocus();
                break;
            case Qt::Key_Tab:
                emit tabPressed();
                qDebug() << "Adas";
                break;
            case Qt::Key_Backtab:
                emit backTabPressed();
                qDebug() << "Adas";
                break;
            default:
                QTextEdit::keyPressEvent(e);
        }
    }

    void TextEdit::setHeight() {
        updateGeometry();
        QSize size = document()->size().toSize();
        setFixedHeight(size.height() + 3);
    }

    QStringList TextEdit::contents() const {
        QString     data   = toPlainText();
        QStringList result = data.split(QRegExp("[\n]"), QString::SkipEmptyParts);
        return result;
    }
} // namespace view2