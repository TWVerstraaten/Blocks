//
// Created by pc on 22-12-20.
//

#include "TextureWrapper.h"

#include <algorithm>
#include <cassert>

double flipIfSmallerThanOne(double a) {
    assert(a > 0);
    if (a < 1) {
        return 1.0 / a;
    } else {
        return a;
    }
}

static double score(const view::Texture* texture, int width, int height) {
    return flipIfSmallerThanOne(width / static_cast<double>(texture->width())) +
           flipIfSmallerThanOne(height / static_cast<double>(texture->height()));
}

const view::Texture* view::TextureWrapper::getTexture(int width, int height) {
    assert(!m_textures.empty());

    const Texture* result = m_textures.front().get();
    for (const auto& texture : m_textures) {
        if (score(result, width, height) > score(texture.get(), width, height)) {
            result = texture.get();
        }
    }
    return result;
}
void view::TextureWrapper::init(view::TextureWrapper::TEXTURE_ENUM textureEnum, SDL_Renderer* renderer) {
    m_textures.clear();
    switch (textureEnum) {
        case TEXTURE_ENUM::ERROR:
            m_textures.emplace_back(Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/error.png", renderer));
            break;
        case TEXTURE_ENUM::ARROW_CW:
            m_textures.emplace_back(Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/arrow_cw.png", renderer));
            break;
        case TEXTURE_ENUM::ARROW_CCW:
            m_textures.emplace_back(Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/arrow_ccw.png", renderer));
            break;
        case TEXTURE_ENUM::CLUSTER:
            m_textures.emplace_back(Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/cluster_large.png", renderer));
            break;
        case TEXTURE_ENUM::KILL:
            m_textures.emplace_back(Texture::createFromImagePath("/home/pc/Documents/c++/Blocks/assets/kill.png", renderer));
            break;
    }
}
