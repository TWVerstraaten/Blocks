//
// Created by pc on 16-12-20.
//

#include "fns.h"

namespace fns {

    std::string trimWhiteSpace(const std::string& string) {
        const auto startIndex = string.find_first_not_of(" \t");
        if (startIndex == std::string::npos) {
            return "";
        } else {
            const auto endIndex = string.find_last_not_of(" \t");
            return string.substr(startIndex, endIndex - startIndex + 1);
        }
    }

    bool empty(const std::string& string) {
        return trimWhiteSpace(string).empty();
    }

} // namespace fns
