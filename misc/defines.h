//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_DEFINES_H
#define BLOCKS_DEFINES_H

#ifdef _WIN32
#include <cstdio>
#endif

#ifdef DEBUG
#define D_NOTE(x) fprintf(stderr, x ": %s:%d\n", __FILE__, __LINE__);
#define D_NOTE_ONCE(x)                                                                                                                               \
    static bool noteSeen = false;                                                                                                                    \
    if (not noteSeen)                                                                                                                                \
        D_NOTE(x)                                                                                                                                    \
    noteSeen = true;
#else
#define D_NOTE(x)
#define D_NOTE_ONCE(x)
#endif

#define D_IT(x) x.begin(), x.end()
#define D_RIT(x) x.rbegin(), x.rend()
#define D_CIT(x) x.cbegin(), x.cend()
#define D_FUNC(name, f) [&]([[maybe_unused]] const auto& name) { return f; }
#define D_VOID_FUNC(name, f) [&]([[maybe_unused]] const auto& name) { f }
#define D_FUNC_2(name1, name2, f) [&]([[maybe_unused]] const auto& name1, [[maybe_unused]] const auto& name2) { return f; }

#endif // BLOCKS_DEFINES_H
