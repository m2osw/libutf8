// Copyright (c) 2013-2022  Made to Order Software Corporation
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


// snapdev
//
#include    <snapdev/hexadecimal_string.h>


// C++
//
#include    <cctype>
#include    <iostream>
#include    <iomanip>


// last include
//
#include    <snapdev/poison.h>



CATCH_TEST_CASE("string_validations", "[strings][valid][u8][u32]")
{
    CATCH_START_SECTION("string_validations: valid ASCII including controls")
    {
        CATCH_REQUIRE(libutf8::is_valid_ascii('\0'));
        CATCH_REQUIRE(libutf8::is_valid_ascii('\0', true));

        CATCH_REQUIRE(libutf8::is_valid_ascii(nullptr));
        CATCH_REQUIRE(libutf8::is_valid_ascii(nullptr, true));
        CATCH_REQUIRE(libutf8::is_valid_ascii(nullptr, false));
        CATCH_REQUIRE(libutf8::is_valid_ascii(""));
        CATCH_REQUIRE(libutf8::is_valid_ascii("", true));
        CATCH_REQUIRE(libutf8::is_valid_ascii("", false));

        char buffer[128];
        for(int idx(0); idx < 127; ++idx)
        {
            CATCH_REQUIRE(libutf8::is_valid_ascii(static_cast<char>(idx)));
            CATCH_REQUIRE(libutf8::is_valid_ascii(static_cast<char>(idx, true)));

            buffer[idx] = idx + 1;
        }
        buffer[127] = '\0';
        CATCH_REQUIRE(libutf8::is_valid_ascii(buffer));
        CATCH_REQUIRE(libutf8::is_valid_ascii(buffer, true));

        std::string const s(buffer);
        CATCH_REQUIRE(libutf8::is_valid_ascii(s));
        CATCH_REQUIRE(libutf8::is_valid_ascii(s, true));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: valid ASCII excluding controls")
    {
        char buffer[128];

        for(int idx(0); idx < 126 - 0x20; ++idx)
        {
            CATCH_REQUIRE(libutf8::is_valid_ascii(static_cast<char>(idx + 0x20), false));

            buffer[idx] = idx + 0x20;
        }
        buffer[126 - 0x20] = '\0';
        CATCH_REQUIRE(libutf8::is_valid_ascii(buffer, false));

        std::string const s(buffer);
        CATCH_REQUIRE(libutf8::is_valid_ascii(s, false));
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid ASCII (extended characters)")
    {
        for(int idx(128); idx < 256; ++idx)
        {
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(static_cast<char>(idx)));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(static_cast<char>(idx), true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(static_cast<char>(idx), false));

            char buffer[2];
            buffer[0] = idx;
            buffer[1] = '\0';
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(buffer));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(buffer, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(buffer, false));

            std::string const s(buffer);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(s));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(s, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(s, false));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid ASCII (controls)")
    {
        for(int idx(1); idx < 0x20; ++idx)
        {
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(static_cast<char>(idx), false));

            char buffer[2];
            buffer[0] = idx;
            buffer[1] = '\0';
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(buffer, false));

            std::string const s(buffer);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(s, false));
        }

        for(int idx(127); idx < 256; ++idx)
        {
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(static_cast<char>(idx), false));

            char buffer[2];
            buffer[0] = idx;
            buffer[1] = '\0';
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(buffer, false));

            std::string const s(buffer);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_ascii(s, false));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: Valid UTF-8")
    {
        // nullptr is considered to be an empty string
        //
        CATCH_REQUIRE(libutf8::is_valid_utf8(nullptr));
        CATCH_REQUIRE(libutf8::is_valid_utf8(""));

        for(char32_t wc(1); wc < 0x110000; ++wc)
        {
            if(wc >= 0xD800 && wc <= 0xDFFF)
            {
                wc = 0xE000;
            }

            std::string const ws(libutf8::to_u8string(wc));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws.c_str()));

            CATCH_REQUIRE(libutf8::is_valid_utf8(ws));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid UTF-8 (UTF-16 surrogates)")
    {
        for(char32_t wc(0xD800); wc < 0xDFFF; ++wc)
        {
            char mb[4];
            mb[0] = static_cast<char>((wc >> 12) | 0xE0);
            mb[1] = ((wc >> 6) & 0x3F) | 0x80;
            mb[2] = (wc & 0x3F) | 0x80;
            mb[3] = '\0';

            CATCH_REQUIRE_FALSE(libutf8::is_valid_utf8(mb));

            std::string const ws(mb);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_utf8(ws));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid UTF-8 (invalid code points)")
    {
        for(int count(0); count < 1000; ++count)
        {
            uint32_t wc(0);
            wc = rand() ^ (rand() << 16);
            if(wc < 0x110000)
            {
                wc += 0x110000;
            }

            char mb[8];
            if(wc < (1UL << 21))
            {
                mb[0] = static_cast<char>((wc >> 18) | 0xF0);
                mb[1] = ((wc >> 12) & 0x3F) | 0x80;
                mb[2] = ((wc >> 6) & 0x3F) | 0x80;
                mb[3] = (wc & 0x3F) | 0x80;
                mb[4] = '\0';
            }
            else if(wc < (1UL << 26))
            {
                mb[0] = static_cast<char>((wc >> 24) | 0xF8);
                mb[1] = ((wc >> 18) & 0x3F) | 0x80;
                mb[2] = ((wc >> 12) & 0x3F) | 0x80;
                mb[3] = ((wc >> 6) & 0x3F) | 0x80;
                mb[4] = (wc & 0x3F) | 0x80;
                mb[5] = '\0';
            }
            else if(wc < (1UL << 31))
            {
                mb[0] = static_cast<char>((wc >> 30) | 0xFC);
                mb[1] = ((wc >> 24) & 0x3F) | 0x80;
                mb[2] = ((wc >> 18) & 0x3F) | 0x80;
                mb[3] = ((wc >> 12) & 0x3F) | 0x80;
                mb[4] = ((wc >> 6) & 0x3F) | 0x80;
                mb[5] = (wc & 0x3F) | 0x80;
                mb[6] = '\0';
            }
            else
            {
                // this is really extreme (negative numbers)
                //
                mb[0] = static_cast<char>(0xFE);
                mb[1] = ((wc >> 30) & 0x3F) | 0x80;
                mb[2] = ((wc >> 24) & 0x3F) | 0x80;
                mb[3] = ((wc >> 18) & 0x3F) | 0x80;
                mb[4] = ((wc >> 12) & 0x3F) | 0x80;
                mb[5] = ((wc >> 6) & 0x3F) | 0x80;
                mb[6] = (wc & 0x3F) | 0x80;
                mb[7] = '\0';
            }

            CATCH_REQUIRE_FALSE(libutf8::is_valid_utf8(mb));

            std::string const ws(mb);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_utf8(ws));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: valid UTF-16 (no surrogates)")
    {
        // nullptr is considered to be an empty string
        //
        CATCH_REQUIRE(libutf8::is_valid_utf8(nullptr));
        CATCH_REQUIRE(libutf8::is_valid_utf8(""));

        for(wchar_t wc(1); wc < 0xFFFF; ++wc)
        {
            if(wc >= 0xD800 && wc <= 0xDFFF)
            {
                wc = 0xDFFF;
                continue;
            }

            wchar_t buf[2];
            buf[0] = wc;
            buf[1] = L'\0';

            std::string const ws1(libutf8::to_u8string(buf));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws1.c_str()));

            std::string const ws2(libutf8::to_u8string(wc));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws2.c_str()));

            char16_t const u16(wc);
            std::string const ws3(libutf8::to_u8string(u16));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws3.c_str()));
        }

        if(sizeof(wchar_t) == 4)
        {
            // on Linux wchar_t is like char32_t
            //
            for(wchar_t wc(0x10000); wc < 0x110000; ++wc)
            {
                wchar_t buf[2];
                buf[0] = wc;
                buf[1] = L'\0';

                std::string const ws1(libutf8::to_u8string(buf));
                CATCH_REQUIRE(libutf8::is_valid_utf8(ws1.c_str()));

                std::string const ws2(libutf8::to_u8string(wc));
                CATCH_REQUIRE(libutf8::is_valid_utf8(ws2.c_str()));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: valid UTF-16 (surrogates)")
    {
        // nullptr is considered to be an empty string
        //
        CATCH_REQUIRE(libutf8::is_valid_utf8(nullptr));
        CATCH_REQUIRE(libutf8::is_valid_utf8(""));

        for(char32_t wc(0x10000); wc < 0x110000; ++wc)
        {
            char16_t buf[3];
            buf[0] = ((wc - 0x10000) >> 10) | 0xD800;
            buf[1] = ((wc - 0x10000) & 0x3FF) | 0xDC00;
            buf[2] = u'\0';

            std::string const ws1(libutf8::to_u8string(buf));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws1.c_str()));

            std::string const ws2(libutf8::to_u8string(buf[0], buf[1]));
            CATCH_REQUIRE(libutf8::is_valid_utf8(ws2.c_str()));

            if(sizeof(wchar_t) == 2)
            {
                // under Windows wchar_t is like char16_t
                //
                std::string const ws3(libutf8::to_u8string(buf));
                CATCH_REQUIRE(libutf8::is_valid_utf8(ws3.c_str()));

                std::string const ws4(libutf8::to_u8string(buf[0], buf[1]));
                CATCH_REQUIRE(libutf8::is_valid_utf8(ws4.c_str()));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: valid UTF-32")
    {
        CATCH_REQUIRE(libutf8::is_valid_unicode(U'\0'));
        CATCH_REQUIRE(libutf8::is_valid_unicode(U'\0', true));
        CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(U'\0', false));

        for(char32_t wc(1); wc < 0x110000; ++wc)
        {
            if(wc >= 0xD800 && wc <= 0xDFFF)
            {
                continue;
            }

            CATCH_REQUIRE(libutf8::is_valid_unicode(wc));
            CATCH_REQUIRE(libutf8::is_valid_unicode(wc, true));

            char32_t buf[2];
            buf[0] = wc;
            buf[1] = U'\0';
            CATCH_REQUIRE(libutf8::is_valid_unicode(buf));
            CATCH_REQUIRE(libutf8::is_valid_unicode(buf, true));

            std::u32string const ws(buf);
            CATCH_REQUIRE(libutf8::is_valid_unicode(ws));
            CATCH_REQUIRE(libutf8::is_valid_unicode(ws, true));

            if(wc >= 0x01 && wc <= 0x1F
            || wc >= 0x7F && wc <= 0x9F)
            {
                CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc, false));
                CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(buf, false));
                CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(ws, false));
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid UTF-32 (UTF-16 surrogates)")
    {
        CATCH_REQUIRE(libutf8::is_valid_unicode(nullptr));
        CATCH_REQUIRE(libutf8::is_valid_unicode(nullptr, true));
        CATCH_REQUIRE(libutf8::is_valid_unicode(nullptr, false));
        CATCH_REQUIRE(libutf8::is_valid_unicode(U""));
        CATCH_REQUIRE(libutf8::is_valid_unicode(U"", true));
        CATCH_REQUIRE(libutf8::is_valid_unicode(U"", false));

        for(char32_t wc(0xD800); wc < 0xDFFF; ++wc)
        {
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc, false));

            char32_t buf[2];
            buf[0] = wc;
            buf[1] = U'\0';
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(buf));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(buf, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(buf, false));

            std::u32string const ws(buf);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(ws));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(ws, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(ws, false));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_validations: invalid UTF-32 (invalid code points)")
    {
        for(int count(0); count < 1000; ++count)
        {
            uint32_t wc(0);
            wc = rand() ^ (rand() << 16);
            while(wc < 0x110000)
            {
                wc = rand() ^ (rand() << 16);
            }

            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc, true));
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(wc, false));

            char32_t buf[2];
            buf[0] = wc;
            buf[1] = U'\0';
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(buf));

            std::u32string const ws(buf);
            CATCH_REQUIRE_FALSE(libutf8::is_valid_unicode(ws));
        }
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("invalid_string_validations", "[strings][invalid][u8][u32]")
{
    CATCH_START_SECTION("invalid_string_validations: invalid unicode (UTF-16 surrogates) to UTF-16")
    {
        for(char32_t wc(0xD800); wc < 0xDFFF; ++wc)
        {
            CATCH_REQUIRE_THROWS_MATCHES(
                      libutf8::to_u16string(wc)
                    , libutf8::libutf8_exception_invalid_parameter
                    , Catch::Matchers::ExceptionMessage(
                              "libutf8_exception: to_u16string(): the input wide character \\u"
                            + snapdev::int_to_hex(wc, true, 4)
                            + " is not a valid Unicode character."));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_string_validations: invalid UTF-16 surrogates")
    {
        // first character has to be a valid HIGH surrogate
        //
        for(char16_t wc1(0xDC00); wc1 < 0xE000; ++wc1)
        {
            char16_t const wc2(rand());
            CATCH_REQUIRE_THROWS_MATCHES(
                      libutf8::to_u8string(wc1, wc2)
                    , libutf8::libutf8_exception_decoding
                    , Catch::Matchers::ExceptionMessage(
                                  "libutf8_exception: to_u8string(char16_t, char16_t): the input did not represent a valid surrogate sequence."));
        }

        // second character has to be a valid LOW surrogate
        //
        for(char16_t wc2(1); wc2 != u'\0'; ++wc2)
        {
            if(wc2 >= 0xDC00 && wc2 <= 0xDFFF)
            {
                wc2 = 0xE000;
            }
            char16_t const wc1((rand() & 0x3FF) + 0xD800);
            CATCH_REQUIRE_THROWS_MATCHES(
                      libutf8::to_u8string(wc1, wc2)
                    , libutf8::libutf8_exception_decoding
                    , Catch::Matchers::ExceptionMessage(
                                  "libutf8_exception: to_u8string(char16_t, char16_t): the input did not represent a valid surrogate sequence."));
        }
    }
    CATCH_END_SECTION()
}




CATCH_TEST_CASE("string_concatenation", "[strings][valid][u8][u32]")
{
    CATCH_START_SECTION("string_concatenation: UTF-8 string + char32")
    {
        std::string const s("test");
        char32_t const wc(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ZUNICODE));
        std::string const sum(s + wc);
        std::string expected(s);
        expected += libutf8::to_u8string(wc);
        CATCH_REQUIRE(sum == expected);

        std::string add(s);
        add += wc;
        CATCH_REQUIRE(add == expected);

        std::string swapped(wc + s);
        CATCH_REQUIRE(swapped == libutf8::to_u8string(wc) + s);

        char const ascii(SNAP_CATCH2_NAMESPACE::random_char(SNAP_CATCH2_NAMESPACE::character_t::CHARACTER_ASCII));
        expected = std::string("test") + std::string(1, ascii);

        std::string ascii_add("test");
        ascii_add += ascii;
        CATCH_REQUIRE(ascii_add == expected);

        ascii_add = "test";
        ascii_add += static_cast<int>(ascii);
        CATCH_REQUIRE(ascii_add == expected);

        ascii_add = "test";
        ascii_add += static_cast<unsigned>(ascii);
        CATCH_REQUIRE(ascii_add == expected);

        ascii_add = "test";
        ascii_add += static_cast<long>(ascii);
        CATCH_REQUIRE(ascii_add == expected);

        ascii_add = "test";
        ascii_add += static_cast<unsigned long>(ascii);
        CATCH_REQUIRE(ascii_add == expected);
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("string_conversions", "[strings][valid][u8][u32]")
{
    CATCH_START_SECTION("string_conversions: test conversion strings (0x0001 to 0xFFFD)")
    {
        std::string str;
        std::u32string u32str, back;
        int i;

        // create a string with all the characters defined in plane 1
        for(i = 1; i < 0x0FFFE; ++i)
        {
            // skip the surrogate, they are not considered valid characters
            //
            if(i >= 0xD800 && i <= 0xDFFF)
            {
                i = 0xDFFF;
                continue;
            }
            u32str += static_cast<char32_t>(i);
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
            if(i >= 0xD800 && i <= 0xDFFF)
            {
                i = 0xDFFF;
                continue;
            }
            CATCH_REQUIRE(*s++ == static_cast<char>((i >> 12) | 0xE0));
            CATCH_REQUIRE(*s++ == static_cast<char>(((i >> 6) & 0x3F) | 0x80));
            CATCH_REQUIRE(*s++ == static_cast<char>((i & 0x3F) | 0x80));
        }

        // verify the UTF-8 to char32_t
        //
        back = libutf8::to_u32string(str);
        CATCH_REQUIRE(back == u32str);

        std::u16string u16str(libutf8::to_u16string(str));
        int pos(0);
        for(i = 1; i < 0x0FFFE; ++i)
        {
            // skip the surrogate, they are not considered valid characters
            //
            if(i >= 0xD800 && i <= 0xDFFF)
            {
                i = 0xDFFF;
                continue;
            }
            CATCH_REQUIRE(u16str[pos] == i);
            ++pos;
        }

        std::string u8str(libutf8::to_u8string(u16str));
        CATCH_REQUIRE(u8str == str);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("string_conversions: test conversion strings (0x10000 to 0x110000)")
    {
        std::string str;
        std::u32string u32str, back;

        // create a string with random large characters
        //
        for(char32_t wc(0x10000); wc < 0x110000; wc += rand() % 1000)
        {
            u32str += static_cast<char32_t>(wc);
        }

        str = libutf8::to_u8string(u32str);

        // the result is always a multiple of 4 (each character is 4 UTF-8
        // bytes)
        //
        CATCH_REQUIRE((str.length() & 3) == 0);

        // verify the UTF-8 string
        //
        std::u32string::size_type const max(u32str.length());
        for(size_t i(0); i < max; ++i)
        {
            char32_t const wc(u32str[i]);
            CATCH_REQUIRE(str[i * 4 + 0] == static_cast<char>(((wc >> 18) & 0x3F) | 0xF0));
            CATCH_REQUIRE(str[i * 4 + 1] == static_cast<char>(((wc >> 12) & 0x3F) | 0x80));
            CATCH_REQUIRE(str[i * 4 + 2] == static_cast<char>(((wc >>  6) & 0x3F) | 0x80));
            CATCH_REQUIRE(str[i * 4 + 3] == static_cast<char>(((wc >>  0) & 0x3F) | 0x80));
        }

        // verify the UTF-8 to char32_t
        //
        back = libutf8::to_u32string(str);
        CATCH_REQUIRE(back == u32str);

        std::u16string u16str(libutf8::to_u16string(str));
        for(size_t i(0); i < max; ++i)
        {
            CATCH_REQUIRE(u16str[i * 2 + 0] == (((u32str[i] - 0x10000) >> 10) & 0x3FF) + 0xD800);
            CATCH_REQUIRE(u16str[i * 2 + 1] == (((u32str[i] - 0x10000) >>  0) & 0x3FF) + 0xDC00);
        }

        std::string u8str(libutf8::to_u8string(u16str));
        CATCH_REQUIRE(u8str == str);
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("invalid_string_conversions", "[strings][invalid][u8][u32]")
{
    CATCH_START_SECTION("invalid_string_conversions: test surrogate string conversion (u8)")
    {
        // create a string with all the characters defined in plane 1
        for(char32_t wc = 0xD800; wc < 0xDFFF; ++wc)
        {
            // skip the surrogate, they are not considered valid characters
            //
            std::string str;
            str += ((wc >> 12) & 0x0F) | 0xE0;
            str += ((wc >>  6) & 0x3F) | 0x80;
            str += ((wc >>  9) & 0x3F) | 0x80;
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u32string(str), libutf8::libutf8_exception_decoding);
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u16string(str), libutf8::libutf8_exception_decoding);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_string_conversions: test surrogate string conversion (u32)")
    {
        // create a string with all the characters defined in plane 1
        for(char32_t wc = 0xD800; wc < 0xDFFF; ++wc)
        {
            // skip the surrogate, they are not considered valid characters
            //
            std::u32string u32str;
            u32str += wc;
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u32str), libutf8::libutf8_exception_encoding);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_string_conversions: test conversion strings between 0x110000 and 0xFFFFFFFF")
    {
        for(char32_t wc(0x110000); wc < 0xFFFFFFFF - 50000; wc += rand() % 50000)
        {
            std::u32string u32str;
            u32str += wc;
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u32str), libutf8::libutf8_exception_encoding);
        }

        // make sure the last few fail
        //
        for(char32_t wc(0xFFFFFFFF); wc > 0xFFFFFFFF - 100; --wc)
        {
            std::u32string u32str;
            u32str += wc;
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u32str), libutf8::libutf8_exception_encoding);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_string_conversions: invalid UTF-16 surrogate usage")
    {
        // missing high surrogate
        {
            std::u16string u16str;
            u16str += 0xDC00 + (rand() & 0x3FF);
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u16str), libutf8::libutf8_exception_decoding);
        }

        // input ends before low surrogate
        {
            std::u16string u16str;
            u16str += 0xD800 + (rand() & 0x3FF);
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u16str), libutf8::libutf8_exception_decoding);
        }

        // two high surrogates in a row
        {
            std::u16string u16str;
            u16str += 0xD800 + (rand() & 0x3FF);
            u16str += 0xD800 + (rand() & 0x3FF);
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u16str), libutf8::libutf8_exception_decoding);
        }

        // high surrogate, no low surrogate
        {
            std::u16string u16str;
            u16str += 0xD800 + (rand() & 0x3FF);
            u16str += 0xE000 + (rand() & 0x1FFF);
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(u16str), libutf8::libutf8_exception_decoding);
        }
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("wc_to_string", "[wc][strings][valid][u8]")
{
    CATCH_START_SECTION("wc_to_string: test wc to u8string conversions between 0 and 0x80")
    {
        for(char32_t wc(0); wc < 0x80; ++wc)
        {
            std::string const str(libutf8::to_u8string(wc));
            CATCH_REQUIRE(str.length() == 1);
            CATCH_REQUIRE(str[0] == static_cast<char>(wc));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("wc_to_string: test wc to u8string conversions between 0x80 and 0x800")
    {
        for(char32_t wc(0x80); wc < 0x800; ++wc)
        {
            std::string const str(libutf8::to_u8string(wc));
            CATCH_REQUIRE(str.length() == 2);
            CATCH_REQUIRE(str[0] == static_cast<char>((wc >> 6) | 0xC0));
            CATCH_REQUIRE(str[1] == static_cast<char>((wc & 0x3F) | 0x80));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("wc_to_string: test wc to u8string conversions between 0x800 and 0x10000")
    {
        for(char32_t wc(0x800); wc < 0x10000; ++wc)
        {
            // skip the surrogate, they are not considered valid characters
            //
            if(wc >= 0xD800 && wc <= 0xDFFF)
            {
                wc = 0xDFFF;
                continue;
            }

            std::string const str(libutf8::to_u8string(wc));
            CATCH_REQUIRE(str.length() == 3);
            CATCH_REQUIRE(str[0] == static_cast<char>((wc >> 12) | 0xE0));
            CATCH_REQUIRE(str[1] == static_cast<char>(((wc >> 6) & 0x3F) | 0x80));
            CATCH_REQUIRE(str[2] == static_cast<char>((wc & 0x3F) | 0x80));
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("wc_to_string: test wc to u8string conversions between 0x10000 and 0x110000")
    {
        for(char32_t wc(0x10000); wc < 0x110000; ++wc)
        {
            std::string const str(libutf8::to_u8string(wc));
            CATCH_REQUIRE(str.length() == 4);
            CATCH_REQUIRE(str[0] == static_cast<char>(((wc >> 18) & 0x3F) | 0xF0));
            CATCH_REQUIRE(str[1] == static_cast<char>(((wc >> 12) & 0x3F) | 0x80));
            CATCH_REQUIRE(str[2] == static_cast<char>(((wc >>  6) & 0x3F) | 0x80));
            CATCH_REQUIRE(str[3] == static_cast<char>(((wc >>  0) & 0x3F) | 0x80));
        }
    }
    CATCH_END_SECTION()
}


CATCH_TEST_CASE("invalid_wc_to_string", "[wc][strings][invalid][u8]")
{
    CATCH_START_SECTION("invalid_wc_to_string: test wc to u8string conversions between 0x800 and 0x10000")
    {
        for(char32_t wc(0xD800); wc < 0xDFFF; ++wc)
        {
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(wc), libutf8::libutf8_exception_encoding);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("invalid_wc_to_string: test wc to u8string conversions between 0x110000 and 0xFFFFFFFF")
    {
        for(char32_t wc(0x110000); wc < 0xFFFFFFFF - 50000; wc += rand() % 50000)
        {
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(wc), libutf8::libutf8_exception_encoding);
        }

        // make sure the last few fail
        //
        for(char32_t wc(0xFFFFFFFF); wc > 0xFFFFFFFF - 100; --wc)
        {
            CATCH_REQUIRE_THROWS_AS(libutf8::to_u8string(wc), libutf8::libutf8_exception_encoding);
        }
    }
    CATCH_END_SECTION()
}



CATCH_TEST_CASE("compare_strings", "[compare][strings][valid][invalid][u8]")
{
    CATCH_START_SECTION("compare_strings: compare UTF-8 strings")
    {
        for(int i(1); i < 0x10000; ++i)
        {
            if(i >= 0xD800 && i <= 0xDFFF)
            {
                i = 0xDFFF;
                continue;
            }

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
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
