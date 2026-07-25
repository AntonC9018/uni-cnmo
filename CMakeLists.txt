cmake_minimum_required(VERSION 3.16)

project(
    cnmo
    VERSION 1.0.0
    DESCRIPTION "Numerical Methods and Optimization coursework"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

option(CNMO_BUILD_APP "Build the Qt/Qwt desktop application" ON)

add_library(cnmo_project_options INTERFACE)
target_compile_features(cnmo_project_options INTERFACE cxx_std_17)

add_library(cnmo_strlib STATIC third_party/strlib/strlib.cpp)
target_include_directories(cnmo_strlib PUBLIC third_party/strlib)
target_link_libraries(cnmo_strlib PUBLIC cnmo_project_options)

add_library(cnmo_tinyexpr STATIC third_party/tinyexpr/tinyexpr.c)
target_include_directories(cnmo_tinyexpr PUBLIC third_party/tinyexpr)

add_library(cnmo_support INTERFACE)
target_include_directories(
    cnmo_support
    INTERFACE
        src
        third_party/mydefines
)
target_link_libraries(
    cnmo_support
    INTERFACE
        cnmo_project_options
        cnmo_strlib
        cnmo_tinyexpr
)

if(CNMO_BUILD_APP)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

    find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
    find_package(Qwt REQUIRED)

    add_executable(
        cnmo
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/mainwindow.ui
        src/func/ui/basicplot.cpp
        src/func/ui/basicplot.h
        src/func/ui/curvetracker.cpp
        src/func/ui/curvetracker.h
        src/func/ui/function_selection.cpp
        src/func/ui/function_selection.h
        src/func/ui/function_selection.ui
        src/rootfinding/ui/root_finding_main.cpp
        src/rootfinding/ui/root_finding_main.h
        src/rootfinding/ui/root_finding_main.ui
        src/rootfinding/ui/zeros_table_model.cpp
        src/rootfinding/ui/zeros_table_model.h
        src/rootfinding/ui/plot.cpp
        src/rootfinding/ui/plot.h
        src/polynomial/ui/poly_main.cpp
        src/polynomial/ui/poly_main.h
        src/polynomial/ui/poly_main.ui
        src/polynomial/ui/plot.cpp
        src/polynomial/ui/plot.h
        src/polynomial/ui/poly_table_model.cpp
        src/polynomial/ui/poly_table_model.h
    )
    set_target_properties(
        cnmo
        PROPERTIES
            AUTOMOC ON
            AUTOUIC ON
            AUTORCC ON
            WIN32_EXECUTABLE ON
            MACOSX_BUNDLE ON
    )
    target_link_libraries(
        cnmo
        PRIVATE
            cnmo_support
            Qt${QT_VERSION_MAJOR}::Widgets
            Qwt::Qwt
    )
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(cnmo PRIVATE -Wno-deprecated-declarations)
    endif()

    install(
        TARGETS cnmo
        BUNDLE DESTINATION .
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    )
endif()
