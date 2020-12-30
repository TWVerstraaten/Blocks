//
// Created by pc on 16-12-20.
//

#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include <map>
#include <string>

namespace model {

    struct Command {

        /****** PUBLIC ENUMS / TYPEDEFS  ******/

        /*
         *  TODO add the types STP and GRB.
         *  Also want to implement IF type, followed by a check such as "IF SZE > 2" or maybe "IF UP"
         *  The command after these is skipped if the condition was false
         *  Will probably need to do this using std::variant for the different action types.
         *  Such branching will complicate the parsing and highlighting of CommandEditBox.
         */
        enum class TYPE { SKP, FWD, BCK, LFT, RHT, STP, GRB };
        enum class MODIFIER { NONE, IGNORE, INCREMENT };

        /****** CONSTRUCTORS / DESTRUCTORS  ******/
        Command() = default;
        Command(TYPE type, MODIFIER modifier);

        /****** PUBLIC STATIC FUNCTIONS  ******/
        static bool        canParse(const std::string& string);
        static bool        isFormatted(const std::string& string);
        static TYPE        typeFromString(const std::string& string);
        static MODIFIER    modifierFromString(const std::string& string);
        static Command     fromString(const std::string& string);
        static std::string stringFromModifier(MODIFIER modifier);
        static std::string stringFromType(TYPE modifier);
        static std::string formatCommandString(const std::string& string);

        /****** DATA MEMBERS  ******/
        TYPE     m_type     = TYPE::FWD;
        MODIFIER m_modifier = MODIFIER::NONE;

        /****** PUBLIC STATIC DATA MEMBERS  ******/
        inline static const std::map<std::string, TYPE>     s_typeMap{{"RHT", TYPE::RHT},
                                                                  {"LFT", TYPE::LFT},
                                                                  {"FWD", TYPE::FWD},
                                                                  {"BCK", TYPE::BCK},
                                                                  {"SKP", TYPE::SKP},
                                                                  {"STP", TYPE::STP},
                                                                  {"GRB", TYPE::GRB}};
        inline static const std::map<std::string, MODIFIER> s_modifierMap{
            {"+", MODIFIER::IGNORE}, {"-", MODIFIER::INCREMENT}, {".", MODIFIER::NONE}};
    };
} // namespace model

#endif // BLOCKS_COMMAND_H
