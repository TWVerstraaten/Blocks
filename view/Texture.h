//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_TEXTURE_H
#define BLOCKS_TEXTURE_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <memory>
#include <string>

namespace view {

    class Texture {

      public:
        Texture() = default;
        ~Texture();

        void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha);
        void render(const SDL_Rect&  destination,
                    SDL_Renderer*    renderer,
                    double           angle  = 0.0,
                    const SDL_Point* center = nullptr,
                    SDL_RendererFlip flip   = SDL_FLIP_NONE);
        int  width() const;
        int  height() const;
        bool loadedCorrectly() const;

        static std::unique_ptr<Texture> createFromImagePath(const std::string& path, SDL_Renderer* renderer);
        static std::unique_ptr<Texture> createFromText(const std::string& textureText,
                                                       SDL_Color          textColor,
                                                       SDL_Renderer*      renderer,
                                                       TTF_Font*          font);

      private:
        bool         m_loadedCorrectly = false;
        SDL_Texture* m_texture         = nullptr;
        size_t       m_width           = 0;
        size_t       m_height          = 0;
    };
} // namespace view

#endif // BLOCKS_TEXTURE_H
