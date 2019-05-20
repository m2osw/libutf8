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
#include "libutf8/libutf8.h"

#include <cctype>

/** \brief Name space of the UTF-8 library.
 *
 * The library to convert UTF-8 strings to UCS-4 (Unices) or UTF-16 strings
 * (MS-Windows) and vice versa.
 */
namespace libutf8
{


/** \brief Compute the UTF-8 encoded representation of wc.
 *
 * This function transforms the UCS-4 (UTF-32) character wc in a
 * UTF-8 encoded series of bytes (called a multi-byte encoded
 * character.)
 *
 * The \p mb buffer MUST be at least 7 bytes to be safe. In general it
 * is a good idea to use 8 bytes.
 *
 * \code
 * ...
 * char mb[8];
 *
 * wctombs(mb, big_char);
 * ...
 * \endcode
 *
 * \note
 * Unicode defines characters to be between zero (0) and 0x10FFFF. Therefore
 * this function should encode the character using 1 to 4 bytes. However,
 * if larger characters were used we still support a lossless encoding.
 * Note however that MS-Windows only supports 21 bits (UTF-16).
 *
 * \note
 * This function is internal because \p mb is not considered safe.
 *
 * \param[out] mb  The output buffer, it will always be null terminated.
 * \param[in] wc  The wide character to convert.
 *
 * \return The number of bytes in mb, not including the null terminator.
 */
int wctombs(char *mb, uint32_t wc)
{
    if(wc < 0x80)
    {
        /* this will also encode '\0'... */
        mb[0] = static_cast<char>(wc);
        mb[1] = '\0';
        return 1;
    }
    if(wc < 0x800)
    {
        mb[0] = static_cast<char>((wc >> 6) | 0xC0);
        mb[1] = (wc & 0x3F) | 0x80;
        mb[2] = '\0';
        return 2;
    }
    if(wc < 0x10000)
    {
        mb[0] = static_cast<char>((wc >> 12) | 0xE0);
        mb[1] = ((wc >> 6) & 0x3F) | 0x80;
        mb[2] = (wc & 0x3F) | 0x80;
        mb[3] = '\0';
        return 3;
    }
    if(wc < 0x200000)
    {
        mb[0] = static_cast<char>((wc >> 18) | 0xF0);
        mb[1] = ((wc >> 12) & 0x3F) | 0x80;
        mb[2] = ((wc >> 6) & 0x3F) | 0x80;
        mb[3] = (wc & 0x3F) | 0x80;
        mb[4] = '\0';
        return 4;
    }
    if(wc < 0x4000000)
    {
        mb[0] = (wc >> 24) | 0xF8;
        mb[1] = ((wc >> 18) & 0x3F) | 0x80;
        mb[2] = ((wc >> 12) & 0x3F) | 0x80;
        mb[3] = ((wc >> 6) & 0x3F) | 0x80;
        mb[4] = (wc & 0x3F) | 0x80;
        mb[5] = '\0';
        return 5;
    }
    if(wc > 0)    // <=> (unsigned long) wc < 0x80000000
    {
        mb[0] = (wc >> 30) | 0xFC;
        mb[1] = ((wc >> 24) & 0x3F) | 0x80;
        mb[2] = ((wc >> 18) & 0x3F) | 0x80;
        mb[3] = ((wc >> 12) & 0x3F) | 0x80;
        mb[4] = ((wc >> 6) & 0x3F) | 0x80;
        mb[5] = (wc & 0x3F) | 0x80;
        mb[6] = '\0';
        return 6;
    }

    /* an invalid wide character (negative!) simply not encoded */
    mb[0] = '\0';
    return 0;
}


/** \brief Convert one multi-byte character to a wide character.
 *
 * This function converts UTF-8 bytes from \p mb to one UCS-4
 * (or UTF-32) wide character. The function automatically
 * increases the pointer in \p mb and simultaneously decreases
 * \p len.
 *
 * \p wc holds the result. If a bad character is encountered, then
 * the function returns -1.
 *
 * Bad characters when converting UTF-8 to wide characters are:
 *
 * \li The stream includes bytes 0x80 to 0xBF without an introducer.
 * \li The stream does not include the right number of 0x80 to 0xBF
 *     bytes after an introducer
 * \li The input ends too early and cannot accommodate the last
 *     encoded character.
 *
 * Note that character code points between 0xD800 and 0xDFFF are
 * not valid characters. These represent low and high surrogates in
 * UTF-16 (2 are necessary to encode one character of 17 or more bits.)
 * At this point this function does not generate an error on those
 * values though.
 *
 * \param[out] wc  The output wide character variable.
 * \param[in,out] mb  The multi-byte input string pointer, returned at the
 *                    following byte.
 * \param[in,out] len  The number of characters left in mb.
 *
 * \return The number of bytes read or -1 if invalid bytes were found.
 */
int mbstowc(uint32_t& wc, const char *& mb, size_t& len)
{
    // define a default output character of NUL
    wc = L'\0';

    // done?
    if(len <= 0)
    {
        return 0;
    }

    // we eat one character from the source minimum
    unsigned char c(*mb++);
    len--;

    if(c < 0x80)
    {
        wc = c;
        return 1;
    }

    // invalid stream?
    if((c >= 0x80 && c <= 0xBF) || c == 0xFE || c == 0xFF)
    {
        // TBD -- should we throw instead?
        // this is bad UTF-8, skip all the invalid bytes
        while(len > 0 && (c = *mb, (c >= 0x80 && c < 0xBF) || c == 0xFE || c == 0xFF))
        {
            mb++;
            len--;
        }
        return -1;
    }

    // use a uint32_t because some wchar_t are not wide
    // enough; generate an error later if that's the case
    // (we are trying to go to UCS-4, not UTF-16, but MS-Windows
    // really only supports UCS-2.)
    uint32_t w(L'\0');
    size_t cnt;

    // note that in current versions of UTF-8 0xFC and 0xF8
    // are not considered valid because they accept a maximum
    // of 20 bites instead of 31
    if(c >= 0xFC)
    {
        w = c & 0x01;
        cnt = 5;
    }
    else if(c >= 0xF8)
    {
        w = c & 0x03;
        cnt = 4;
    }
    else if(c >= 0xF0)
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
    if(len < cnt)
    {
        while(len > 0 && (c = *mb, c >= 0x80 && c <= 0xBF))
        {
            len--;
            mb++;
        }
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
            len += l;
            return -1;
        }
        w = (w << 6) | (c & 0x3F);
    }

    wc = w;

    return static_cast<int>(cnt + 1);
}


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
std::string wcstombs(const std::wstring& str)
{
    char mb[8];
    std::string result;
    for(const wchar_t *s(str.c_str()); *s != L'\0'; ++s)
    {
#ifdef MO_WINDOWS
        // newer versions of MS-Windows actually support UTF-16
        uint32_t wc(static_cast<unsigned short>(*s));
        if((wc & 0xF800) == 0xD800)
        {
            // UTF-16 encoded, verify that the two surrogates are correct
            if((wc & 0x0400) != 0 || s[1] == L'\0' || (s[1] & 0xFC00) != 0xDC00)
            {
                // invalid surrogate character, skip at this point
                continue;
            }
            ++s;
            wc = ((wc << 10) + static_cast<unsigned short>(*s)) + (0x10000 - (0xD800 << 10) - 0xDC00);
        }
        wctombs(mb, wc);
#else
        wctombs(mb, *s);
#endif
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
std::wstring mbstowcs(const std::string& str)
{
    std::wstring result;
    size_t len(str.length());
    for(const char *mb(str.c_str()); len > 0; )
    {
        uint32_t wc;
        if(mbstowc(wc, mb, len) > 0)
        {
#ifdef MO_WINDOWS
            // newer versions of MS-Windows actually support UTF-16
            if(wc < 0x00110000) // characters that are too large (over 21 bits limit) are ignored
            {
                if(wc >= 0x10000)
                {
                    result += static_cast<wchar_t>((wc >> 10) + (0xD800 - (0x10000 >> 10)));
                    result += static_cast<wchar_t>(((wc & 0x03FF) + 0xDC00));
                }
                else
                {
                    result += static_cast<wchar_t>(wc);
                }
            }
#else
            result += static_cast<wchar_t>(wc);
#endif
        }
    }
    return result;
}


/** \brief Determine the length of the UTF-8 string.
 *
 * This function counts the number of characters in the specified UTF-8
 * string. It is optimized for speed for the UTF-8 encoding.
 *
 * The function currently ignores 0xFE and 0xFF bytes. Also, it does not
 * verifies whether the string is valid UTF-8.
 *
 * \param[in] str  The string to compute the length in characters of.
 *
 * \return The number of characters in the UTF-8 string.
 */
size_t mbslen(const std::string& str)
{
    size_t result(0);
    for(const char *s(str.c_str()); *s != '\0'; ++s)
    {
        unsigned char c(*s);
        if((c < 0x80 || c > 0xBF) && c != 0xFE && c != 0xFF)
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
 * Note that contrary to most algorith, the function uses uppercase characters
 * because MS-Windows uses such to compare filenames against each others.
 *
 * Also, the MS-Windows comparison algorithm handles 16 bit characters only
 * for filenames. This means characters that are over 16 bits are not tested
 * as you would otherwise expect. Now to be 100% correct, we'd need their
 * exact toupper() tables. I will not go to that extend though.
 *
 * Finally, this function throws if any one character in the input is not
 * valid. This means only characters that fit in 20 bits are supported and
 * the UTF-8 stream must be 100% valid.
 *
 * \warning
 * This code is certainly not a 1 to 1 match to the CompareStringOrdinal()
 * function which is the one used by the Kernel to compare filenames.
 * However, I think that the only difference is the toupper() call. The
 * one under Microsoft will most certainly have a somewhat different
 * algorithm to generate uppercase characters. Speaking of which that
 * means the compare will change with time as Unicode makes changes to
 * their uppercase algorithm. Note that I did look at the assembly
 * language version of that function so I can tell that I'm doing the
 * same thing, also I'll have a test to verify that as closely as
 * possible under MS-Windows, but that won't really compare whether
 * the Linux version functions the same way.
 *
 * \param[in] lhs  The left handside string to compare.
 * \param[in] rhs  The right handside string to compare.
 *
 * \return -1 if lhs < rhs, 0 if lhs == rhs, and 1 if lhs > rhs
 */
int mbscasecmp(const std::string& lhs, const std::string& rhs)
{
    size_t llen(lhs.length());
    const char *lmb(lhs.c_str());

    size_t rlen(rhs.length());
    const char *rmb(rhs.c_str());

    uint32_t lwc_upper(0);
    uint32_t rwc_upper(0);

    while((llen > 0 || lwc_upper != 0) && (rlen > 0 || rwc_upper != 0))
    {
        uint32_t lwc;
        uint32_t rwc;
        int r;

        if(lwc_upper != 0)
        {
            lwc = lwc_upper;
            lwc_upper = 0;
        }
        else
        {
            r = mbstowc(lwc, lmb, llen);
            if(r < 0)
            {
                throw libutf8_exception_decoding("the lhs string includes invalid UTF-8 bytes");
            }
            if(lwc >= 0x00110000)
            {
                throw libutf8_exception_decoding("characters over 20 bits, found in lhs, are not supported by MS-Windows");
            }
            if(lwc >= 0x10000)
            {
                lwc_upper = (lwc & 0x03FF) + 0xDC00;
                lwc = (lwc >> 10) + (0xD800 - (0x10000 >> 10));
            }
        }

        if(rwc_upper != 0)
        {
            rwc = rwc_upper;
            rwc_upper = 0;
        }
        else
        {
            r = mbstowc(rwc, rmb, rlen);
            if(r < 0)
            {
                throw libutf8_exception_decoding("the rhs string includes invalid UTF-8 bytes");
            }
            if(rwc >= 0x00110000)
            {
                throw libutf8_exception_decoding("characters over 20 bits, found in rhs, are not supported by MS-Windows");
            }
            if(rwc >= 0x10000)
            {
                rwc_upper = (rwc & 0x03FF) + 0xDC00;
                rwc = (rwc >> 10) + (0xD800 - (0x10000 >> 10));
            }
        }

        // if equal as is, avoid the uppercase test
        if(lwc != rwc)
        {
            uint32_t lu = std::toupper(lwc);
            uint32_t ru = std::toupper(rwc);
            if(lu != ru)
            {
                // not equal, we return comparing uppercase characters!
                return lu < ru ? -1 : 1;
            }
        }
    }

    // check which end of string we reached
    bool le(llen == 0 && lwc_upper == 0);
    bool re(rlen == 0 && rwc_upper == 0);
    return le && re ? 0 : (le ? -1 : 1);
}

} // libutf8 namespace
// vim: ts=4 sw=4 et
