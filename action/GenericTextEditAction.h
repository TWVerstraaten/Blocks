////
//// Created by teunv on 1/19/2021.
////
//
//#ifndef BLOCKS_GENERICTEXTEDITACTION_H
//#define BLOCKS_GENERICTEXTEDITACTION_H
//
//#include "Action.h"
//
//#include <QCursor>
//#include <QString>
//#include <QTextCursor>
//
//namespace view {
//    class CentralWidget;
//}
//
//namespace action {
//
//    class GenericTextEditAction : public Action {
//
//      public:
//        GenericTextEditAction(view::CentralWidget* centralWidget, size_t index, QString old, QTextCursor oldCursor, QString aNew, QTextCursor newCursor);
//
//        void                      undo() override;
//        void                      redo() override;
//        [[nodiscard]] ACTION_TYPE type() const override;
//
//      private:
//        view::CentralWidget* m_centralWidget;
//        size_t               m_index;
//        QString              m_old;
//        QTextCursor          m_oldCursor;
//        QString              m_new;
//        QTextCursor          m_newCursor;
//    };
//
//} // namespace action
//
//#endif // BLOCKS_GENERICTEXTEDITACTION_H
