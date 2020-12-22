//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ASSETHANDLER_H
#define BLOCKS_ASSETHANDLER_H

#include "../model/Level.h"
#include "Font.h"
#include "Texture.h"
#include "TextureWrapper.h"

#include <map>
#include <memory>

namespace view {

    class ScreenCoordinates;

    class AssetHandler {
      public:
        enum class FONT_ENUM { MAIN };

        AssetHandler()                          = default;
        AssetHandler(const AssetHandler& other) = delete;

        void init(SDL_Renderer* renderer);

        bool renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                           const SDL_Rect&              destination,
                           SDL_Renderer*                renderer,
                           double                       angle  = 0.0,
                           const SDL_Point*             center = nullptr,
                           SDL_RendererFlip             flip   = SDL_FLIP_NONE) const;
        bool renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                           const ScreenCoordinates&     screenCoordinates,
                           int                          width,
                           int                          height,
                           SDL_Renderer*                renderer,
                           double                       angle  = 0.0,
                           const SDL_Point*             center = nullptr,
                           SDL_RendererFlip             flip   = SDL_FLIP_NONE) const;

        static bool renderTexture(Texture*         texture,
                                  const SDL_Rect&  destination,
                                  SDL_Renderer*    renderer,
                                  double           angle  = 0.0,
                                  const SDL_Point* center = nullptr,
                                  SDL_RendererFlip flip   = SDL_FLIP_NONE);
        static bool renderTexture(Texture*                 texture,
                                  const ScreenCoordinates& screenCoordinates,
                                  int                      width,
                                  int                      height,
                                  SDL_Renderer*            renderer,
                                  double                   angle  = 0.0,
                                  const SDL_Point*         center = nullptr,
                                  SDL_RendererFlip         flip   = SDL_FLIP_NONE);

        const Font* font(FONT_ENUM fontEnum) const;

        static TextureWrapper::TEXTURE_ENUM getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type);
        static TextureWrapper::TEXTURE_ENUM getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type);

      private:
        void loadTextureWrapper(TextureWrapper::TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        std::map<TextureWrapper::TEXTURE_ENUM, TextureWrapper> m_textures;
        std::map<FONT_ENUM, std::unique_ptr<Font>>             m_fonts;
    };
} // namespace view
#endif // BLOCKS_ASSETHANDLER_H
