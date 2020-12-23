//
// Created by pc on 15-12-20.
//

#include "Application_Level.h"

#include "Application_Edit.h"
#include "Application_Run.h"

#include <cassert>

Application_Level::Application_Level() = default;

void Application_Level::run() {
    bool isRunning = true;
    while (isRunning) {
        switch (editLevel()) {
            case EDIT_MODE::QUIT:
                isRunning = false;
                break;
            case EDIT_MODE::EDITING:
                assert(false);
            case EDIT_MODE::DONE_EDITING:
                break;
        }

        if (not isRunning) {
            break;
        }

        switch (runLevel()) {
            case RUN_MODE::QUIT:
                isRunning = false;
                break;
            case RUN_MODE::RUNNING:
                break;
            case RUN_MODE::COMPLETED:
                std::cout << "Completed, how?\n";
                break;
            case RUN_MODE::FAILED:
                std::cout << "Failed, how?\n";
                break;
            case RUN_MODE::GIVE_UP:
                std::cout << "Gave up.\n";
                break;
        }
    }
}

Application_Level::EDIT_MODE Application_Level::editLevel() {
    bool isRunning = true;

    SDL_Event        event;
    Application_Edit editApp(&m_model, &m_view);
    EDIT_MODE        currentMode;
    while (isRunning) {
        while (SDL_PollEvent(&event) > 0) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if (editApp.hasFocus() && editApp.canStart()) {
                        switch (event.key.keysym.sym) {
                            case SDLK_SPACE:
                                editApp.finalize();
                                return EDIT_MODE::DONE_EDITING;
                            case SDLK_TAB:
                                editApp.finalize();
                                m_pauseAfterFirstStep = true;
                                return EDIT_MODE::DONE_EDITING;
                            case SDLK_1:
                                editApp.finalize();
                                m_timeStep = global::m_timeStepSlow;
                                return EDIT_MODE::DONE_EDITING;
                            case SDLK_2:
                                editApp.finalize();
                                m_timeStep = global::m_timeStepMedium;
                                return EDIT_MODE::DONE_EDITING;
                            case SDLK_3:
                                editApp.finalize();
                                m_timeStep = global::m_timeStepFast;
                                return EDIT_MODE::DONE_EDITING;
                        }
                    }
                default:
                    editApp.handleEvent(event);
            }
        }
        currentMode = editApp.performSingleLoop();
        switch (currentMode) {
            case EDIT_MODE::EDITING:
                break;
            default:
                m_timeStep = editApp.timeStep();
                return currentMode;
        }
    }
    return EDIT_MODE::QUIT;
}

Application_Level::RUN_MODE Application_Level::runLevel() {
    bool isRunning = true;

    SDL_Event       event;
    Application_Run runApp(m_model, &m_view);
    runApp.setTimeStep(m_timeStep);
    runApp.setPauseAfterNextStep(m_pauseAfterFirstStep);

    RUN_MODE currentMode;
    while (isRunning) {
        while (SDL_PollEvent(&event) > 0) {
            switch (event.type) {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default:
                    runApp.handleEvent(event);
            }
        }
        currentMode = runApp.performSingleLoop();
        switch (currentMode) {
            case RUN_MODE::RUNNING:
                break;
            default:
                return currentMode;
        }
    }
    return RUN_MODE::QUIT;
}
