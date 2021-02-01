//
// Created by teunv on 1/18/2021.
//

#include "SyntaxHighlighter.h"

#include "../../model/command/CommandParser.h"

namespace view::widget {

    SyntaxHighlighter::SyntaxHighlighter(QTextDocument* textDocument) : QSyntaxHighlighter(textDocument) {
        m_errorFormat.setFontUnderline(true);
        m_errorFormat.setUnderlineColor(Qt::red);
        m_errorFormat.setForeground(Qt::red);

        m_noteFormat.setForeground(Qt::gray);
    }

    void SyntaxHighlighter::highlightBlock(const QString& text) {
        const auto trimmed = text.trimmed();
        if (not model::CommandParser::canParse(text.toStdString())) {
            setFormat(0, text.length(), m_errorFormat);
            setCurrentBlockState(currentBlockState() + 1);
        } else if ((not text.isEmpty()) && text.front() == "#") {
            setFormat(0, text.length(), m_noteFormat);
        }
    }
} // namespace view