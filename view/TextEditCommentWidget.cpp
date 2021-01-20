//
// Created by teunv on 1/20/2021.
//

#include "TextEditCommentWidget.h"

#include "color.h"

#include <QFontDatabase>
#include <QPainter>

view::TextEditCommentWidget::TextEditCommentWidget(QWidget* parent) : QWidget(parent) {
    const int id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Regular.ttf");
    assert(id >= 0);
    const QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont         font(family, 10);
    setFont(font);

    QPalette palette = this->palette();
    palette.setColor(QPalette::Text, Qt::black);
    setPalette(palette);
}

void view::TextEditCommentWidget::setWidth(size_t w) {
    setFixedWidth(w);
}

void view::TextEditCommentWidget::setLineHeight(size_t lineHeight) {
    m_lineHeight = lineHeight;
}

void view::TextEditCommentWidget::setBackgroundColor(const QColor& color) {
    QPalette pal = palette();
    pal.setColor(QPalette::Window, color);
    setAutoFillBackground(true);
    setPalette(pal);
}

void view::TextEditCommentWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.setPen(Qt::darkGray);
    for (const auto& [index, comment] : m_comments) {
        painter.drawText(2, m_topMargin + m_lineHeight * (index + 1), comment);
    }
}

void view::TextEditCommentWidget::addComment(size_t index, const QString& string) {
    m_comments.emplace_back(index, string);
}

void view::TextEditCommentWidget::setComments(std::vector<std::pair<size_t, QString>>&& comments) {
    m_comments = std::move(comments);
}

void view::TextEditCommentWidget::setTopMargin(size_t topMargin) {
    m_topMargin = topMargin;
}

void view::TextEditCommentWidget::fillLineNumbers(size_t lineNumbers) {
    m_comments.clear();
    for (size_t i = 0; i != lineNumbers; ++i) {
        addComment(i, QString("%1.").arg(i + 1));
    }
}

void view::TextEditCommentWidget::clearComments() {
    m_comments = {};
}
