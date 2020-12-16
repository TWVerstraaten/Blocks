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

class AssetHandler {
  public:
    void init(SDL_Renderer* renderer);

    enum class TEXTURE_ENUM { ONE, TWO, ARROW_CW, ARROW_CCW, CLUSTER, KILL };
    enum class FONT_ENUM { ONE, TWO };

    bool renderTexture(TEXTURE_ENUM textureEnum, const SDL_Rect& destination, SDL_Renderer* renderer,
                       double angle = 0.0, const SDL_Point* center = nullptr,
                       SDL_RendererFlip flip = SDL_FLIP_NONE) const;

    static TEXTURE_ENUM getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type);
    static TEXTURE_ENUM getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type);

  private:
    std::map<TEXTURE_ENUM, std::unique_ptr<Texture>> m_textures;
    std::map<FONT_ENUM, Font>                        m_fonts;
};

#endif // BLOCKS_ASSETHANDLER_H
