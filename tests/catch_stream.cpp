// Copyright (c) 2013-2025  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/libutf8
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

// libutf8
//
#include    <libutf8/libutf8.h>


// unit test
//
#include    "catch_main.h"


// C++
//
#include    <cctype>
#include    <iostream>
#include    <iomanip>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("stream", "[stream][valid]")
{
    CATCH_START_SECTION("stream: write a char32_t to a stream")
    {
        for(int i(0); i < 1000; ++i)
        {
            char32_t const wc(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));

            std::stringstream ss;
            ss << libutf8::to_u8string(wc);

            CATCH_REQUIRE(ss.str() == libutf8::to_u8string(wc));
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
