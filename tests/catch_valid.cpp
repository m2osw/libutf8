// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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
#include    <libutf8/exception.h>
#include    <libutf8/libutf8.h>


// unit test
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/hexadecimal_string.h>


// C++
//
#include    <cctype>
#include    <iostream>
#include    <iomanip>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("make_valid", "[strings][valid][u8]")
{
    CATCH_START_SECTION("make_valid: test bad encoding (1 byte when 2 necessary)")
    {
        for(char32_t two_bytes(0x80); two_bytes < 0x800; ++two_bytes)
        {
            char const byte1(static_cast<char>((two_bytes >> 6) | 0xC0));
            char32_t const vc1(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const vc2(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const fix_char(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            std::string invalid_string;
            invalid_string += vc1;
            invalid_string += byte1;
            invalid_string += vc2;
            std::string expected_string;
            expected_string += vc1;
            expected_string += fix_char;
            expected_string += vc2;
            CATCH_REQUIRE_FALSE(libutf8::make_u8string_valid(invalid_string, fix_char));
            CATCH_REQUIRE(invalid_string == expected_string);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("make_valid: test bad encoding (2 bytes when 3 necessary)")
    {
        for(char32_t two_bytes(0x800); two_bytes < 0x10000; ++two_bytes)
        {
            // Note: this includes the UTF-16 surrogates which are also
            //       considered invalid
            //
            char const byte1(static_cast<char>((two_bytes >> 12) | 0xE0));
            char const byte2(((two_bytes >> 6) & 0x3F) | 0x80);
            char32_t const vc1(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const vc2(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const fix_char(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            std::string invalid_string;
            invalid_string += vc1;
            invalid_string += byte1;
            invalid_string += byte2;
            invalid_string += vc2;
            std::string expected_string;
            expected_string += vc1;
            expected_string += fix_char;
            expected_string += vc2;
            CATCH_REQUIRE_FALSE(libutf8::make_u8string_valid(invalid_string, fix_char));
            CATCH_REQUIRE(invalid_string == expected_string);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("make_valid: test bad encoding (3 bytes when 4 necessary)")
    {
        for(char32_t two_bytes(0x10000); two_bytes < 0x110000; ++two_bytes)
        {
            char const byte1(static_cast<char>((two_bytes >> 18) | 0xF0));
            char const byte2(((two_bytes >> 12) & 0x3F) | 0x80);
            char const byte3(((two_bytes >> 6) & 0x3F) | 0x80);
            char32_t const vc1(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const vc2(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            char32_t const fix_char(random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_UNICODE));
            std::string invalid_string;
            invalid_string += vc1;
            invalid_string += byte1;
            invalid_string += byte2;
            invalid_string += byte3;
            invalid_string += vc2;
            std::string expected_string;
            expected_string += vc1;
            expected_string += fix_char;
            expected_string += vc2;
            CATCH_REQUIRE_FALSE(libutf8::make_u8string_valid(invalid_string, fix_char));
            CATCH_REQUIRE(invalid_string == expected_string);
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
