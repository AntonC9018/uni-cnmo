cmake_minimum_required(VERSION 3.5)

project(cnmo LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ui header file generation
# https://cmake.org/cmake/help/latest/prop_tgt/AUTOUIC.html#prop_tgt:AUTOUIC
set(CMAKE_AUTOUIC ON)

# In short, the Qt preprocessor
# Basically expands some macros
# https://cmake.org/cmake/help/latest/prop_tgt/AUTOMOC.html#prop_tgt:AUTOMOC
# https://doc.qt.io/archives/qt-4.8/moc.html
set(CMAKE_AUTOMOC ON)

set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check https://doc.qt.io/qt/deployment-android.html for more information.
# They need to be set before the find_package( ...) calls below.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

# https://doc.qt.io/qt-5/cmake-get-started.html
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

qt_add_executable(cnmo ${PROJECT_SOURCES})

target_link_libraries(cnmo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)