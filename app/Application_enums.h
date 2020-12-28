//
// Created by pc on 28-12-20.
//

#ifndef BLOCKS_APPLICATION_ENUMS_H
#define BLOCKS_APPLICATION_ENUMS_H

namespace app {
    enum class RUN_MODE { QUIT, RUNNING, COMPLETED, FAILED, GIVE_UP };
    enum class EDIT_MODE { QUIT, EDITING, DONE_EDITING };
    enum class APP_MODE { EDITING, RUNNING };
} // namespace app

#endif // BLOCKS_APPLICATION_ENUMS_H
