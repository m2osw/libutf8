/*    tests/bom.cpp
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
#include "main.h"

// libutf8 lib
//
#include "libutf8/caseinsensitivestring.h"

// C++ lib
//
#include <cctype>
#include <iostream>


namespace
{


libutf8::case_insensitive_string get_time(std::string & result)
{
    time_t const now(time(NULL));
    struct tm t;
    localtime_r(&now, &t);
    char buf[256];
    strftime(buf, sizeof(buf), "%T", &t);
    buf[sizeof(buf) - 1] = '\0';
    result = buf;
    libutf8::case_insensitive_string r(buf);
    r += " PST";
    return r;
}

std::string get_date(std::string & result)
{
    time_t const now(time(NULL));
    struct tm t;
    localtime_r(&now, &t);
    char buf[256];
    strftime(buf, sizeof(buf), "%F", &t);
    buf[sizeof(buf) - 1] = '\0';
    result = buf;
    libutf8::case_insensitive_string r(buf);
    r += " plus a few days";
    return r;
}



}



CATCH_TEST_CASE("case_insensitive", "[string],[compare],[insensitive]")
{
    CATCH_START_SECTION("Verify Case Insensitive String Constructors")
    {
        {
            libutf8::case_insensitive_string empty;
            CATCH_REQUIRE(empty.empty());
        }

        {
            std::allocator<char> allocator;
            libutf8::case_insensitive_string empty(allocator);
            CATCH_REQUIRE(empty.empty());
        }

        {
            libutf8::case_insensitive_string dashes(10, '-');
            CATCH_REQUIRE(dashes == "----------");
        }

        {
            libutf8::case_insensitive_string name("alexis");
            CATCH_REQUIRE(name == "alexis");
        }

        {
            libutf8::case_insensitive_string name("alexis", 4);
            CATCH_REQUIRE(name == "alex");
        }

        {
            libutf8::case_insensitive_string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string section(name, 2);
            CATCH_REQUIRE(section == "exis");
        }

        {
            libutf8::case_insensitive_string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string section(name, 2, 2);
            CATCH_REQUIRE(section == "ex");
        }

        {
            std::string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string section(name, 2);
            CATCH_REQUIRE(section == "exis");
        }

        {
            std::string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string section(name, 2, 2);
            CATCH_REQUIRE(section == "ex");
        }

        {
            libutf8::case_insensitive_string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string section(name.begin() + 2, name.end() - 2);
            CATCH_REQUIRE(section == "ex");
        }

        {
            std::string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string full(name);
            CATCH_REQUIRE(full == "alexis");
        }

        {
            libutf8::case_insensitive_string name("alexis");
            CATCH_REQUIRE(name == "alexis");

            libutf8::case_insensitive_string full(name);
            CATCH_REQUIRE(full == "alexis");
        }

        {
            libutf8::case_insensitive_string name({'a', 'l', 'e', 'x', 'i', 's'});
            CATCH_REQUIRE(name == "alexis");
        }

        {
            std::string expected("not this");
            libutf8::case_insensitive_string now(get_time(expected));
            CATCH_REQUIRE(expected + " PST" == now);
        }

        {
            std::allocator<char> allocator;
            std::string expected("not this");
            libutf8::case_insensitive_string now(get_time(expected), allocator);
            CATCH_REQUIRE(expected + " PST" == now);
        }

        {
            std::string expected("not this");
            libutf8::case_insensitive_string now(get_date(expected));
            CATCH_REQUIRE(now == expected + " plus a few days");
        }

        {
            std::allocator<char> allocator;
            std::string expected("not this");
            libutf8::case_insensitive_string now(get_date(expected), allocator);
            CATCH_REQUIRE(now == expected + " plus a few days");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("Verify Case Insensitive String Comparators")
    {
        {
            libutf8::case_insensitive_string name1("Alexis");
            libutf8::case_insensitive_string name2("alexis");
            CATCH_REQUIRE(name1 == name2);
            CATCH_REQUIRE_FALSE(name1 != name2);
            CATCH_REQUIRE_FALSE(name1 > name2);
            CATCH_REQUIRE(name1 >= name2);
            CATCH_REQUIRE_FALSE(name1 < name2);
            CATCH_REQUIRE(name1 <= name2);
        }

        {
            libutf8::case_insensitive_string name1("Alexis");
            libutf8::case_insensitive_string name2("Wilke");
            CATCH_REQUIRE_FALSE(name1 == name2);
            CATCH_REQUIRE(name1 != name2);
            CATCH_REQUIRE_FALSE(name1 > name2);
            CATCH_REQUIRE_FALSE(name1 >= name2);
            CATCH_REQUIRE(name1 < name2);
            CATCH_REQUIRE(name1 <= name2);
        }

        {
            libutf8::case_insensitive_string name1("Alexis");
            std::string name2("alexis");
            CATCH_REQUIRE(name1 == name2);
            CATCH_REQUIRE_FALSE(name1 != name2);
            CATCH_REQUIRE_FALSE(name1 > name2);
            CATCH_REQUIRE(name1 >= name2);
            CATCH_REQUIRE_FALSE(name1 < name2);
            CATCH_REQUIRE(name1 <= name2);
        }

        {
            std::string name1("Alexis");
            libutf8::case_insensitive_string name2("Wilke");
            CATCH_REQUIRE_FALSE(name1 == name2);
            CATCH_REQUIRE(name1 != name2);
            CATCH_REQUIRE_FALSE(name1 > name2);
            CATCH_REQUIRE_FALSE(name1 >= name2);
            CATCH_REQUIRE(name1 < name2);
            CATCH_REQUIRE(name1 <= name2);
        }

        {
            libutf8::case_insensitive_string name1("Alexis");
            CATCH_REQUIRE(name1 == "alexis");
            CATCH_REQUIRE_FALSE(name1 != "alexis");
            CATCH_REQUIRE_FALSE(name1 > "alexis");
            CATCH_REQUIRE(name1 >= "alexis");
            CATCH_REQUIRE_FALSE(name1 < "alexis");
            CATCH_REQUIRE(name1 <= "alexis");
        }

        {
            libutf8::case_insensitive_string name2("Wilke");
            CATCH_REQUIRE_FALSE("Alexis" == name2);
            CATCH_REQUIRE("Alexis" != name2);
            CATCH_REQUIRE_FALSE("Alexis" > name2);
            CATCH_REQUIRE_FALSE("Alexis" >= name2);
            CATCH_REQUIRE("Alexis" < name2);
            CATCH_REQUIRE("Alexis" <= name2);
        }
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
