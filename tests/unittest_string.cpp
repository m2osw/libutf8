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

// unit test
//
#include "unittest_main.h"

// libutf8 lib
//
#include "libutf8/exception.h"
#include "libutf8/libutf8.h"

// catch2 lib
//
#include <catch2/catch.hpp>

// C++ lib
//
#include <cctype>
#include <iostream>
#include <iomanip>


CATCH_TEST_CASE("string conversions", "strings")
{
    CATCH_START_SECTION("test conversion strings")
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
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("compare strings", "strings")
{
    CATCH_START_SECTION("compare UTF-8 strings")
        for(int i(1); i < 0x10000; ++i)
        {
            // as is against itself
            std::u32string in;
            in += static_cast<char32_t>(i);
            std::string mb(libutf8::to_u8string(in));
            CATCH_REQUIRE(libutf8::u8casecmp(mb, mb) == 0);

            // as is against uppercase
            std::u32string uin;
            uin += std::towupper(static_cast<char32_t>(i));
            std::string umb(libutf8::to_u8string(uin));
            CATCH_REQUIRE(libutf8::u8casecmp(mb, umb) == 0);

            // as is against lowercase
            std::u32string lin;
            lin += std::towlower(static_cast<char32_t>(i));
            std::string lmb(libutf8::to_u8string(lin));
            CATCH_REQUIRE(libutf8::u8casecmp(mb, lmb) == 0);

            // random
            for(int j(0); j < 30; ++j)
            {
                char32_t const rwc(unittest::rand_char());
                in += rwc;
                uin += std::towupper(rwc);
                lin += std::towlower(rwc);

                std::string rmb(libutf8::to_u8string(in));
                CATCH_REQUIRE(libutf8::u8casecmp(rmb, rmb) == 0);
                std::string rumb(libutf8::to_u8string(uin));
                CATCH_REQUIRE(libutf8::u8casecmp(rmb, rumb) == 0);
                std::string rlmb(libutf8::to_u8string(lin));
                CATCH_REQUIRE(libutf8::u8casecmp(rmb, rlmb) == 0);

                if(rwc >= 0x80 && rand() % 100 == 0)
                {
                    rmb.resize(rmb.length() - 1);
                    CATCH_REQUIRE_THROWS_AS(libutf8::u8casecmp(rmb, rlmb) == 0, libutf8::libutf8_exception_decoding);
                    CATCH_REQUIRE_THROWS_AS(libutf8::u8casecmp(rlmb, rmb) == 0, libutf8::libutf8_exception_decoding);
                }
            }

            char32_t wc(unittest::rand_char());
            in += wc;
            std::string emb(libutf8::to_u8string(in));
            CATCH_REQUIRE(libutf8::u8casecmp(emb, emb) == 0);
            CATCH_REQUIRE(libutf8::u8casecmp(emb, umb) == 1);
            CATCH_REQUIRE(libutf8::u8casecmp(emb, lmb) == 1);
            CATCH_REQUIRE(libutf8::u8casecmp(umb, emb) == -1);
            CATCH_REQUIRE(libutf8::u8casecmp(lmb, emb) == -1);

            {
                wchar_t lwc(unittest::rand_char());
                lin += std::towlower(lwc);
                std::string elmb(libutf8::to_u8string(lin));
//std::cerr << "LOWER compare U+" << std::hex << std::setw(4) << static_cast<int>(wc)
//                         << "/" << std::setw(4) << std::towlower(wc)
//                         << " with U+" << std::setw(4) << static_cast<int>(lwc)
//                         << "/" << std::setw(4) << std::towlower(lwc)
//                         << " wc < lwc -> " << std::setw(4) << (std::towlower(wc) < std::towlower(lwc))
//                         << "\n" << std::dec;
//std::cerr << " result: [" << libutf8::u8casecmp(emb, elmb) << "]\n";
                if(std::towlower(wc) == std::towlower(lwc))
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == 0);
                }
                else if(std::towlower(wc) < std::towlower(lwc))
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == -1);
                    CATCH_REQUIRE(libutf8::u8casecmp(lmb, elmb) == -1);
                }
                else
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, elmb) == 1);
                    CATCH_REQUIRE(libutf8::u8casecmp(lmb, elmb) == -1);
                }
            }

            // here we check with an uppercase character, but notice that the
            // compare uses lowercase!
            {
                char32_t uwc(unittest::rand_char());
                uin += std::towupper(uwc);
                std::string const eumb(libutf8::to_u8string(uin));
//std::cerr << "UPPER compare U+" << std::hex << std::setw(4) << static_cast<int>(wc)
//                         << "/" << std::setw(4) << std::towlower(wc)
//                         << " with U+" << std::setw(4) << static_cast<int>(uwc)
//                         << "/" << std::setw(4) << std::towlower(uwc)
//                         << " wc < uwc -> " << std::setw(4) << (std::towlower(wc) < std::towlower(uwc))
//                         << "\n" << std::dec;
//std::cerr << " result: [" << libutf8::u8casecmp(emb, eumb) << "]\n";
                if(std::towlower(wc) == std::towlower(uwc))
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == 0);
                }
                else if(std::towlower(wc) < std::towlower(uwc))
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == -1);
                }
                else
                {
                    CATCH_REQUIRE(libutf8::u8casecmp(emb, eumb) == 1);
                }
            }
        }
    CATCH_END_SECTION()
}


// With MS-Windows, we can check that our functions work the same way
// (return the expected value) as this Windows API function:
// 
// CompareStringOrdinal(L"This string", 11, L"That string", 11, TRUE);


// vim: ts=4 sw=4 et
