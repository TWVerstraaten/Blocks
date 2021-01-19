//
// Created by teunv on 1/18/2021.
//

#include "SyntaxHighlighter.h"

#include "model/command/CommandParser.h"

view::SyntaxHighlighter::SyntaxHighlighter(QTextDocument* textDocument) : QSyntaxHighlighter(textDocument) {
    m_errorFormat.setFontUnderline(true);
    m_errorFormat.setUnderlineColor(Qt::red);
    m_errorFormat.setForeground(Qt::red);

    m_noteFormat.setForeground(Qt::gray);
}

void view::SyntaxHighlighter::highlightBlock(const QString& text) {
    const auto trimmed = text.trimmed();
    if (not model::CommandParser::canParse(text.toStdString())) {
        setFormat(0, text.length(), m_errorFormat);
    } else if ((not text.isEmpty()) && text.front() == "#") {
        setFormat(0, text.length(), m_noteFormat);
    }
}
