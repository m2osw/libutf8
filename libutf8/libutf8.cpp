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

// libutf8 lib
//
#include "libutf8/exception.h"
#include "libutf8/libutf8.h"
#include "libutf8/libutf8base.h"

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




/** \brief Converts a wide string to a UTF-8 string.
 *
 * This function converts a wide character string (wchar_t) to a
 * UTF-8 string. Note that the wchar_t is 16 bits under
 * MS-Windows and represents UTF-16. This function takes that
 * in account.
 *
 * The resulting UTF-8 string is always 100% exact.
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

    char mb[8];
    for(char32_t const *s(str.c_str()); *s != L'\0'; ++s)
    {
//#ifdef MO_WINDOWS
//        // newer versions of MS-Windows actually support UTF-16
//        char32_t wc(static_cast<unsigned short>(*s));
//        if((wc & 0xF800) == 0xD800)
//        {
//            // UTF-16 encoded, verify that the two surrogates are correct
//            if((wc & 0x0400) != 0 || s[1] == L'\0' || (s[1] & 0xFC00) != 0xDC00)
//            {
//                // invalid surrogate character, skip at this point
//                continue;
//            }
//            ++s;
//            wc = ((wc << 10) + static_cast<unsigned short>(*s)) + (0x10000 - (0xD800 << 10) - 0xDC00);
//        }
//        wctombs(mb, wc);
//#else
        wctombs(mb, *s, sizeof(mb));
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
    for(char const * mb(str.c_str()); len > 0; )
    {
        char32_t wc;
        if(mbstowc(wc, mb, len) > 0)
        {
//#ifdef MO_WINDOWS
//            // newer versions of MS-Windows actually support UTF-16
//            if(wc < 0x00110000) // characters that are too large (over 21 bits limit) are ignored
//            {
//                if(wc >= 0x10000)
//                {
//                    result += static_cast<wchar_t>((wc >> 10) + (0xD800 - (0x10000 >> 10)));
//                    result += static_cast<wchar_t>(((wc & 0x03FF) + 0xDC00));
//                }
//                else
//                {
//                    result += static_cast<wchar_t>(wc);
//                }
//            }
//#else
            result += wc;
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
    for(const char *s(str.c_str()); *s != '\0'; ++s)
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
    size_t llen(lhs.length());
    char const * lmb(lhs.c_str());

    size_t rlen(rhs.length());
    char const * rmb(rhs.c_str());

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
