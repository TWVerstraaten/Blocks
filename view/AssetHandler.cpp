//
// Created by pc on 16-12-20.
//

#include "AssetHandler.h"

#include <cassert>

void AssetHandler::init(SDL_Renderer* renderer) {
    m_textures[TEXTURE_ENUM::ARROW_CW] =
        Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/arrow_cw.png", renderer);
    m_textures[TEXTURE_ENUM::ARROW_CCW] =
        Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/arrow_ccw.png", renderer);
    m_textures[TEXTURE_ENUM::CLUSTER] =
        Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/cluster.png", renderer);
    m_textures[TEXTURE_ENUM::KILL] =
        Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/kill.png", renderer);
    m_textures[TEXTURE_ENUM::ERROR] =
        Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/error.png", renderer);
}

bool AssetHandler::renderTexture(AssetHandler::TEXTURE_ENUM textureEnum, const SDL_Rect& destination,
                                 SDL_Renderer* renderer, double angle, const SDL_Point* center,
                                 SDL_RendererFlip flip) const {
    assert(m_textures.find(textureEnum) != m_textures.end());
    if (m_textures.at(textureEnum)->loadedCorrectly()) {
        m_textures.at(textureEnum)->render(destination, renderer, angle, center, flip);
        return true;
    } else {
        return false;
    }
}

AssetHandler::TEXTURE_ENUM AssetHandler::getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type) {
    switch (type) {
        case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
            return TEXTURE_ENUM::ARROW_CW;
        case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
            return TEXTURE_ENUM::ARROW_CCW;
        default:
            return TEXTURE_ENUM::ERROR;
    }
}

AssetHandler::TEXTURE_ENUM AssetHandler::getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type) {
    switch (type) {
        case model::Level::INSTANT_BLOCK_TYPE::KILL:
            return TEXTURE_ENUM::KILL;
        default:
            return TEXTURE_ENUM::ERROR;
    }
}
