#ifndef BLOCKS_TEXTEDIT_H
#define BLOCKS_TEXTEDIT_H

#include <QPlainTextEdit>
#include <string>
#include <vector>

namespace view::widget {

    class CommandEdit;
    class SyntaxHighlighter;

    class TextEdit : public QPlainTextEdit {
        Q_OBJECT

      public:
        TextEdit(CommandEdit* commandEditBox, const QString& string);
        ~TextEdit() override;

        void setHeight();
        void keyPressEvent(QKeyEvent* event) override;
        void sendUndo();
        void connectSignals();
        void setSelection(size_t actionIndex);

        [[nodiscard]] size_t                   lineHeight() const;
        [[nodiscard]] size_t                   topMargin() const;
        [[nodiscard]] size_t                   nThOpaqueLine(size_t n) const;
        [[nodiscard]] std::vector<std::string> contents() const;

      protected:
        [[nodiscard]] QSize sizeHint() const override;

      signals:
        void tabPressed();
        void backTabPressed();

      private:
        void highlightLine(const QTextCursor& cursor);

        CommandEdit*       m_commandEditBox    = nullptr;
        SyntaxHighlighter* m_syntaxHighlighter = nullptr;
        size_t             m_lineHeight;
    };
} // namespace view::widget

#endif // BLOCKS_TEXTEDIT_H
