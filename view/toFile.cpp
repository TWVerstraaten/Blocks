//
// Created by teunv on 1/19/2021.
//

#include "toFile.h"

QString view::toFile(model::DYNAMIC_BLOCK_TYPE type) {
    switch (type) {
        case model::DYNAMIC_BLOCK_TYPE::ROTATE_CW:
            return QString(":/assets/arrow_cw.png");
        case model::DYNAMIC_BLOCK_TYPE::ROTATE_CCW:
            return QString(":/assets/arrow_ccw.png");
    }
    assert(false);
    return "";
}

QString view::toFile(model::INSTANT_BLOCK_TYPE type) {
    switch (type) {
        case model::INSTANT_BLOCK_TYPE::KILL:
            return QString(":/assets/kill.png");
    }
    assert(false);
    return "";
}
