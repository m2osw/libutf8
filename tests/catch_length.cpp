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
#include    <libutf8/exception.h>
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



CATCH_TEST_CASE("string_length", "[strings][valid][length][u8][u16][u32]")
{
    CATCH_START_SECTION("string_length: length of valid Unicode strings")
    {
        for(int idx(0); idx < 100; ++idx)
        {
            std::size_t const length(rand() % 100 + 1);
            std::u32string str32;
            for(std::size_t j(0); j < length; ++j)
            {
                char32_t const c(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));
                str32 += c;
            }
            CATCH_REQUIRE(libutf8::is_valid_unicode(str32));
            CATCH_REQUIRE(str32.length() == length);

            std::string str8(libutf8::to_u8string(str32));
            CATCH_REQUIRE(libutf8::is_valid_utf8(str8));
            CATCH_REQUIRE(str8.length() >= length);
            CATCH_REQUIRE(libutf8::u8length(str8) == length);

            std::u16string str16(libutf8::to_u16string(str8));
            CATCH_REQUIRE(libutf8::is_valid_utf16(str16));
            CATCH_REQUIRE(str16.length() >= length);
            CATCH_REQUIRE(static_cast<std::size_t>(libutf8::u16length(str16)) == length);
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("invalid_utf16_string_length", "[strings][invalid][length][u16]")
{
    CATCH_START_SECTION("invalid_utf16_string_length: invalid UTF-16 returns -1 for length")
    {
        for(int idx(0); idx < 100; ++idx)
        {
            std::size_t const length(rand() % 30 + 5);
            char16_t bad_char(rand() & 0x03FF);
            std::size_t bad_pos(length / 2);
            switch(idx % 3)
            {
            case 0:
                bad_char += 0xDC00;     // low without a high
                break;

            case 1:
                bad_char += 0xD800;     // high not followed by a low
                break;

            case 2:
                bad_char += 0xD800;     // high followed by u'\0'
                bad_pos = length - 1;
                break;

            }
            std::u16string str16;
            for(std::size_t j(0); j < length; ++j)
            {
                char32_t const wc(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));
                str16 += libutf8::to_u16string(wc);
                if(j == bad_pos)
                {
                    str16 += bad_char;
                }
            }

            CATCH_REQUIRE_FALSE(libutf8::is_valid_utf16(str16));
            CATCH_REQUIRE(libutf8::u16length(str16) == -1);
        }
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
