#include "CommandScroll.h"

#include "../../misc/defines.h"
#include "../../model/Cluster.h"
#include "../../view/color.h"
#include "CentralWidget.h"
#include "TextEdit.h"

#include <QDebug>
#include <QScrollBar>

namespace view {

    CommandScroll::CommandScroll(CentralWidget* centralWidget) : QScrollArea(centralWidget), m_centralWidget(centralWidget) {
        auto* widget = new QWidget(centralWidget);
        m_layout     = new QVBoxLayout(widget);
        m_layout->addStretch();
        setWidget(widget);

        setMaximumWidth(200);
        setMinimumWidth(200);
        setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

        setWidgetResizable(true);

        QPalette pal = palette();
        pal.setColor(QPalette::Window, view::color::WIDGET_DARK);
        setAutoFillBackground(true);
        setPalette(pal);
    }

    CommandScroll::~CommandScroll() {
        disconnect();
        for (auto& commandEditBox : m_commandEditBoxes) {
            m_layout->removeWidget(commandEditBox.get());
        }
    }

    void CommandScroll::moveFocusToNext() {
        if (m_commandEditBoxes.size() > 1) {
            auto it = std::find_if(D_RIT(m_commandEditBoxes), D_FUNC(commandEditWidget, commandEditWidget->textEdit()->hasFocus()));
            if (it != m_commandEditBoxes.rend() && it != m_commandEditBoxes.rbegin()) {
                (*std::prev(it))->textEdit()->setFocus();
            }
        }
    }

    void CommandScroll::moveFocusToPrevious() {
        if (m_commandEditBoxes.size() > 1) {
            auto it = std::find_if(D_IT(m_commandEditBoxes), D_FUNC(commandEditWidget, commandEditWidget->textEdit()->hasFocus()));
            if (it != m_commandEditBoxes.end() && it != m_commandEditBoxes.begin()) {
                (*std::prev(it))->textEdit()->setFocus();
            }
        }
    }

    void CommandScroll::addNeeded(std::vector<model::Cluster>& clusters) {
        for (auto& cluster : clusters) {
            if (std::find_if(D_CIT(m_commandEditBoxes), D_FUNC(box, box->index() == cluster.index())) == m_commandEditBoxes.end()) {
                add(cluster);
            }
        }
    }

    void CommandScroll::add(model::Cluster& cluster) {
        assert(std::find_if(D_CIT(m_commandEditBoxes), D_FUNC(box, box->index() == cluster.index())) == m_commandEditBoxes.cend());

        auto it = std::find_if(D_IT(m_stashedCommandEditBoxes), D_FUNC(widget, widget->index() == cluster.index()));
        if (it != m_stashedCommandEditBoxes.end()) {
            m_layout->insertWidget(m_layout->count() - 1, it->get());
            (*it)->show();
            connect((*it)->textEdit(), &TextEdit::tabPressed, this, &CommandScroll::moveFocusToNext);
            connect((*it)->textEdit(), &TextEdit::backTabPressed, this, &CommandScroll::moveFocusToPrevious);
            (*it)->connectSignals();
            m_commandEditBoxes.emplace_back(std::move(*it));
            m_stashedCommandEditBoxes.erase(it);
        } else {
            auto* commandEditBox = new CommandEdit(this, cluster);
            m_commandEditBoxes.emplace_back(commandEditBox);
            connect(commandEditBox->textEdit(), &TextEdit::tabPressed, this, &CommandScroll::moveFocusToNext);
            connect(commandEditBox->textEdit(), &TextEdit::backTabPressed, this, &CommandScroll::moveFocusToPrevious);
            m_layout->insertWidget(m_layout->count() - 1, commandEditBox);
        }
    }

    CentralWidget* CommandScroll::centralWidget() const {
        return m_centralWidget;
    }

    void CommandScroll::removeFromLayout(size_t index) {
        auto it = std::find_if(D_IT(m_commandEditBoxes), D_FUNC(box, box->index() == index));
        assert(it != m_commandEditBoxes.end());
        it->get()->textEdit()->disconnect();
        it->get()->setVisible(false);
        it->get()->setParent(nullptr);
        auto* commandEditBox = it->release();
        m_commandEditBoxes.erase(it);
        update();
        stash(std::unique_ptr<CommandEdit>(commandEditBox));
    }

    void CommandScroll::addToLayout(std::unique_ptr<CommandEdit>&& commandEditBox) {
        commandEditBox->setParent(this);
        m_layout->insertWidget(m_layout->count() - 1, commandEditBox.get());
        m_commandEditBoxes.emplace_back(std::move(commandEditBox));
        m_commandEditBoxes.back()->show();
    }

    CommandEdit* CommandScroll::withIndex(size_t index) {
        auto it = std::find_if(D_IT(m_commandEditBoxes), D_FUNC(box, box->index() == index));
        assert(it != m_commandEditBoxes.end());
        return it->get();
    }

    void CommandScroll::updateSelection() {
        for (auto& box : m_commandEditBoxes) {
            box->updateSelection();
        }
    }

    void CommandScroll::disable() {
        for (auto& box : m_commandEditBoxes) {
            box->textEdit()->setReadOnly(true);
            box->setStyleSheet(QString("QTextEdit { background-color: %0 }").arg(QColor(Qt::gray).lighter().name(QColor::HexRgb)));
            box->disconnectCommandVectorUpdate();
        }
    }

    void CommandScroll::removeUnneeded(std::vector<model::Cluster>& clusters) {
        for (size_t i = 0; i != m_commandEditBoxes.size();) {
            if (std::find_if(D_CIT(clusters), D_FUNC(cluster, cluster.index() == m_commandEditBoxes.at(i)->index())) == clusters.end()) {
                removeFromLayout(m_commandEditBoxes.at(i)->index());
            } else {
                ++i;
            }
        }
    }

    void CommandScroll::setShouldStashCommandEditBoxes(bool shouldStashCommandEditBoxes) {
        m_shouldStashCommandEditBoxes = shouldStashCommandEditBoxes;
    }

    void CommandScroll::stash(std::unique_ptr<CommandEdit>&& commandEditWidget) {
        if (m_shouldStashCommandEditBoxes) {
            m_stashedCommandEditBoxes.emplace_back(std::move(commandEditWidget));
        }
    }

} // namespace view
