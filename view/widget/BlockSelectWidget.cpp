//
// Created by teunv on 1/18/2021.
//

#include "BlockSelectWidget.h"

#include "ImageButton.h"

#include <QGridLayout>

namespace view::widget {

    BlockSelectWidget::BlockSelectWidget(QWidget* parent) : QWidget(parent) {
        auto* l = new QGridLayout(this);
        for (size_t i = 0; i != m_blocks.size(); ++i) {
            auto* button = new ImageButton(this, m_blocks.at(i), QSize{50, 50});
            connect(button, &QPushButton::clicked, [i, this]() { setSelectedIndex(i); });
            l->addWidget(button, 0, static_cast<int>(i));
            m_imageButtons.push_back(button);
        }

        l->setSpacing(16);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
        setSelectedIndex(0);
        setAttribute(Qt::WA_StyledBackground, true);
    }

    BlockSelectWidget::~BlockSelectWidget() {
        disconnect();
    }

    BlockType BlockSelectWidget::selectedBlockType() const {
        return m_blocks.at(m_selectedIndex);
    }

    void BlockSelectWidget::setSelectedIndex(size_t selectedIndex) {
        assert(selectedIndex < m_blocks.size());
        m_selectedIndex = selectedIndex;
        for (size_t i = 0; i != m_blocks.size(); ++i) {
            if (i != selectedIndex) {
                m_imageButtons[i]->setUnSelected();
            }
        }
        m_imageButtons[m_selectedIndex]->setSelected();
    }

} // namespace view::widget