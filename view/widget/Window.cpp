#include "Window.h"

#include "../../Io/SettingsManager.h"
#include "../../audio/AudioManager.h"
#include "CentralWidget.h"
#include "MainMenuWidget.h"

#include <QEvent>
#include <QFile>
#include <QKeyEvent>
namespace view::widget {

    Window::Window(QWidget* parent) : QMainWindow(parent) {
        setMinimumWidth(600);
        setMinimumHeight(600);
        resize(1000, 800);

        QFile file(":/assets/stylesheet.qss");
        file.open(QFile::ReadOnly);
        assert(file.isOpen());
        setStyleSheet(QLatin1String(file.readAll()));

        audio::AudioManager::init();

        m_mainMenuWidget = new MainMenuWidget(this);
        m_mainMenuWidget->installEventFilter(this);
        toMainMenu();

        setWindowTitle(tr("Blocks"));
    }

    void Window::toMainMenu() {
        m_mainMenuWidget->setParent(this);
        connect(m_mainMenuWidget->levelSelectWidget(), &widget::LevelSelectWidget::levelSelected, this, &Window::startLevel);
        setCentralWidget(m_mainMenuWidget);
    }

    void Window::startLevel(const std::string& path) {
        m_mainMenuWidget->setParent(nullptr);
        auto* centralWidget = new CentralWidget(path, this);
        connect(this, &Window::undo, centralWidget, &CentralWidget::undo);
        connect(this, &Window::redo, centralWidget, &CentralWidget::redo);
        centralWidget->installEventFilter(this);
        connect(centralWidget, &CentralWidget::stopLevel, this, &Window::toMainMenu);
        setCentralWidget(centralWidget);
        centralWidget->setFocus();
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

    void Window::setWindowMode(WINDOW_MODE mode) {
        switch (mode) {
            case WINDOW_MODE::FULL_SCREEN:
                showFullScreen();
                setWindowFlags(windowFlags() & (~Qt::FramelessWindowHint));
                break;
            case WINDOW_MODE::BORDERLESS:
                hide();
                showMaximized();
                setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
                show();
                break;
            case WINDOW_MODE::MAXIMIZED:
                setWindowFlags(windowFlags() & (~Qt::FramelessWindowHint));
                showMaximized();
                break;
        }
    }

} // namespace view::widget
