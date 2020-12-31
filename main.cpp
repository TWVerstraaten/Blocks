#define SDL_MAIN_HANDLED

#include "app/Application.h"

int main(int argc, char* argv[]) {

    //    model::CommandVector c{{"LFT 1", "RHT 2", " ### ", "FWD", "BCK 2 "}};
    //
    //    for (size_t i = 0; i != 8; ++i) {
    //        std::cout << model::CommandParser::toString(c.currentCommand()) << '\n';
    //        c.increment();
    //    }

    app::Application applicationLevel;
    applicationLevel.run();

    return 0;
}
