//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_DEFINES_H
#define BLOCKS_DEFINES_H

#define __NOTE(x) fprintf(stderr, x ": %s:%d\n", __FILE__, __LINE__);
#define __NOTE_ONCE(x)                                                                                                                               \
    static bool noteSeen = false;                                                                                                                    \
    if (not noteSeen)                                                                                                                                \
        __NOTE(x)                                                                                                                                    \
    noteSeen = true;

#define __IT(x) x.begin(), x.end()
#define __CIT(x) x.cbegin(), x.cend()
#define __FUNC(name, f) [&](const auto& name) { return f; }
#define __FUNC_2(name1, name2, f) [&](const auto& name1, const auto& name2) { return f; }

#endif // BLOCKS_DEFINES_H
