//
// Created by pc on 21-01-21.
//

#ifndef BLOCKS_FONTMANAGER_H
#define BLOCKS_FONTMANAGER_H

#include "Font_enums.h"

#include <QFont>
#include <map>

namespace view {

    class FontManager {

      public:
        static const QFont& font(FONT_ENUM fontEnum, uint16_t size);

      private:
        FontManager() = default;

        static void build(FONT_ENUM fontEnum, uint16_t size);

        inline static std::map<FONT_ENUM, uint16_t>                         s_ids;
        inline static std::map<std::pair<FONT_ENUM, uint16_t>, const QFont> s_fonts;
    };

} // namespace view

#endif // BLOCKS_FONTMANAGER_H
