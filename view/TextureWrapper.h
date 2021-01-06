//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_TEXTUREWRAPPER_H
#define BLOCKS_TEXTUREWRAPPER_H

#include "Texture.h"

#include <memory>
#include <vector>
#include "Texture_enums.h"

namespace view {
    class TextureWrapper {

      public:

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        TextureWrapper(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        /****** CONST GETTERS  ******/
        [[nodiscard]] Texture* texture(int width, int height) const;

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void init(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        /****** DATA MEMBERS  ******/
        std::vector<std::unique_ptr<Texture>> m_textures;
    };
} // namespace view

#endif // BLOCKS_TEXTUREWRAPPER_H
