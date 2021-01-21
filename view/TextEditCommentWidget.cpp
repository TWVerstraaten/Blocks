//
// Created by teunv on 1/20/2021.
//

#include "TextEditCommentWidget.h"

#include "FontManager.h"

#include <QFontDatabase>
#include <QPainter>
namespace view {

    TextEditCommentWidget::TextEditCommentWidget(QWidget* parent) : QWidget(parent) {
        setFont(FontManager::font(FONT_ENUM::UBUNTU_REGULAR, 9));

        QPalette palette = this->palette();
        palette.setColor(QPalette::Text, Qt::black);
        setPalette(palette);
    }

    void TextEditCommentWidget::setWidth(size_t w) {
        setFixedWidth(w);
    }

    void TextEditCommentWidget::setLineHeight(size_t lineHeight) {
        m_lineHeight = lineHeight;
    }

    void TextEditCommentWidget::setBackgroundColor(const QColor& color) {
        QPalette pal = palette();
        pal.setColor(QPalette::Window, color);
        setAutoFillBackground(true);
        setPalette(pal);
    }

    void TextEditCommentWidget::paintEvent(QPaintEvent* event) {
        QWidget::paintEvent(event);
        QPainter painter(this);
        painter.setPen(Qt::darkGray);
        for (const auto& [index, comment] : m_comments) {
            painter.drawText(2, m_topMargin + m_lineHeight * (index + 1), comment);
        }
    }

    void TextEditCommentWidget::addComment(size_t index, const QString& string) {
        m_comments.emplace_back(index, string);
        update();
    }

    void TextEditCommentWidget::setComments(std::vector<std::pair<size_t, QString>>&& comments) {
        m_comments = std::move(comments);
    }

    void TextEditCommentWidget::setTopMargin(size_t topMargin) {
        m_topMargin = topMargin;
    }

    void TextEditCommentWidget::fillLineNumbers(size_t lineNumbers) {
        m_comments.clear();
        for (size_t i = 0; i != lineNumbers; ++i) {
            addComment(i, QString("%1.").arg(i + 1));
        }
    }

    void TextEditCommentWidget::clearComments() {
        m_comments = {};
    }
} // namespace view