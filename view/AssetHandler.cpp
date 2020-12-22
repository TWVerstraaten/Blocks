//
// Created by pc on 16-12-20.
//

#include "AssetHandler.h"

#include "../aux/Aux.h"
#include "ScreenCoordinates.h"

#include <cassert>
#include <memory>

namespace view {
    void AssetHandler::init(SDL_Renderer* renderer) {
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ARROW_CW, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ARROW_CCW, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::CLUSTER, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::KILL, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ERROR, renderer);

        m_fonts[FONT_ENUM::MAIN] = std::unique_ptr<Font>(new Font("/home/pc/Documents/c++/Blocks/assets/UbuntuMono-Bold.ttf", 28));
    }

    bool AssetHandler::renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                                     const SDL_Rect&              destination,
                                     SDL_Renderer*                renderer,
                                     double                       angle,
                                     const SDL_Point*             center,
                                     SDL_RendererFlip             flip) const {
        assert(m_textures.find(textureEnum) != m_textures.end());
        auto* texture = m_textures.at(textureEnum).getTexture(destination.w, destination.h);
        if (texture->loadedCorrectly()) {
            texture->render(aux::pad(destination, 1), renderer, angle, center, flip);
            return true;
        } else {
            return false;
        }
    }

    bool AssetHandler::renderTexture(Texture*         texture,
                                     const SDL_Rect&  destination,
                                     SDL_Renderer*    renderer,
                                     double           angle,
                                     const SDL_Point* center,
                                     SDL_RendererFlip flip) {
        if (texture->loadedCorrectly()) {
            texture->render(destination, renderer, angle, center, flip);
            return true;
        } else {
            return false;
        }
    }

    bool AssetHandler::renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                                     const ScreenCoordinates&     screenCoordinates,
                                     int                          width,
                                     int                          height,
                                     SDL_Renderer*                renderer,
                                     double                       angle,
                                     const SDL_Point*             center,
                                     SDL_RendererFlip             flip) const {
        return renderTexture(textureEnum,
                             {static_cast<int>(screenCoordinates.x()), static_cast<int>(screenCoordinates.y()), width, height},
                             renderer,
                             angle,
                             center,
                             flip);
    }

    bool AssetHandler::renderTexture(Texture*                 texture,
                                     const ScreenCoordinates& screenCoordinates,
                                     int                      width,
                                     int                      height,
                                     SDL_Renderer*            renderer,
                                     double                   angle,
                                     const SDL_Point*         center,
                                     SDL_RendererFlip         flip) {
        return renderTexture(texture,
                             {static_cast<int>(screenCoordinates.x()), static_cast<int>(screenCoordinates.y()), width, height},
                             renderer,
                             angle,
                             center,
                             flip);
    }

    TextureWrapper::TEXTURE_ENUM AssetHandler::getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type) {
        switch (type) {
            case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                return TextureWrapper::TEXTURE_ENUM::ARROW_CW;
            case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                return TextureWrapper::TEXTURE_ENUM::ARROW_CCW;
            default:
                return TextureWrapper::TEXTURE_ENUM::ERROR;
        }
    }

    TextureWrapper::TEXTURE_ENUM AssetHandler::getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type) {
        switch (type) {
            case model::Level::INSTANT_BLOCK_TYPE::KILL:
                return TextureWrapper::TEXTURE_ENUM::KILL;
            default:
                return TextureWrapper::TEXTURE_ENUM::ERROR;
        }
    }

    const Font* AssetHandler::font(AssetHandler::FONT_ENUM fontEnum) const {
        assert(m_fonts.find(fontEnum) != m_fonts.end());
        return m_fonts.at(fontEnum).get();
    }

    void AssetHandler::loadTextureWrapper(TextureWrapper::TEXTURE_ENUM textureEnum, SDL_Renderer* renderer) {
        m_textures.insert({textureEnum, TextureWrapper(textureEnum, renderer)});
    }

} // namespace view
