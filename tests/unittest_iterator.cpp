/*    unittest_iterator.cpp
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
#include "libutf8/base.h"
#include "libutf8/iterator.h"
#include "libutf8/libutf8.h"

// catch lib
//
#include <catch2/catch.hpp>

// C++ lib
//
#include <cctype>
#include <iostream>


CATCH_TEST_CASE("libutf8 iterator", "iterator")
{
    CATCH_START_SECTION("valid iterators tests")
        char32_t p(0);
        do
        {
            p = rand() % 0x11 * 0x10000;
        }
        while(p == 0 || (p >= 0xD800 && p <= 0xDFFF));

        for(char32_t plan(0); plan < 0x110000; plan += 0x10000)
        {
            // create one plan in one string
            //
            std::string str;
            str.reserve(0x10000 * 4);
            for(char32_t wc(0); wc < 0x10000; ++wc)
            {
                if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                {
                    wc = 0xDFFF;
                    continue;
                }
                char buf[libutf8::MBS_MIN_BUFFER_LENGTH];
                CATCH_REQUIRE(libutf8::wctombs(buf, wc + plan, sizeof(buf)) >= 1);
                if(plan == 0 && wc == 0)
                {
                    // this is a special case as buf[0] = '\0' and the += with
                    // the string won't work
                    //
                    str += '\0';
                }
                else
                {
                    str += buf;
                }
            }
//std::cerr << "-------------- Plan " << static_cast<int>(plan) << " String ready " << str.length() << " ...\n";

            {
                libutf8::utf8_iterator it(str);

                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(it == str.cbegin());
                CATCH_REQUIRE(it != str.end());
                CATCH_REQUIRE(it != str.cend());

                CATCH_REQUIRE(str.begin() == it);
                CATCH_REQUIRE(str.cbegin() == it);
                CATCH_REQUIRE(str.end() != it);
                CATCH_REQUIRE(str.cend() != it);

                for(char32_t wc(0); wc < 0x10000; ++wc)
                {
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xDFFF;
                        continue;
                    }
                    CATCH_REQUIRE(*it == wc + plan);
                    ++it;
                }

                CATCH_REQUIRE(it != str.begin());
                CATCH_REQUIRE(it != str.cbegin());
                CATCH_REQUIRE(it == str.end());
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE(str.begin() != it);
                CATCH_REQUIRE(str.cbegin() != it);
                CATCH_REQUIRE(str.end() == it);
                CATCH_REQUIRE(str.cend() == it);

                CATCH_REQUIRE(*it == EOF);
                ++it;
                it++;
                CATCH_REQUIRE(it == str.cend());

                for(char32_t wc(0x10000); wc > 0; )
                {
                    --wc;
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xD800;
                        continue;
                    }
                    --it;
                    CATCH_REQUIRE(*it == wc + plan);
                }

                --it;
                it--;

                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE(!it.bad());
            }

            if(plan == p)
            {
                libutf8::utf8_iterator it(str);

                for(char32_t wc(0); wc < 0x10000; ++wc)
                {
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xDFFF;
                        continue;
                    }
                    CATCH_REQUIRE(*it++ == wc + plan);
                }

                CATCH_REQUIRE(it == str.end());
                it++;
                ++it;
                CATCH_REQUIRE(it == str.end());

                for(char32_t wc(0x10000); wc > 0; )
                {
                    --wc;
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xD800;
                        continue;
                    }
                    CATCH_REQUIRE(*--it == wc + plan);
                }

                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(str.begin() == it);
                it--;
                --it;
                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(str.begin() == it);
            }

            if(plan == (p + 1) % 0x11)
            {
                libutf8::utf8_iterator it(str);

                for(char32_t wc(0); wc < 0x10000; ++wc)
                {
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xDFFF;
                        continue;
                    }
                    CATCH_REQUIRE(*it == wc + plan);
                    it++;
                }

                for(char32_t wc(0x10000); wc > 0; )
                {
                    --wc;
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xD800;
                        continue;
                    }
                    it--;
                    CATCH_REQUIRE(*it == wc + plan);
                }
            }
        }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("libutf8 iterator invalid string", "iterator,invalid")
{
    CATCH_START_SECTION("iterators with invalid characters (bad UTF-8)")
        for(int repeat(0); repeat < 100; ++repeat)
        {
            // create one plan in one string
            //
            constexpr size_t STR_LENGTH = 4;
            char32_t wc;
            std::u32string wstr;
            wstr.reserve(STR_LENGTH);
            for(size_t idx(0); idx < STR_LENGTH; ++idx)
            {
                do
                {
                    wc = unittest::rand_char(true);
                }
                while(wc < 0x80);
                wstr += wc;
            }
            std::string str(libutf8::to_u8string(wstr));

//std::cerr << "-------------- Plan " << static_cast<int>(plan) << " String ready " << str.length() << " ...\n";

            // first verify that it works
            //
            std::string::size_type pos[STR_LENGTH];
            {
                libutf8::utf8_iterator it(str);

                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(it == str.cbegin());
                CATCH_REQUIRE(it != str.end());
                CATCH_REQUIRE(it != str.cend());

                CATCH_REQUIRE(str.begin()  == it);
                CATCH_REQUIRE(str.cbegin() == it);
                CATCH_REQUIRE(str.end()    != it);
                CATCH_REQUIRE(str.cend()   != it);

                for(size_t idx(0); idx < STR_LENGTH; ++idx)
                {
                    CATCH_REQUIRE(*it == wstr[idx]);
                    if(rand() % 2 == 0)
                    {
                        pos[idx] = it - str.begin();
                    }
                    else
                    {
                        pos[idx] = -(str.begin() - it);
                    }
                    ++it;
                }

                CATCH_REQUIRE(it != str.begin());
                CATCH_REQUIRE(it != str.cbegin());
                CATCH_REQUIRE(it == str.end());
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE(str.begin()  != it);
                CATCH_REQUIRE(str.cbegin() != it);
                CATCH_REQUIRE(str.end()    == it);
                CATCH_REQUIRE(str.cend()   == it);

                CATCH_REQUIRE(*it == EOF);
                ++it;
                it++;
                CATCH_REQUIRE(it == str.cend());
            }

            {
                libutf8::utf8_iterator it(str);

                str[pos[1]] = rand() % 0x40 + 0x80;

                CATCH_REQUIRE(*it++ == wstr[0]);
                CATCH_REQUIRE(*it++ == L'\0');       // we broke this one
                CATCH_REQUIRE(*it++ == wstr[2]);
                CATCH_REQUIRE(*it++ == wstr[3]);
                CATCH_REQUIRE(*it++ == EOF);
            }

            {
                str.erase(str.length() - 1);
                libutf8::utf8_iterator it(str);

                str[pos[1]] = rand() % 0x40 + 0x80;

                CATCH_REQUIRE(*it++ == wstr[0]);
                CATCH_REQUIRE(*it++ == L'\0');
                CATCH_REQUIRE(*it++ == wstr[2]);
                CATCH_REQUIRE(*it++ == L'\0');
            }
        }
    CATCH_END_SECTION()

    CATCH_START_SECTION("iterators with invalid characters (too large)")
        for(char32_t wc(0x110000); wc < 0x1FFFFF; ++wc)
        {
            // since this character is not we have to encode it _manually_
            //
            char buf[4];
            buf[0] = 0xF0 | ((wc >> 18) & 0x07);
            buf[1] = 0x80 | ((wc >> 12) & 0x3F);
            buf[2] = 0x80 | ((wc >>  6) & 0x3F);
            buf[3] = 0x80 | ((wc >>  0) & 0x3F);

            std::string str(buf, 4);

            // first verify that it works
            //
            {
                libutf8::utf8_iterator it(str);

                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(it == str.cbegin());
                CATCH_REQUIRE(it != str.end());
                CATCH_REQUIRE(it != str.cend());

                CATCH_REQUIRE(str.begin()  == it);
                CATCH_REQUIRE(str.cbegin() == it);
                CATCH_REQUIRE(str.end()    != it);
                CATCH_REQUIRE(str.cend()   != it);

                CATCH_REQUIRE(*it == '\0');
                ++it;

                CATCH_REQUIRE(it != str.begin());
                CATCH_REQUIRE(it != str.cbegin());
                CATCH_REQUIRE(it == str.end());
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE(str.begin()  != it);
                CATCH_REQUIRE(str.cbegin() != it);
                CATCH_REQUIRE(str.end()    == it);
                CATCH_REQUIRE(str.cend()   == it);

                CATCH_REQUIRE(*it == EOF);
                ++it;
                it++;
                CATCH_REQUIRE(it == str.cend());
            }
        }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
