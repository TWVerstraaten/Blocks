//
// Created by teunv on 1/20/2021.
//

#ifndef BLOCKS_TEXTEDITSIDEBAR_H
#define BLOCKS_TEXTEDITSIDEBAR_H

#include <QString>
#include <QWidget>
#include <tuple>
#include <vector>

namespace view {

    class TextEditSideBar : public QWidget {

      public:
        explicit TextEditSideBar(QWidget* parent);

        void setWidth(size_t w);
        void setLineHeight(size_t lineHeight);
        void setTopMargin(size_t topMargin);
        void setBackgroundColor(const QColor& color);
        void addComment(size_t index, const QString& string);
        void setComments(std::vector<std::pair<size_t, QString>>&& comments);
        void clearComments();

      public slots:
        void fillLineNumbers(size_t lineNumbers);

      protected:
        void paintEvent(QPaintEvent* event) override;

      private:
        std::vector<std::pair<size_t, QString>> m_comments;
        size_t                                  m_lineHeight;
        size_t                                  m_topMargin;
    };

} // namespace view

#endif // BLOCKS_TEXTEDITSIDEBAR_H
