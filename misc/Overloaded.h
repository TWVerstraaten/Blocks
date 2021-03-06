//
// Created by pc on 29-12-20.
//

#ifndef BLOCKS_OVERLOADED_H
#define BLOCKS_OVERLOADED_H

// Following two definitions from https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

#endif // BLOCKS_OVERLOADED_H
