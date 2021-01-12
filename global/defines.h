//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_DEFINES_H
#define BLOCKS_DEFINES_H

#ifdef DEBUG
#define __NOTE(x) fprintf(stderr, x ": %s:%d\n", __FILE__, __LINE__);
#define __NOTE_ONCE(x)                                                                                                                               \
    static bool noteSeen = false;                                                                                                                    \
    if (not noteSeen)                                                                                                                                \
        __NOTE(x)                                                                                                                                    \
    noteSeen = true;
#else
#define __NOTE(x)
#define __NOTE_ONCE(x)
#endif

#define __IT(x) x.begin(), x.end()
#define __CIT(x) x.cbegin(), x.cend()
#define __FUNC(name, f) [&]([[maybe_unused]] const auto& name) { return f; }
#define __VOID_FUNC(name, f) [&]([[maybe_unused]] const auto& name) { f }
#define __FUNC_2(name1, name2, f) [&]([[maybe_unused]] const auto& name1, [[maybe_unused]] const auto& name2) { return f; }

#endif // BLOCKS_DEFINES_H
