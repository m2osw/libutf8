# Copyright (c) 2012-2022  Made to Order Software Corp.  All Rights Reserved
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
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.

##
## show-utf8
##
project(show-utf8)

add_executable(${PROJECT_NAME}
    show_utf8.cpp
)

target_link_libraries(${PROJECT_NAME}
    utf8
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin
)


##
## unicode-data-parser
##
project(unicode-data-parser)

add_executable(${PROJECT_NAME}
    unicode_data_parser.cpp
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${ADVGETOPT_INCLUDE_DIRS}
        ${LIBEXCEPT_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    utf8
    ${LIBEXCEPT_LIBRARIES}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin
)


# vim: ts=4 sw=4 et
