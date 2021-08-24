# Copyright (c) 2000-2021  Made to Order Software Corporation
#
# https://snapwebsites.org/project/libutf8
# contact@m2osw.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.

##
## utf8 library
##
project(utf8)

# Put the version in the header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

add_library(${PROJECT_NAME} SHARED
    base.cpp
    iterator.cpp
    libutf8.cpp
    unicode_data.cpp
    unicode_data_file.cpp
    version.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${LIBEXCEPT_INCLUDE_DIRS}
        ${SNAPDEV_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION
        ${LIBUTF8_VERSION_MAJOR}.${LIBUTF8_VERSION_MINOR}

    SOVERSION
        ${LIBUTF8_VERSION_MAJOR}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin

    LIBRARY DESTINATION
        lib

    ARCHIVE DESTINATION
        lib
)

install(
    FILES
        base.h
        caseinsensitivestring.h
        exception.h
        iterator.h
        libutf8.h
        unicode_data.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/libutf8
)


# vim: ts=4 sw=4 et
