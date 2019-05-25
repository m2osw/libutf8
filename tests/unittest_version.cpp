/*
 * Files:
 *    tests/unittest_version.cpp
 *
 * License:
 *    Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved
 *
 *    https://snapwebsites.org/
 *    contact@m2osw.com
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Authors:
 *    Alexis Wilke   alexis@m2osw.com
 */

// self
//
#include "unittest_main.h"

// libutf8 lib
//
#include "libutf8/version.h"



namespace
{


} // no name namespace


CATCH_TEST_CASE( "Version", "version" )
{
    CATCH_START_SECTION("verify runtime vs compile time version numbers")
        CATCH_REQUIRE(libutf8::get_major_version()   == LIBUTF8_VERSION_MAJOR);
        CATCH_REQUIRE(libutf8::get_release_version() == LIBUTF8_VERSION_MINOR);
        CATCH_REQUIRE(libutf8::get_patch_version()   == LIBUTF8_VERSION_PATCH);
        CATCH_REQUIRE(strcmp(libutf8::get_version_string(), LIBUTF8_VERSION_STRING) == 0);
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
