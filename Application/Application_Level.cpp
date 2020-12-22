//
// Created by pc on 15-12-20.
//

#include "Application_Level.h"

#include "Application_Edit.h"
#include "Application_Run.h"

Application_Level::Application_Level() = default;

void Application_Level::run() {

    bool isRunning = true;

    while (isRunning) {
        Application_Edit applicationEdit(&m_model, &m_view);
        if (applicationEdit.run() == Application_Edit::EXIT_CODE::QUIT) {
            isRunning = false;
        } else {
            Application_Run applicationRun(m_model, &m_view);
            applicationRun.setTimeStep(applicationEdit.timeStep());
            switch (applicationRun.run()) {
                case Application_Run::EXIT_CODE::QUIT:
                    isRunning = false;
                    break;
                case Application_Run::EXIT_CODE::COMPLETED:
                    std::cout << "COMPLETED LEVEL. Hoe?\n";
                    break;
                case Application_Run::EXIT_CODE::GIVE_UP:
                    break;
                case Application_Run::EXIT_CODE::FAILED:
                    std::cout << "FAILED LEVEL. Hoe?\n";
                    break;
            }
        }
    }
}