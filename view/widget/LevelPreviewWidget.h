//
// Created by teunv on 2/2/2021.
//

#ifndef BLOCKS_LEVELPREVIEWWIDGET_H
#define BLOCKS_LEVELPREVIEWWIDGET_H

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

namespace view::widget {

    class LevelPreviewWidget : public QWidget {

        Q_OBJECT

      public:
        LevelPreviewWidget(QWidget* parent, const QString& path);

      signals:

        void levelSelected(const std::string& path);

      private:
        void buildScrollArea();
        void buildPreviewLabel();

        bool eventFilter(QObject* watched, QEvent* event) override;

        const QString m_path;
        QLabel*       m_label  = nullptr;
        QHBoxLayout*  m_layout = nullptr;
    };

} // namespace view::widget

#endif // BLOCKS_LEVELPREVIEWWIDGET_H
