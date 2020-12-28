//
// Created by pc on 15-12-20.
//

#include "Application_Level.h"

#include "Application_Edit.h"
#include "Application_Run.h"

#include <algorithm>
#include <cassert>

namespace app {
    Application_Level::Application_Level()
        : m_scrollArea({cst::INITIAL_SCREEN_WIDTH - cst::COMMAND_SCROLL_AREA_WIDTH,
                        0,
                        cst::COMMAND_SCROLL_AREA_WIDTH,
                        static_cast<int>(cst::INITIAL_SCREEN_HEIGHT)}) {
        m_model.init();
        m_scrollArea.init(m_view.assets());
    }

    void Application_Level::run() {
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

    EDIT_MODE Application_Level::editLevel(Application_Edit& editApp) {
        bool isRunning = true;

        SDL_Event event;

        EDIT_MODE currentMode;
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
                                case SDLK_TAB:
                                    editApp.finalize();
                                    m_pauseAfterFirstStep = true;
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_1:
                                    editApp.finalize();
                                    m_timeStep = cst::TIME_STEP_SLOW;
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_2:
                                    editApp.finalize();
                                    m_timeStep = cst::TIME_STEP_MEDIUM;
                                    return EDIT_MODE::DONE_EDITING;
                                case SDLK_3:
                                    editApp.finalize();
                                    m_timeStep = cst::TIME_STEP_FAST;
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

    RUN_MODE Application_Level::runLevel() {
        bool isRunning = true;

        SDL_Event       event;
        Application_Run runApp(m_model, &m_view, m_scrollArea);
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

    static void removeActionBoxesOfRemovedClusters(const std::list<model::Cluster>& clusters,
                                                   view::widget::ScrollArea*        scrollArea) {
        scrollArea->children().remove_if([&](const view::widget::CommandEditBox& box) {
            return std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) {
                       return cluster.index() == box.index();
                   }) == clusters.end();
        });
    }

    static void highlightActionBoxLines(const std::list<model::Cluster>& clusters,
                                        view::widget::ScrollArea*        scrollArea) {
        for (auto& commandEditBox : scrollArea->children()) {
            auto it = std::find_if(clusters.begin(), clusters.end(), [&](const auto& cluster) {
                return cluster.index() == commandEditBox.index();
            });
            assert(it != clusters.end());
            if (it->isAlive()) {
                commandEditBox.setHighLightedLine(it->commandIndex());
            }
            commandEditBox.setActive(it->isAlive());
        }
    }

    static void addActionBoxesOfNewClusters(const std::list<model::Cluster>& clusters,
                                            view::widget::ScrollArea*        scrollArea) {
        auto it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
            return std::find_if(scrollArea->children().begin(),
                                scrollArea->children().end(),
                                [&](const view::widget::CommandEditBox& box) {
                                    return box.index() == cluster.index();
                                }) == scrollArea->children().end();
        });
        while (it != clusters.end()) {
            scrollArea->addCommandEditBox(*it);
            it = std::find_if(clusters.begin(), clusters.end(), [&](const model::Cluster& cluster) {
                return std::find_if(scrollArea->children().begin(),
                                    scrollArea->children().end(),
                                    [&](const view::widget::CommandEditBox& box) {
                                        return box.index() == cluster.index();
                                    }) == scrollArea->children().end();
            });
        }
    }

    void Application_Level::updateCommandScrollArea(const std::list<model::Cluster>& clusters,
                                                    view::widget::ScrollArea*        scrollArea,
                                                    APP_MODE                         mode) {
        removeActionBoxesOfRemovedClusters(clusters, scrollArea);
        if (mode == APP_MODE::RUNNING) {
            highlightActionBoxLines(clusters, scrollArea);
        }
        addActionBoxesOfNewClusters(clusters, scrollArea);
        scrollArea->setHeightAndPositions();
    }

    void Application_Level::handleWindowEvent() {
        const auto windowSize = m_view.windowSize();
        m_scrollArea.setX(windowSize.x() - cst::COMMAND_SCROLL_AREA_WIDTH);
        m_scrollArea.setHeight(windowSize.y());
    }
} // namespace app