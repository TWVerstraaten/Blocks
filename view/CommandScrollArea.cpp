#include "CommandScrollArea.h"

#include "CentralWidget.h"
#include "TextEdit.h"
#include "color.h"
#include "global/defines.h"
#include "model/Cluster.h"

#include <QDebug>
#include <QScrollBar>

namespace view {

    CommandScrollArea::CommandScrollArea(CentralWidget* centralWidget) : QScrollArea(centralWidget), m_centralWidget(centralWidget) {
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

    CommandScrollArea::~CommandScrollArea() {
        for (auto& commandEditBox : m_commandEditBoxes) {
            m_layout->removeWidget(commandEditBox.get());
        }
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

    void CommandScrollArea::addNeeded(std::list<model::Cluster>& clusters) {
        for (auto& cluster : clusters) {
            if (std::find_if(D_CIT(m_commandEditBoxes), D_FUNC(box, box->index() == cluster.index())) == m_commandEditBoxes.end()) {
                add(cluster);
            }
        }
    }

    void CommandScrollArea::add(model::Cluster& cluster) {
        assert(std::find_if(D_CIT(m_commandEditBoxes), D_FUNC(box, box->index() == cluster.index())) == m_commandEditBoxes.cend());
        auto* commandEditBox = new CommandEditWidget(this, cluster);
        m_commandEditBoxes.emplace_back(commandEditBox);
        connect(commandEditBox->textEdit(), &TextEdit::tabPressed, this, &CommandScrollArea::moveFocusToNext);
        connect(commandEditBox->textEdit(), &TextEdit::backTabPressed, this, &CommandScrollArea::moveFocusToPrevious);
        m_layout->insertWidget(m_layout->count() - 1, commandEditBox);
    }

    CentralWidget* CommandScrollArea::centralWidget() const {
        return m_centralWidget;
    }

    std::unique_ptr<CommandEditWidget> CommandScrollArea::removeFromLayout(size_t index) {
        auto it = std::find_if(D_IT(m_commandEditBoxes), D_FUNC(box, box->index() == index));
        assert(it != m_commandEditBoxes.end());
        it->get()->setVisible(false);
        it->get()->setParent(nullptr);
        auto* commandEditBox = it->release();
        m_commandEditBoxes.erase(it);
        update();
        return std::unique_ptr<CommandEditWidget>(commandEditBox);
    }

    void CommandScrollArea::addToLayout(std::unique_ptr<CommandEditWidget>&& commandEditBox) {
        commandEditBox->setParent(this);
        m_layout->insertWidget(m_layout->count() - 1, commandEditBox.get());
        commandEditBox->setCommandVectorPointer();
        m_commandEditBoxes.emplace_back(std::move(commandEditBox));
        m_commandEditBoxes.back()->show();
    }

    CommandEditWidget* CommandScrollArea::withIndex(size_t index) {
        auto it = std::find_if(D_IT(m_commandEditBoxes), D_FUNC(box, box->index() == index));
        assert(it != m_commandEditBoxes.end());
        return it->get();
    }

    void CommandScrollArea::updateSelection() {
        for (auto& box : m_commandEditBoxes) {
            box->updateSelection();
        }
    }

    void CommandScrollArea::disable() {
        for (auto& box : m_commandEditBoxes) {
            box->textEdit()->setReadOnly(true);
            box->setStyleSheet(QString("QTextEdit { background-color: %0 }").arg(QColor(Qt::gray).lighter().name(QColor::HexRgb)));
            box->disconnectCommandVectorUpdate();
        }
    }

} // namespace view
