cmake_minimum_required(VERSION 3.16)
project(asset_browser
  VERSION "1.0.0"
  DESCRIPTION "asset browser"
  LANGUAGES CXX
)
message(STATUS "Initiating compile using CMake ${CMAKE_VERSION}")

# add cmake/ directory as cmake module path
list(INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
        "Default build type: Debug" FORCE)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines")
option(BUILD_SERVER "" ON)
option(BUILD_CLIENT "" OFF)
option(PLUGIN_AUTOMASK "" OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

include(FindCcache)

find_package(ZLIB REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(RapidJSON REQUIRED)
find_package(PNG REQUIRED)
find_package(OpenGL REQUIRED)

find_package(QCoro6 REQUIRED COMPONENTS Core Network DBus)
qcoro_enable_coroutines()
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets Concurrent Multimedia WebSockets Sql HttpServer OpenGL QuickWidgets Gui Quick Qml ShaderTools)

add_subdirectory(src/shared)
add_subdirectory(src/client)

if(BUILD_SERVER)
  add_subdirectory(src/server)
endif()

add_executable(QtConsoleApp
    main.cpp
)

target_link_libraries(QtConsoleApp
    QCoro6::Network
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
)