#
# qspacemidi - 3Dconnexion SpaceNavigator to MIDI CC bridge
#
# Copyright (C) 2026 Jakob Flierl <jakob.flierl@gmail.com>
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
#

cmake_minimum_required(VERSION 3.24)

project(qspacemidi
    VERSION 1.0.1
    DESCRIPTION "Map a 3Dconnexion SpaceNavigator to MIDI CC messages"
    HOMEPAGE_URL "https://github.com/koppi/qspacemidi"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(QSPACEMIDI_WERROR "Treat warnings as errors" OFF)

# ---------------------------------------------------------------------------
# Qt 6
# ---------------------------------------------------------------------------

find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Widgets)
qt_standard_project_setup()

# ---------------------------------------------------------------------------
# libremidi
# ---------------------------------------------------------------------------

include(FetchContent)

set(LIBREMIDI_EXAMPLES OFF CACHE BOOL "" FORCE)
set(LIBREMIDI_TESTS OFF CACHE BOOL "" FORCE)
set(LIBREMIDI_NO_JACK ON CACHE BOOL "" FORCE)
set(LIBREMIDI_NO_PIPEWIRE ON CACHE BOOL "" FORCE)
set(LIBREMIDI_NO_NETWORK ON CACHE BOOL "" FORCE)
set(LIBREMIDI_NO_KEYBOARD ON CACHE BOOL "" FORCE)
# Use std::variant rather than Boost.Variant2: it keeps the dependency set
# small and libremidi's set_client_name() helper only works with std::visit.
set(LIBREMIDI_NO_BOOST ON CACHE BOOL "" FORCE)

# libremidi installs its headers, its static library and its CMake package.
# We link it statically and ship only our executable, so from CMake 3.28 on
# its install rules are dropped with EXCLUDE_FROM_ALL.  Older CMake does not
# know the keyword; there the Debian packaging removes the files again.
set(QSPACEMIDI_FETCH_OPTIONS "")
if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.28)
    list(APPEND QSPACEMIDI_FETCH_OPTIONS EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    libremidi
    GIT_REPOSITORY https://github.com/atsushieno/libremidi.git
    GIT_TAG        master
    GIT_SHALLOW 1
    ${QSPACEMIDI_FETCH_OPTIONS}
    CMAKE_ARGS -DLIBREMIDI_USE_WinRT=OFF -DLIBREMIDI_NO_WINUWP=ON
)
FetchContent_MakeAvailable(libremidi)

# ---------------------------------------------------------------------------
# Application
# ---------------------------------------------------------------------------

set(QSPACEMIDI_SOURCES
    src/main.cpp
    src/aboutdialog.cpp
    src/aboutdialog.h
    src/autostart.cpp
    src/autostart.h
    src/mainwindow.cpp
    src/mainwindow.h
    src/midiengine.cpp
    src/midiengine.h
    src/mapping.cpp
    src/mapping.h
    src/settings.cpp
    src/settings.h
    src/widgets/axisrow.cpp
    src/widgets/axisrow.h
    src/widgets/knob.cpp
    src/widgets/knob.h
    src/widgets/levelmeter.cpp
    src/widgets/levelmeter.h
    src/thirdparty/spacenavigator/spacenavigator.cpp
    src/thirdparty/spacenavigator/spacenavigator.h
)

if(WIN32)
    list(APPEND QSPACEMIDI_SOURCES resources/qspacemidi.rc)
endif()

qt_add_executable(qspacemidi WIN32 MACOSX_BUNDLE ${QSPACEMIDI_SOURCES})

# Icons and the Aqua style sheet, reachable as ":/icons/..." and ":/style/...".
qt_add_resources(qspacemidi "qspacemidi_assets"
    PREFIX "/"
    BASE resources
    FILES
        resources/icons/qspacemidi.png
        resources/icons/qspacemidi-128.png
        resources/icons/qspacemidi-64.png
        resources/style/qspacemidi.qss)

target_include_directories(qspacemidi PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/thirdparty/spacenavigator)

target_link_libraries(qspacemidi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    libremidi::libremidi)

target_compile_definitions(qspacemidi PRIVATE
    QSPACEMIDI_VERSION="${PROJECT_VERSION}")

if(MSVC)
    target_compile_options(qspacemidi PRIVATE /W3 /utf-8 /Zc:__cplusplus)
    if(QSPACEMIDI_WERROR)
        target_compile_options(qspacemidi PRIVATE /WX)
    endif()
else()
    target_compile_options(qspacemidi PRIVATE -Wall -Wextra -Wno-unused-parameter)
    if(QSPACEMIDI_WERROR)
        target_compile_options(qspacemidi PRIVATE -Werror)
    endif()
endif()

# Platform back-ends of the vendored SpaceNavigator driver.  No 3DxWare /
# 3Dconnexion driver is required on any platform: the device is talked to
# directly through evdev (Linux), IOKit HID (macOS) and raw HID (Windows).
if(WIN32)
    target_link_libraries(qspacemidi PRIVATE hid setupapi)
elseif(APPLE)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation REQUIRED)
    target_link_libraries(qspacemidi PRIVATE ${IOKIT_LIBRARY} ${COREFOUNDATION_LIBRARY})
endif()

if(APPLE)
    set(QSPACEMIDI_ICNS "${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/qspacemidi.icns")
    target_sources(qspacemidi PRIVATE "${QSPACEMIDI_ICNS}")
    set_source_files_properties("${QSPACEMIDI_ICNS}" PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(qspacemidi PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME "QSpaceMIDI"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.koppi.qspacemidi"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_ICON_FILE "qspacemidi.icns"
        MACOSX_BUNDLE_COPYRIGHT "LGPL-2.1-or-later")
endif()

# ---------------------------------------------------------------------------
# Translations
#
# The .ts files are compiled with whichever lrelease we can find and are
# embedded as ":/i18n/qspacemidi_<lang>.qm".  Qt6::LinguistTools is not
# required, so a plain qtbase installation is enough to build.
# ---------------------------------------------------------------------------

option(QSPACEMIDI_TRANSLATIONS "Build and embed the UI translations" ON)

set(QSPACEMIDI_LANGUAGES da de fi fr it ja ru zh_CN)

if(QSPACEMIDI_TRANSLATIONS)
    find_program(QSPACEMIDI_LRELEASE
        NAMES lrelease-qt6 lrelease6 lrelease
        HINTS "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_BINS}"
              "${QT6_INSTALL_PREFIX}/bin"
              "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBEXECS}"
        DOC "Qt Linguist release tool")

    if(NOT QSPACEMIDI_LRELEASE)
        message(FATAL_ERROR
            "lrelease was not found; install the Qt 6 Linguist tools "
            "(qt6-l10n-tools / qt6-tools-dev-tools on Debian) or configure "
            "with -DQSPACEMIDI_TRANSLATIONS=OFF.")
    endif()

    set(QSPACEMIDI_QM_FILES "")
    set(QSPACEMIDI_TS_FILES "")
    foreach(language IN LISTS QSPACEMIDI_LANGUAGES)
        set(ts "${CMAKE_CURRENT_SOURCE_DIR}/translations/qspacemidi_${language}.ts")
        list(APPEND QSPACEMIDI_TS_FILES "${ts}")
        set(qm "${CMAKE_CURRENT_BINARY_DIR}/i18n/qspacemidi_${language}.qm")
        add_custom_command(
            OUTPUT "${qm}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/i18n"
            COMMAND "${QSPACEMIDI_LRELEASE}" -silent "${ts}" -qm "${qm}"
            DEPENDS "${ts}"
            COMMENT "Compiling ${language} translation"
            VERBATIM)
        list(APPEND QSPACEMIDI_QM_FILES "${qm}")
    endforeach()

    add_custom_target(qspacemidi_translations DEPENDS ${QSPACEMIDI_QM_FILES})
    add_dependencies(qspacemidi qspacemidi_translations)

    qt_add_resources(qspacemidi "qspacemidi_translations"
        PREFIX "/i18n"
        BASE "${CMAKE_CURRENT_BINARY_DIR}/i18n"
        FILES ${QSPACEMIDI_QM_FILES})

    target_compile_definitions(qspacemidi PRIVATE QSPACEMIDI_HAS_TRANSLATIONS)

    # Convenience target for maintainers: refresh the .ts files from source.
    find_program(QSPACEMIDI_LUPDATE
        NAMES lupdate-qt6 lupdate6 lupdate
        HINTS "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_BINS}"
              "${QT6_INSTALL_PREFIX}/bin"
              "${QT6_INSTALL_PREFIX}/${QT6_INSTALL_LIBEXECS}")
    if(QSPACEMIDI_LUPDATE)
        add_custom_target(update_translations
            COMMAND "${QSPACEMIDI_LUPDATE}" -no-obsolete -locations none
                    "${CMAKE_CURRENT_SOURCE_DIR}/src"
                    -ts ${QSPACEMIDI_TS_FILES}
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            COMMENT "Updating the .ts files from the sources"
            VERBATIM)
    endif()
endif()

# ---------------------------------------------------------------------------
# Installation
# ---------------------------------------------------------------------------

include(GNUInstallDirs)

install(TARGETS qspacemidi
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(UNIX AND NOT APPLE)
    install(FILES packaging/qspacemidi.desktop
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/applications)
    install(FILES resources/icons/qspacemidi.svg
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps)
    install(FILES packaging/org.koppi.qspacemidi.metainfo.xml
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/metainfo)
    # /etc for a local install; distribution packages point this at
    # /usr/lib/udev/rules.d, which is where packaged rules belong.
    set(QSPACEMIDI_UDEV_RULES_DIR "${CMAKE_INSTALL_FULL_SYSCONFDIR}/udev/rules.d"
        CACHE PATH "Directory the udev rules are installed to")
    install(FILES packaging/99-spacenavigator.rules
        DESTINATION "${QSPACEMIDI_UDEV_RULES_DIR}")
    install(FILES docs/qspacemidi.1
        DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
endif()

install(FILES README.md COPYING COPYING.LESSER
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

# ---------------------------------------------------------------------------
# Packaging
# ---------------------------------------------------------------------------

set(CPACK_PACKAGE_NAME "qspacemidi")
set(CPACK_PACKAGE_VENDOR "Jakob Flierl")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_CONTACT "jakob.flierl@gmail.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING.LESSER")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "QSpaceMIDI")
set(CPACK_STRIP_FILES ON)

set(CPACK_DEBIAN_PACKAGE_SECTION "sound")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${PROJECT_HOMEPAGE_URL}")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;TGZ")
else()
    set(CPACK_GENERATOR "ZIP")
endif()

include(CPack)
