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

// libutf8 lib
//
#include "libutf8/libutf8.h"
#include "libutf8/libutf8base.h"

// catch lib
//
#include <catch2/catch.hpp>

// C++ lib
//
#include <cctype>
#include <iostream>


CATCH_TEST_CASE("libutf8 iterator", "iterator")
{
    char32_t p(rand() % 0x11 * 0x10000);
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
        }

        //if(plan == p)
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
        }

        if(plan == (p + 1) % 0x11 || 1)
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
}



// vim: ts=4 sw=4 et
