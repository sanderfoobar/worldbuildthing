file(GLOB ENGINE_SOURCE_FILES
    "*.h"
    "*.cpp"
    "engine/*.h"
    "engine/*.cpp"
    "gl/*.h"
    "gl/*.cpp"
    "gl2/*.h"
    "gl2/shaders/*.h"
    "gl2/*.cpp"
    "lib/*.h"
    "lib/*.cpp"
    "lib/qt/*.h"
    "lib/qt/*.cpp"
    "lib/vmfpp/*.h"
    "lib/vmfpp/*.cpp"
    "lib/vmfpp/detail/*.h"
    "lib/vmfpp/detail/*.cpp"
    "lib/vmfpp/structs/*.h"
    "lib/vmfpp/structs/*.cpp"
)

qt6_add_resources(RESOURCES
        assets.qrc
        assets/icons_blender/blender_icons.qrc
        lib/qt/theme.qrc
)

qt_add_executable(client main.cpp
        ${ENGINE_SOURCE_FILES}
        ${RESOURCES}
        ${GENERATED_SHADERS}
)

qt_add_shaders(client "shaders"
        PREFIX "/"
        FILES
        "qml/viewport_main.frag"
        OUTPUTS
        "qml/viewport_main.frag.qsb"
)

target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(client PUBLIC
        QCoro6::Network
        Qt6::Core
        Qt6::Widgets
        Qt6::Network

        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::Multimedia
        Qt6::Network
        Qt6::Qml
        Qt6::WebSockets
        Qt6::QuickWidgets
        Qt6::Sql
        PNG::PNG
        shared
        # ${CMAKE_SOURCE_DIR}/libvtflib.so
)

qt6_add_qml_module(client
        URI Main
        VERSION 1.0
        OUTPUT_DIRECTORY qml_main
        RESOURCE_PREFIX ""
        QML_FILES
        qml/Main.qml
        qml/ViewportMenuBarTextButton.qml
)

#set(ENV{QSB_PROFILE} "330")
#qt6_add_shaders(client
#        GLSL "300es,330"
#        PREFIX /
#        FILES
#        qml/rounded.vert
#        qml/rounded.frag
#)


if(DEBUG)
    message(STATUS "DEBUG BUILD")
    target_compile_definitions(client PRIVATE DEBUG=1)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(PROJECT_LINK_LIBS -lGL)
    target_compile_options(client PRIVATE -Wno-deprecated-declarations)
endif()

target_link_libraries(client PRIVATE
        ${PROJECT_LINK_LIBS}
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Qt6::OpenGL
        OpenGL::GL)

