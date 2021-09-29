// Copyright (c) 2000-2021  Made to Order Software Corp.  All Rights Reserved
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

/** \file
 * \brief Implementation of the JSON token parser.
 *
 * This file is the implementation of the JSON token parser.
 *
 * This is primarily an example of how to use the libutf8 library. It is
 * pretty easy to parse JSON tokens since there are only a few. This is
 * expected to be used to read simple structures written using JSON. It
 * is not expected to be a full JSON parser (see our as2js project for
 * such).
 */

// self
//
#include    "libutf8/json_tokens.h"

#include    "libutf8/exception.h"
#include    "libutf8/libutf8.h"


// C++ lib
//
#include    <iostream>
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>



namespace libutf8
{



/** \brief Initialize the JSON tokens object with the specified string.
 *
 * At this time, this class is given one string of the entire JSON to be
 * parsed. The next_token() function gives you the next token until you
 * get the TOKEN_END which marks the end of the input string.
 *
 * \param[in] input  The input to be parsed.
 */
json_tokens::json_tokens(std::string const & input)
    : f_input(input)
    , f_iterator(f_input)
{
}


/** \brief Get one character.
 *
 * This function returns the next character from the input string.
 *
 * If the unget() function was used, then those characters are returned first.
 *
 * When the end of the input string is reached, the function returns L'\0'
 * characters.
 *
 * \return The next character.
 */
char32_t json_tokens::getc()
{
    if(f_unget_pos > 0)
    {
        --f_unget_pos;
        return f_unget[f_unget_pos];
    }

    char32_t c(*f_iterator++);

    if(c == '\r')
    {
        c = *f_iterator;
        if(c == '\n')
        {
            ++f_iterator;
        }
        else
        {
            c = '\n';
        }
    }

    if(c == '\n')
    {
        ++f_line;
        f_column = 1;
    }
    else
    {
        ++f_column;
    }

    return c;
}


/** \brief Restore (unget) one character.
 *
 * In many cases, we need to retrieve yet another character to know whether
 * we reached the end of a token or not (i.e. the end of a number), that
 * extra character needs to be pushed back as it is not part of the token
 * but may be part of the next token.
 *
 * \note
 * We don't simply `--f_iterator` because we transform the `"\\r\\n"`
 * sequence to just `"\\n"` and also increase the `f_line`/`f_column`
 * accordingly. It could be quite complicated to unget properly in those
 * cases.
 *
 * \param[in] c  The character to push back.
 */
void json_tokens::ungetc(char32_t c)
{
    if(f_unget_pos >= std::size(f_unget))
    {
        throw libutf8_exception_overflow("too many ungetc() calls, f_unget buffer is full.");  // LCOV_EXCL_LINE
    }

    f_unget[f_unget_pos] = c;
    ++f_unget_pos;
}


/** \brief Return the line number.
 *
 * This function returns the line number on which the last token was read.
 *
 * \return line number of the last token.
 */
int json_tokens::line() const
{
    return f_last_line;
}


/** \brief Return the column number.
 *
 * This function returns the column number on which the last token was
 * read.
 *
 * \return column number of the last token.
 */
int json_tokens::column() const
{
    return f_last_column;
}


/** \brief Retrieve the next token.
 *
 * This function reads characters and build one token and returns it. If
 * the end of the input string is reached, then the function returns
 * the end token (TOKEN_END). Further calls once the end was reached will
 * always return the TOKEN_END character.
 *
 * The sequence of tokens is not handled by the class. You are expected
 * to create the "yacc". The basics are:
 *
 * \code
 *     start: value
 *     value: number | string | object | array
 *     number: TOKEN_NUMBER
 *     string: TOKEN_STRING
 *     object: TOKEN_OPEN_OBJECT field_list TOKEN_CLOSE_OBJECT
 *     array: TOKEN_OPEN_ARRAY item_list TOKEN_CLOSE_ARRAY
 *     field_list: <empty> | field | field TOKEN_COMMA field_list
 *     field: string TOKEN_COLON value
 *     item_list: <empty> | item | item TOKEN_COMMA item_list
 *     item: value
 * \endcode
 *
 * When the function returns an error (i.e. token_t::TOKEN_ERROR), you can
 * get the error message using the error() function.
 *
 * \return The next token.
 *
 * \sa https://www.json.org/
 */
token_t json_tokens::next_token()
{
    for(;;)
    {
        f_last_line = f_line;
        f_last_column = f_column;
        char32_t c(getc());
        switch(c)
        {
        case U'[':
            return token_t::TOKEN_OPEN_ARRAY;

        case U']':
            return token_t::TOKEN_CLOSE_ARRAY;

        case U'{':
            return token_t::TOKEN_OPEN_OBJECT;

        case U'}':
            return token_t::TOKEN_CLOSE_OBJECT;

        case U'0':
        case U'1':
        case U'2':
        case U'3':
        case U'4':
        case U'5':
        case U'6':
        case U'7':
        case U'8':
        case U'9':
        case U'-':
            {
                double sign(1.0);
                if(c == U'-')
                {
                    sign = -1.0;
                    c = getc();
                    if(c < U'0' || c > U'9')
                    {
                        ungetc(c);
                        f_error = "found unexpected character: ";
                        add_error_character(U'-');
                        return token_t::TOKEN_ERROR;
                    }
                }
                f_number = 0.0;
                if(c >= U'1' && c <= U'9')
                {
                    do
                    {
                        f_number *= 10.0;
                        f_number += static_cast<double>(c - U'0');
                        c = getc();
                    }
                    while(c >= U'0' && c <= U'9');
                }
                else if(c != U'0')
                {
                    throw libutf8_logic_exception("somehow c is U'0' when it should not be possible here.");  // LCOV_EXCL_LINE
                }
                else
                {
                    c = getc();
                }
                if(c == U'.')
                {
                    constexpr double const one_tenth(1.0 / 10.0);
                    double fraction(1.0);
                    for(;;)
                    {
                        c = getc();
                        if(c < U'0' || c > U'9')
                        {
                            break;
                        }
                        fraction *= one_tenth;
                        f_number += (c - U'0') * fraction;
                    }
                    if(fraction >= 1.0)
                    {
                        f_error = "number cannot end with a period (\"1.\" is not valid JSON)";
                        return token_t::TOKEN_ERROR;
                    }
                }
                if(c == U'e' || c == U'E')
                {
                    double exponent_sign(1.0);
                    c = getc();
                    if(c == U'+')
                    {
                        c = getc();
                    }
                    else if(c == U'-')
                    {
                        c = getc();
                        exponent_sign = -1.0;
                    }
                    if(c < U'0' || c > U'9')
                    {
                        f_error = "number exponent must include at least one digit";
                        return token_t::TOKEN_ERROR;
                    }
                    double exponent(0.0);
                    while(c >= U'0' && c <= U'9')
                    {
                        exponent *= 10.0;
                        exponent += c - U'0';
                        c = getc();
                    }
                    f_number *= pow(10.0, exponent * exponent_sign);
                }
                ungetc(c);
                f_number *= sign;
            }
            return token_t::TOKEN_NUMBER;

        case U'"':
            f_string.clear();
            for(;;)
            {
                c = getc();
                if(c == U'"')
                {
                    break;
                }
                if(c == EOS)
                {
                    f_error = "unclosed string";
                    return token_t::TOKEN_ERROR;
                }
                if(c == U'\\')
                {
                    c = getc();
                    switch(c)
                    {
                    case U'\\':
                    case U'"':
                    case U'/':
                        f_string += c;
                        break;

                    case U'b':
                        f_string += '\b';
                        break;

                    case U'f':
                        f_string += '\f';
                        break;

                    case U'n':
                        f_string += '\n';
                        break;

                    case U'r':
                        f_string += '\r';
                        break;

                    case U't':
                        f_string += '\t';
                        break;

                    case U'u':
                        // this is a UTF-16 character, so we need to
                        // handle the 0xD800 - 0xDFFF code points
                        {
                            char32_t u(char16(c));
                            if(u == EOS)
                            {
                                f_error = "invalid unicode character: ";
                                add_error_character(c);
                                return token_t::TOKEN_ERROR;
                            }
                            surrogate_t high_surrogate(is_surrogate(u));

                            switch(high_surrogate)
                            {
                            case surrogate_t::SURROGATE_HIGH: // UTF-16, must be followed by a low
                                {
                                    c = getc();
                                    if(c != '\\')
                                    {
                                        f_error = "expected a low surrogate right after a high surrogate, backslash (\\) mising";
                                        return token_t::TOKEN_ERROR;
                                    }
                                    c = getc();
                                    if(c != 'u')
                                    {
                                        f_error = "expected a low surrogate right after a high surrogate, 'u' missing";
                                        return token_t::TOKEN_ERROR;
                                    }
                                    char32_t l(char16(c));
                                    if(l == EOS)
                                    {
                                        f_error = "invalid unicode character: ";
                                        add_error_character(c);
                                        return token_t::TOKEN_ERROR;
                                    }
                                    surrogate_t low_surrogate(is_surrogate(l));
                                    if(low_surrogate != surrogate_t::SURROGATE_LOW)
                                    {
                                        f_error = "expected a low surrogate right after a high surrogate";
                                        return token_t::TOKEN_ERROR;
                                    }
                                    u = ((u & 0x3FF) << 10) + (l & 0x3FF) + 0x10000;
                                }
                                break;

                            case surrogate_t::SURROGATE_LOW:
                                {
                                    std::stringstream ss;
                                    ss << std::hex << static_cast<int>(u);
                                    f_error = "low surrogate \\u";
                                    f_error += ss.str();
                                    f_error += " found before a high surrogate";
                                }
                                return token_t::TOKEN_ERROR;

                            case surrogate_t::SURROGATE_NO:
                                break;

                            }
                            f_string += to_u8string(u);
                        }
                        break;

                    default:
                        f_error = "unexpected escape character: ";
                        add_error_character(c);
                        return token_t::TOKEN_ERROR;

                    }
                }
                else if(c == U'\0')
                {
                    f_error = "unexpected NULL character in string";
                    return token_t::TOKEN_ERROR;
                }
                else
                {
                    f_string += to_u8string(c);
                }
            }
            return token_t::TOKEN_STRING;

        case U',':
            return token_t::TOKEN_COMMA;

        case U':':
            return token_t::TOKEN_COLON;

        case U't':
            c = getc();
            if(c == U'r')
            {
                c = getc();
                if(c == U'u')
                {
                    c = getc();
                    if(c == U'e')
                    {
                        return token_t::TOKEN_TRUE;
                    }
                    ungetc(c);
                    ungetc(U'u');
                }
                else
                {
                    ungetc(c);
                }
                ungetc(U'r');
            }
            else
            {
                ungetc(c);
            }
            f_error = "found unexpected character: ";
            add_error_character(U't');
            return token_t::TOKEN_ERROR;

        case 'f':
            c = getc();
            if(c == U'a')
            {
                c = getc();
                if(c == U'l')
                {
                    c = getc();
                    if(c == U's')
                    {
                        c = getc();
                        if(c == U'e')
                        {
                            return token_t::TOKEN_FALSE;
                        }
                        ungetc(c);
                        ungetc(U's');
                    }
                    else
                    {
                        ungetc(c);
                    }
                    ungetc(U'l');
                }
                else
                {
                    ungetc(c);
                }
                ungetc(U'a');
            }
            else
            {
                ungetc(c);
            }
            f_error = "found unexpected character: ";
            add_error_character(U'f');
            return token_t::TOKEN_ERROR;

        case U'n':
            c = getc();
            if(c == U'u')
            {
                c = getc();
                if(c == U'l')
                {
                    c = getc();
                    if(c == U'l')
                    {
                        return token_t::TOKEN_NULL;
                    }
                    ungetc(c);
                    ungetc(U'l');
                }
                else
                {
                    ungetc(c);
                }
                ungetc(U'u');
            }
            else
            {
                ungetc(c);
            }
            f_error = "found unexpected character: ";
            add_error_character(U'n');
            return token_t::TOKEN_ERROR;

        case U' ':
        case U'\t':
        case U'\n':  // this includes the U'\r' (see getc())
            // ignore blanks between tokens
            break;

        case U'\0':
            f_error = "found unexpected NULL character";
            return token_t::TOKEN_ERROR;

        case EOS:
            return token_t::TOKEN_END;

        default:
            f_error = "found unexpected character: ";
            add_error_character(c);
            return token_t::TOKEN_ERROR;

        }
    }
}


char32_t json_tokens::char16(char32_t & c)
{
    char32_t u(0);
    for(int i(0); i < 4; ++i)
    {
        u <<= 4;
        c = getc();
        if(c >= U'0' && c <= U'9')
        {
            u += c - U'0';
        }
        else if((c >= U'a' && c <= U'f')
             || (c >= U'A' && c <= U'F'))
        {
            u += (c | 0x20) - (U'a' - 10);
        }
        else
        {
            return EOS;
        }
    }
    return u;
}


void json_tokens::add_error_character(char32_t c)
{
    f_error += '\'';
    if(c < 0x20)
    {
        f_error += '^';
        f_error += c + 0x40;
    }
    else if(c >= 0x80 && c < 0xA0)
    {
        f_error += '@';
        f_error += c - 0x40;
    }
    else if(c == EOS)
    {
        f_error += "EOS";
    }
    else
    {
        f_error += to_u8string(c);
    }
    f_error += '\'';
}


/** \brief Return the number token.
 *
 * When the last call to the get_token() function returned the
 * token_t::TOKEN_NUMBER, then the resulting number is saved in
 * a field which can be retrieved with this function.
 *
 * Note that numbers in JSON are always represented by a double.
 * A double can perfectly hold integers, though.
 *
 * \return The last number read by get_token().
 */
double json_tokens::number() const
{
    return f_number;
}


/** \brief Return the string token.
 *
 * When the get_token() function returns the token_t::TOKEN_STRING value,
 * then the string can be retrieved with this function.
 *
 * \return The last string token read.
 */
std::string const & json_tokens::string() const
{
    return f_string;
}


/** \brief Return the last error message.
 *
 * This function returns the last error message. This represents the
 * error that last occurred when the next_token() function returned
 * token_t::TOKEN_ERROR.
 *
 * \return The last error message.
 */
std::string const & json_tokens::error() const
{
    return f_error;
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
