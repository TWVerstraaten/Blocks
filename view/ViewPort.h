//
// Created by pc on 15-12-20.
//

#ifndef BLOCKS_VIEWPORT_H
#define BLOCKS_VIEWPORT_H

#include "../app/Application_constants.h"
#include "ScreenXy.h"

namespace view {

    class ViewPort {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        ViewPort() = default;

        /****** CONST GETTERS  ******/
        [[nodiscard]] int blockSizeInScreen() const;
        [[nodiscard]] int xOffset() const;
        [[nodiscard]] int yOffset() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] int worldToScreen(int worldLength) const;
        [[nodiscard]] int screenToWorld(int worldLength) const;

        /****** NON CONST FUNCTIONS  ******/
        void zoom(int dZoom, const view::ScreenXy& point);
        void translate(int dx, int dy);
        void setScale(double scale);
        void setOffset(int xOffset, int yOffset);

      private:
        /****** DATA MEMBERS  ******/
        double m_scale   = 1.0;
        int    m_zoom    = 0;
        int    m_xOffset = 0;
        int    m_yOffset = 0;
    };
} // namespace view
#endif // BLOCKS_VIEWPORT_H
