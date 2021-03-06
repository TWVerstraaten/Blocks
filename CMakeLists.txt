cmake_minimum_required(VERSION 3.5)
project(Blocks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Blocks_SOURCE_DIR}/bin)

if (WIN32)
    set(BOOST_INCLUDE_DIRS "C:\\Projects\\ext")
    include_directories(${BOOST_INCLUDE_DIRS})

    set(QT_QPA_PLATFORM_PLUGIN_PATH "C:\\Projects\\Qt\\5.15.2\\mingw81_64\\plugins\\platforms")
    set(CMAKE_PREFIX_PATH "C:\\Projects\\Qt\\5.15.2\\mingw81_64\\lib\\cmake")
endif (WIN32)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
#add_definitions(-DNDEBUG)
add_definitions(-DDEBUG)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(ThirdParty/SFML)

include_directories(ThirdParty/SFML/include)

add_executable(Blocks main.cpp resources.qrc model/Cluster.cpp model/Cluster.h view/ViewPort.cpp view/ViewPort.h model/Model.cpp model/Model.h model/Level.cpp model/Level.h model/GridXy.cpp model/GridXy.h view/ScreenXy.cpp view/ScreenXy.h model/WorldXy.cpp model/WorldXy.h model/WorldLine.h misc/geom.cpp misc/geom.h action/Action.h model/BlockType_typedef.h misc/Overloaded.h model/Cluster_enums.h model/cmd/CommandParser.cpp model/cmd/CommandParser.h model/cmd/Command_enums.h model/cmd/Command_Simple.h model/cmd/Command_Modified.h model/cmd/Command_RepeatWrapper.h model/cmd/Command.h model/cmd/CommandVector.cpp model/cmd/CommandVector.h model/cmd/Command_Error.h model/cmd/Command.cpp misc/defines.h app/Application_constants.h view/View_constants.h model/WorldLine.cpp model/Model_typedefs.h model/cmd/Command_Label.h model/cmd/Command_Jump.h view/toColor.h view/widget/CommandEdit.cpp view/widget/CommandEdit.h view/widget/CommandScroll.cpp view/widget/CommandScroll.h view/widget/MainView.cpp view/widget/MainView.h view/widget/Window.cpp view/widget/Window.h view/widget/TextEdit.cpp view/widget/TextEdit.h view/widget/CentralWidget.cpp view/widget/CentralWidget.h action/TextEditAction.cpp action/TextEditAction.h action/NewClusterAction.cpp action/NewClusterAction.h action/DeleteClusterAction.cpp action/DeleteClusterAction.h action/RemoveBlockFromClusterAction.cpp action/RemoveBlockFromClusterAction.h action/MergeClusterAction.cpp action/MergeClusterAction.h action/SplitDisconnectedAction.cpp action/SplitDisconnectedAction.h view/widget/SyntaxHighlighter.cpp view/widget/SyntaxHighlighter.h view/widget/MainViewPainter.cpp view/widget/MainViewPainter.h view/widget/BlockSelectWidget.cpp view/widget/BlockSelectWidget.h view/widget/ImageButton.cpp view/widget/ImageButton.h view/BlockType.h view/toFile.h view/toPixmap.h contr/MainViewMouseManager.cpp contr/MainViewMouseManager.h action/AddFloorBlockAction.cpp action/AddFloorBlockAction.h action/ChangeFloorBlockAction.cpp action/ChangeFloorBlockAction.h action/AddLevelBlockAction.cpp action/AddLevelBlockAction.h action/ChangeLevelBlockAction.cpp action/ChangeLevelBlockAction.h action/RemoveLevelBlockAction.cpp action/RemoveLevelBlockAction.h action/RemoveFloorBlockAction.cpp action/RemoveFloorBlockAction.h view/toPixmap.cpp view/toFile.cpp model/PhaseTransformation.h view/widget/TextEditSideBar.cpp view/widget/TextEditSideBar.h view/widget/CentralWidget_enums.h view/FontManager.cpp view/FontManager.h view/Font_enums.h format.h datstr/CircularBuffer.cpp datstr/CircularBuffer.h view/PixmapManager.cpp view/PixmapManager.h datstr/Stash.h misc/AlignedRectangle.cpp misc/AlignedRectangle.h contr/MainInterface.cpp contr/MainInterface.h audio/AudioManager.cpp audio/AudioManager.h audio/Audio_enums.h audio/AudioSettings.h audio/AudioSettings.cpp Io/Serializer.cpp Io/Serializer.h view/widget/MainMenuWidget.cpp view/widget/MainMenuWidget.h view/widget/LevelSelectWidget.cpp view/widget/LevelSelectWidget.h view/ModelToPixmap.cpp view/ModelToPixmap.h view/widget/LevelPreviewWidget.cpp view/widget/LevelPreviewWidget.h Io/SettingsManager.cpp Io/SettingsManager.h view/widget/SettingsWidget.cpp view/widget/SettingsWidget.h view/widget/Window_enums.h audio/SoundEffect.cpp audio/SoundEffect.h audio/Music.cpp audio/Music.h model/GridXyContainer.cpp model/GridXyContainer.h)

add_custom_target(
        DatCopy
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/dat/ ${Blocks_SOURCE_DIR}/bin/dat/
)
add_dependencies(Blocks DatCopy)

target_link_libraries(Blocks sfml-audio)
target_link_libraries(Blocks Qt5::Core Qt5::Widgets)

target_compile_options(Blocks PRIVATE -Wall -Wextra -pedantic)