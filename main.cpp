#define SDL_MAIN_HANDLED

#include "app/Application.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    app::Application applicationLevel;
    applicationLevel.run();
    return 0;
}
