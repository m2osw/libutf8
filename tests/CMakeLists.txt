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
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

# Note that if user does not have Catch then this does not get included
# in the final output
project(unittest)

find_package( Catch2 )

if( Catch2_FOUND )

    add_definitions(
        -DCATCH_CONFIG_PREFIX_ALL
    )

    include_directories(
        ${CMAKE_BINARY_DIR}
        ${PROJECT_SOURCE_DIR}
        ${CATCH_INCLUDE_DIR}
        ${LIBEXCEPT_INCLUDE_DIRS}
    )

    add_executable( ${PROJECT_NAME}
        main.cpp

        bom.cpp
        caseinsensitive.cpp
        character.cpp
        iterator.cpp
        string.cpp
        version.cpp
    )

    target_link_libraries( ${PROJECT_NAME}
        utf8
    )

    add_test( unittests ${PROJECT_NAME} )

else( Catch2_FOUND )

    message("snapcatch2 not found... no test will be built.")

endif( Catch2_FOUND )

# vim: ts=4 sw=4 et
