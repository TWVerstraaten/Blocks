#define SDL_MAIN_HANDLED

#include "Application/Application_Level.h"

int main(int argc, char* argv[]) {
    Application_Level applicationLevel;
    applicationLevel.run();

    return 0;
}
