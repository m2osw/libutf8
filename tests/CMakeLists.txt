#
# File:
#      tests/CMakeLists.txt
#
# Description:
#      Definitions to create all the unit tests of the libutf8 library.
#
# Documentation:
#      See the CMake documentation.
#
# License:
#      Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved
#
#      https://sourceforge.net/projects/unigw/
#      contact@m2osw.com
#
#      Permission is hereby granted, free of charge, to any person obtaining a
#      copy of this software and associated documentation files (the
#      "Software"), to deal in the Software without restriction, including
#      without limitation the rights to use, copy, modify, merge, publish,
#      distribute, sublicense, and/or sell copies of the Software, and to
#      permit persons to whom the Software is furnished to do so, subject to
#      the following conditions:
#
#      The above copyright notice and this permission notice shall be included
#      in all copies or substantial portions of the Software.
#
#      THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
#      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#      MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#      IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#      CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
#      TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
#      SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# Note that if user does not have Catch then this does not get included
# in the final output
project(unittest)

find_package( Catch2 )

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()
#
#message(STATUS "---- OUR OWN VAR?  ${CATCH_INCLUDE_DIR} ----")

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
