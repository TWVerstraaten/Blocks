//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_BLOCKSELECTWIDGET_H
#define BLOCKS_BLOCKSELECTWIDGET_H

#include "view/BlockType.h"

#include <QWidget>
#include <vector>

namespace view {

    class ImageButton;

    class BlockSelectWidget : public QWidget {

      public:
        explicit BlockSelectWidget(QWidget* parent);

        [[nodiscard]] BlockType selectedBlockType() const;

      private slots:
        void setSelectedIndex(size_t selectedIndex);

      private:
        std::vector<ImageButton*> m_imageButtons;

        size_t                 m_selectedIndex = 0;
        std::vector<BlockType> m_blocks        = {view::CLUSTER_BLOCK::CLUSTER,
                                           model::FLOOR_BLOCK_TYPE::LEVEL,
                                           model::FLOOR_BLOCK_TYPE::START,
                                           model::FLOOR_BLOCK_TYPE::SPLICE,
                                           model::FLOOR_BLOCK_TYPE::CONWAY,
                                           model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW,
                                           model::DYNAMIC_BLOCK_TYPE::ROTATE_CW,
                                           model::INSTANT_BLOCK_TYPE::KILL};
    };
} // namespace view

#endif // BLOCKS_BLOCKSELECTWIDGET_H
