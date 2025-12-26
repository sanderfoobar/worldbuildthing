set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCE_FILES
    "*.h"
    "*.cpp"
    "lib/*.h"
    "lib/*.cpp"
    "lib/bcrypt/*.h"
    "lib/bcrypt/*.cpp"
    "core/*.h"
    "core/*.cpp"
    "lib/stb/*.h"
    "lib/stb/*.cpp"
    "lib/network/*.h"
    "lib/network/*.cpp"
    "models/*.h"
    "models/*.cpp"
)

qt_add_resources(GENERATED_RESOURCES assets.qrc)
add_library(shared STATIC
    ${SOURCE_FILES}
    ${GENERATED_RESOURCES}
)

target_link_libraries(shared PUBLIC
    QCoro6::Network
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
    Qt6::WebSockets
    Qt6::Sql
    PNG::PNG
    ZLIB::ZLIB
)

target_include_directories(shared PUBLIC
    ${CMAKE_SOURCE_DIR}/src/
)

target_compile_options(shared PUBLIC
    ${RTCOM-EVENTLOGGER_CFLAGS_OTHER}
    ${OSSO_ABOOK_CFLAGS_OTHER}
    ${GTK_CFLAGS_OTHER}
)

#if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_compile_definitions(shared PUBLIC GENERATE_VTF=1)
#endif()
