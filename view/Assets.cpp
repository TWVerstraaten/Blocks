//
// Created by pc on 16-12-20.
//

#include "Assets.h"

#include "../global/cst.h"
#include "../global/geom.h"
#include "ScreenXY.h"

#include <cassert>

namespace view {
    void Assets::init(SDL_Renderer* renderer) {
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ARROW_CW, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ARROW_CCW, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::CLUSTER, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::KILL, renderer);
        loadTextureWrapper(TextureWrapper::TEXTURE_ENUM::ERROR, renderer);

        m_fonts[FONT_ENUM::MAIN] = std::make_unique<Font>("assets/UbuntuMono-Bold.ttf", cst::EDIT_BOX_FONT_SIZE);
        m_initialized            = true;
    }

    bool Assets::renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                               const SDL_Rect&              destination,
                               SDL_Renderer*                renderer,
                               double                       angle,
                               const SDL_Point*             center,
                               SDL_RendererFlip             flip) const {
        assert(m_textures.find(textureEnum) != m_textures.end());
        auto* texture = m_textures.at(textureEnum).getTexture(destination.w, destination.h);
        if (texture->loadedCorrectly()) {
            texture->render(geom::pad(destination, 1), renderer, angle, center, flip);
            return true;
        } else {
            return false;
        }
    }

    bool Assets::renderTexture(Texture*         texture,
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

    bool Assets::renderTexture(TextureWrapper::TEXTURE_ENUM textureEnum,
                               const ScreenXY&              screenXY,
                               int                          width,
                               int                          height,
                               SDL_Renderer*                renderer,
                               double                       angle,
                               const SDL_Point*             center,
                               SDL_RendererFlip             flip) const {
        if (width < 0) {
            return renderTexture(textureEnum, {screenXY.x() + width, screenXY.y()}, -width, height, renderer, angle, center, flip);
        } else if (height < 0) {
            return renderTexture(textureEnum, {screenXY.x(), screenXY.y() + height}, width, -height, renderer, angle, center, flip);
        } else {
            return renderTexture(textureEnum, {(screenXY.x()), (screenXY.y()), width, height}, renderer, angle, center, flip);
        }
    }

    bool Assets::renderTexture(Texture*         texture,
                               const ScreenXY&  screenXY,
                               int              width,
                               int              height,
                               SDL_Renderer*    renderer,
                               double           angle,
                               const SDL_Point* center,
                               SDL_RendererFlip flip) {
        return renderTexture(texture, {(screenXY.x()), (screenXY.y()), width, height}, renderer, angle, center, flip);
    }

    TextureWrapper::TEXTURE_ENUM Assets::getTextureEnum(model::Level::DYNAMIC_BLOCK_TYPE type) {
        switch (type) {
            case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                return TextureWrapper::TEXTURE_ENUM::ARROW_CW;
            case model::Level::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                return TextureWrapper::TEXTURE_ENUM::ARROW_CCW;
            default:
                return TextureWrapper::TEXTURE_ENUM::ERROR;
        }
    }

    TextureWrapper::TEXTURE_ENUM Assets::getTextureEnum(model::Level::INSTANT_BLOCK_TYPE type) {
        switch (type) {
            case model::Level::INSTANT_BLOCK_TYPE::KILL:
                return TextureWrapper::TEXTURE_ENUM::KILL;
            default:
                return TextureWrapper::TEXTURE_ENUM::ERROR;
        }
    }

    const Font* Assets::font(Assets::FONT_ENUM fontEnum) const {
        assert(m_fonts.find(fontEnum) != m_fonts.end());
        assert(m_fonts.at(fontEnum)->loadedCorrectly());
        return m_fonts.at(fontEnum).get();
    }

    void Assets::loadTextureWrapper(TextureWrapper::TEXTURE_ENUM textureEnum, SDL_Renderer* renderer) {
        m_textures.insert({textureEnum, TextureWrapper(textureEnum, renderer)});
    }

    void Assets::renderText(const std::string& text, const ScreenXY& screenXY, SDL_Renderer* renderer) {
        const auto texture = view::Texture::createFromText(text, cst::color::BLACK, renderer, m_fonts[FONT_ENUM::MAIN].get()->font());
        renderTexture(texture.get(), screenXY, texture->width(), texture->height(), renderer);
    }

    bool Assets::initialized() const {
        return m_initialized;
    }

} // namespace view
