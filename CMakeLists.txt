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

set(CMAKE_CXX_STANDARD 17)
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

# The directory where you store most of your .o objects
# In my case, it is in the parent directory
set(ONE_LIBS_DIRECTORY ${CMAKE_SOURCE_DIR}/../libs)

# The git submodules directory
set(SUBMODULES_DIRECTORY ${CMAKE_SOURCE_DIR}/third_party)


set(PROJECT_SOURCES

    src/main.cpp

    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui

    src/rootfinding/ui/root_finding_main.cpp
    src/rootfinding/ui/root_finding_main.h
    src/rootfinding/ui/root_finding_main.ui

    src/func/ui/function_selection.cpp
    src/func/ui/function_selection.h
    src/func/ui/function_selection.ui

    src/rootfinding/ui/zeros_table_model.cpp
    src/rootfinding/ui/zeros_table_model.h
    src/rootfinding/ui/curvetracker.cpp
    src/rootfinding/ui/plot.cpp

    ${SUBMODULES_DIRECTORY}/strlib/strlib.cpp
)

# Tried to use exprtk, but it was too large (20+ mins to compile, 30+ mins to link).
# https://stackoverflow.com/questions/11783932/how-do-i-add-a-linker-or-compile-flag-in-a-cmake-file
# https://stackoverflow.com/questions/14125007/gcc-string-table-overflow-error-during-compilation
# https://digitalkarabela.com/mingw-w64-how-to-fix-file-too-big-too-many-sections/
# set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wa,-mbig-obj -O2")

# Remove Qwt's deprecation warnings
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wno-deprecated-declarations")

add_executable(cnmo ${PROJECT_SOURCES})

target_link_libraries(cnmo PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Use qwt for graphs.
target_link_libraries(cnmo PRIVATE 
    ${ONE_LIBS_DIRECTORY}/qwt/libqwt.a 
    ${ONE_LIBS_DIRECTORY}/qwt/libqwtd.a 
    ${ONE_LIBS_DIRECTORY}/qwt/libqwtmathml.a
    ${ONE_LIBS_DIRECTORY}/qwt/libqwtmathmld.a

    # tinyexpr is already precompiled with -O2
    # I do not yet know how to compile things separately with CMake.
    ${SUBMODULES_DIRECTORY}/tinyexpr/tinyexpr.o
)

# I store includes (header files / header file libraries) next to libs.
include_directories(cnmo PUBLIC ../includes)

# Qwt is also there in my case
include_directories(cnmo PUBLIC ../includes/qwt)

# Include some local dirs so that ui compiler works correctly
include_directories(cnmo PUBLIC  src)
include_directories(cnmo PUBLIC ${SUBMODULES_DIRECTORY})

# Include my libraries
include_directories(cnmo PUBLIC ${SUBMODULES_DIRECTORY}/strlib)
include_directories(cnmo PUBLIC ${SUBMODULES_DIRECTORY}/mydefines)

# Expression parsing
include_directories(cnmo PUBLIC ${SUBMODULES_DIRECTORY}/tinyexpr)