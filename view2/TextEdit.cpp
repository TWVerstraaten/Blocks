#include "TextEdit.h"

#include <QDebug>
#include <QFontDatabase>
#include <QKeyEvent>
#include <QTimer>

namespace view2 {
    TextEdit::TextEdit(QWidget* parent, const QString& string) : QTextEdit(parent) {

        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);

        setMaximumWidth(200);
        document()->adjustSize();

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);

        QFont font("UbuntuMono-Regular", 10, QFont::Normal);
        setFont(font);
        append(string);
    }

    void TextEdit::keyPressEvent(QKeyEvent* event) {
        switch (event->key()) {
            case Qt::Key_Escape:
                clearFocus();
                break;
            case Qt::Key_Tab:
                emit tabPressed();
                break;
            case Qt::Key_Backtab:
                emit backTabPressed();
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
    }

    std::vector<std::string> TextEdit::contents() const {
        QString                  data       = toPlainText();
        QStringList              stringList = data.split(QRegExp("[\n]"), Qt::KeepEmptyParts);
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