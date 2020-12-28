#define SDL_MAIN_HANDLED

#include "app/Application_Level.h"

int main(int argc, char* argv[]) {
    app::Application_Level applicationLevel;
    applicationLevel.run();

    return 0;
}
