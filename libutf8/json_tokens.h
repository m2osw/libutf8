// Copyright (c) 2000-2022  Made to Order Software Corp.  All Rights Reserved
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
// 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.
#pragma once

/** \file
 * \brief The declarations of the JSON tokens class.
 *
 * This file is the declarations of the JSON tokens class one can use to
 * interpret the contents of a JSON file.
 *
 * The idea of this simple parser is to (1) show how one can use the
 * libutf8 library and (2) give you the ability to parse a simple JSON
 * structure.
 */

// self
//
#include    <libutf8/iterator.h>


// C++ lib
//
#include    <cmath>
#include    <string>



namespace libutf8
{


enum class token_t
{
    TOKEN_END,
    TOKEN_ERROR,
    TOKEN_OPEN_ARRAY,
    TOKEN_CLOSE_ARRAY,
    TOKEN_OPEN_OBJECT,
    TOKEN_CLOSE_OBJECT,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NULL,
};


class json_tokens
{
public:
                        json_tokens(std::string const & input);

    int                 line() const;
    int                 column() const;
    token_t             next_token();
    double              number() const;
    std::string const & string() const;
    std::string const & error() const;

private:
    char32_t            getc();
    void                ungetc(char32_t c);
    char32_t            char16(char32_t & c);
    void                add_error_character(char32_t c);

    std::string         f_input = std::string();
    utf8_iterator       f_iterator; // initialize in the constructor
    char32_t            f_unget[16];
    std::size_t         f_unget_pos = 0;
    std::uint32_t       f_line = 1;
    std::uint32_t       f_last_line = 0;
    std::uint32_t       f_column = 1;
    std::uint32_t       f_last_column = 0;
    double              f_number = 0.0;
    std::string         f_string = std::string();
    std::string         f_error = std::string();
};


} // libutf8 namespace
// vim: ts=4 sw=4 et
