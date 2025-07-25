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
#include    <libutf8/iterator.h>

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



CATCH_TEST_CASE("libutf8_iterator", "[iterator]")
{
    CATCH_START_SECTION("libutf8_iterator: valid iterators tests")
    {
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
                libutf8::utf8_iterator it_end(str, true);
                libutf8::utf8_iterator it_next(str);
                ++it_next;

                CATCH_REQUIRE(it == str.begin());
                CATCH_REQUIRE(it == str.cbegin());
                CATCH_REQUIRE(it != str.end());
                CATCH_REQUIRE(it != str.cend());

                CATCH_REQUIRE(it == it);
                CATCH_REQUIRE(it != it_end);
                CATCH_REQUIRE(it != it_next);

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

                CATCH_REQUIRE(*it == libutf8::EOS);
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
                CATCH_REQUIRE_FALSE(it.bad());
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
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());
                ++it;
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());
                CATCH_REQUIRE(it == str.end());
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());

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

            if(plan == (p + 0x10000) % 0x110000)
            {
                libutf8::utf8_iterator it(str);
                libutf8::utf8_iterator start(str);
                CATCH_REQUIRE(it - start == 0);
                CATCH_REQUIRE(start - it == 0);

                for(char32_t wc(0); wc < 0x10000; ++wc)
                {
                    if(plan == 0 && wc >= 0xD800 && wc <= 0xDFFF)
                    {
                        wc = 0xDFFF;
                        continue;
                    }
                    CATCH_REQUIRE(*it == wc + plan);
                    it++;

                    libutf8::utf8_iterator zero(it);
                    zero.rewind();
                    CATCH_REQUIRE(zero == start);
                }

                libutf8::utf8_iterator copy(it);
                CATCH_REQUIRE(static_cast<std::size_t>(it - start) == str.length());
                CATCH_REQUIRE(static_cast<std::size_t>(copy - start) == str.length());
                CATCH_REQUIRE(copy - it == 0);
                CATCH_REQUIRE(it - copy == 0);
                copy.rewind();
                CATCH_REQUIRE(copy - start == 0);
                CATCH_REQUIRE(start - copy == 0);
                CATCH_REQUIRE(static_cast<std::size_t>(start - copy) == 0);
                CATCH_REQUIRE(static_cast<std::size_t>(copy - start) == 0);

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
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("libutf8_iterator_invalid_string", "[iterator],[invalid]")
{
    CATCH_START_SECTION("libutf8_iterator_invalid_string: iterators with invalid characters (bad UTF-8)")
    {
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

                CATCH_REQUIRE(*it == libutf8::EOS);
                ++it;
                it++;
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());
            }

            {
                libutf8::utf8_iterator it(str);

                str[pos[1]] = rand() % 0x40 + 0x80;

                CATCH_REQUIRE(*it++ == wstr[0]);
                CATCH_REQUIRE(*it++ == libutf8::NOT_A_CHARACTER);       // we broke this one
                CATCH_REQUIRE(*it++ == wstr[2]);
                CATCH_REQUIRE(*it++ == wstr[3]);
                CATCH_REQUIRE(*it++ == libutf8::EOS);

                CATCH_REQUIRE_FALSE(it.good());
                CATCH_REQUIRE(it.bad());
                it.clear();
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());
            }

            {
                str.erase(str.length() - 1);
                libutf8::utf8_iterator it(str);

                str[pos[1]] = rand() % 0x40 + 0x80;

                CATCH_REQUIRE(*it++ == wstr[0]);
                CATCH_REQUIRE(*it++ == libutf8::NOT_A_CHARACTER);
                CATCH_REQUIRE(*it++ == wstr[2]);
                CATCH_REQUIRE(*it++ == libutf8::NOT_A_CHARACTER);

                CATCH_REQUIRE_FALSE(it.good());
                CATCH_REQUIRE(it.bad());
                it.clear();
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("libutf8_iterator_invalid_string: iterators with invalid characters (too large)")
    {
        for(char32_t wc(0x110000); wc < 0x1FFFFF; ++wc)
        {
            // since this character is not valid
            // we have to encode it _manually_
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

                CATCH_REQUIRE(*it == libutf8::NOT_A_CHARACTER);

                CATCH_REQUIRE_FALSE(it.good());
                CATCH_REQUIRE(it.bad());
                it.clear();
                CATCH_REQUIRE(it.good());
                CATCH_REQUIRE_FALSE(it.bad());

                ++it;

                CATCH_REQUIRE(it != str.begin());
                CATCH_REQUIRE(it != str.cbegin());
                CATCH_REQUIRE(it == str.end());
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE(str.begin()  != it);
                CATCH_REQUIRE(str.cbegin() != it);
                CATCH_REQUIRE(str.end()    == it);
                CATCH_REQUIRE(str.cend()   == it);

                CATCH_REQUIRE(*it == libutf8::EOS);
                ++it;
                it++;
                CATCH_REQUIRE(it == str.cend());

                CATCH_REQUIRE_FALSE(it.good());
                CATCH_REQUIRE(it.bad());
            }
        }
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
