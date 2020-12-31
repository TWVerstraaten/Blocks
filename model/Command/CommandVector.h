//
// Created by pc on 30-12-20.
//

#ifndef BLOCKS_COMMANDVECTOR_H
#define BLOCKS_COMMANDVECTOR_H

#include "Command.h"

#include <string>
#include <vector>

namespace model {

    class CommandVector {

      public:
        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        explicit CommandVector(const std::vector<std::string>& strings);

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool    wellFormed() const;
        [[nodiscard]] bool    empty() const;
        [[nodiscard]] Command currentCommand() const;

        /****** NON CONST FUNCTIONS  ******/
        void increment();
        void set(const std::vector<std::string>& strings);

      private:
        /****** DATA MEMBERS  ******/
        size_t               m_commandIndex = 0;
        size_t               m_repeatCount  = 0;
        std::vector<Command> m_commands;
    };

} // namespace model

#endif // BLOCKS_COMMANDVECTOR_H
