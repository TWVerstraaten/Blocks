//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_LEVELSELECTWIDGET_H
#define BLOCKS_LEVELSELECTWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

namespace view::widget {

    class LevelSelectWidget : public QWidget {
        Q_OBJECT

      public:
        explicit LevelSelectWidget(QWidget* parent);

      signals:
        void levelSelected(const std::string& path);

      private slots:
        void populatePreviewWidget(const QString& path);

      private:
        void buildScrollArea();

        QHBoxLayout* m_hBoxLayout;
        QWidget*     m_rightWidget;
    };

} // namespace view::widget

#endif // BLOCKS_LEVELSELECTWIDGET_H
