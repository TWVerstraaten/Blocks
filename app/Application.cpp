//
// Created by pc on 15-12-20.
//

#include "Application.h"

#include "../view/View_constants.h"
#include "../view/widget/ScrollArea_constants.h"
#include "Application_Edit.h"
#include "Application_Run.h"

#include <cassert>

namespace app {
    Application::Application()
        : m_scrollArea({view::INITIAL_SCREEN_WIDTH - view::widget::COMMAND_SCROLL_AREA_WIDTH,
                        0,
                        view::widget::COMMAND_SCROLL_AREA_WIDTH,
                        view::INITIAL_SCREEN_HEIGHT}) {
        m_model.init();
        m_scrollArea.init(m_view.assets());
    }

    void Application::run() {
        bool isRunning = true;

        Application_Edit editApp(&m_model, &m_view, &m_scrollArea);
        while (isRunning) {
            SDL_StartTextInput();
            switch (editLevel(editApp)) {
                case EDIT_MODE::QUIT:
                    isRunning = false;
                    break;
                case EDIT_MODE::EDITING:
                    assert(false);
                case EDIT_MODE::DONE_EDITING:
                    break;
            }
            SDL_StopTextInput();
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

    EDIT_MODE Application::editLevel(Application_Edit& editApp) {
        m_runningScrollArea = nullptr;
        m_appMode           = APP_MODE::EDITING;
        SDL_Event event;
        EDIT_MODE currentMode;
        bool      isRunning = true;
        while (isRunning) {
            while (SDL_PollEvent(&event) > 0) {
                switch (event.type) {
                    case SDL_QUIT:
                        isRunning = false;
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            handleWindowEvent();
                        }
                        break;
                    case SDL_KEYDOWN:
                        if (editApp.hasFocus() && editApp.canStart()) {
                            switch (event.key.keysym.sym) {
                                case SDLK_SPACE:
                                    editApp.finalize();
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_1:
                                    editApp.finalize();
                                    m_timeStep = TIME_STEP_SLOW;
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_2:
                                    editApp.finalize();
                                    m_timeStep = TIME_STEP_MEDIUM;
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_3:
                                    editApp.finalize();
                                    m_timeStep = TIME_STEP_FAST;
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
                    return currentMode;
            }
        }
        return EDIT_MODE::QUIT;
    }

    RUN_MODE Application::runLevel() {
        m_appMode = APP_MODE::RUNNING;
        SDL_Event       event;
        Application_Run runApp(m_model, &m_view, m_scrollArea);
        m_runningScrollArea = &runApp.scrollArea();
        runApp.setTimeStep(m_timeStep);
        RUN_MODE currentMode;
        bool     isRunning = true;
        while (isRunning) {
            while (SDL_PollEvent(&event) > 0) {
                switch (event.type) {
                    case SDL_QUIT:
                        isRunning = false;
                        break;
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            handleWindowEvent();
                        }
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

    void Application::handleWindowEvent() {
        const auto windowSize = m_view.windowSize();
        m_scrollArea.setX(windowSize.x() - view::widget::COMMAND_SCROLL_AREA_WIDTH);
        m_scrollArea.setHeight(windowSize.y());
        if (m_appMode == APP_MODE::RUNNING) {
            assert(m_runningScrollArea != nullptr);
            m_runningScrollArea->setX(windowSize.x() - view::widget::COMMAND_SCROLL_AREA_WIDTH);
            m_runningScrollArea->setHeight(windowSize.y());
        }
    }
} // namespace app