//
// Created by pc on 02-02-21.
//

#ifndef BLOCKS_STYLE_H
#define BLOCKS_STYLE_H

#include <QString>

namespace view ::style {

    class Style {

      public:
        static const QString s_stylesheet;

      private:
        static QString buildStyleSheet();
    };
} // namespace view::style

#endif // BLOCKS_STYLE_H
