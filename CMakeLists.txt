# Copyright (C) 2000-2021  Made to Order Software Corporation
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

cmake_minimum_required(VERSION 2.8.4)

project(utf8_library)

find_package(SnapCMakeModules REQUIRED)
find_package(LibExcept        REQUIRED)
find_package(SnapDev          REQUIRED)
find_package(SnapDoxygen              )

SnapGetVersion(LIBUTF8 ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_subdirectory(libutf8)
add_subdirectory(tools  )
add_subdirectory(conf   )
add_subdirectory(doc    )
add_subdirectory(cmake  )
add_subdirectory(tests  )

# vim: ts=4 sw=4 et
