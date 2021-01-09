#define SDL_MAIN_HANDLED

#include "app/Application.h"
#include "model/command/CommandParser.h"

int main(int argc, char* argv[]) {
    app::Application applicationLevel;
    applicationLevel.run();

    //    std::vector<std::string>   strings = {"BCK", " ", "+    FWD", "LBL DSA"};
    //    const model::CommandVector cv(strings);
    //    std::cout << cv.toString() << '\n';
    return 0;
}
