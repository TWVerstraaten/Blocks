//
// Created by pc on 15-12-20.
//

#include "Cluster.h"

#include <cassert>
#include <iostream>
#include <queue>

namespace model {

    Cluster::Cluster(Matrix&& matrix, size_t rowOffset, size_t columnOffset)
        : m_rowOffset(rowOffset), m_columnOffset(columnOffset), m_matrix(matrix) {
    }

    void Cluster::doStep() {
        doAction();
    }

    int Cluster::rowOffset() const {
        return m_rowOffset;
    }

    int Cluster::columnOffset() const {
        return m_columnOffset;
    }

    void Cluster::doAction() {
        if (m_actions.empty()) {
            return;
        }
        switch (m_actions[m_actionIndex]) {
            case Action::MOVE_UP:
                m_rowOffset -= 1;
                break;
            case Action::MOVE_DOWN:
                m_rowOffset += 1;
                break;
            case Action::MOVE_LEFT:
                m_columnOffset -= 1;
                break;
            case Action::MOVE_RIGHT:
                m_columnOffset += 1;
                break;
        }
        ++m_actionIndex;
        m_actionIndex %= m_actions.size();
    }

    void Cluster::rotateClockWiseAbout(const IndexPair& indexPair) {
        m_rowOffset += indexPair.row() - indexPair.column();
        m_columnOffset += indexPair.column() - m_matrix.rowCount() + 1 + indexPair.row();
        for (auto& action : m_actions) {
            action = rotateActionClockWise(action);
        }
        m_matrix = m_matrix.rotateClockWise();
    }

    const Matrix& Cluster::matrix() const {
        return m_matrix;
    }

    void Cluster::rotateCounterClockWiseAbout(const IndexPair& indexPair) {
        m_rowOffset += indexPair.row() + indexPair.column() - m_matrix.columnCount() + 1;
        m_columnOffset += indexPair.column() - indexPair.row();
        for (auto& action : m_actions) {
            action = rotateActionCounterClockWise(action);
        }
        m_matrix = m_matrix.rotateCounterClockWise();
    }

    Cluster::Action Cluster::rotateActionClockWise(Cluster::Action action) {
        switch (action) {
            case Action::MOVE_UP:
                return Action::MOVE_RIGHT;
            case Action::MOVE_DOWN:
                return Action::MOVE_LEFT;
            case Action::MOVE_LEFT:
                return Action::MOVE_UP;
            case Action::MOVE_RIGHT:
                return Action::MOVE_DOWN;
            default:
                assert(false);
        }
    }

    Cluster::Action Cluster::rotateActionCounterClockWise(Cluster::Action action) {
        switch (action) {
            case Action::MOVE_UP:
                return Action::MOVE_LEFT;
            case Action::MOVE_DOWN:
                return Action::MOVE_RIGHT;
            case Action::MOVE_LEFT:
                return Action::MOVE_DOWN;
            case Action::MOVE_RIGHT:
                return Action::MOVE_UP;
            default:
                assert(false);
        }
    }

    void Cluster::addAction(Cluster::Action action) {
        m_actions.push_back(action);
    }

    void Cluster::removeBLock(const IndexPair& indexPair) {
        m_matrix.set(indexPair.row(), indexPair.column(), false);
    }


    bool Cluster::empty() const {
        return m_matrix.begin().isDone();
    }

} // namespace model