//
// Created by pc on 21-01-21.
//

#include "FontManager.h"

#include <QDebug>
#include <QFontDatabase>

namespace view {

    const QFont& FontManager::font(FONT_ENUM fontEnum, size_t size) {
        if (s_fonts.find({fontEnum, size}) == s_fonts.end()) {
            build(fontEnum, size);
        }
        return s_fonts.at({fontEnum, size});
    }

    static QString fromEnum(FONT_ENUM fontEnum) {
        switch (fontEnum) {
            case FONT_ENUM::ANON_PRO_BOLD:
                return ":/assets/Anonymous_Pro_B.ttf";
            case FONT_ENUM::ANON_PRO_ITALIC:
                return ":/assets/Anonymous_Pro_I.ttf";
            case FONT_ENUM::ANON_PRO_BOLD_ITALIC:
                return ":/assets/Anonymous_Pro_BI.ttf";
            case FONT_ENUM::ANON_PRO:
                return ":/assets/Anonymous_Pro.ttf";
            case FONT_ENUM::UBUNTU_MONO:
                return ":/assets/UbuntuMono-Regular.ttf";
            case FONT_ENUM::UBUNTU_MONO_BOLD:
                return ":/assets/UbuntuMono-Bold.ttf";
            case FONT_ENUM::UBUNTU_MONO_ITALIC:
                return ":/assets/UbuntuMono-Italic.ttf";
            case FONT_ENUM::UBUNTU_MONO_BOLD_ITALIC:
                return ":/assets/UbuntuMono-BoldItalic.ttf";
        }
        assert(false);
        return "";
    }

    void FontManager::build(FONT_ENUM fontEnum, int size) {
        if (s_ids.find(fontEnum) == s_ids.end()) {
            int id = QFontDatabase::addApplicationFont(fromEnum(fontEnum));
            assert(id >= 0);
            s_ids.emplace(fontEnum, id);
        }
        const auto family = QFontDatabase::applicationFontFamilies(s_ids[fontEnum]).at(0);
        auto       font   = QFont(family, size);

        s_fonts.emplace(std::make_pair(fontEnum, size), font);
    }

} // namespace view
