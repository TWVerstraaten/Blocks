#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "SyntaxHighlighter.h"

#include <QTextEdit>
#include <string>
#include <vector>

namespace view {
    class CommandEditBox;

    class TextEdit : public QTextEdit {
        Q_OBJECT

      public:
        TextEdit(CommandEditBox* commandEditBox, const QString& string);
        ~TextEdit() override;

        void setHeight();
        void keyPressEvent(QKeyEvent* event) override;
        void sendUndo();
        void setSelection(size_t index);

        [[nodiscard]] std::vector<std::string> contents() const;

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      signals:

        void tabPressed();
        void backTabPressed();

      private:
        size_t nThOpaqueLine(size_t n);

        CommandEditBox*    m_commandEditBox;
        SyntaxHighlighter* m_syntaxHighlighter;
    };
} // namespace view

#endif // TEXTEDIT_H
