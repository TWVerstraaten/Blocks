//
// Created by teunv on 1/8/2021.
//

#ifndef BLOCKS_DEFINES_H
#define BLOCKS_DEFINES_H

#define _NOTE_(x) fprintf(stderr, x ": %s:%d\n", __FILE__, __LINE__);
#define _NOTE_ONCE_(x)                                                                                                                               \
    static bool noteSeen = false;                                                                                                                    \
    if (not noteSeen)                                                                                                                                \
        _NOTE_(x)                                                                                                                                    \
    noteSeen = true;

#define _IT_(x) x.begin(), x.end()
#define _CIT_(x) x.cbegin(), x.cend()
#define _FUNC_(name, f) [&](const auto& name) { return f; }
#define _FUNC_2_(name1, name2, f) [&](const auto& name1, const auto& name2) { return f; }

#endif // BLOCKS_DEFINES_H
