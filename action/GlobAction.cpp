//
// Created by teunv on 1/17/2021.
//

#include "GlobAction.h"

void action::GlobAction::undoAction(app::ApplicationEdit& applicationEdit) {
}

void action::GlobAction::redoAction(app::ApplicationEdit& applicationEdit) {
}

void action::GlobAction::undoAction() {
    for (auto it = m_actions.rbegin(); it != m_actions.rend(); ++it) {
        it->get()->undoAction();
    }
}

void action::GlobAction::redoAction() {
    for (auto& action : m_actions) {
        action->redoAction();
    }
}

action::ACTION_TYPE action::GlobAction::type() const {
    return ACTION_TYPE::GLOB;
}

bool action::GlobAction::isEmpty() {
    return m_actions.empty();
}

void action::GlobAction::add(std::unique_ptr<Action>&& action) {
    m_actions.emplace_back(std::move(action));
}

std::unique_ptr<action::Action>&& action::GlobAction::first() {
    return std::move(m_actions.front());
}

bool action::GlobAction::single() const {
    return m_actions.size() == 1;
}
