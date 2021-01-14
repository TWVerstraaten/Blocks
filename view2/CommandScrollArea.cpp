#include "CommandScrollArea.h"

#include <QDebug>
namespace view2 {

    CommandScrollArea::CommandScrollArea(QWidget* parent) : QScrollArea(parent) {
        QWidget* widget = new QWidget(parent);
        m_layout        = new QVBoxLayout(widget);
        m_layout->addStretch();
        setWidget(widget);

        setMinimumWidth(120);
        setMaximumWidth(140);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        for (int i = 0; i < 3; i++) {
            add();
        }

        setWidgetResizable(true);
        resize(sizeHint());
    }

    void CommandScrollArea::add() {
        m_commandEditBoxes.emplace_back(new CommandEditBox(parentWidget()));
        connect(m_commandEditBoxes.back()->textEdit(), &TextEdit::tabPressed, this, &CommandScrollArea::moveFocusToNext);
        connect(m_commandEditBoxes.back()->textEdit(), &TextEdit::backTabPressed, this, &CommandScrollArea::moveFocusToPrevious);
        m_layout->insertWidget(m_layout->count() - 1, m_commandEditBoxes.back());
    }

    void CommandScrollArea::remove() {
        if (m_commandEditBoxes.empty()) {
            return;
        }
        m_layout->removeWidget(m_commandEditBoxes[0]);
        delete m_commandEditBoxes[0];
        m_commandEditBoxes.erase(m_commandEditBoxes.begin());
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
} // namespace view2