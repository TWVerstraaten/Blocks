//
// Created by pc on 16-12-20.
//

#include "Texture.h"

#include <SDL_image.h>
#include <cassert>
#include <iostream>

namespace view {
    Texture::Texture(SDL_Texture* texture, int width, int height) : m_loadedCorrectly(true), m_texture(texture), m_width(width), m_height(height) {
    }

    Texture::~Texture() {
        free();
    }

    std::unique_ptr<Texture> Texture::createFromImagePath(const std::string& path, SDL_Renderer* renderer) {
        std::unique_ptr<Texture> result{new Texture()};

        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == nullptr) {
            std::cout << "Unable to load image " << path.c_str() << ", SDL_image Error: " << IMG_GetError() << '\n';
        } else {
            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

            result->m_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
            if (result->m_texture == nullptr) {
                printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            } else {
                result->m_width           = loadedSurface->w;
                result->m_height          = loadedSurface->h;
                result->m_loadedCorrectly = true;
                SDL_FreeSurface(loadedSurface);
            }
        }
        return result;
    }

    std::unique_ptr<Texture> Texture::createFromText(const std::string& textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* font) {
        std::unique_ptr<Texture> result{new Texture()};

        SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), textColor);
        if (textSurface == nullptr) {
            printf("Unable to drawRectangle text surface! SDL_ttf Error: %s\n", TTF_GetError());
            return result;
        }

        result->m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (result->m_texture == nullptr) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
            return result;
        }
        result->m_width  = textSurface->w;
        result->m_height = textSurface->h;

        SDL_FreeSurface(textSurface);

        result->m_loadedCorrectly = result->m_texture != nullptr;
        return result;
    }

    void Texture::free() {
        if (m_texture != nullptr) {
            SDL_DestroyTexture(m_texture);
            m_loadedCorrectly = false;
            m_texture         = nullptr;
            m_width           = 0;
            m_height          = 0;
        }
    }

    void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
        SDL_SetTextureColorMod(m_texture, red, green, blue);
    }

    void Texture::setBlendMode(SDL_BlendMode blending) {
        SDL_SetTextureBlendMode(m_texture, blending);
    }

    void Texture::setAlpha(Uint8 alpha) {
        SDL_SetTextureAlphaMod(m_texture, alpha);
    }

    void Texture::render(const SDL_Rect& destination, SDL_Renderer* renderer, double angle, const SDL_Point* center, SDL_RendererFlip flip) {
        assert(m_loadedCorrectly);
        assert(m_texture != nullptr);

        SDL_RenderCopyEx(renderer, m_texture, nullptr, &destination, angle, center, flip);
    }

    int Texture::width() const {
        return m_width;
    }

    int Texture::height() const {
        return m_height;
    }

    bool Texture::loadedCorrectly() const {
        return m_loadedCorrectly;
    }
} // namespace view