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
        explicit CommandVector(const std::vector<std::string>& strings);

        /****** CONST GETTERS  ******/
        [[nodiscard]] size_t                          commandIndex() const;
        [[nodiscard]] size_t                          size() const;
        [[nodiscard]] size_t                          repeatCount() const;
        [[nodiscard]] const std::vector<Command>&     commands() const;
        [[nodiscard]] const std::vector<std::string>& strings() const;

        /****** CONST FUNCTIONS  ******/
        [[nodiscard]] bool             wellFormed() const;
        [[nodiscard]] bool             empty() const;
        [[nodiscard]] Command          currentCommand() const;
        [[nodiscard]] COMMAND_TYPE     currentType() const;
        [[nodiscard]] COMMAND_MODIFIER currentModifier() const;
        [[nodiscard]] bool             currentIsRepeat() const;

        /****** NON CONST FUNCTIONS  ******/
        void clear();
        void increment();
        void set(const std::vector<std::string>& strings);

      private:
        /****** PRIVATE STATIC FUNCTIONS  ******/
        static COMMAND_TYPE     getType(const Command& c);
        static COMMAND_MODIFIER getModifier(const Command& c);

        /****** DATA MEMBERS  ******/
        size_t                   m_commandIndex = 0;
        size_t                   m_repeatCount  = 0;
        std::vector<Command>     m_commands;
        std::vector<std::string> m_strings;
    };

} // namespace model

#endif // BLOCKS_COMMANDVECTOR_H
