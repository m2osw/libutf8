// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
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
#include    <libutf8/base.h>
#include    <libutf8/exception.h>


// unit test
//
#include    "catch_main.h"


// C++
//
#include    <cctype>
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("character_conversions", "[characters]")
{
    CATCH_START_SECTION("character_conversions: Verify minimum buffer length for MBS conversions")
    {
        CATCH_REQUIRE(libutf8::MBS_MIN_BUFFER_LENGTH >= 5);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("character_conversions: Test UTF-32 to UTF-8 with characters between 0x00 and 0x7F inclusive")
    {
        for(char32_t wc(0); wc < 0x000080; ++wc)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
            CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 0), libutf8::libutf8_logic_exception);
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 1);

            CATCH_REQUIRE(static_cast<char32_t>(buf[0]) == wc);
            CATCH_REQUIRE(buf[1] == '\0');

            char32_t back(rand());
            char const * s(buf);
            size_t len(1);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 1);
            CATCH_REQUIRE(back == wc);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("character_conversions: Test UTF-32 to UTF-8 with characters between 0x80 and 0x7FF inclusive")
    {
        for(char32_t wc(0x000080); wc < 0x000800; ++wc)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
            CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 0), libutf8::libutf8_logic_exception);
            CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 1), libutf8::libutf8_logic_exception);
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 2);

            char32_t const found(((static_cast<char32_t>(buf[0]) & 0x1F) << 6)
                               | ((static_cast<char32_t>(buf[1]) & 0x3F) << 0));
            CATCH_REQUIRE(found == wc);
            CATCH_REQUIRE(buf[2] == '\0');

            char32_t back(rand());
            char const * s(buf);
            size_t len(2);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 2);
            CATCH_REQUIRE(back == wc);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("character_conversions: Test UTF-32 to UTF-8 with characters between 0x800 and 0xFFFF inclusive")
    {
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
            if(rand() % 10 == 0)
            {
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 0), libutf8::libutf8_logic_exception);
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 1), libutf8::libutf8_logic_exception);
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 2), libutf8::libutf8_logic_exception);
            }
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 3);

            char32_t const found(((static_cast<char32_t>(buf[0]) & 0x0F) << 12)
                               | ((static_cast<char32_t>(buf[1]) & 0x3F) <<  6)
                               | ((static_cast<char32_t>(buf[2]) & 0x3F) <<  0));
            CATCH_REQUIRE(found == wc);
            CATCH_REQUIRE(buf[3] == '\0');

            char32_t back(rand());
            char const * s(buf);
            size_t len(3);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 3);
            CATCH_REQUIRE(back == wc);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("character_conversions: Test UTF-32 to UTF-8 with characters between 0x10000 and 0x10FFFF inclusive")
    {
        for(char32_t wc(0x010000); wc < 0x110000; ++wc)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
            if(rand() % 100 == 0)
            {
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 0), libutf8::libutf8_logic_exception);
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 1), libutf8::libutf8_logic_exception);
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 2), libutf8::libutf8_logic_exception);
                CATCH_REQUIRE_THROWS_AS(libutf8::wctombs(buf, wc, 3), libutf8::libutf8_logic_exception);
            }
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 4);

            char32_t const found(((static_cast<char32_t>(buf[0]) & 0x07) << 18)
                               | ((static_cast<char32_t>(buf[1]) & 0x3F) << 12)
                               | ((static_cast<char32_t>(buf[2]) & 0x3F) <<  6)
                               | ((static_cast<char32_t>(buf[3]) & 0x3F) <<  0));
            CATCH_REQUIRE(found == wc);
            CATCH_REQUIRE(buf[4] == '\0');

            char32_t back(rand());
            char const * s(buf);
            size_t len(4);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == 4);
            CATCH_REQUIRE(back == wc);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("character_conversions: Test UTF-32 to UTF-8 with an empty input string")
    {
        for(char32_t repeat(0); repeat < 10; ++repeat)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
            char copy[libutf8::MBS_MIN_BUFFER_LENGTH];
            for(size_t idx(0); idx < libutf8::MBS_MIN_BUFFER_LENGTH; ++idx)
            {
                buf[idx] = rand();
                copy[idx] = buf[idx];
            }
            char const * s(buf);
            char32_t null = rand();
            size_t len(0);
            CATCH_REQUIRE(libutf8::mbstowc(null, s, len) == 0);
            CATCH_REQUIRE(null == '\0');
            for(size_t idx(0); idx < libutf8::MBS_MIN_BUFFER_LENGTH; ++idx)
            {
                CATCH_REQUIRE(copy[idx] == buf[idx]);
            }
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("invalid_utf32_to_utf8", "[characters],[invalid]")
{
    CATCH_START_SECTION("invalid_utf32_to_utf8: Verify that surrogates do not work in UTF-8")
    {
        for(char32_t wc(0x00D800); wc < 0x00E000; ++wc)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH]
            {
                'n', 'o', 't', 'e', 'm' //, 'p', 't', 'y'
            };
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == -1);
            CATCH_REQUIRE(buf[0] == '\0');
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_utf32_to_utf8: Verify that too large a number is not supported")
    {
        for(int repeat(0); repeat < 1000; ++repeat)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH]
            {
                'n', 'o', 't', 'e', 'm' //, 'p', 't', 'y'
            };
            char32_t wc(0);
            do
            {
                wc = (rand() << 16) + (rand() & 0x0000FFFF);
            }
            while(wc < 0x110000);
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == -1);
            CATCH_REQUIRE(buf[0] == '\0');
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("invalid_utf8_to_utf32", "[characters],[invalid]")
{
    CATCH_START_SECTION("invalid_utf8_to_utf32: Verify that surrogates do not work in UTF-8")
    {
        for(char32_t wc(0x00D800); wc < 0x00E000; ++wc)
        {
//int mbstowc(char32_t & wc, char const * & mb, size_t & len);
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH] = {};
            buf[0] = static_cast<char>((wc >> 12) | 0xE0);
            buf[1] = ((wc >> 6) & 0x3F) | 0x80;
            buf[2] = (wc & 0x3F) | 0x80;
            buf[3] = '\0';
            char const * s = buf;
            size_t len(3);
            char32_t cwc(rand());
            CATCH_REQUIRE(libutf8::mbstowc(cwc, s, len) == -1);
            CATCH_REQUIRE(cwc == libutf8::NOT_A_CHARACTER);
            char const c1(static_cast<char>((wc >> 12) | 0xE0));
            CATCH_REQUIRE(buf[0] == c1);
            char const c2(static_cast<char>(((wc >> 6) & 0x3F) | 0x80));
            CATCH_REQUIRE(buf[1] == c2);
            char const c3((wc & 0x3F) | 0x80);
            CATCH_REQUIRE(buf[2] == c3);
            CATCH_REQUIRE(buf[3] == '\0');
            CATCH_REQUIRE(s == buf + 3);
            CATCH_REQUIRE(len == 0);
        }
    }
    CATCH_END_SECTION()

    //CATCH_START_SECTION("invalid_utf8_to_utf32: Verify that too large a number is not supported")
    //{
    //    for(int idx(0); idx < 1000; ++idx)
    //    {
    //        char buf[libutf8::MBS_MIN_BUFFER_LENGTH]
    //        {
    //            'n', 'o', 't', 'e', 'm' //, 'p', 't', 'y'
    //        };
    //        char32_t wc(0);
    //        do
    //        {
    //            wc = (rand() << 16) + (rand() & 0x0000FFFF);
    //        }
    //        while(wc < 0x110000);
    //        CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == -1);
    //        CATCH_REQUIRE(buf[0] == libutf8::NOT_A_CHARACTER);
    //    }
    //}
    //CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_utf8_to_utf32: Test an invalid UTF-8 sequence (0x80 to 0x7FF)")
    {
        for(char32_t wc(0x000080); wc < 0x000800; ++wc)
        {
            char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
            CATCH_REQUIRE(libutf8::wctombs(buf, wc, sizeof(buf)) == 2);

            char32_t const found(((static_cast<char32_t>(buf[0]) & 0x1F) << 6)
                               | ((static_cast<char32_t>(buf[1]) & 0x3F) << 0));
            CATCH_REQUIRE(found == wc);
            CATCH_REQUIRE(buf[2] == '\0');

            // too short
            //
            char32_t back(rand());
            char const * s(buf);
            size_t len(1);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);

            // invalid middle byte
            //
            char const second_byte(buf[1]);
            back = rand();
            s = buf;
            int c(rand() % (255 - 0x40) + 1);
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[1] = static_cast<char>(c);
            len = 2;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 1) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 1);
            buf[1] = second_byte;

            // invalid introducer (0x80 to 0xBF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 64 + 0x80;
            len = 2;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 2) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);

            // invalid introducer (0xF8 to 0xFF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 8 + 0xF8;
            len = 2;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 2) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_utf8_to_utf32: Test an invalid UTF-8 sequence (0x800 to 0xFFFF)")
    {
        for(char32_t wc(0x000800); wc < 0x010000; ++wc)
        {
            if(wc >= 0xD800 && wc <= 0xDFFF)
            {
                // skip UTF-16 surrogates -- this is not the test for those
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

            // too short
            //
            char32_t back(rand());
            char const * s(buf);
            size_t len(2);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);

            // invalid middle byte
            //
            char const second_byte(buf[1]);
            back = rand();
            s = buf;
            int c(rand() % (255 - 0x40) + 1);
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[1] = static_cast<char>(c);
            len = 3;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 1) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 2);
            buf[1] = second_byte;

            char const third_byte(buf[2]);
            back = rand();
            s = buf;
            c = rand() % (255 - 0x40) + 1;
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[2] = static_cast<char>(c);
            len = 3;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 2) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 1);
            buf[2] = third_byte;

            // invalid introducer (0x80 to 0xBF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 64 + 0x80;
            len = 3;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 3) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);

            // invalid introducer (0xF8 to 0xFF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 8 + 0xF8;
            len = 3;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 3) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_utf8_to_utf32: Test UTF-32 to UTF-8 with characters between 0x010000 and 0x110000 inclusive")
    {
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

            char32_t back(rand());
            char const * s(buf);

            // too short
            //
            size_t len(3);
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);

            // invalid middle byte
            //
            char const second_byte(buf[1]);
            back = rand();
            s = buf;
            int c(rand() % (255 - 0x40) + 1);
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[1] = static_cast<char>(c);
            len = 4;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 1) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 3);
            buf[1] = second_byte;

            char const third_byte(buf[2]);
            back = rand();
            s = buf;
            c = rand() % (255 - 0x40) + 1;
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[2] = static_cast<char>(c);
            len = 4;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 2) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 2);
            buf[2] = third_byte;

            char const forth_byte(buf[3]);
            back = rand();
            s = buf;
            c = rand() % (255 - 0x40) + 1;
            if(c >= 0x80)
            {
                c += 0x40;
            }
            buf[3] = static_cast<char>(c);
            len = 4;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 3) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 1);
            buf[3] = forth_byte;

            // invalid introducer (0x80 to 0xBF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 64 + 0x80;
            len = 3;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 3) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);

            // invalid introducer (0x80 to 0xBF)
            //
            back = rand();
            buf[0] = rand() % 64 + 0x80;
            s = buf;
            len = 4;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 4) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);

            // invalid introducer (0xF8 to 0xFF)
            //
            back = rand();
            s = buf;
            buf[0] = rand() % 8 + 0xF8;
            len = 4;
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);
            CATCH_REQUIRE(reinterpret_cast<void const *>(buf + 4) == reinterpret_cast<void const *>(s));
            CATCH_REQUIRE(len == 0);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_utf8_to_utf32: Test three random characters, destroy the second one and make sure it gets skipped properly")
    {
        for(int repeat(0); repeat < 1000; ++repeat)
        {
            char32_t wc[3]
            {
                unittest::rand_char(true),
                unittest::rand_char(true),
                unittest::rand_char(true),
            };
            size_t sz[3] = {};

            char buf[libutf8::MBS_MIN_BUFFER_LENGTH * 3];
            char * s(buf);
            sz[0] += libutf8::wctombs(s, wc[0], sizeof(buf));
            s += sz[0];
            sz[1] = libutf8::wctombs(s, wc[1], sizeof(buf));
            s += sz[1];
            sz[2] = libutf8::wctombs(s, wc[2], sizeof(buf));

            char32_t back(rand());
            s = buf;
            buf[sz[0]] = rand() % 64 + 0x80;
            size_t len(sizeof(buf));
            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) != -1);
            CATCH_REQUIRE(back == wc[0]);

            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) == -1);
            CATCH_REQUIRE(back == libutf8::NOT_A_CHARACTER);

            CATCH_REQUIRE(libutf8::mbstowc(back, s, len) != -1);
            CATCH_REQUIRE(back == wc[2]);
        }
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
