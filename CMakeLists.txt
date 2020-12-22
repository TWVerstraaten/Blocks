cmake_minimum_required(VERSION 3.17)
project(Blocks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

SET(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2_ttf -lSDL2_image")

set(CMAKE_CXX_STANDARD 11)

add_executable(Blocks main.cpp view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/Grid.cpp view/Grid.h model/Model.cpp model/Model.h Application_Level.cpp Application_Level.h model/Level.cpp model/Level.h model/GridCoordinates.cpp model/GridCoordinates.h model/Enums.h model/ClusterAction.cpp model/ClusterAction.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/AssetHandler.cpp view/AssetHandler.h view/widget/ActionEditBox.cpp view/widget/ActionEditBox.h view/widget/Widget.cpp view/widget/Widget.h aux/Aux.cpp aux/Aux.h view/widget/SelectionData.cpp view/widget/SelectionData.h view/ScreenCoordinates.cpp view/ScreenCoordinates.h model/WorldCoordinates.cpp model/WorldCoordinates.h model/WorldVector.cpp model/WorldVector.h view/ScreenVector.cpp view/ScreenVector.h view/Color.cpp view/Color.h Application_Edit.cpp Application_Edit.h Application_Run.cpp Application_Run.h)