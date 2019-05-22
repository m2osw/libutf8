/*    unittest_character.cpp
 *    Copyright (C) 2013-2019  Made to Order Software Corporation
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
 *    Authors
 *    Alexis Wilke   alexis@m2osw.com
 */

#include "libutf8/libutf8base.h"

#include <catch2/catch.hpp>

#include <cctype>


CATCH_TEST_CASE("Character Conversions", "characters")
{
    CATCH_REQUIRE(libutf8::MBS_MIN_BUFFER_LENGTH >= 5);

    for(char32_t wc(0); wc < 0x000080; ++wc)
    {
        char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
        CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 1);

        CATCH_REQUIRE(static_cast<char32_t>(buf[0]) == wc);
        CATCH_REQUIRE(buf[1] == '\0');

        char32_t back;
        char const * s(buf);
        size_t len(1);
        CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 1);
        CATCH_REQUIRE(back == wc);
    }

    for(char32_t wc(0x000080); wc < 0x000800; ++wc)
    {
        char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
        CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 2);

        char32_t const found(((static_cast<char32_t>(buf[0]) & 0x1F) << 6)
                           | ((static_cast<char32_t>(buf[1]) & 0x3F) << 0));
        CATCH_REQUIRE(found == wc);
        CATCH_REQUIRE(buf[2] == '\0');

        char32_t back;
        char const * s(buf);
        size_t len(2);
        CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 2);
        CATCH_REQUIRE(back == wc);
    }

    for(char32_t wc(0x000800); wc < 0x010000; ++wc)
    {
        if(wc >= 0xD800 && wc <= 0xDFFF)
        {
            // skip UTF-16 surrogates
            //
            wc = 0xDFFF;
            continue;
        }

        char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
        CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 3);

        char32_t const found(((static_cast<char32_t>(buf[0]) & 0x0F) << 12)
                           | ((static_cast<char32_t>(buf[1]) & 0x3F) <<  6)
                           | ((static_cast<char32_t>(buf[2]) & 0x3F) <<  0));
        CATCH_REQUIRE(found == wc);
        CATCH_REQUIRE(buf[3] == '\0');

        char32_t back;
        char const * s(buf);
        size_t len(3);
        CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 3);
        CATCH_REQUIRE(back == wc);
    }

    for(char32_t wc(0x010000); wc < 0x110000; ++wc)
    {
        char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
        CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 4);

        char32_t const found(((static_cast<char32_t>(buf[0]) & 0x07) << 18)
                           | ((static_cast<char32_t>(buf[1]) & 0x3F) << 12)
                           | ((static_cast<char32_t>(buf[2]) & 0x3F) <<  6)
                           | ((static_cast<char32_t>(buf[3]) & 0x3F) <<  0));
        CATCH_REQUIRE(found == wc);
        CATCH_REQUIRE(buf[4] == '\0');

        char32_t back;
        char const * s(buf);
        size_t len(4);
        CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 4);
        CATCH_REQUIRE(back == wc);
    }
}



// vim: ts=4 sw=4 et
