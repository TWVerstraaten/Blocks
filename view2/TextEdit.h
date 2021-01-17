#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <string>
#include <vector>

namespace view2 {
    class CommandEditBox;

    class TextEdit : public QTextEdit {
        Q_OBJECT
      public:
        TextEdit(CommandEditBox* commandEditBox, const QString& string);

        [[nodiscard]] std::vector<std::string> contents() const;
        void                                   setHeight();
        void                                   keyPressEvent(QKeyEvent* event) override;
        void                                   sendUndo();

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      signals:

        void tabPressed();
        void backTabPressed();

      private:
        CommandEditBox* m_commandEditBox;
    };
} // namespace view2

#endif // TEXTEDIT_H
