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
        CommandVector() = default;
        //        CommandVector(const CommandVector&) = default;
        //        CommandVector( CommandVector&&) = default;
        explicit CommandVector(const std::vector<std::string>& strings);

        /****** CONST GETTERS  ******/
        [[nodiscard]] size_t                      commandIndex() const;
        [[nodiscard]] const std::vector<Command>& commands() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool             wellFormed() const;
        [[nodiscard]] bool             empty() const;
        [[nodiscard]] Command          currentCommand() const;
        [[nodiscard]] COMMAND_TYPE     currentType() const;
        [[nodiscard]] COMMAND_MODIFIER currentModifier() const;

        /****** NON CONST FUNCTIONS  ******/
        void clear();
        void increment();
        void set(const std::vector<std::string>& strings);

      private:
        static COMMAND_TYPE     type(const Command& c);
        static COMMAND_MODIFIER modifier(const Command& c);

        /****** DATA MEMBERS  ******/
        size_t               m_commandIndex = 0;
        size_t               m_repeatCount  = 0;
        std::vector<Command> m_commands;
    };

} // namespace model

#endif // BLOCKS_COMMANDVECTOR_H
