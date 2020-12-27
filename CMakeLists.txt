cmake_minimum_required(VERSION 3.5)
project(Blocks)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Blocks_SOURCE_DIR}/bin)

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
set(ASSETS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

# Copy assets
file(GLOB ASSET_FILES ${Blocks_SOURCE_DIR}/assets/*)
set(assets_files "")
foreach (file ${ASSET_FILES})
    get_filename_component(nam ${file} NAME)
    add_custom_command(
            OUTPUT ${ASSETS_DIRECTORY}/${nam}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} ${ASSETS_DIRECTORY}/${nam}
            DEPENDS ${file}
            COMMENT "Copying ${file}"
            VERBATIM
    )
    list(APPEND assets_files ${ASSETS_DIRECTORY}/${nam})
endforeach ()

if (WIN32)
    set(SDL2_INCLUDE_DIRS "C:/build/SDL2-2.0.14/include")
    set(SDL2_TTF_INCLUDE_DIRS "C:/build/SDL2_ttf-2.0.15")
    set(SDL2_IMAGE_INCLUDE_DIRS "C:/build/SDL2_image-2.0.5")

    include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

    # Copy dlls
    file(GLOB DLL_FILES "C:/build/dll/*")
    set(dll_file "" )
    foreach (file ${DLL_FILES})
        get_filename_component(nam ${file} NAME)
        add_custom_command(
                OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${nam}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
                DEPENDS ${file}
                COMMENT "Copying ${file} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${nam}"
                VERBATIM
        )
        list(APPEND dll_file ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${nam})
    endforeach ()

    link_directories(C:/build/dll)
endif (WIN32)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_LINK_FLAGS "-lSDL2 -lSDL2_image -lSDL2_ttf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

add_executable(Blocks ${dll_file} ${assets_files} main.cpp model/Actions/ClearBlockAction.cpp model/Actions/ClearBlockAction.h model/Command.cpp model/Command.h view/widget/ScrollArea.cpp view/widget/ScrollArea.h view/View.cpp view/View.h view/Rectangle.cpp view/Rectangle.h model/Cluster.cpp model/Cluster.h view/ViewPort.cpp view/ViewPort.h model/Model.cpp model/Model.h Application/Application_Level.cpp Application/Application_Level.h model/Level.cpp model/Level.h model/GridXY.cpp model/GridXY.h model/Enums.h model/Command.cpp model/Command.h view/Texture.cpp view/Texture.h view/Font.cpp view/Font.h view/Assets.cpp view/Assets.h view/widget/LineEditBox.cpp view/widget/LineEditBox.h view/widget/RectWidget.cpp view/widget/RectWidget.h global/fns.cpp global/fns.h view/widget/SelectionData.cpp view/widget/SelectionData.h view/ScreenXY.cpp view/ScreenXY.h model/WorldXY.cpp model/WorldXY.h view/Color.cpp Application/Application_Edit.cpp Application/Application_Edit.h Application/Application_Run.cpp Application/Application_Run.h view/TextureWrapper.cpp view/TextureWrapper.h global/cst.h view/widget/Widget.cpp view/widget/Widget.h view/Mouse.cpp view/Mouse.h view/widget/ActionEditBox.cpp view/widget/ActionEditBox.h model/Line.cpp model/Line.h global/alg.cpp global/alg.h global/geom.cpp global/geom.h model/Actions/Action.cpp model/Actions/Action.h)

if (UNIX)
    target_include_directories(Blocks PRIVATE /usr/include/SDL2)
endif (UNIX)
