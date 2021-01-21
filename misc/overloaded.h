//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_OVERLOADED_H
#define BLOCKS_OVERLOADED_H

// Following two definitions from https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

#endif // BLOCKS_OVERLOADED_H
