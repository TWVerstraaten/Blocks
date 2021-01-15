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

        QPalette pal = palette();

        pal.setColor(QPalette::Window, Qt::green);
        setAutoFillBackground(true);
        setPalette(pal);
        connect(this, &QTextEdit::textChanged, this, &TextEdit::setHeight);

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
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
        size.setHeight(size.height());
        return size;
    }

} // namespace view2