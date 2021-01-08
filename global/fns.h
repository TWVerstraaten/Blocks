//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_FNS_H
#define BLOCKS_FNS_H

#include <cassert>
#include <string>

namespace fns {

    template <typename Type>
    Type clamp(Type value, Type lower, Type upper) {
        assert(lower <= upper);
        return value < lower ? lower : (value > upper ? upper : value);
    }

} // namespace fns

#endif // BLOCKS_FNS_H
