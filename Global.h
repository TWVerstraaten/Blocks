//
// Created by pc on 22-12-20.
//

#ifndef BLOCKS_GLOBAL_H
#define BLOCKS_GLOBAL_H

#include <SDL2/SDL.h>

#define STUBBED(x)                                                                                                                         \
    do {                                                                                                                                   \
        static bool seen_this = false;                                                                                                     \
        if (!seen_this) {                                                                                                                  \
            seen_this = true;                                                                                                              \
            fprintf(stderr, "STUBBED: %s at %s (%s:%d)\n", x, __FUNCTION__, __FILE__, __LINE__);                                           \
        }                                                                                                                                  \
    } while (0)

namespace global {

    static const Uint32 m_timeStepSlow   = 1000;
    static const Uint32 m_timeStepMedium = 300;
    static const Uint32 m_timeStepFast   = 50;

} // namespace global

#endif // BLOCKS_GLOBAL_H
