// Copyright (c) 2021-2023  Made to Order Software Corp.  All Rights Reserved
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
#include    <libutf8/libutf8.h>


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



CATCH_TEST_CASE("bom", "[characters],[bom]")
{
    CATCH_START_SECTION("bom: Verify the BOM character")
        CATCH_REQUIRE(libutf8::BOM_CHAR == 0xFEFF);
    CATCH_END_SECTION()

    CATCH_START_SECTION("bom: Verify with a string that's too small")
    {
        CATCH_REQUIRE(libutf8::start_with_bom(nullptr, rand()) == libutf8::bom_t::BOM_NONE);
        CATCH_REQUIRE(libutf8::start_with_bom("", 0) == libutf8::bom_t::BOM_NONE);
        CATCH_REQUIRE(libutf8::start_with_bom("a", 1) == libutf8::bom_t::BOM_NONE);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("bom: Verify the five BOMs as is")
    {
        char buf[4];
        char32_t const bom(libutf8::BOM_CHAR);

        // UTF-8
        buf[0] = static_cast<char>((bom >> 12) | 0xE0);
        buf[1] = static_cast<char>(((bom >>  6) & 0x3F) | 0x80);
        buf[2] = static_cast<char>(((bom >>  0) & 0x3F) | 0x80);
        buf[3] = '?';
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF8);

        // UTF-16 Little Endian
        buf[0] = static_cast<char>(bom >> 0);
        buf[1] = static_cast<char>(bom >> 8);
        buf[2] = static_cast<char>(0x00);
        buf[3] = static_cast<char>(0x34);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_LE);

        // UTF-16 Little Endian (with a zero in the next 2 bytes)
        buf[0] = static_cast<char>(bom >> 0);
        buf[1] = static_cast<char>(bom >> 8);
        buf[2] = static_cast<char>(0x12);
        buf[3] = static_cast<char>(0x00);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_LE);

        // UTF-16 Little Endian (with a zero in the next 2 bytes)
        buf[0] = static_cast<char>(bom >> 0);
        buf[1] = static_cast<char>(bom >> 8);
        buf[2] = static_cast<char>(0x12);
        buf[3] = static_cast<char>(0x34);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_LE);

        // UTF-16 Big Endian
        buf[0] = static_cast<char>(bom >> 8);
        buf[1] = static_cast<char>(bom >> 0);
        buf[2] = static_cast<char>(0xAB);
        buf[3] = static_cast<char>(0xCD);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_BE);

        // UTF-16 Big Endian (with a zero in the next 2 bytes)
        buf[0] = static_cast<char>(bom >> 8);
        buf[1] = static_cast<char>(bom >> 0);
        buf[2] = static_cast<char>(0x00);
        buf[3] = static_cast<char>(0xCD);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_BE);

        // UTF-16 Big Endian (with a zero in the next 2 bytes)
        buf[0] = static_cast<char>(bom >> 8);
        buf[1] = static_cast<char>(bom >> 0);
        buf[2] = static_cast<char>(0xAB);
        buf[3] = static_cast<char>(0x00);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF16_BE);

        // UTF-32 Little Endian
        buf[0] = static_cast<char>(bom >>  0);
        buf[1] = static_cast<char>(bom >>  8);
        buf[2] = static_cast<char>(bom >> 16);
        buf[3] = static_cast<char>(bom >> 24);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF32_LE);

        // UTF-32 Big Endian
        buf[0] = static_cast<char>(bom >> 24);
        buf[1] = static_cast<char>(bom >> 16);
        buf[2] = static_cast<char>(bom >>  8);
        buf[3] = static_cast<char>(bom >>  0);
        CATCH_REQUIRE(libutf8::start_with_bom(buf, sizeof(buf)) == libutf8::bom_t::BOM_UTF32_BE);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("bom: Verify the five BOMs as is")
    {
        char buf[4];

        // unknown 1 byte (well... 1 byte is never really known...)
        buf[0] = '?';
        CATCH_REQUIRE(libutf8::start_with_bom(buf, 1) == libutf8::bom_t::BOM_NONE);

        // unknown 2 bytes
        buf[0] = 'Q';
        buf[1] = '?';
        CATCH_REQUIRE(libutf8::start_with_bom(buf, 2) == libutf8::bom_t::BOM_NONE);

        // unknown 3 bytes
        buf[0] = 'B';
        buf[1] = 'O';
        buf[2] = 'M';
        CATCH_REQUIRE(libutf8::start_with_bom(buf, 3) == libutf8::bom_t::BOM_NONE);

        // unknown 4 bytes
        buf[0] = 'B';
        buf[1] = 'O';
        buf[2] = 'M';
        buf[3] = '?';
        CATCH_REQUIRE(libutf8::start_with_bom(buf, 4) == libutf8::bom_t::BOM_NONE);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("bom: Verify u32string that starts with a BOM (CPU Endianness)")
    {
        std::u32string u32str;
        u32str += libutf8::BOM_CHAR;
        u32str += unittest::rand_char(true);
        size_t const size(u32str.length() * sizeof(std::u32string::value_type));
        for(int idx(static_cast<int>(size)); idx >= 0; --idx)
        {
            if(static_cast<size_t>(idx) >= sizeof(std::u32string::value_type))
            {
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
                CATCH_REQUIRE(libutf8::start_with_bom(reinterpret_cast<char const *>(u32str.c_str()), idx) == libutf8::bom_t::BOM_UTF32_BE);
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
                CATCH_REQUIRE(libutf8::start_with_bom(reinterpret_cast<char const *>(u32str.c_str()), idx) == libutf8::bom_t::BOM_UTF32_LE);
#else
#error "Unsupported endianness"
#endif
            }
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
            else if(static_cast<size_t>(idx) >= sizeof(std::u16string::value_type))
            {
                CATCH_REQUIRE(libutf8::start_with_bom(reinterpret_cast<char const *>(u32str.c_str()), idx) == libutf8::bom_t::BOM_UTF16_LE);
            }
#endif
            else
            {
                // too short
                //
                CATCH_REQUIRE(libutf8::start_with_bom(reinterpret_cast<char const *>(u32str.c_str()), idx) == libutf8::bom_t::BOM_NONE);
            }
        }
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
