#include "Window.h"

#include "../../audio/AudioManager.h"
#include "CentralWidget.h"
#include "MainMenuWidget.h"

#include <QEvent>
#include <QFile>
#include <QKeyEvent>

namespace view::widget {

    Window::Window(QWidget* parent) : QMainWindow(parent) {

        QFile file(":/assets/stylesheet.qss");
        file.open(QFile::ReadOnly);
        assert(file.isOpen());
        setStyleSheet(QLatin1String(file.readAll()));

        toMainMenu();

        audio::AudioManager::init(this);
        setWindowTitle(tr("Blocks"));
        resize(1000, 800);
    }

    void Window::toMainMenu() {
        auto* mainMenuWidget = new widget::MainMenuWidget(this);
        mainMenuWidget->installEventFilter(this);
        connect(mainMenuWidget->levelSelectWidget(), &widget::LevelSelectWidget::levelSelected, this, &Window::startLevel);
        setCentralWidget(mainMenuWidget);
    }

    void Window::startLevel(const std::string& path) {
        auto* centralWidget = new CentralWidget(path, this);
        connect(this, &Window::undo, centralWidget, &CentralWidget::undo);
        connect(this, &Window::redo, centralWidget, &CentralWidget::redo);
        centralWidget->installEventFilter(this);
        connect(centralWidget, &CentralWidget::quit, this, &Window::toMainMenu);
        setCentralWidget(centralWidget);
    }

    bool Window::eventFilter([[maybe_unused]] QObject* watched, QEvent* event) {
        if (event->type() != QEvent::KeyPress) {
            return false;
        }

        auto*        keyEvent = dynamic_cast<QKeyEvent*>(event);
        QKeySequence keySequence{static_cast<int>(keyEvent->modifiers()) + keyEvent->key()};

        if (keySequence == QKeySequence::Undo) {
            emit undo();
            return true;
        } else if (keySequence == QKeySequence::Redo) {
            emit redo();
            return true;
        }
        return false;
    }

} // namespace view::widget