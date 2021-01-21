//
// Created by pc on 21-01-21.
//

#include "FontManager.h"

#include <QDebug>
#include <QFontDatabase>

namespace view {

    std::map<std::pair<FONT_ENUM, size_t>, const QFont> FontManager::s_fonts = {};
    std::map<FONT_ENUM, int>                            FontManager::s_ids   = {};

    const QFont& FontManager::font(FONT_ENUM fontEnum, size_t size) {
        if (s_fonts.find({fontEnum, size}) == s_fonts.end()) {
            build(fontEnum, size);
        }
        return s_fonts.at({fontEnum, size});
    }

    void FontManager::build(FONT_ENUM fontEnum, size_t size) {
        if (s_ids.find(fontEnum) == s_ids.end()) {
            int id;
            switch (fontEnum) {
                case FONT_ENUM::UBUNTU_BOLD:
                    id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Italic.ttf");
                    break;
                case FONT_ENUM::UBUNTU_ITALIC:
                    id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Italic.ttf");
                    break;
                case FONT_ENUM::UBUNTU_REGULAR:
                    id = QFontDatabase::addApplicationFont(":/assets/UbuntuMono-Italic.ttf");
                    break;
            }
            assert(id >= 0);
            s_ids.emplace(fontEnum, id);
        }
        const QString family = QFontDatabase::applicationFontFamilies(s_ids[fontEnum]).at(0);
        s_fonts.emplace(std::make_pair(fontEnum, size), QFont(family, size));
    }

} // namespace view
