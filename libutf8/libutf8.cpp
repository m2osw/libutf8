/*    libutf8.cpp -- convert between wchar_t and UTF-8 encodings
 *    Copyright (C) 2000-2015  Made to Order Software Corporation
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

/** \file
 * \brief Implementation of the UTF-8 functions.
 *
 * This file is the implementation of the UTF-8 functions of the libutf8
 * library. It simply is a set of functions to convert between different
 * character sets in a lossless manner. At this point it supports UTF-8,
 * UCS-4, and UTF-16 formats.
 *
 * Contrary to many of the system functions, these functions do not take
 * anything from the system in account (the locale can be anything, it does
 * not change the exact behavior of these functions.)
 *
 * Also similar functionality is found on Unices and MS-Windows, it was
 * simpler to just implement these few functions than to try to have a
 * converter that is sure not to use a locale and this way we can use
 * standard strings (std::string and std::wstring) instead of having to
 * call C functions.
 */

// self
//
#include "libutf8/libutf8.h"

// libutf8 lib
//
#include "libutf8/base.h"
#include "libutf8/exception.h"

// C++ lib
//
#include <cwctype>



/** \brief Name space of the UTF-8 library.
 *
 * The library to convert UTF-8 strings to UCS-4 (Unices) or UTF-16 strings
 * (MS-Windows) and vice versa.
 */
namespace libutf8
{




/** \brief Check whether \p str starts with a BOM or not.
 *
 * This function checks the first few bytes of the buffer pointed by \p str
 * to see whether it starts with a BOM.
 *
 * We support 5 different types:
 *
 * * UTF-8
 * * UTF-16 in Little Endian or Big Endian
 * * UTF-32 in Little Endian or Big Endian
 *
 * If none match, then the function returns bom_t::BOM_NONE.
 *
 * \param[in] str  The buffer to check.
 * \param[in] len  The length of the buffer.
 *
 * \return One of the bom_t enumeration types.
 */
bom_t start_with_bom(char const * str, size_t len)
{
    if(len < 2)
    {
        // buffer too small for any BOM
        //
        return bom_t::BOM_NONE;
    }

    unsigned char const * s(reinterpret_cast<unsigned char const *>(str));

    if(s[0] == 0xFF
    && s[1] == 0xFE)
    {
        if(len < 4
        || s[2] != 0x00
        || s[3] != 0x00)
        {
            return bom_t::BOM_UTF16_LE;
        }
    }

    if(s[0] == 0xFE
    && s[1] == 0xFF)
    {
        if(len < 4
        || s[2] != 0x00
        || s[3] != 0x00)
        {
            return bom_t::BOM_UTF16_BE;
        }
    }

    if(len < 3)
    {
        return bom_t::BOM_NONE;
    }

    if(s[0] == 0xEF
    && s[1] == 0xBB
    && s[2] == 0xBF)
    {
        return bom_t::BOM_UTF8;
    }

    if(len < 4)
    {
        return bom_t::BOM_NONE;
    }

    if(s[0] == 0xFF
    && s[1] == 0xFE
    && s[2] == 0x00
    && s[3] == 0x00)
    {
        return bom_t::BOM_UTF32_LE;
    }

    if(s[0] == 0x00
    && s[1] == 0x00
    && s[2] == 0xFE
    && s[3] == 0xFF)
    {
        return bom_t::BOM_UTF32_BE;
    }

    return bom_t::BOM_NONE;
}


/** \brief Converts a UTF-32 string to a UTF-8 string.
 *
 * This function converts a UTF-32 character string (char32_t) to a
 * UTF-8 string.
 *
 * \note
 * The input string may include '\0' characters.
 *
 * \exception libutf8_exception_encoding
 * The input character must be a valid UTF-32 character or this exception
 * gets raised.
 *
 * \param[in] str  The wide character string to convert to UTF-8.
 *
 * \return The converted string.
 */
std::string to_u8string(std::u32string const & str)
{
    // TODO: calculate resulting string size and preallocate buffer (reserve)
    //
    std::string result;

    char mb[MBS_MIN_BUFFER_LENGTH];
    std::u32string::size_type const max(str.length());
    std::u32string::value_type const * s(str.c_str());
    for(std::u32string::size_type idx(0); idx < max; ++idx)
    {
        std::u32string::value_type const wc(s[idx]);
        if(wc < 0x80)
        {
            // using the `mb` string below would not work for '\0'
            // (i.e. mb would look like an empty string)
            //
            // and since all code bytes below 0x80 can be copied as
            // is we do that here (much faster 99% of the time!)
            //
            result += static_cast<std::string::value_type>(wc);
        }
        else
        {
            if(wctombs(mb, wc, sizeof(mb)) < 0)
            {
                throw libutf8_exception_encoding(
                          "to_u8string(u32string): the input wide character with code "
                        + std::to_string(static_cast<std::uint32_t>(wc))
                        + " is not a valid UTF-32 character.");
            }
            result += mb;
        }
    }

    return result;
}


/** \brief Converts a UTF-16 string to a UTF-8 string.
 *
 * This function converts a UTF-16 string (char16_t) to a
 * UTF-8 string.
 *
 * \note
 * The input string may include '\0' characters.
 *
 * \exception libutf8_exception_decoding
 * The input string must be a valid UTF-16 string or this exception
 * gets raised.
 *
 * \exception libutf8_exception_encoding
 * This exception should not occur since all UTF-16 characters are supported
 * in UTF-8.
 *
 * \param[in] str  The wide character string to convert to UTF-8.
 *
 * \return The converted string.
 */
std::string to_u8string(std::u16string const & str)
{
    // TODO: calculate resulting string size and preallocate buffer (reserve)
    //
    std::string result;

    char mb[MBS_MIN_BUFFER_LENGTH];
    std::u16string::size_type const max(str.length());
    std::u16string::value_type const * s(str.c_str());
    for(std::u32string::size_type idx(0); idx < max; ++idx)
    {
        char32_t wc(static_cast<char32_t>(s[idx]));
        if(wc < 0x80)
        {
            // using the `mb` string below would not work for '\0'
            // (i.e. mb would look like an empty string)
            //
            // and since all code bytes below 0x80 can be copied as
            // is we do that here (much faster 99% of the time!)
            //
            result += static_cast<std::string::value_type>(wc);
        }
        else
        {
            // convert the UTF-16 character in a UTF-32 character
            //
            if((wc & 0xFFFFF800) == 0xD800)
            {
                // large character, verify that the two surrogates are correct
                //
                if((wc & 0x0400) != 0)
                {
                    // 0xDC00 to 0xDFFF; introducer missing
                    //
                    throw libutf8_exception_decoding("to_u8string(): found a high UTF-16 surrogate without the low surrogate.");
                }
                if(idx + 1 >= max)
                {
                    // must be followed by a code between 0xDC00 and 0xDFFF
                    //
                    throw libutf8_exception_decoding("to_u8string(): the high UTF-16 surrogate is not followed by the low surrogate.");
                }
                if((s[idx + 1] & 0xFC00) != 0xDC00)
                {
                    if((s[idx + 1] & 0xFC00) != 0xD800)
                    {
                        throw libutf8_exception_decoding("to_u8string(): found two high UTF-16 surrogates in a row.");
                    }
                    else
                    {
                        throw libutf8_exception_decoding("to_u8string(): found a high UTF-16 surrogate without a low surrogate afterward.");
                    }
                }

                ++idx;
                wc = ((wc << 10)
                   + static_cast<char32_t>(s[idx]))
                   + (static_cast<char32_t>(0x10000)
                   - (static_cast<char32_t>(0xD800) << 10)
                   - static_cast<char32_t>(0xDC00));
            }

            if(wctombs(mb, wc, sizeof(mb)) < 0)
            {
                // this should not happen since all UTF-16 characters are
                // considered valid when surrogates are valid
                //
                throw libutf8_exception_encoding("to_u8string(u16string): the input wide character is not a valid UTF-32 character."); // LCOV_EXCL_LINE
            }
            result += mb;
        }
    }

    return result;
}


/** \brief Converts a wide character to a UTF-8 string.
 *
 * This function converts a wide character (char32_t) to a
 * UTF-8 std::string.
 *
 * \warning
 * The character U'\0' does not get added to the result. In that
 * situation the function returns an empty string.
 *
 * \exception libutf8_exception_encoding
 * The input character must be a valid UTF-32 character or this exception
 * gets raised.
 *
 * \param[in] wc  The wide character to convert to UTF-8.
 *
 * \return The converted string.
 */
std::string to_u8string(char32_t wc)
{
    // TODO: calculate resulting string size and preallocate buffer (reserve)
    //
    std::string result;

    if(wc == U'\0')
    {
        // using the `mb` string would not work for '\0'
        //
        result += '\0';
    }
    else
    {
        char mb[MBS_MIN_BUFFER_LENGTH];
        if(wctombs(mb, wc, sizeof(mb)) < 0)
        {
            throw libutf8_exception_encoding("to_u8string(char32_t): the input wide character is not a valid UTF-32 character.");
        }
        result += mb;
    }

    return result;
}


/** \brief Transform a UTF-8 string to a wide character string.
 *
 * This function transforms the specified string, \p str, from the
 * UTF-8 encoding to the wchar_t encoding, which is supposed to
 * be UCS-4 / UTF-32 under Unices and UTF-16 under Microsoft Windows.
 *
 * Note that UTF-16 is limited to 20 bits, which UTF-8 is supposed to
 * be limited too as well, although we accept up to 31 bits. This means
 * the conversion under Microsoft Windows is not the same as under
 * Unices.
 *
 * \param[in] str  The string to convert to a wide string.
 *
 * \return A wide string which is a representation of the UTF-8 input string.
 */
std::u32string to_u32string(std::string const & str)
{
    std::u32string result;
    result.reserve(u8length(str));  // avoid realloc(), in some cases this ends up being a little slower, with larger strings, much faster

    size_t len(str.length());
    for(std::string::value_type const * mb(str.c_str()); len > 0; )
    {
        char32_t wc;
        if(mbstowc(wc, mb, len) < 0)
        {
            throw libutf8_exception_decoding("to_u16string(): a UTF-8 character could not be extracted.");
        }

        result += wc;
    }

    return result;
}


/** \brief Transform a UTF-8 string to a UTF-16 character string.
 *
 * This function transforms the specified string, \p str, from the
 * UTF-8 encoding to the UTF-16 encoding.
 *
 * \param[in] str  The string to convert to a UTF-16 string.
 *
 * \return A wide string which is a representation of the UTF-8 input string.
 */
std::u16string to_u16string(std::string const & str)
{
    std::u16string result;
    result.reserve(u8length(str));  // avoid realloc(), works in most cases, but really we need a u8length() if converted to u16 characters

    std::string::size_type len(str.length());
    for(std::string::value_type const * mb(str.c_str()); len > 0; )
    {
        char32_t wc;
        if(mbstowc(wc, mb, len) < 0)
        {
            throw libutf8_exception_decoding("to_u16string(): a UTF-8 character could not be extracted.");
        }

        if(wc >= 0x10000)
        {
            result += static_cast<std::u16string::value_type>((wc >> 10) + (0xD800 - (0x10000 >> 10)));
            result += static_cast<std::u16string::value_type>(((wc & 0x03FF) + 0xDC00));
        }
        else
        {
            result += static_cast<std::u16string::value_type>(wc);
        }
    }

    return result;
}


/** \brief Determine the length of the UTF-8 string.
 *
 * This function counts the number of characters in the specified UTF-8
 * string. It is optimized for speed for the UTF-8 encoding.
 *
 * \note
 * The function currently ignores 0xF8 to 0xFF bytes even though those are
 * not valid in a UTF-8 string. Similarly, it does not check whether the
 * sequence represents a character more than 0x10FFFF or a surrogate.
 * That being said, it works beautifully for valid UTF-8 strings.
 *
 * \param[in] str  The string to compute the length in characters of.
 *
 * \return The number of characters in the UTF-8 string.
 */
size_t u8length(std::string const & str)
{
    size_t result(0);
    for(std::string::value_type const *s(str.c_str()); *s != '\0'; ++s)
    {
        unsigned char c(*s);
        if((c < 0x80 || c > 0xBF) && c < 0xF8)
        {
            ++result;
        }
    }
    return result;
}


/** \brief Compare lhs against rhs in case insensitive manner.
 *
 * This function compares two UTF-8 strings against each others and return
 * the order in which they are defined.
 *
 * As expected in Unicode, we use lowercase characters. However, we convert
 * the characters one at a time. This means certain sequences will not be
 * compared properly in a full locale manner. If such is required, please
 * convert the strings to `std::u32string` and then use a collate function
 * that works against UTF-32 characters.
 *
 * \exception libutf8_exception_decoding
 * This function raises the decoding exception if one of the input strings
 * includes an invalid UTF-8 sequence of characters.
 *
 * \param[in] lhs  The left handside string to compare.
 * \param[in] rhs  The right handside string to compare.
 *
 * \return -1 if lhs < rhs, 0 if lhs == rhs, and 1 if lhs > rhs
 */
int u8casecmp(std::string const & lhs, std::string const & rhs)
{
    std::string::size_type llen(lhs.length());
    std::string::value_type const * lmb(lhs.c_str());

    std::string::size_type rlen(rhs.length());
    std::string::value_type const * rmb(rhs.c_str());

    while(llen > 0 && rlen > 0)
    {
        char32_t lwc;
        if(mbstowc(lwc, lmb, llen) < 0)
        {
            throw libutf8_exception_decoding("u8casecmp(): the lhs string includes invalid UTF-8 bytes");
        }

        char32_t rwc;
        if(mbstowc(rwc, rmb, rlen) < 0)
        {
            throw libutf8_exception_decoding("u8casecmp(): the rhs string includes invalid UTF-8 bytes");
        }

        // if equal as is, avoid the lowercase test
        //
        if(lwc != rwc)
        {
            char32_t const ll = std::towlower(lwc);
            char32_t const rl = std::towlower(rwc);
            if(ll != rl)
            {
                // not equal, we return comparing lowercase characters!
                //
                return ll < rl ? -1 : 1;
            }
        }
    }

    // check which end of string we reached
    //
    return llen == 0 && rlen == 0
                ? 0
                : (llen == 0 ? -1 : 1);
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
