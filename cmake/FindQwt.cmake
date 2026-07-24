# Locate a Qwt installation and expose it as Qwt::Qwt.
#
# Hints:
#   Qwt_ROOT              Installation prefix
#   QWT_INCLUDE_DIR       Directory containing qwt_plot.h
#   QWT_LIBRARY           Qwt library file

set(_Qwt_library_names qwt)
if(QT_VERSION_MAJOR EQUAL 6)
    list(PREPEND _Qwt_library_names qwt-qt6 qwt6)
elseif(QT_VERSION_MAJOR EQUAL 5)
    list(PREPEND _Qwt_library_names qwt-qt5 qwt5)
endif()

find_path(
    QWT_INCLUDE_DIR
    NAMES qwt_plot.h
    HINTS "${Qwt_ROOT}" "$ENV{QWT_ROOT}"
    PATH_SUFFIXES include include/qwt qwt qwt-qt5 qwt-qt6
)
find_library(
    QWT_LIBRARY
    NAMES ${_Qwt_library_names}
    HINTS "${Qwt_ROOT}" "$ENV{QWT_ROOT}"
    PATH_SUFFIXES lib lib64
)

if(QWT_INCLUDE_DIR)
    file(
        STRINGS "${QWT_INCLUDE_DIR}/qwt_global.h"
        _Qwt_version_line
        REGEX "^#define[ \t]+QWT_VERSION_STR[ \t]+\"[^\"]+\""
    )
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" Qwt_VERSION "${_Qwt_version_line}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    Qwt
    REQUIRED_VARS QWT_LIBRARY QWT_INCLUDE_DIR
    VERSION_VAR Qwt_VERSION
)

if(Qwt_FOUND AND NOT TARGET Qwt::Qwt)
    add_library(Qwt::Qwt UNKNOWN IMPORTED)
    set_target_properties(
        Qwt::Qwt
        PROPERTIES
            IMPORTED_LOCATION "${QWT_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${QWT_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(QWT_INCLUDE_DIR QWT_LIBRARY)
