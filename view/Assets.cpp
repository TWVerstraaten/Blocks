//
// Created by pc on 16-12-20.
//

#include "Assets.h"

#include "../global/cst.h"
#include "../global/geom.h"
#include "ScreenXY.h"
#include "color.h"

#include <cassert>

namespace view {
    void Assets::init(SDL_Renderer* renderer) {
        loadTextureWrapper(TEXTURE_ENUM::ARROW_CW, renderer);
        loadTextureWrapper(TEXTURE_ENUM::ARROW_CCW, renderer);
        loadTextureWrapper(TEXTURE_ENUM::CLUSTER, renderer);
        loadTextureWrapper(TEXTURE_ENUM::KILL, renderer);
        loadTextureWrapper(TEXTURE_ENUM::ERROR, renderer);
        loadTextureWrapper(TEXTURE_ENUM::WHITE, renderer);

        m_fonts[FONT_ENUM::MAIN]  = std::make_unique<Font>("assets/UbuntuMono-Bold.ttf", cst::MAIN_FONT_SIZE);
        m_fonts[FONT_ENUM::SMALL] = std::make_unique<Font>("assets/UbuntuMono-Bold.ttf", cst::SMALL_FONT_SIZE);
        m_initialized             = true;
    }

    bool Assets::renderTexture(TEXTURE_ENUM     textureEnum,
                               const SDL_Rect&  destination,
                               SDL_Renderer*    renderer,
                               double           angle,
                               const SDL_Point* center,
                               SDL_RendererFlip flip) const {
        assert(m_textures.find(textureEnum) != m_textures.end());
        auto* texture = m_textures.at(textureEnum).texture(destination.w, destination.h);
        if (texture->loadedCorrectly()) {
            texture->render(geom::pad(destination, 1), renderer, angle, center, flip);
            return true;
        } else {
            return false;
        }
    }

    bool Assets::renderTexture(
        Texture* texture, const SDL_Rect& destination, SDL_Renderer* renderer, double angle, const SDL_Point* center, SDL_RendererFlip flip) {
        if (texture->loadedCorrectly()) {
            texture->render(destination, renderer, angle, center, flip);
            return true;
        } else {
            return false;
        }
    }

    bool Assets::renderTexture(TEXTURE_ENUM     textureEnum,
                               const ScreenXY&  screenXY,
                               int              width,
                               int              height,
                               SDL_Renderer*    renderer,
                               double           angle,
                               const SDL_Point* center,
                               SDL_RendererFlip flip) const {
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

    TEXTURE_ENUM Assets::getTextureEnum(model::DYNAMIC_BLOCK_TYPE type) {
        switch (type) {
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
                return TEXTURE_ENUM::ARROW_CW;
            case model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
                return TEXTURE_ENUM::ARROW_CCW;
        }
        return TEXTURE_ENUM::ERROR;
    }

    TEXTURE_ENUM Assets::getTextureEnum(model::INSTANT_BLOCK_TYPE type) {
        switch (type) {
            case model::INSTANT_BLOCK_TYPE::KILL:
                return TEXTURE_ENUM::KILL;
        }
        return TEXTURE_ENUM::ERROR;
    }

    const Font* Assets::font(FONT_ENUM fontEnum) const {
        assert(m_fonts.find(fontEnum) != m_fonts.end());
        assert(m_fonts.at(fontEnum)->loadedCorrectly());
        return m_fonts.at(fontEnum).get();
    }

    void Assets::loadTextureWrapper(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer) {
        m_textures.insert({textureEnum, TextureWrapper(textureEnum, renderer)});
    }

    void Assets::renderText(
        const std::string& text, const ScreenXY& screenXY, SDL_Renderer* renderer, FONT_ENUM fontEnum, const SDL_Color& color) const {
        assert(m_fonts.find(fontEnum) != m_fonts.end());
        const auto texture = view::Texture::createFromText(text, color, renderer, m_fonts.at(fontEnum).get()->font());
        renderTexture(texture.get(), screenXY, texture->width(), texture->height(), renderer);
    }

    bool Assets::initialized() const {
        return m_initialized;
    }

    Texture* Assets::getTexture(TEXTURE_ENUM textureEnum, int width, int height) {
        return m_textures.at(textureEnum).texture(width, height);
    }

} // namespace view
