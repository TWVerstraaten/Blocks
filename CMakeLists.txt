cmake_minimum_required(VERSION 3.17)
project(Blocks)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -pedantic")

SET(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2_ttf -lSDL2_image")

set(CMAKE_CXX_STANDARD 11)

add_executable(Blocks main.cpp view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/Grid.cpp view/Grid.h model/Model.cpp model/Model.h Application.cpp Application.h model/Level.cpp model/Level.h model/IndexPair.cpp model/IndexPair.h model/Enums.h model/ClusterAction.cpp model/ClusterAction.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/AssetHandler.cpp view/AssetHandler.h view/ActionEditBox.cpp view/ActionEditBox.h view/Widget.cpp view/Widget.h view/Aux.cpp view/Aux.h)