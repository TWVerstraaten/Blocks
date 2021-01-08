//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMAND_ENUMS_H
#define BLOCKS_COMMAND_ENUMS_H

namespace model {
    enum class COMMAND_TYPE { NONE, SKP, FWD, BCK, LFT, RHT, STP, GRB, SPL };
    enum class COMMAND_LOOP { FORI };
    enum class COMMAND_IF { IFSIZE };
    enum class COMMAND_COMPARE { GREATER, GREATER_EQUAL, LESS, LESS_EQUAL, EQUAL };
    enum class COMMAND_MODIFIER { NONE, IGNORE, INCREMENT };

} // namespace model

#endif // BLOCKS_COMMAND_ENUMS_H
