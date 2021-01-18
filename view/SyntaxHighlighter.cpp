//
// Created by teunv on 1/18/2021.
//

#include "SyntaxHighlighter.h"

#include "model/command/CommandParser.h"

view::SyntaxHighlighter::SyntaxHighlighter(QTextDocument* textDocument) : QSyntaxHighlighter(textDocument) {
    m_errorFormat.setFontWeight(QFont::Bold);
    m_errorFormat.setFontUnderline(true);
    m_errorFormat.setUnderlineColor(Qt::red);

    m_normalFormat.setFontWeight(QFont::StyleItalic);
    m_normalFormat.setForeground(Qt::black);
}

void view::SyntaxHighlighter::highlightBlock(const QString& text) {

    if (model::CommandParser::canParse(text.toStdString())) {
        return;
    } else {
        setFormat(0, text.length(), m_errorFormat);
    }
}
