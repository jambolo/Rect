cmake_minimum_required(VERSION 3.23)

project(Rect
    VERSION 0.1.0
    DESCRIPTION "Operations on rectangles with integer coordinates"
    LANGUAGES CXX
)

option(BUILD_SHARED_LIBS "Build libraries as shared" OFF)
option(RECT_BUILD_DOCS "Build API documentation with Doxygen" OFF)

#########################################################################
# Build                                                                 #
#########################################################################

add_library(Rect)
add_library(Rect::Rect ALIAS Rect)

target_sources(Rect
    PRIVATE
        Rect.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES include/Rect/Rect.h
)

target_include_directories(Rect
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Rect
)

target_compile_features(Rect PUBLIC cxx_std_17)

set_target_properties(Rect PROPERTIES
    CXX_EXTENSIONS OFF
    EXPORT_NAME Rect
)

target_compile_definitions(Rect
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>
        $<$<CXX_COMPILER_ID:MSVC>:VC_EXTRALEAN>
        $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
)

#########################################################################
# Documentation                                                         #
#########################################################################

if(RECT_BUILD_DOCS AND PROJECT_IS_TOP_LEVEL)
    find_package(Doxygen QUIET)
    if(Doxygen_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
        doxygen_add_docs(doxygen
            COMMENT "Generating API documentation with Doxygen"
            CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        )
    else()
        message(WARNING "Doxygen not found; docs target unavailable")
    endif()
endif()

#########################################################################
# Testing                                                               #
#########################################################################

if(PROJECT_IS_TOP_LEVEL)
    include(CTest)
endif()

if(BUILD_TESTING)
    add_subdirectory(test)
endif()

#########################################################################
# Installation                                                          #
#########################################################################

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(rect_install_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake/Rect")

install(TARGETS Rect
    EXPORT RectTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT RectTargets
    FILE RectTargets.cmake
    NAMESPACE Rect::
    DESTINATION ${rect_install_cmakedir}
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/RectConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/RectConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/RectConfig.cmake"
    INSTALL_DESTINATION "${rect_install_cmakedir}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/RectConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/RectConfigVersion.cmake"
    DESTINATION "${rect_install_cmakedir}"
)

export(EXPORT RectTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/RectTargets.cmake"
    NAMESPACE Rect::
)

if(PROJECT_IS_TOP_LEVEL)
    export(PACKAGE Rect)
endif()
