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

# Tried to use exprtk, but it was too large (20+ mins to compile, 30+ mins to link).
# https://stackoverflow.com/questions/11783932/how-do-i-add-a-linker-or-compile-flag-in-a-cmake-file
# https://stackoverflow.com/questions/14125007/gcc-string-table-overflow-error-during-compilation
# https://digitalkarabela.com/mingw-w64-how-to-fix-file-too-big-too-many-sections/
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wa,-mbig-obj -O2")

qt_add_executable(cnmo ${PROJECT_SOURCES})

target_link_libraries(cnmo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# The directory where you store most of your .o objects
# In my case, it is in the parent directory
set(ONE_LIBS_DIRECTORY ${CMAKE_SOURCE_DIR}/../libs)

# For now, use this library: https://github.com/cparse/cparse
# The problem with this implementation is that there is an issue with error handling. 
# See: https://github.com/cparse/cparse/issues/78
target_link_libraries(cnmo PRIVATE 
    ${ONE_LIBS_DIRECTORY}/cparse/core-shunting-yard.o
    ${ONE_LIBS_DIRECTORY}/cparse/my-features.o 
)

# I store includes (header files / header file libraries) next to libs.
include_directories(cnmo PUBLIC ../includes)