cmake_minimum_required(VERSION 3.1.0)

project(WildflowerCrossStitch)

# major.minor.patch:
#     odd patch is development
#     even patch is release
set(WFXS_VERSION 0.0.1)
configure_file(xs/XSVersion.h.in xs/XSVersion.h)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(xs)

# Probe UI
find_package(PkgConfig)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Haiku")
    set(UI_HAIKU_DEF TRUE)
endif()
find_package(Qt4)
if(Qt4_FOUND OR QT4_FOUND)
    set(UI_QT_DEF TRUE)
endif()
pkg_check_modules(GTKMM gtkmm-3.0)
if(GTKMM_LIBRARIES)
    set(UI_GTKMM_DEF TRUE)
endif()
set(UI_WX_DEF FALSE)

option(UI_HAIKU "UI for Haiku" ${UI_HAIKU_DEF})
option(UI_QT "UI for Qt" ${UI_QT_DEF})
option(UI_GTKMM "UI for GTK" ${UI_GTKMM_DEF})
option(UI_WX "UI for WxWidgets (currently unmaintained)" ${UI_WX_DEF})

if(UI_HAIKU)
    add_subdirectory(BAF)
    add_subdirectory(haiku)
endif()

if(UI_QT)
    add_subdirectory(qt)
    # Enable running from the build dir, by adding a link to resources.
    add_custom_target(qtResources ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources qt/resources)
endif()

if(UI_GTKMM)
    add_subdirectory(gtk)
    # Enable running from the build dir, by adding a link to resources.
    add_custom_target(gtkResources ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources gtk/resources)
endif()

if(UI_WX)
    add_subdirectory(wx)
    # Enable running from the build dir, by adding a link to resources.
    add_custom_target(wxResources ALL
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/resources wx/resources)
endif()
