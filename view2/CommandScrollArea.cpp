#include "CommandScrollArea.h"

#include "global/defines.h"

#include <QDebug>
#include <QScrollBar>
#include <algorithm>

namespace view2 {

    CommandScrollArea::CommandScrollArea(QWidget* parent) : QScrollArea(parent), m_undoGroup(this) {
        auto* widget = new QWidget(parent);
        m_layout     = new QVBoxLayout(widget);
        m_layout->addStretch();
        setWidget(widget);

        setMaximumWidth(200);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

        setWidgetResizable(true);
    }

    void CommandScrollArea::moveFocusToNext() {
        if (m_commandEditBoxes.size() < 2) {
            return;
        }
        for (auto it = m_commandEditBoxes.begin(); std::next(it) != m_commandEditBoxes.end(); ++it) {
            if ((*it)->textEdit()->hasFocus()) {
                (*std::next(it))->textEdit()->setFocus();
                return;
            }
        }
    }

    void CommandScrollArea::moveFocusToPrevious() {
        if (m_commandEditBoxes.size() < 2) {
            return;
        }
        for (auto it = std::next(m_commandEditBoxes.begin()); it != m_commandEditBoxes.end(); ++it) {
            if ((*it)->textEdit()->hasFocus()) {
                (*std::prev(it))->textEdit()->setFocus();
                return;
            }
        }
    }

    void CommandScrollArea::removeUnnecessary(const std::list<model::Cluster>& clusters) {
        for (size_t i = 0; i != m_commandEditBoxes.size(); ++i) {
            if (std::find_if(D_CIT(clusters), D_FUNC(cluster, cluster.index() == m_commandEditBoxes.at(i)->index())) == clusters.end()) {
                m_layout->removeWidget(m_commandEditBoxes[i]);
                delete m_commandEditBoxes[i];
                m_commandEditBoxes[i] = nullptr;
            }
        }
        m_commandEditBoxes.erase(std::remove_if(D_IT(m_commandEditBoxes), D_FUNC(box, box == nullptr)), m_commandEditBoxes.end());
    }

    void CommandScrollArea::addNeeded(std::list<model::Cluster>& clusters) {
        for (auto& cluster : clusters) {
            if (std::find_if(D_CIT(m_commandEditBoxes), D_FUNC(box, box->index() == cluster.index())) == m_commandEditBoxes.end()) {
                add(cluster);
            }
        }
    }

    void CommandScrollArea::add(model::Cluster& cluster) {
        auto* commandEditBox = new CommandEditBox(parentWidget(), cluster);
        m_commandEditBoxes.emplace_back(commandEditBox);
        connect(commandEditBox->textEdit(), &TextEdit::tabPressed, this, &CommandScrollArea::moveFocusToNext);
        connect(commandEditBox->textEdit(), &TextEdit::backTabPressed, this, &CommandScrollArea::moveFocusToPrevious);
        m_layout->insertWidget(m_layout->count() - 1, commandEditBox);
    }
} // namespace view2
