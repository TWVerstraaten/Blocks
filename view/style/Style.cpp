//
// Created by pc on 02-02-21.
//

#include "Style.h"

#include <QColor>
#include <QDebug>
#include <map>

static std::map<QString, QColor> s_replacementMap{};

const QString view::style::Style::s_stylesheet = buildStyleSheet();

#define ADD_COLOR(name, r, g, b) s_replacementMap.insert({#name, QColor{r, g, b, 255}})

static void setColors() {
    ADD_COLOR(_COLOR_ONE, 140, 140, 180);
    ADD_COLOR(_COLOR_TWO, 210, 210, 255);
    ADD_COLOR(_COLOR_THREE, 47, 85, 183);
    //    ADD_COLOR(_COLOR_FOUR, 220, 227, 221);
}

QString view::style::Style::buildStyleSheet() {

    setColors();
    QString base = ""
                   "QPushButton{"
                   "    background: _COLOR_THREE;"
                   "    border-width: 0px;"
                   "    border-radius: 0px;"
                   "    padding: 5px"
                   "}"
                   "QTabWidget::tab-bar {\n"
                   "        border: 1px _COLOR_TWO;\n"
                   "}\n"
                   "QTabBar::tab {\n"
                   "        background: _COLOR_ONE;\n"
                   "        color: black;\n"
                   "        padding: 10px;\n"
                   "}\n"
                   "QTabBar::tab:selected {\n"
                   "        background: _COLOR_TWO;\n"
                   "}\n"
                   "QTabWidget {\n"
                   "    background: _COLOR_ONE;"
                   "}"
                   "QWidget#LevelSelectScroll {\n"
                   "    background: _COLOR_TWO;\n"
                   "}\n"
                   "QWidget#CommandScroll {\n"
                   "    background: _COLOR_ONE;\n"
                   "}\n"
                   "QTabWidget {\n"
                   "    background: _COLOR_ONE;\n"
                   "}\n"
                   "QLabel#CommandEditLabel {\n"
                   "    background: _COLOR_THREE;\n"
                   "    padding: 3px;"
                   "}\n"
                   "QWidget#TextEditSideBar {\n"
                   "    background: _COLOR_TWO;\n"
                   "    color: black;\n"
                   "}\n"
                   "QPlainTextEdit#TextEdit {\n"
                   "   selection-color: white;\n"
                   "   selection-background-color: _COLOR_THREE;\n"
                   "}";
    for (const auto& [str, col] : s_replacementMap) {
        base.replace(str, col.name(QColor::HexRgb));
    }
    assert(not base.contains('_'));
    return base;
}
