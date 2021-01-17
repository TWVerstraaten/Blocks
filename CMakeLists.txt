cmake_minimum_required(VERSION 3.5)
project(Blocks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Blocks_SOURCE_DIR}/bin)

if (WIN32)
    set(BOOST_INCLUDE_DIRS "C:\\Projects\\ext")
    set(SDL2_INCLUDE_DIRS "C:\\Projects\\ext\\SDL2-2.0.14\\include")
    set(SDL2_TTF_INCLUDE_DIRS "C:\\Projects\\ext\\SDL2_ttf-2.0.15")
    set(SDL2_IMAGE_INCLUDE_DIRS "C:\\Projects\\ext\\SDL2_image-2.0.5")

    include_directories(${BOOST_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})
    link_directories(C:\\Projects\\ext\\dlls)
endif (WIN32)

if (UNIX)
    include_directories(/usr/include/SDL2)
endif (UNIX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_definitions(-DDEBUG)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_PREFIX_PATH "C:\\Projects\\Qt\\5.15.2\\mingw81_64\\lib\\cmake")
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (WIN32)
    set(QT_QPA_PLATFORM_PLUGIN_PATH "C:\\Projects\\Qt\\5.15.2\\mingw81_64\\plugins\\platforms")
endif (WIN32)

qt5_add_resources(SOURCES textures.qrc)
qt5_add_resources(SOURCES shaders.qrc)

add_executable(Blocks main.cpp app/ModelViewInterface.cpp app/ModelViewInterface.h action/GenericCommandEditBoxAction.cpp action/GenericCommandEditBoxAction.cpp action/RemoveBlockFromClusterAction.cpp action/RemoveBlockFromClusterAction.h action/RemoveClusterAction.cpp action/RemoveClusterAction.h action/AddBlockToClusterAction.cpp action/AddBlockToClusterAction.h action/GenericModelAction.cpp action/GenericModelAction.h action/AddClusterAction.cpp action/AddClusterAction.h view/widget/ScrollArea.cpp view/widget/ScrollArea.h view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/ViewPort.cpp view/ViewPort.h model/Model.cpp model/Model.h app/Application.cpp app/Application.h model/Level.cpp model/Level.h model/GridXY.cpp model/GridXY.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/Assets.cpp view/Assets.h view/widget/LineEditBox.cpp view/widget/LineEditBox.h view/widget/RectWidget.cpp view/widget/RectWidget.h view/widget/SelectionData.cpp view/widget/SelectionData.h view/ScreenXY.cpp view/ScreenXY.h model/WorldXY.cpp model/WorldXY.h app/ApplicationEdit.cpp app/ApplicationEdit.h app/ApplicationRun.cpp app/ApplicationRun.h view/TextureWrapper.cpp view/TextureWrapper.h view/widget/Widget.cpp view/widget/Widget.h view/Mouse.cpp view/Mouse.h view/widget/CommandEditBox.cpp view/widget/CommandEditBox.h model/WorldLine.h global/geom.cpp global/geom.h action/Action.h view/widget/BlockSelectWidget.cpp view/widget/BlockSelectWidget.h model/BlockType_typedef.h global/overloaded.h action/AddLevelBlockAction.cpp action/AddLevelBlockAction.h action/RemoveLevelBlockAction.cpp action/RemoveLevelBlockAction.h model/Cluster_enums.h model/command/CommandParser.cpp model/command/CommandParser.h model/command/Command_enums.h model/command/Command_Simple.h model/command/Command_Modified.h model/command/Command_RepeatWrapper.h model/command/Command.h model/command/CommandVector.cpp model/command/CommandVector.h model/command/Command_Error.h model/command/Command.cpp view/Texture_enums.h view/Font_enums.h global/defines.h app/Application_constants.h view/View_constants.h view/Font_constants.h view/widget/LineEditBox_constants.h view/widget/RectWidget_constants.h view/widget/BlockSelectWidget_constants.h view/widget/ScrollArea_constants.h model/WorldLine.cpp model/Model_typedefs.h model/command/Command_Label.h model/command/Command_Jump.h view/toColor.h view2/CommandEditBox.cpp view2/CommandEditBox.h view2/CommandScrollArea.cpp view2/CommandScrollArea.h view2/MainView.cpp view2/MainView.h view2/MainWindow.cpp view2/MainWindow.h view2/TextEdit.cpp view2/TextEdit.h view2/CentralWidget.cpp view2/CentralWidget.h action/GenericTextEditAction.cpp action/GenericTextEditAction.h action/NewClusterAction.cpp action/NewClusterAction.h)

target_link_libraries(Blocks Qt5::Widgets)
target_link_libraries(Blocks SDL2)
target_link_libraries(Blocks SDL2_image)
target_link_libraries(Blocks SDL2_ttf)
target_compile_options(Blocks PRIVATE -Wall -Wextra -pedantic)