# Find Qwt and expose it through the Qwt::Qwt imported target.
#
# Set Qwt_ROOT (or QWT_ROOT) to the root of a Qwt installation when it is not
# installed in a standard location.

include(FindPackageHandleStandardArgs)
include(SelectLibraryConfigurations)

set(QWT_ROOT "" CACHE PATH "Root directory of a Qwt installation")

set(_Qwt_root_hints "${QWT_ROOT}")
if(DEFINED Qwt_ROOT)
    list(APPEND _Qwt_root_hints "${Qwt_ROOT}")
endif()
if(DEFINED ENV{Qwt_ROOT})
    list(APPEND _Qwt_root_hints "$ENV{Qwt_ROOT}")
endif()
if(DEFINED ENV{QWT_ROOT})
    list(APPEND _Qwt_root_hints "$ENV{QWT_ROOT}")
endif()

find_path(
    Qwt_INCLUDE_DIR
    NAMES qwt_plot.h
    HINTS ${_Qwt_root_hints}
    PATH_SUFFIXES include include/qwt qwt src
)

set(_Qwt_release_names
    "qwt-qt${QT_VERSION_MAJOR}"
    "qwt${QT_VERSION_MAJOR}"
    qwt
)
set(_Qwt_debug_names
    "qwt-qt${QT_VERSION_MAJOR}d"
    "qwt${QT_VERSION_MAJOR}d"
    qwtd
)

find_library(
    Qwt_LIBRARY_RELEASE
    NAMES ${_Qwt_release_names}
    HINTS ${_Qwt_root_hints}
    PATH_SUFFIXES lib
)
find_library(
    Qwt_LIBRARY_DEBUG
    NAMES ${_Qwt_debug_names}
    HINTS ${_Qwt_root_hints}
    PATH_SUFFIXES lib
)

select_library_configurations(Qwt)

if(Qwt_INCLUDE_DIR AND EXISTS "${Qwt_INCLUDE_DIR}/qwt_global.h")
    file(
        STRINGS "${Qwt_INCLUDE_DIR}/qwt_global.h"
        _Qwt_version_line
        REGEX "^#define[ \t]+QWT_VERSION_STR[ \t]+\"[^\"]+\""
    )
    string(
        REGEX REPLACE
        "^#define[ \t]+QWT_VERSION_STR[ \t]+\"([^\"]+)\".*"
        "\\1"
        Qwt_VERSION
        "${_Qwt_version_line}"
    )
endif()

find_package_handle_standard_args(
    Qwt
    REQUIRED_VARS Qwt_LIBRARY Qwt_INCLUDE_DIR
    VERSION_VAR Qwt_VERSION
)

if(Qwt_FOUND AND NOT TARGET Qwt::Qwt)
    add_library(Qwt::Qwt UNKNOWN IMPORTED)
    set_target_properties(Qwt::Qwt PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Qwt_INCLUDE_DIR}"
    )

    if(Qwt_LIBRARY_RELEASE)
        set_property(
            TARGET Qwt::Qwt APPEND PROPERTY
            IMPORTED_CONFIGURATIONS RELEASE
        )
        set_target_properties(Qwt::Qwt PROPERTIES
            IMPORTED_LOCATION_RELEASE "${Qwt_LIBRARY_RELEASE}"
            MAP_IMPORTED_CONFIG_MINSIZEREL RELEASE
            MAP_IMPORTED_CONFIG_RELWITHDEBINFO RELEASE
        )
    endif()

    if(Qwt_LIBRARY_DEBUG)
        set_property(
            TARGET Qwt::Qwt APPEND PROPERTY
            IMPORTED_CONFIGURATIONS DEBUG
        )
        set_target_properties(Qwt::Qwt PROPERTIES
            IMPORTED_LOCATION_DEBUG "${Qwt_LIBRARY_DEBUG}"
        )
    endif()

    if(WIN32)
        set(_Qwt_runtime_hints ${_Qwt_root_hints})
        foreach(_Qwt_library IN ITEMS "${Qwt_LIBRARY_RELEASE}" "${Qwt_LIBRARY_DEBUG}")
            if(_Qwt_library)
                get_filename_component(_Qwt_library_directory "${_Qwt_library}" DIRECTORY)
                get_filename_component(_Qwt_library_parent "${_Qwt_library_directory}" DIRECTORY)
                list(APPEND _Qwt_runtime_hints
                    "${_Qwt_library_directory}"
                    "${_Qwt_library_parent}"
                )
            endif()
        endforeach()

        find_file(
            Qwt_RUNTIME_RELEASE
            NAMES
                "qwt-qt${QT_VERSION_MAJOR}.dll"
                "qwt${QT_VERSION_MAJOR}.dll"
                qwt.dll
            HINTS ${_Qwt_runtime_hints}
            PATH_SUFFIXES bin lib
        )
        find_file(
            Qwt_RUNTIME_DEBUG
            NAMES
                "qwt-qt${QT_VERSION_MAJOR}d.dll"
                "qwt${QT_VERSION_MAJOR}d.dll"
                qwtd.dll
            HINTS ${_Qwt_runtime_hints}
            PATH_SUFFIXES bin lib
        )

        if(Qwt_RUNTIME_RELEASE)
            set_target_properties(Qwt::Qwt PROPERTIES
                IMPORTED_IMPLIB_RELEASE "${Qwt_LIBRARY_RELEASE}"
                IMPORTED_LOCATION_RELEASE "${Qwt_RUNTIME_RELEASE}"
            )
        endif()
        if(Qwt_RUNTIME_DEBUG)
            set_target_properties(Qwt::Qwt PROPERTIES
                IMPORTED_IMPLIB_DEBUG "${Qwt_LIBRARY_DEBUG}"
                IMPORTED_LOCATION_DEBUG "${Qwt_RUNTIME_DEBUG}"
            )
        endif()
        if(Qwt_RUNTIME_RELEASE OR Qwt_RUNTIME_DEBUG)
            set_property(
                TARGET Qwt::Qwt APPEND PROPERTY
                INTERFACE_COMPILE_DEFINITIONS QWT_DLL
            )
        endif()
    endif()
endif()

mark_as_advanced(
    QWT_ROOT
    Qwt_INCLUDE_DIR
    Qwt_LIBRARY_DEBUG
    Qwt_LIBRARY_RELEASE
    Qwt_RUNTIME_DEBUG
    Qwt_RUNTIME_RELEASE
)
