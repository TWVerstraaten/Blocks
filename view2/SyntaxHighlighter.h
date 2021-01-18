//
// Created by teunv on 1/18/2021.
//

#ifndef BLOCKS_SYNTAXHIGHLIGHTER_H
#define BLOCKS_SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>

namespace view2 {

    class SyntaxHighlighter : public QSyntaxHighlighter {

      public:
        explicit SyntaxHighlighter(QTextDocument* textDocument);

      protected:
        void highlightBlock(const QString& text) override;

      private:
        QTextCharFormat m_errorFormat;
        QTextCharFormat m_normalFormat;
    };
} // namespace view2

#endif // BLOCKS_SYNTAXHIGHLIGHTER_H
