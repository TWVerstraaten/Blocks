//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_TEXTUREWRAPPER_H
#define BLOCKS_TEXTUREWRAPPER_H

#include "Texture.h"

#include <memory>
#include <vector>

namespace view {
    class TextureWrapper {

      public:
        /****** PUBLIC ENUMS / TYPEDEFS  ******/
        enum class TEXTURE_ENUM { ERROR, ARROW_CW, ARROW_CCW, CLUSTER, KILL };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        TextureWrapper(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        /****** CONST GETTERS  ******/
        Texture* texture(int width, int height) const;

      private:
        /****** PRIVATE NON CONST FUNCTIONS  ******/
        void init(TEXTURE_ENUM textureEnum, SDL_Renderer* renderer);

        /****** DATA MEMBERS  ******/
        std::vector<std::unique_ptr<Texture>> m_textures;
    };
} // namespace view

#endif // BLOCKS_TEXTUREWRAPPER_H
