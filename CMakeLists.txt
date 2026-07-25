cmake_minimum_required(VERSION 3.21)

project(
    cnmo
    VERSION 0.1.0
    DESCRIPTION "Numerical Methods and Optimization assignments"
    LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qwt 6 REQUIRED)

add_library(cnmo_tinyexpr STATIC
    third_party/tinyexpr/tinyexpr.c
)
add_library(cnmo::tinyexpr ALIAS cnmo_tinyexpr)

target_compile_features(cnmo_tinyexpr PRIVATE c_std_99)
target_include_directories(cnmo_tinyexpr
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/tinyexpr"
)

if(NOT WIN32)
    target_link_libraries(cnmo_tinyexpr PUBLIC m)
endif()

add_library(cnmo_strlib STATIC
    third_party/strlib/strlib.cpp
)
add_library(cnmo::strlib ALIAS cnmo_strlib)

target_compile_features(cnmo_strlib PUBLIC cxx_std_17)
target_include_directories(cnmo_strlib
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/strlib"
)

add_library(cnmo_mydefines INTERFACE)
add_library(cnmo::mydefines ALIAS cnmo_mydefines)

target_include_directories(cnmo_mydefines
    INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/third_party/mydefines"
)

add_executable(cnmo WIN32
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

    src/polynomial/ui/plot.cpp
    src/polynomial/ui/plot.h
    src/polynomial/ui/poly_main.cpp
    src/polynomial/ui/poly_main.h
    src/polynomial/ui/poly_main.ui
    src/polynomial/ui/poly_table_model.cpp
    src/polynomial/ui/poly_table_model.h

    src/rootfinding/ui/plot.cpp
    src/rootfinding/ui/plot.h
    src/rootfinding/ui/root_finding_main.cpp
    src/rootfinding/ui/root_finding_main.h
    src/rootfinding/ui/root_finding_main.ui
    src/rootfinding/ui/zeros_table_model.cpp
    src/rootfinding/ui/zeros_table_model.h
    src/rootfinding/ui/zeros_table_row.h
)

set_target_properties(cnmo PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

target_compile_features(cnmo PRIVATE cxx_std_17)
target_include_directories(cnmo
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(cnmo
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qwt::Qwt
        cnmo::tinyexpr
        cnmo::strlib
        cnmo::mydefines
)

install(
    TARGETS cnmo
    RUNTIME_DEPENDENCY_SET cnmo_runtime_dependencies
    RUNTIME DESTINATION .
    BUNDLE DESTINATION .
)
install(FILES LICENSE DESTINATION .)

if(WIN32)
    if(MSVC)
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION ".")
        include(InstallRequiredSystemLibraries)
    endif()

    get_target_property(
        _qt_qmake_executable
        Qt${QT_VERSION_MAJOR}::qmake
        IMPORTED_LOCATION
    )
    if(_qt_qmake_executable)
        get_filename_component(_qt_bin_dir "${_qt_qmake_executable}" DIRECTORY)
    endif()

    find_program(
        WINDEPLOYQT_EXECUTABLE
        NAMES windeployqt windeployqt.exe
        HINTS "${_qt_bin_dir}"
        DOC "Path to Qt's Windows deployment tool"
        REQUIRED
    )
    get_filename_component(_qt_bin_dir "${WINDEPLOYQT_EXECUTABLE}" DIRECTORY)
    get_filename_component(_compiler_bin_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)

    install(
        RUNTIME_DEPENDENCY_SET cnmo_runtime_dependencies
        DIRECTORIES
            "$<TARGET_FILE_DIR:Qwt::Qwt>"
            "${_qt_bin_dir}"
            "${_compiler_bin_dir}"
        PRE_EXCLUDE_REGEXES
            "api-ms-.*"
            "ext-ms-.*"
        POST_EXCLUDE_REGEXES
            ".*[\\\\/]system32[\\\\/].*"
        RUNTIME DESTINATION .
        LIBRARY DESTINATION .
    )

    set(CNMO_EXECUTABLE_NAME "cnmo${CMAKE_EXECUTABLE_SUFFIX}")
    configure_file(
        cmake/DeployQt.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/DeployQt.cmake"
        @ONLY
    )
    install(SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/DeployQt.cmake")
endif()

string(TOLOWER "${CMAKE_SYSTEM_NAME}" CNMO_PACKAGE_SYSTEM)
if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64|arm64|ARM64)$")
    set(CNMO_PACKAGE_ARCHITECTURE "arm64")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CNMO_PACKAGE_ARCHITECTURE "x86_64")
else()
    set(CNMO_PACKAGE_ARCHITECTURE "x86")
endif()

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_DIRECTORY "${CMAKE_BINARY_DIR}/packages")
set(
    CPACK_PACKAGE_FILE_NAME
    "${PROJECT_NAME}-${PROJECT_VERSION}-${CNMO_PACKAGE_SYSTEM}-${CNMO_PACKAGE_ARCHITECTURE}"
)
set(CPACK_PACKAGE_CHECKSUM "SHA256")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

include(CPack)
