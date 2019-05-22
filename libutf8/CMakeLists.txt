#
# File:
#       libutf8/CMakeLists.txt
#
# Description:
#       A simple to use C++ library to convert UTF-8 strings to and from wide
#       character strings (UTF-32).
#
# Documentation:
#       See the CMake documentation.
#
# License:
#       Copyright (c) 2013-2019 Made to Order Software Corp.
#
#       https://snapwebsites.org/projects/libutf8
#       contact@m2osw.com
#
#       Permission is hereby granted, free of charge, to any person obtaining a
#       copy of this software and associated documentation files (the
#       "Software"), to deal in the Software without restriction, including
#       without limitation the rights to use, copy, modify, merge, publish,
#       distribute, sublicense, and/or sell copies of the Software, and to
#       permit persons to whom the Software is furnished to do so, subject to
#       the following conditions:
#
#       The above copyright notice and this permission notice shall be included
#       in all copies or substantial portions of the Software.
#
#       THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#       OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#       MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#       IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#       CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#       TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#       SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

##
## utf8 library
##
project(utf8)

# Put the version in the header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

add_library( ${PROJECT_NAME} SHARED
    libutf8.cpp
    libutf8base.cpp
    libutf8_iterator.cpp
    version.cpp
)

include_directories(
    ${LIBEXCEPT_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
)

set_target_properties( ${PROJECT_NAME} PROPERTIES
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
        exception.h
        libutf8.h
        libutf8base.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/libutf8
)


# vim: ts=4 sw=4 et
