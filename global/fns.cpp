//
// Created by pc on 16-12-20.
//

#include "fns.h"

namespace fns {


    bool empty(const std::string& string) {
        return string.find_first_not_of(' ') == std::string::npos;
    }

} // namespace fns
