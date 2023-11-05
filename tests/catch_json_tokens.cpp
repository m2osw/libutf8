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
#include    <libutf8/json_tokens.h>

#include    <libutf8/libutf8.h>


// unit test
//
#include    "catch_main.h"


// snapdev
//
#include    <snapdev/file_contents.h>


// C++
//
#include    <cctype>
#include    <iomanip>
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
CATCH_TEST_CASE("json_tokens", "[json][iterator]")
{
    CATCH_START_SECTION("json_tokens: valid JSON tokens")
    {
        std::string valid_json(
            "{\n"
                "\"array-of-numbers\": [\n"
                    "\t1,\r\n"
                    "\t1.0,\r"
                    "\t-0.1\r\n"
                "]   ,  \n"
                "\"color\"      :true  ,\n"
                "\"temporary\"  :false,   \r"
                "\"flowers\":null\r\n"
            "}"
        );

#if 0
        snap::file_contents f("test.json");
        f.contents(valid_json);
        f.write_all();
#endif

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "array-of-numbers");
        CATCH_REQUIRE(jt.line() == 2);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.line() == 2);
        CATCH_REQUIRE(jt.column() == 19);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_ARRAY);
        CATCH_REQUIRE(jt.line() == 2);
        CATCH_REQUIRE(jt.column() == 21);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE(jt.number() == 1.0_a);
        CATCH_REQUIRE(jt.line() == 3);
        CATCH_REQUIRE(jt.column() == 2);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.line() == 3);
        CATCH_REQUIRE(jt.column() == 3 + 1);    // error due to the unget
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE(jt.number() == 1.0_a);
        CATCH_REQUIRE(jt.line() == 4);
        CATCH_REQUIRE(jt.column() == 2);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.line() == 4);
        CATCH_REQUIRE(jt.column() == 5 + 1);    // error due to the unget
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE(jt.number() == -0.1_a);
        CATCH_REQUIRE(jt.line() == 5);
        CATCH_REQUIRE(jt.column() == 2);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_ARRAY);
        CATCH_REQUIRE(jt.line() == 6);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.line() == 6);
        CATCH_REQUIRE(jt.column() == 5);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "color");
        CATCH_REQUIRE(jt.line() == 7);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.line() == 7);
        CATCH_REQUIRE(jt.column() == 14);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_TRUE);
        CATCH_REQUIRE(jt.line() == 7);
        CATCH_REQUIRE(jt.column() == 15);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.line() == 7);
        CATCH_REQUIRE(jt.column() == 21);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "temporary");
        CATCH_REQUIRE(jt.line() == 8);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.line() == 8);
        CATCH_REQUIRE(jt.column() == 14);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_FALSE);
        CATCH_REQUIRE(jt.line() == 8);
        CATCH_REQUIRE(jt.column() == 15);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.line() == 8);
        CATCH_REQUIRE(jt.column() == 20);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "flowers");
        CATCH_REQUIRE(jt.line() == 9);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.line() == 9);
        CATCH_REQUIRE(jt.column() == 10);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NULL);
        CATCH_REQUIRE(jt.line() == 9);
        CATCH_REQUIRE(jt.column() == 11);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
        CATCH_REQUIRE(jt.line() == 10);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens: valid JSON numbers")
    {
        std::string valid_json(
            "["
                "733,"
                "-1892,"
                "-1.892,"
                "-9.892e33,"
                "101.302e+3,"
                "5031.70232e-13"
            "]"
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_ARRAY);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), 733.0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), -1892.0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), -1.892);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), -9.892e33);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), 101.302e3);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_NUMBER);
        CATCH_REQUIRE_FLOATING_POINT(jt.number(), 5031.70232e-13);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_ARRAY);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens: valid JSON special escaped characters")
    {
        std::string valid_json(
            "{"
                "\"backslash\":\"\\\\\","
                "\"quote\":\"\\\"\","
                "\"slash\":\"\\/\","
                "\"backspace\":\"\\b\","
                "\"formfeed\":\"\\f\","
                "\"newline\":\"\\n\","
                "\"carriage-return\":\"\\r\","
                "\"tab\":\"\\t\""
            "}"
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "backslash");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\\");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "quote");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\"");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "slash");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "/");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "backspace");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\b");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "formfeed");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\f");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "newline");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\n");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "carriage-return");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\r");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COMMA);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "tab");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
        CATCH_REQUIRE(jt.string() == "\t");
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens: valid JSON unicode escaped characters")
    {
        for(char32_t c(1); c < 0x110000; ++c)
        {
            if(c >= 0xD800 && c <= 0xDFFF)
            {
                continue;
            }

            std::stringstream ss;
            std::string valid_json;
            valid_json += "{\"unicode\":\"\\u";
            if(c > 0xFFFF)
            {
                int s(c - 0x10000);
                ss << std::hex << std::setw(4) << std::setfill('0') << ((s >> 10) | 0xD800)
                   << "\\u" << std::setw(4) << std::setfill('0') << ((s & 0x3FF) | 0xDC00);
            }
            else
            {
                ss << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
            }
            valid_json += ss.str();
            valid_json += "\"}";

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "unicode");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            std::string expected(libutf8::to_u8string(c));
            CATCH_REQUIRE_LONG_STRING(jt.string(), expected);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_END);
        }
    }
    CATCH_END_SECTION()
}
#pragma GCC diagnostic pop


CATCH_TEST_CASE("json_tokens_invalid", "[json][iterator][invalid]")
{
    CATCH_START_SECTION("json_tokens_invalid: invalid JSON negative number")
    {
        std::string valid_json(
            "-a"
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "found unexpected character: '-'");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON number with fraction")
    {
        std::string valid_json(
            "-3., 2."
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "number cannot end with a period (\"1.\" is not valid JSON)");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON number exponent")
    {
        std::string valid_json(
            "-3.0e+a, 2.1"
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "number exponent must include at least one digit");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON number with fraction")
    {
        std::string valid_json(
            "\"back\\slash\""
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "unexpected escape character: 's'");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: unsupported JSON backslash character")
    {
        {
            std::string valid_json(
                "\"back\\slash\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "unexpected escape character: 's'");
        }

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: too short")
    {
        {
            std::string valid_json(
                "\"\\u0"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }

        {
            std::string valid_json(
                "\"\\u20"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }

        {
            std::string valid_json(
                "\"\\u301"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: low surrogate missing backslash")
    {
        std::string valid_json(
            "\"\\uD91Fmissing\""
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "expected a low surrogate right after a high surrogate, backslash (\\) mising");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: low surrogate missing 'u'")
    {
        std::string valid_json(
            "\"\\uD91F\\missing\""
        );

        libutf8::json_tokens jt(valid_json);
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "expected a low surrogate right after a high surrogate, 'u' missing");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: low surrogate expected")
    {
        {
            std::string valid_json(
                "\"\\uD91F\\u0010\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "expected a low surrogate right after a high surrogate");
        }

        {
            std::string valid_json(
                "\"\\uD91F\\uDBFF\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "expected a low surrogate right after a high surrogate");
        }

        {
            std::string valid_json(
                "\"\\uD91F\\uE030"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "expected a low surrogate right after a high surrogate");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: low surrogate too short")
    {
        {
            std::string valid_json(
                "\"\\uD91F\\u0"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }

        {
            std::string valid_json(
                "\"\\uD91F\\u0f"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }

        {
            std::string valid_json(
                "\"\\uD91F\\u0fa"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'EOS'");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: invalid hexadecimal digit (low surrogate)")
    {
        {
            std::string valid_json(
                "\"\\udb31\\u0t\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 't'");
        }

        {
            std::string valid_json(
                "\"\\udb31\\u3eg\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'g'");
        }

        {
            std::string valid_json(
                "\"\\udb31\\ua3e!\""
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: '!'");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: low surrogate first")
    {
        for(char32_t c(0xDC00); c <= 0xDFFF; ++c)
        {
            std::stringstream ss;
            ss << "\\u" << std::hex << std::setw(4)
                << std::setfill('0') << static_cast<int>(c);

            // breaking up line so it compiles on lunar
            //
            //libutf8::json_tokens jt("\"" + ss.str() + "\"");
            std::string str("\"");
            str += ss.str();
            str += "\"";
            libutf8::json_tokens jt(str);

            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            std::string const msg("low surrogate " + ss.str()
                                    + " found before a high surrogate");
            CATCH_REQUIRE(jt.error() == msg);
        }

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: invalid JSON unicode: invalid hexadecimal digit")
    {
        {
            std::string valid_json(
                "\"\\u5z"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: 'z'");
        }

        {
            std::string valid_json(
                "\"\\uaa$"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: '$'");
        }

        {
            std::string valid_json(
                "\"\\ua9a\001"
            );

            libutf8::json_tokens jt(valid_json);
            CATCH_REQUIRE(jt.line() == 0);
            CATCH_REQUIRE(jt.column() == 0);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.line() == 1);
            CATCH_REQUIRE(jt.column() == 1);
            CATCH_REQUIRE(jt.error() == "invalid unicode character: '^A'");
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: unterminated JSON string")
    {
        libutf8::json_tokens jt("\"unterminated");
        CATCH_REQUIRE(jt.line() == 0);
        CATCH_REQUIRE(jt.column() == 0);
        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.line() == 1);
        CATCH_REQUIRE(jt.column() == 1);
        CATCH_REQUIRE(jt.error() == "unclosed string");

    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: JSON true with missing characters")
    {
        {
            libutf8::json_tokens jt("{\"bad-true\":tru}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-true");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 't'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'r'");
        }

        {
            libutf8::json_tokens jt("{\"bad-true\":tr}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-true");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 't'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'r'");
        }

        {
            libutf8::json_tokens jt("{\"bad-true\":t}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-true");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 't'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: JSON false with missing characters")
    {
        {
            libutf8::json_tokens jt("{\"bad-false\":fals}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-false");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'f'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'a'");
        }

        {
            libutf8::json_tokens jt("{\"bad-false\":fal}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-false");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'f'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'a'");
        }

        {
            libutf8::json_tokens jt("{\"bad-false\":fa}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-false");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'f'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'a'");
        }

        {
            libutf8::json_tokens jt("{\"bad-false\":f}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-false");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'f'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: JSON null with missing characters")
    {
        {
            libutf8::json_tokens jt("{\"bad-null\":nul}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-null");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'n'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'u'");
        }

        {
            libutf8::json_tokens jt("{\"bad-null\":nu}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-null");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'n'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'u'");
        }

        {
            libutf8::json_tokens jt("{\"bad-null\":n}");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_OPEN_OBJECT);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_STRING);
            CATCH_REQUIRE(jt.string() == "bad-null");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_COLON);
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            CATCH_REQUIRE(jt.error() == "found unexpected character: 'n'");
            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_CLOSE_OBJECT);
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: unexpected JSON characters")
    {
        for(char32_t c(1); c < 0x110000; ++c)
        {
            if(c >= 0xD800 && c <= 0xDFFF)
            {
                continue;
            }
            switch(c)
            {
            case '"':
            case '{':
            case '}':
            case '[':
            case ']':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case ' ':
            case '\t':
            case '\r':
            case '\n':
            case 'n':
            case 't':
            case 'f':
            case ',':
            case ':':
                continue;

            }

            std::string invalid_json(libutf8::to_u8string(c));
            libutf8::json_tokens jt(invalid_json);

            CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
            if(c < 0x20)
            {
                CATCH_REQUIRE(jt.error() == "found unexpected character: '^" + libutf8::to_u8string(static_cast<char32_t>(c + 0x40)) + "'");
            }
            else if(c >= 0x80 && c < 0xA0)
            {
                CATCH_REQUIRE(jt.error() == "found unexpected character: '@" + libutf8::to_u8string(static_cast<char32_t>(c - 0x40)) + "'");
            }
            else
            {
                CATCH_REQUIRE(jt.error() == "found unexpected character: '" + libutf8::to_u8string(c) + "'");
            }
        }
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: unexpected '\\0' in JSON")
    {
        std::string invalid_json;
        invalid_json += '\0';
        libutf8::json_tokens jt(invalid_json);

        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.error() == "found unexpected NULL character");
    }
    CATCH_END_SECTION()

    CATCH_START_SECTION("json_tokens_invalid: unexpected '\\0' in JSON string")
    {
        std::string invalid_json("\"string");
        invalid_json += '\0';
        invalid_json += "with null\"";
        libutf8::json_tokens jt(invalid_json);

        CATCH_REQUIRE(jt.next_token() == libutf8::token_t::TOKEN_ERROR);
        CATCH_REQUIRE(jt.error() == "unexpected NULL character in string");
    }
    CATCH_END_SECTION()
}



// vim: ts=4 sw=4 et
