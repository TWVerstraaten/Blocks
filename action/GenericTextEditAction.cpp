////
//// Created by teunv on 1/19/2021.
////
//
//#include "GenericTextEditAction.h"
//
//#include "../view/CentralWidget.h"
//
//#include <utility>
//
//action::GenericTextEditAction::GenericTextEditAction(
//    view::CentralWidget* centralWidget, size_t index, QString old, QTextCursor oldCursor, QString aNew, QTextCursor newCursor)
//    : m_centralWidget(centralWidget), m_index(index), m_old(std::move(old)), m_oldCursor(oldCursor), m_new(std::move(aNew)), m_newCursor(newCursor) {
//    setText(QString("Text edit action for %1").arg(m_index));
//}
//
//void action::GenericTextEditAction::undo() {
//    m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setBlockUndoCreation(true);
//    m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setPlainText(m_old);
//    m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setTextCursor(m_oldCursor);
//    m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setBlockUndoCreation(false);
//}
//
//void action::GenericTextEditAction::redo() {
//    if (not m_blockInitial) {
//        m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setBlockUndoCreation(true);
//        m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setPlainText(m_new);
//        m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setTextCursor(m_newCursor);
//        m_centralWidget->commandScrollArea()->withIndex(m_index)->textEdit()->setBlockUndoCreation(false);
//    } else {
//        m_blockInitial = false;
//    }
//}
//
//action::ACTION_TYPE action::GenericTextEditAction::type() const {
//    return ACTION_TYPE::TEXT_EDIT_ACTION;
//}
