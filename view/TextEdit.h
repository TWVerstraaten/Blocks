#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "SyntaxHighlighter.h"

#include <QTextEdit>
#include <string>
#include <vector>

namespace view {
    class CommandEditWidget;

    class TextEdit : public QTextEdit {
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

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      signals:

        void tabPressed();
        void backTabPressed();

      private:
        void highlightLine(const QTextCursor& cursor);

        size_t nThOpaqueLine(size_t n);

        CommandEditWidget*    m_commandEditBox;
        SyntaxHighlighter* m_syntaxHighlighter;
    };
} // namespace view

#endif // TEXTEDIT_H
