cmake_minimum_required(VERSION 3.17)
project(Blocks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

SET(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2_ttf -lSDL2_image")

set(CMAKE_CXX_STANDARD 14)

add_executable(Blocks main.cpp view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/ViewPort.cpp view/ViewPort.h model/Model.cpp model/Model.h Application/Application_Level.cpp Application/Application_Level.h model/Level.cpp model/Level.h model/GridXY.cpp model/GridXY.h model/Enums.h model/Action.cpp model/Action.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/Assets.cpp view/Assets.h view/widget/LineEditBox.cpp view/widget/LineEditBox.h view/widget/RectWidget.cpp view/widget/RectWidget.h aux/Aux.cpp aux/Aux.h view/widget/SelectionData.cpp view/widget/SelectionData.h view/ScreenXY.cpp view/ScreenXY.h model/WorldXY.cpp model/WorldXY.h model/WorldVector.cpp model/WorldVector.h view/ScreenVector.cpp view/ScreenVector.h view/Color.cpp view/Color.h Application/Application_Edit.cpp Application/Application_Edit.h Application/Application_Run.cpp Application/Application_Run.h view/TextureWrapper.cpp view/TextureWrapper.h Global.h view/widget/Widget.cpp view/widget/Widget.h view/Mouse.cpp view/Mouse.h view/widget/ActionEditBox.cpp view/widget/ActionEditBox.h)