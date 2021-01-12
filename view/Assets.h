//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ASSETS_H
#define BLOCKS_ASSETS_H

#include "../model/Level_enums.h"
#include "Font.h"
#include "Font_enums.h"
#include "TextureWrapper.h"
#include "Texture_enums.h"
#include "color.h"

#include <map>
#include <memory>

namespace view {

    class ScreenXY;
    class Font;

    class Assets {
      public:
        /****** PUBLIC STATIC FUNCTIONS  ******/
        static bool         renderTexture(Texture*         texture,
                                          const SDL_Rect&  destination,
                                          SDL_Renderer*    renderer,
                                          double           angle  = 0.0,
                                          const SDL_Point* center = nullptr,
                                          SDL_RendererFlip flip   = SDL_FLIP_NONE);
        static bool         renderTexture(Texture*         texture,
                                          const ScreenXY&  screenXY,
                                          int              width,
                                          int              height,
                                          SDL_Renderer*    renderer,
                                          double           angle  = 0.0,
                                          const SDL_Point* center = nullptr,
                                          SDL_RendererFlip flip   = SDL_FLIP_NONE);
        static TEXTURE_ENUM getTextureEnum(model::DYNAMIC_BLOCK_TYPE type);
        static TEXTURE_ENUM getTextureEnum(model::INSTANT_BLOCK_TYPE type);

        [[nodiscard]] static const Font* font(FONT_ENUM fontEnum);

        /****** CONST FUNCTIONS  ******/
        static bool renderTexture(TEXTURE_ENUM     textureEnum,
                                  const SDL_Rect&  destination,
                                  SDL_Renderer*    renderer,
                                  double           angle  = 0.0,
                                  const SDL_Point* center = nullptr,
                                  SDL_RendererFlip flip   = SDL_FLIP_NONE);
        static bool renderTexture(TEXTURE_ENUM     textureEnum,
                                  const ScreenXY&  screenXY,
                                  int              width,
                                  int              height,
                                  SDL_Renderer*    renderer,
                                  double           angle  = 0.0,
                                  const SDL_Point* center = nullptr,
                                  SDL_RendererFlip flip   = SDL_FLIP_NONE);

        /****** NON CONST FUNCTIONS  ******/
        static void     init(SDL_Renderer* renderer);
        static void     renderText(const std::string& text,
                                   const ScreenXY&    screenXY,
                                   SDL_Renderer*      renderer,
                                   FONT_ENUM          fontEnum = FONT_ENUM::MAIN,
                                   const SDL_Color&   color    = color::BLACK);
        static Texture* getTexture(TEXTURE_ENUM textureEnum, int width, int height);
        static void     release();

      private:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Assets() = default;

        /****** PRIVATE NON CONST FUNCTIONS  ******/
        static void loadTextureWrapper(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        friend std::unique_ptr<Assets> build();

        /****** DATA MEMBERS  ******/
        static std::unique_ptr<Assets>                    s_assets;
        static std::map<TEXTURE_ENUM, TextureWrapper>     m_textures;
        static std::map<FONT_ENUM, std::unique_ptr<Font>> m_fonts;
    };
} // namespace view
#endif // BLOCKS_ASSETS_H
