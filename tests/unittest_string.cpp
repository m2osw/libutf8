/*    unittest_string.cpp
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

// libutf8 lib
//
#include "libutf8/libutf8.h"

// catch2 lib
//
#include <catch2/catch.hpp>

// C++ lib
//
#include <cctype>


CATCH_TEST_CASE("LibUtf8UnitTests::conversions","LibUtf8UnitTests")
{
    std::string str;
    std::u32string u32str, back;
    int i;

    // create a string with all the characters defined in plane 1
    for(i = 1; i < 0x0FFFE; ++i)
    {
        // skip the surrogate, they are not considered valid characters
        //
        if(i < 0xD800 || i > 0xDFFF)
        {
            u32str += static_cast<char32_t>(i);
        }
    }

    str = libutf8::to_u8string(u32str);

    // verify the UTF-8 string
    //
    char const *s(str.c_str());
    for(i = 1; i < 0x080; ++i)
    {
        CATCH_REQUIRE(*s++ == static_cast<char>(i));
    }
    for(; i < 0x0800; ++i)
    {
        CATCH_REQUIRE(*s++ == static_cast<char>((i >> 6) | 0xC0));
        CATCH_REQUIRE(*s++ == static_cast<char>((i & 0x3F) | 0x80));
    }
    for(; i < 0x0FFFE; ++i)
    {
        if(i < 0xD800 || i > 0xDFFF)
        {
            CATCH_REQUIRE(*s++ == static_cast<char>((i >> 12) | 0xE0));
            CATCH_REQUIRE(*s++ == static_cast<char>(((i >> 6) & 0x3F) | 0x80));
            CATCH_REQUIRE(*s++ == static_cast<char>((i & 0x3F) | 0x80));
        }
    }

    // verify the UTF-8 to char32_t
    //
    back = libutf8::to_u32string(str);
    CATCH_REQUIRE(back == u32str);
}



char32_t rand_char()
{
    char32_t wc;
    do
    {
        wc = rand() & 0x0FFFF;
    }
    while(wc == 0);
    return wc;
}

CATCH_TEST_CASE("LibUtf8UnitTests::compare","LibUtf8UnitTests")
{
    for(int i(1); i < 0x10000; ++i)
    {
        // as is
        std::u32string in;
        in += static_cast<char32_t>(i);
        std::string mb(libutf8::to_u8string(in));
        CATCH_REQUIRE(libutf8::u8casecmp(mb, mb) == 0);

        // as is against uppercase
        std::u32string uin;
        uin += std::toupper(static_cast<char32_t>(i));
        std::string umb(libutf8::to_u8string(uin));
        CATCH_REQUIRE(libutf8::u8casecmp(mb, umb) == 0);

        // as is against lowercase
        std::u32string lin;
        lin += std::tolower(static_cast<char32_t>(i));
        std::string lmb(libutf8::to_u8string(lin));
        CATCH_REQUIRE(libutf8::u8casecmp(mb, lmb) == 0);

        // random
        for(int j(0); j < 3; ++j)
        {
            char32_t rwc(rand_char() & 0x0FFFF);
            in += rwc;
            uin += std::toupper(rwc);
            lin += std::tolower(rwc);

            std::string rmb(libutf8::to_u8string(in));
            CATCH_REQUIRE(libutf8::u8casecmp(rmb, rmb) == 0);
            std::string rumb(libutf8::to_u8string(uin));
            CATCH_REQUIRE(libutf8::u8casecmp(rmb, rumb) == 0);
            std::string rlmb(libutf8::to_u8string(lin));
            CATCH_REQUIRE(libutf8::u8casecmp(rmb, rlmb) == 0);
        }

        char32_t wc(rand_char() & 0x0FFFF);
        in += wc;
        std::string emb(libutf8::to_u8string(in));
        CATCH_REQUIRE(libutf8::u8casecmp(emb, emb) == 0);
        CATCH_REQUIRE(libutf8::u8casecmp(emb, umb) == 1);
        CATCH_REQUIRE(libutf8::u8casecmp(emb, lmb) == 1);

        {
            wchar_t uwc(rand_char() & 0x0FFFF);
            uin += std::toupper(uwc);
            std::string eumb(libutf8::to_u8string(uin));
//printf("UPPER compare U+%04X/%04X with U+%04X/%04X\n", wc, std::toupper(wc), uwc, std::toupper(uwc));
//printf(" result: [%d]\n", libutf8::u8casecmp(emb, eumb));
            if(std::toupper(wc) == std::toupper(uwc))
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == 0);
            }
            else if(std::toupper(wc) < std::toupper(uwc))
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == -1);
            }
            else
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == 1);
            }
            CATCH_REQUIRE(libutf8::u8casecmp(lmb, eumb) == -1);
        }

        // here we check with a lowercase character, but notice that the
        // compare uses uppercase!
        {
            char32_t lwc(rand_char() & 0x0FFFF);
            lin += std::tolower(lwc);
            std::string const elmb(libutf8::to_u8string(lin));
//printf("LOWER compare U+%04X/%04X with U+%04X/%04X\n", wc, std::toupper(wc), lwc, std::toupper(lwc));
//printf(" result: [%d]\n", libutf8::u8casecmp(emb, elmb));
            if(std::toupper(wc) == std::toupper(lwc))
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == 0);
            }
            else if(std::toupper(wc) < std::toupper(lwc))
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == -1);
            }
            else
            {
                CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == 1);
            }
        }
    }
}


// With MS-Windows, we can check that our functions work the same way
// (return the expected value) as this Windows API function:
// 
// CompareStringOrdinal(L"This string", 11, L"That string", 11, TRUE);


// vim: ts=4 sw=4 et
