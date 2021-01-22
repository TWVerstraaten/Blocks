////
//// Created by pc on 22-12-20.
////
//
//#include "ApplicationRun.h"
//
//#include "../misc/defines.h"
//#include "../misc/geom.h"
//#include "../view/Mouse.h"
//#include "ModelViewInterface.h"
//
//#include <cassert>
//
//namespace app {
//    ApplicationRun::ApplicationRun(const model::Model& model, view::View* view, const view::widget::ScrollArea& scrollArea)
//        : m_view(view), m_scrollArea(scrollArea) {
//        m_model             = model;
//        m_timeSinceLastStep = 0;
//        m_previousTime      = SDL_GetTicks();
//    }
//
//    void ApplicationRun::mouseWheelEvent(const SDL_Event& event) {
//        m_view->zoom(event.wheel.y);
//    }
//
//    void ApplicationRun::keyEvent(const SDL_Event& event) {
//        if (event.type == SDL_KEYDOWN) {
//            switch (event.key.keysym.sym) {
//                case SDLK_ESCAPE:
//                    m_runningMode = RUN_MODE::GIVE_UP;
//                    break;
//                case SDLK_SPACE:
//                    togglePause();
//                    break;
//                case SDLK_1:
//                    setTimeStep(TIME_STEP_SLOW);
//                    break;
//                case SDLK_2:
//                    setTimeStep(TIME_STEP_MEDIUM);
//                    break;
//                case SDLK_3:
//                    setTimeStep(TIME_STEP_FAST);
//                    break;
//                default:
//                    break;
//            }
//        }
//    }
//
//    void ApplicationRun::mouseClickEvent(const SDL_Event& event) {
//        switch (event.button.button) {
//            case SDL_BUTTON_RIGHT:
//                m_rightMouseButtonPressed = true;
//                m_previousMousePosition   = view::Mouse::mouseXy();
//                break;
//            case SDL_BUTTON_LEFT:
//                m_leftMouseButtonPressed = true;
//                m_previousMousePosition  = view::Mouse::mouseXy();
//                break;
//            default:
//                break;
//        }
//    }
//
//    void ApplicationRun::mouseReleaseEvent(const SDL_Event& event) {
//        switch (event.button.button) {
//            case SDL_BUTTON_RIGHT:
//                m_rightMouseButtonPressed = false;
//                break;
//            case SDL_BUTTON_LEFT:
//                m_leftMouseButtonPressed = false;
//                break;
//            default:
//                break;
//        }
//    }
//
//    void ApplicationRun::mouseMoveEvent([[maybe_unused]] const SDL_Event& event) {
//        if (m_rightMouseButtonPressed) {
//            const auto mouseXy = view::Mouse::mouseXy();
//            m_view->translate((mouseXy.x() - m_previousMousePosition.x()), mouseXy.y() - m_previousMousePosition.y());
//            m_previousMousePosition = mouseXy;
//        }
//    }
//
//    void ApplicationRun::setTimeStep(Uint32 timeStep) {
//        m_paused   = false;
//        m_timeStep = timeStep;
//    }
//
//    void ApplicationRun::togglePause() {
//        m_paused = !m_paused;
//    }
//
//    void ApplicationRun::initializeInteractStep() {
//        for (auto& cluster : m_model.clusters()) {
//            if (cluster.isAlive()) {
//                cluster.resetPhase();
//                cluster.incrementCommandIndex();
//            }
//        }
//        ModelViewInterface::interactWithInstantBlocks(m_model, m_scrollArea);
//        doConwayStep();
//
//        for (auto& cluster : m_model.clusters()) {
//            ModelViewInterface::splitIfDisconnected(m_model, m_scrollArea, cluster);
//        }
//
//        m_model.level().buildSides();
//        ModelViewInterface::removeActionBoxesOfRemovedClusters(m_model.clusters(), m_scrollArea);
//        ModelViewInterface::updateSelection(m_model.clusters(), m_scrollArea);
//        m_currentStep = CURRENT_STEP::INTERACT;
//    }
//
//    void ApplicationRun::initializeMovingStep() {
//        m_model.resetPhase();
//        for (auto& cluster : m_model.clusters()) {
//            ModelViewInterface::stopSpliceOrKillIfNeeded(m_model.level(), cluster);
//        }
//        addStoppedClustersToLevel();
//        killDoubleGrabbers();
//        for (auto& cluster : m_model.clusters()) {
//            if (not ModelViewInterface::interactWithDynamicBlocks(m_model.level(), cluster)) {
//                cluster.doCommand(m_model);
//            }
//        }
//        for (auto& cluster : m_model.clusters()) {
//            cluster.buildSides();
//        }
//        m_model.level().buildSides();
//        ModelViewInterface::updateCommandScrollArea(m_model, m_scrollArea, APP_MODE::RUNNING);
//        m_currentStep = CURRENT_STEP::MOVING;
//    }
//
//    RUN_MODE ApplicationRun::performSingleLoop() {
//        const auto currentTime = SDL_GetTicks();
//        if (not m_paused) {
//            m_timeSinceLastStep += SDL_GetTicks() - m_previousTime;
//            const auto dt = currentTime - m_previousTime;
//            switch (m_currentStep) {
//                case CURRENT_STEP::MOVING:
//                    if (static_cast<Uint32>(m_timeSinceLastStep) >= m_timeStep) {
//                        initializeInteractStep();
//                        m_timeSinceLastStep %= m_timeStep;
//                    } else {
//                        m_model.update(dt / static_cast<double>(m_timeStep));
//                    }
//                    break;
//                case CURRENT_STEP::INTERACT:
//                    if (static_cast<Uint32>(m_timeSinceLastStep) >= m_timeStep) {
//                        initializeMovingStep();
//                        m_timeSinceLastStep %= m_timeStep;
//                        m_model.update(dt / static_cast<double>(m_timeStep));
//                    }
//                    break;
//            }
//        }
//        draw();
//        m_previousTime = currentTime;
//        return m_runningMode;
//    }
//
//    void ApplicationRun::handleEvent(const SDL_Event& event) {
//        switch (event.type) {
//            case SDL_KEYDOWN:
//            case SDL_KEYUP:
//                keyEvent(event);
//                break;
//            case SDL_WINDOWEVENT:
//                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {}
//                break;
//            case SDL_MOUSEBUTTONDOWN:
//                mouseClickEvent(event);
//                break;
//            case SDL_MOUSEBUTTONUP:
//                mouseReleaseEvent(event);
//                break;
//            case SDL_MOUSEMOTION:
//                mouseMoveEvent(event);
//                break;
//            case SDL_MOUSEWHEEL: {
//                mouseWheelEvent(event);
//            }
//        }
//    }
//
//    view::widget::ScrollArea& ApplicationRun::scrollArea() {
//        return m_scrollArea;
//    }
//
//    void ApplicationRun::addStoppedClustersToLevel() {
//        auto it = std::partition(D_IT(m_model.clusters()), D_FUNC(cluster, cluster.state() != model::CLUSTER_STATE::STOPPED));
//        m_model.level().stoppedClusters().splice(m_model.level().stoppedClusters().end(), m_model.clusters(), it, m_model.clusters().end());
//    }
//
//    void ApplicationRun::draw() {
//        m_view->draw(m_model);
//        m_view->drawScrollArea(&m_scrollArea);
//        m_view->renderPresent();
//    }
//
//    void ApplicationRun::killDoubleGrabbers() {
//        std::vector<model::Cluster*> killList;
//        for (auto& stoppedCluster : m_model.level().stoppedClusters()) {
//            const auto                   adjacentClusters = geom::neighbors(m_model.clusters(), stoppedCluster);
//            std::vector<model::Cluster*> grabbers;
//            std::copy_if(D_CIT(adjacentClusters),
//                         std::back_inserter(grabbers),
//                         D_FUNC(cluster, cluster->commandVector().currentType() == model::COMMAND_TYPE::GRB));
//            if (grabbers.size() > 1) {
//                std::copy(D_IT(grabbers), std::back_inserter(killList));
//            }
//        }
//        for (auto* cluster : killList) {
//            cluster->kill();
//        }
//    }
//
//    void ApplicationRun::doConwayStep() {
//        std::vector<model::GridXy> toRemove;
//        std::vector<model::GridXy> toAdd;
//        const auto                 conwayBlocks = m_model.level().blocks(model::FLOOR_BLOCK_TYPE::CONWAY);
//        for (const auto conwayBlock : conwayBlocks) {
//            size_t neighborCount = 0;
//            for (int i = -1; i <= 1; ++i) {
//                for (int j = -1; j <= 1; ++j) {
//                    if (i == 0 && j == 0) {
//                        continue;
//                    }
//                    const auto adjacentBlock = conwayBlock + model::GridXy{i, j};
//                    if ((not m_model.noLiveOrStoppedClusterOnBlock(adjacentBlock))) {
//                        ++neighborCount;
//                    }
//                }
//            }
//            //            static const std::vector<size_t> B = {3, 6, 7, 8};
//            //            static const std::vector<size_t> S = {3, 4, 6, 7, 8};
//            static const std::vector<size_t> B        = {3};
//            static const std::vector<size_t> S        = {2, 3};
//            const bool                       occupied = not m_model.noLiveOrStoppedClusterOnBlock(conwayBlock);
//            if (occupied && (std::find(D_CIT(S), neighborCount) == S.end())) {
//                toRemove.emplace_back(conwayBlock);
//            } else if ((not occupied) && (std::find(D_CIT(B), neighborCount) != B.end())) {
//                toAdd.emplace_back(conwayBlock);
//            }
//        }
//        for (const auto& remove : toRemove) {
//            auto it = m_model.clusterContaining(remove);
//            if (it != m_model.clusters().end()) {
//                it->removeGridXy(remove);
//            } else {
//                it = std::find_if(D_IT(m_model.level().stoppedClusters()), D_FUNC(cluster, cluster.contains(remove)));
//                assert(it != m_model.level().stoppedClusters().end());
//                it->removeGridXy(remove);
//            }
//        }
//        for (const auto& add : toAdd) {
//            const auto neighbors = geom::neighbors(m_model.clusters(), add);
//            if (neighbors.size() == 1) {
//                neighbors.front()->addGridXy(add);
//            } else {
//                const auto stoppedNeighbors = geom::neighbors(m_model.level().stoppedClusters(), add);
//                if (stoppedNeighbors.size() == 1) {
//                    stoppedNeighbors.front()->addGridXy(add);
//                } else {
//                    m_model.level().stoppedClusters().emplace_back(model::GridXyVector{add}, "Conway");
//                }
//            }
//        }
//        m_model.clusters().remove_if(D_FUNC(cluster, cluster.isEmpty()));
//        m_model.level().stoppedClusters().remove_if(D_FUNC(cluster, cluster.isEmpty()));
//    }
//} // namespace app