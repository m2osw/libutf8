# - Find LibUtf8
#
# LIBUTF8_FOUND        - System has LibUtf8
# LIBUTF8_INCLUDE_DIRS - The LibUtf8 include directories
# LIBUTF8_LIBRARIES    - The libraries needed to use LibUtf8
# LIBUTF8_DEFINITIONS  - Compiler switches required for using LibUtf8
#
# License:
#
# Copyright (c) 2011-2025  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/libutf8
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

find_path(
    LIBUTF8_INCLUDE_DIR
        libutf8/libutf8.h

    PATHS
        ENV LIBUTF8_INCLUDE_DIR
)

find_library(
    LIBUTF8_LIBRARY
        utf8

    PATHS
        ${LIBUTF8_LIBRARY_DIR}
        ENV LIBUTF8_LIBRARY
)

mark_as_advanced(
    LIBUTF8_INCLUDE_DIR
    LIBUTF8_LIBRARY
)

set(LIBUTF8_INCLUDE_DIRS ${LIBUTF8_INCLUDE_DIR})
set(LIBUTF8_LIBRARIES    ${LIBUTF8_LIBRARY})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(
    LibUtf8
    REQUIRED_VARS
        LIBUTF8_INCLUDE_DIR
        LIBUTF8_LIBRARY
)

# vim: ts=4 sw=4 et
