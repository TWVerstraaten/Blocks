#define SDL_MAIN_HANDLED

#include "app/Application.h"
#include "model/Command/CommandParser.h"

int main(int argc, char* argv[]) {

    model::CommandParser::parseString(" BCK  31");

    //    app::Application applicationLevel;
    //    applicationLevel.run();

    return 0;
}
