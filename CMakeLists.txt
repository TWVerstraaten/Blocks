cmake_minimum_required(VERSION 3.5)
project(Blocks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Blocks_SOURCE_DIR}/bin)

if (WIN32)
    set(SDL2_INCLUDE_DIRS "C:/build/SDL2-2.0.14/include")
    set(SDL2_TTF_INCLUDE_DIRS "C:/build/SDL2_ttf-2.0.15")
    set(SDL2_IMAGE_INCLUDE_DIRS "C:/build/SDL2_image-2.0.5")

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    link_directories(C:/build/dll)
endif (WIN32)

if (UNIX)
    include_directories(/usr/include/SDL2)
endif (UNIX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2_image -lSDL2_ttf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

add_executable(Blocks ${dll_file} ${assets_files} main.cpp app/ModelViewInterface.cpp app/ModelViewInterface.h action/GenericCommandEditBoxAction.cpp action/GenericCommandEditBoxAction.cpp action/RemoveBlockFromClusterAction.cpp action/RemoveBlockFromClusterAction.h action/RemoveClusterAction.cpp action/RemoveClusterAction.h action/AddBlockToClusterAction.cpp action/AddBlockToClusterAction.h action/GenericModelAction.cpp action/GenericModelAction.h action/AddClusterAction.cpp action/AddClusterAction.h model/Command.cpp model/Command.h view/widget/ScrollArea.cpp view/widget/ScrollArea.h view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/ViewPort.cpp view/ViewPort.h model/Model.cpp model/Model.h app/Application.cpp app/Application.h model/Level.cpp model/Level.h model/GridXY.cpp model/GridXY.h model/Command.cpp model/Command.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/Assets.cpp view/Assets.h view/widget/LineEditBox.cpp view/widget/LineEditBox.h view/widget/RectWidget.cpp view/widget/RectWidget.h global/fns.cpp global/fns.h view/widget/SelectionData.cpp view/widget/SelectionData.h view/ScreenXY.cpp view/ScreenXY.h model/WorldXY.cpp model/WorldXY.h app/Application_Edit.cpp app/Application_Edit.h app/Application_Run.cpp app/Application_Run.h view/TextureWrapper.cpp view/TextureWrapper.h global/cst.h view/widget/Widget.cpp view/widget/Widget.h view/Mouse.cpp view/Mouse.h view/widget/CommandEditBox.cpp view/widget/CommandEditBox.h model/Line.h global/alg.cpp global/alg.h global/geom.cpp global/geom.h action/Action.h view/widget/BlockSelectWidget.cpp view/widget/BlockSelectWidget.h model/BlockType_typedef.h global/overloaded.h action/AddLevelBlockAction.cpp action/AddLevelBlockAction.h action/RemoveLevelBlockAction.cpp action/RemoveLevelBlockAction.h model/Cluster_enums.h)