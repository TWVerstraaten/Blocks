//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_COMMAND_TEMP_H
#define BLOCKS_COMMAND_TEMP_H

#include "Command/Command_enums.h"

#include <map>
#include <string>

namespace model {

    struct Command_Temp {

        /****** PUBLIC ENUMS / TYPEDEFS  ******/

        /*
         *  TODO add the types STP and GRB.
         *  Also want to implement IF type, followed by a check such as "IF SZE > 2" or maybe "IF UP"
         *  The command after these is skipped if the condition was false
         *  Will probably need to do this using std::variant for the different action types.
         *  Such branching will complicate the parsing and highlighting of CommandEditBox.
         */

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Command_Temp() = default;
        Command_Temp(COMMAND_TYPE type, COMMAND_MODIFIER modifier);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static bool             canParse(const std::string& string);
        static bool             isFormatted(const std::string& string);
        static COMMAND_TYPE     typeFromString(const std::string& string);
        static COMMAND_MODIFIER modifierFromString(const std::string& string);
        static Command_Temp     fromString(const std::string& string);
        static std::string      stringFromModifier(COMMAND_MODIFIER modifier);
        static std::string      stringFromType(COMMAND_TYPE type);
        static std::string      formatCommandString(const std::string& string);

        /****** DATA MEMBERS  ******/
        COMMAND_TYPE     m_type     = COMMAND_TYPE::FWD;
        COMMAND_MODIFIER m_modifier = COMMAND_MODIFIER::NONE;

        /****** PUBLIC STATIC DATA MEMBERS  ******/
        inline static const std::map<std::string, COMMAND_TYPE>     s_typeMap{{"RHT", COMMAND_TYPE::RHT},
                                                                          {"LFT", COMMAND_TYPE::LFT},
                                                                          {"FWD", COMMAND_TYPE::FWD},
                                                                          {"BCK", COMMAND_TYPE::BCK},
                                                                          {"SKP", COMMAND_TYPE::SKP},
                                                                          {"STP", COMMAND_TYPE::STP},
                                                                          {"GRB", COMMAND_TYPE::GRB},
                                                                          {"SPL", COMMAND_TYPE::SPL}};
        inline static const std::map<std::string, COMMAND_MODIFIER> s_modifierMap{
            {"+", COMMAND_MODIFIER::IGNORE}, {"-", COMMAND_MODIFIER::INCREMENT}, {".", COMMAND_MODIFIER::NONE}};
    };
} // namespace model

#endif // BLOCKS_COMMAND_TEMP_H
