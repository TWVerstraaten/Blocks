#define SDL_MAIN_HANDLED

#include "app/Application.h"

int main(int argc, char* argv[]) {
    app::Application applicationLevel;
    applicationLevel.run();

    return 0;
}
