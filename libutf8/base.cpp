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
#include    "libutf8/base.h"


// libutf8 lib
//
#include    "libutf8/exception.h"


// C++ lib
//
#include    <cctype>
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>



/** \brief Name space of the UTF-8 library.
 *
 * The libutf8 library is used to seamlessly handle UTF-8 strings. It also
 * is used to convert betwee UTF-8, UTF-16, and UTF-32 strings.
 *
 * \todo
 * Implement the UTF-16 functions.
 */
namespace libutf8
{


/** \var constexpr std::size_t MBS_MIN_BUFFER_LENGTH
 * \brief Minimum buffer length to support any UTF-8 characters.
 *
 * When converting a UTF-32 character to UTF-8, it makes use of an output
 * buffer. The size of that output buffer should be at least
 * MBS_MIN_BUFFER_LENGTH to accomodate any UTF-32 character.
 *
 * Note that the size includes space for a null terminator (`'\0'`).
 *
 * The size of your buffer can be smaller as long as the UTF-32 character
 * fits into it, the wctombs() function will not fail.
 */


/** \brief Compute the UTF-8 encoded representation of wc.
 *
 * This function transforms the UTF-32 character \p wc in a
 * UTF-8 encoded series of bytes (called a multi-byte encoded
 * character.) The resulting string is null (`'\0'`) terminated.
 *
 * The \p mb buffer should be at least MBS_MIN_BUFFER_LENGTH bytes.
 * If less space is required, the function does not report a problem,
 * though. This allows to get the total size of a conversion and then
 * do the full conversion to that one buffer without the need to
 * add unnecessary bytes at the end of your destination buffer.
 *
 * \code
 * ...
 * char mb[MBS_MIN_BUFFER_LENGTH];
 *
 * wctombs(mb, big_char, sizeof(mb));
 * ...
 * \endcode
 *
 * The function does not encode invalid characters. When such is
 * passed to the function, the \p mb string is turned in a null
 * terminated string and the function returns 0. We avoid an
 * exception here because that way you can quickly check whether
 * a string of `char32_t` characters is valid or not.
 *
 * \note
 * Unicode defines valid characters only between zero (0) and 0x10FFFF.
 * Therefore this function encodes the character using 1 to 4 bytes plus
 * one for the null terminator.
 *
 * \warning
 * The function does not raise an error if the input \p wc character
 * is considered invalid (a UTF-16 surrogate or larger than 0x10FFFF.)
 * Instead it returns 0 and sets the \p mb string to the empty string.
 *
 * \exception libutf8_logic_exception
 * The function raises this exception if the destination buffer is too
 * small for the conversion. Don't forget that we add a null terminator
 * so if the character needs 3 UTF-8 bytes, we will check for a buffer
 * of at least 4 bytes to consider it valid.
 *
 * \param[out] mb  The output buffer, it will always be null terminated.
 * \param[in] wc  The wide character to convert.
 * \param[in] len  The length of \p mb.
 *
 * \return The number of bytes in mb, not including the null terminator.
 */
int wctombs(char * mb, char32_t wc, size_t len)
{
    auto verify_length = [&len](size_t required_len)
    {
        if(len < required_len)
        {
            throw libutf8_logic_exception("wctombs() called with an output buffer which is too small.");
        }
    };

    if(wc < 0x80)
    {
        verify_length(2);

        /* this will also encode '\0'... */
        mb[0] = static_cast<char>(wc);
        mb[1] = '\0';
        return 1;
    }
    if(wc < 0x800)
    {
        verify_length(3);

        mb[0] = static_cast<char>((wc >> 6) | 0xC0);
        mb[1] = (wc & 0x3F) | 0x80;
        mb[2] = '\0';
        return 2;
    }

    // avoid encoding the UTF-16 surrogate because those code points do not
    // represent characters
    //
    if(wc < 0xD800 || wc > 0xDFFF)
    {
        if(wc < 0x10000)
        {
            verify_length(4);

            mb[0] = static_cast<char>((wc >> 12) | 0xE0);
            mb[1] = ((wc >> 6) & 0x3F) | 0x80;
            mb[2] = (wc & 0x3F) | 0x80;
            mb[3] = '\0';
            return 3;
        }
        if(wc < 0x110000)
        {
            verify_length(5);

            mb[0] = static_cast<char>((wc >> 18) | 0xF0);
            mb[1] = ((wc >> 12) & 0x3F) | 0x80;
            mb[2] = ((wc >> 6) & 0x3F) | 0x80;
            mb[3] = (wc & 0x3F) | 0x80;
            mb[4] = '\0';
            return 4;
        }
    }

    verify_length(1);

    /* an invalid wide character */
    mb[0] = '\0';
    return -1;
}


/** \brief Convert one multi-byte character to a wide character.
 *
 * This function converts UTF-8 bytes from \p mb to one UTF-32
 * wide character and saves the result in \p wc. The function
 * automatically increases the pointer in \p mb and simultaneously
 * decreases the \p len parameter.
 *
 * \p wc holds the resulting wide character, a character between
 * `'\0'` (NUL) and `0x10FFFF` and it returns the number of bytes
 * that were used from \p mb. If a bad character is encountered,
 * then the function returns -1 and the bad sequence of bytes is
 * skipped so only one error will be reported for one bad sequence.
 *
 * Bad characters when converting UTF-8 to wide characters are:
 *
 * \li The stream includes bytes 0x80 to 0xBF without an introducer.
 * \li The stream does not include the right number of 0x80 to 0xBF
 *     bytes after an introducer.
 * \li The input ends too early and cannot accommodate the last
 *     encoded character.
 * \li The codes 0xF8 to 0xFF were found in the input string.
 * \li The resulting \p wc value would be larger than 0x10FFFF.
 * \li The resulting \p wc value represents a UTF-16 surrogate
 *     value (a number between 0xD800 and 0xDFFF).
 *
 * Code points between 0xD800 and 0xDFFF are not valid characters.
 * These represent low and high surrogates in UTF-16 (2 are
 * necessary to encode one character of 17 or more bits.)
 *
 * The function returns 0 and sets \p wc to the NUL character (`U'\0'`)
 * if the \p len parameter is zero (i.e. empty string.)
 *
 * \note
 * The function converts a NUL character (`'\0'`) in the
 * input string as a NUL wide character (`U'\0'`) and returns 1. It
 * does not see the NUL character as the end of the string.
 *
 * \warning
 * The function does not throw on invalid input. It is the responsibility
 * of the caller to do so if necessary. This is useful to very an UTF-8
 * string without having to catch an exception.
 *
 * \param[out] wc  The output wide character variable.
 * \param[in,out] mb  The multi-byte input string pointer, returned at the
 *                    following byte.
 * \param[in,out] len  The number of characters left in mb.
 *
 * \return The number of bytes read or -1 if invalid bytes were found.
 */
int mbstowc(char32_t & wc, char const * & mb, size_t & len)
{
    auto skip = [](char const * & skip_mb, size_t & skip_len)
    {
        for(unsigned char b(0)
            ; skip_len > 0 && (b = *skip_mb, (b >= 0x80 && b <= 0xBF) || b >= 0xF5)
            ; ++skip_mb , --skip_len);
    };

    // default output character is NUL
    //
    wc = U'\0';

    // already done?
    //
    if(len <= 0)
    {
        return 0;
    }

    // we eat one character from the source minimum
    //
    unsigned char c(*mb++);
    --len;

    if(c < 0x80)
    {
        wc = c;
        return 1;
    }

    // invalid stream?
    //
    if((c >= 0x80 && c <= 0xBF) || c >= 0xF5)
    {
        // this is bad UTF-8, skip all the invalid bytes
        //
        skip(mb, len);
        return -1;
    }

    char32_t w(U'\0');
    size_t cnt(0);

    if(c >= 0xF0)
    {
        w = c & 0x07;
        cnt = 3;
    }
    else if(c >= 0xE0)
    {
        w = c & 0x0F;
        cnt = 2;
    }
    else /*if(c >= 0xC0)*/    // always true so we don't have to check
    {
        w = c & 0x1F;
        cnt = 1;
    }

    // enough data in the input? if not, that's an error
    //
    if(len < cnt)
    {
        skip(mb, len);
        return -1;
    }
    len -= cnt;

    for(size_t l(cnt); l > 0; --l, mb++)
    {
        c = *mb;
        if(c < 0x80 || c > 0xBF)
        {
            // we got an invalid sequence!
            // restore whatever is left in len
            //
            len += l;
            return -1;
        }
        w = (w << 6) | (c & 0x3F);
    }

    if(w >= 0x110000
    || (w >= 0x00D800 && w <= 0x00DFFF))
    {
        // character out of range or UTF-16 surrogate
        // it can happen with sequences starting with 0xF7
        //
        return -1;
    }

    wc = w;

    return static_cast<int>(cnt + 1);
}


/** \brief An overload with a non-const string.
 *
 * Since we are passing a reference to the \p mb string, whether it is
 * const or non-const matter to the call. So here we offer a non-const
 * version even though the string doesn't get modified.
 *
 * \param[out] wc  The output wide character variable.
 * \param[in,out] mb  The multi-byte input string pointer, returned at the
 *                    following byte.
 * \param[in,out] len  The number of characters left in mb.
 *
 * \return The number of bytes read or -1 if invalid bytes were found.
 */
int mbstowc(char32_t & wc, char * & mb, size_t & len)
{
    return mbstowc(wc, const_cast<char const * &>(mb), len);
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
