//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_ASSETHANDLER_H
#define BLOCKS_ASSETHANDLER_H

#include "../model/Level.h"
#include "Font.h"
#include "Texture.h"

#include <map>
#include <memory>

namespace view {

    class ScreenCoordinates;

    class AssetHandler {
      public:
        AssetHandler()                          = default;
        AssetHandler(const AssetHandler& other) = delete;

        void init(SDL_Renderer* renderer);

        enum class TEXTURE_ENUM { ERROR, ARROW_CW, ARROW_CCW, CLUSTER, KILL };
        enum class FONT_ENUM { MAIN };

        bool        renderTexture(TEXTURE_ENUM textureEnum, const SDL_Rect& destination, SDL_Renderer* renderer, double angle = 0.0,
                                  const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) const;
        static bool renderTexture(Texture* texture, const SDL_Rect& destination, SDL_Renderer* renderer, double angle = 0.0,
                                  const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
        bool        renderTexture(TEXTURE_ENUM textureEnum, const ScreenCoordinates& screenCoordinates, int width, int height,
                                  SDL_Renderer* renderer, double angle = 0.0, const SDL_Point* center = nullptr,
                                  SDL_RendererFlip flip = SDL_FLIP_NONE) const;
        static bool renderTexture(Texture* texture, const ScreenCoordinates& screenCoordinates, int width, int height,
                                  SDL_Renderer* renderer, double angle = 0.0, const SDL_Point* center = nullptr,
                                  SDL_RendererFlip flip = SDL_FLIP_NONE);

        const Font* font(FONT_ENUM fontEnum) const;

        static TEXTURE_ENUM getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type);
        static TEXTURE_ENUM getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type);

      private:
        std::map<TEXTURE_ENUM, std::unique_ptr<Texture>> m_textures;
        std::map<FONT_ENUM, std::unique_ptr<Font>>       m_fonts;
    };
} // namespace view
#endif // BLOCKS_ASSETHANDLER_H
