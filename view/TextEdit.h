#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "SyntaxHighlighter.h"

#include <QPlainTextEdit>
#include <QTextEdit>
#include <string>
#include <vector>

namespace view {
    class CommandEditWidget;

    class TextEdit : public QPlainTextEdit {
        Q_OBJECT

      public:
        TextEdit(CommandEditWidget* commandEditBox, const QString& string);
        ~TextEdit() override;

        void setHeight();
        void keyPressEvent(QKeyEvent* event) override;
        void sendUndo();
        void setSelection(size_t actionIndex);

        [[nodiscard]] size_t                   lineHeight() const;
        [[nodiscard]] size_t                   topMargin() const;
        [[nodiscard]] std::vector<std::string> contents() const;
        [[nodiscard]] size_t                   nThOpaqueLine(size_t n) const;

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      signals:

        void tabPressed();
        void backTabPressed();

      private:
        void highlightLine(const QTextCursor& cursor);

        CommandEditWidget* m_commandEditBox;
        SyntaxHighlighter* m_syntaxHighlighter;
        size_t             m_lineHeight;
    };
} // namespace view

#endif // TEXTEDIT_H
