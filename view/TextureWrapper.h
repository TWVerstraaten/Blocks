//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_TEXTUREWRAPPER_H
#define BLOCKS_TEXTUREWRAPPER_H

#include "Texture.h"

#include <memory>
#include <vector>

class SDL_Renderer;

namespace view {
    class TextureWrapper {

      public:
        enum class TEXTURE_ENUM { ERROR, ARROW_CW, ARROW_CCW, CLUSTER, KILL };

        TextureWrapper(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        Texture* getTexture(int width, int height)const;

      private:
        void                                  init(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);
        std::vector<std::unique_ptr<Texture>> m_textures;
    };
} // namespace view

#endif // BLOCKS_TEXTUREWRAPPER_H
