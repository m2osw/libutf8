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
#include    "libutf8/libutf8.h"

#include    "libutf8/base.h"
#include    "libutf8/exception.h"


// snapdev
//
#include    <snapdev/hexadecimal_string.h>


// C++
//
#include    <cwctype>


// last include
//
#include    <snapdev/poison.h>



/** \brief Name space of the UTF-8 library.
 *
 * The library to convert UTF-8 strings to UCS-4 (Unices) or UTF-16 strings
 * (MS-Windows) and vice versa.
 */
namespace libutf8
{




/** \brief Validate an ASCII characters.
 *
 * This function checks whether a character is considered an ASCII character
 * or not.
 *
 * \param[in] c  The string to be validated.
 * \param[in] ctrl  Set to true to also accept controls.
 *
 * \return true if the string is empty, nullptr, or only includes ASCII
 *         characters.
 */
bool is_valid_ascii(char c, bool ctrl)
{
    if(ctrl)
    {
        return static_cast<unsigned char>(c) < 0x80;
    }

    return static_cast<unsigned char>(c) > 0x1F
        && static_cast<unsigned char>(c) < 0x7F;
}


/** \brief Validate a string as ASCII characters.
 *
 * This function checks that all the characters in a string are comprised
 * only of ACSII characters (code bytes 0x01 to 0x7F, since 0x00 is viewed
 * as the end of the string).
 *
 * When the ctrl parameter is set to true, controls are accepted.
 *
 * \note
 * This function is used to validate headers from a POST because those
 * just cannot include characters other than ASCII. Actually, most
 * controls are also forbidden.
 *
 * \param[in] str  The string to be validated.
 * \param[in] ctrl  Set to true to also accept controls.
 *
 * \return true if the string is empty, nullptr, or only includes ASCII
 *         characters.
 */
bool is_valid_ascii(char const *str, bool ctrl)
{
    if(str != nullptr)
    {
        for(; *str != '\0'; ++str)
        {
            if(!is_valid_ascii(*str, ctrl))
            {
                return false;
            }
        }
    }

    return true;
}


/** \brief Validate a string as ASCII characters.
 *
 * This function is an overload which accepts an std::string as input.
 *
 * \param[in] str  The string to be validated.
 * \param[in] ctrl  Set to true to also accept controls.
 *
 * \return true if the string is empty, nullptr, or only includes ASCII
 *         characters.
 */
bool is_valid_ascii(std::string const & str, bool ctrl)
{
    return is_valid_ascii(str.c_str(), ctrl);
}


/** \brief Check whether a string is valid UTF-8 or not.
 *
 * This function is used to verify that an input string is valid
 * UTF-8. The function checks each byte and if all the bytes represent
 * a valid UTF-8 stream it returns true, otherwise it returns false.
 *
 * This function is much faster than running a full conversion if you
 * do not need the result since it does not write anything to memory.
 * Note also that this function does not throw on invalid characters
 * whereas the convertion functions do.
 *
 * \note
 * This test is done on data received from clients to make sure that
 * the form data encoding was respected. We only support UTF-8 forms
 * so any client that does not is pretty much limited to sending
 * ASCII characters...
 *
 * Source: http://stackoverflow.com/questions/1031645/how-to-detect-utf-8-in-plain-c
 * Source: http://www.w3.org/International/questions/qa-forms-utf-8
 *
 * \note
 * The test ensures proper encoding of UTF-8 in the range 0 to
 * 0x10FFFF and also that UTF-16 surrogate aren't used as characters
 * (i.e. code points 0xD800 to 0xDFFF). No other code points are considered
 * invalid (i.e. 0xFFFE is not a valid character, but this function does
 * not return false when it finds such.)
 *
 * The Perl expression:
 *
 * \code
 * $field =~
 *   m/\A(
 *      [\x09\x0A\x0D\x20-\x7E]            # ASCII
 *    | [\xC2-\xDF][\x80-\xBF]             # non-overlong 2-byte
 *    |  \xE0[\xA0-\xBF][\x80-\xBF]        # excluding overlongs
 *    | [\xE1-\xEC\xEE\xEF][\x80-\xBF]{2}  # straight 3-byte
 *    |  \xED[\x80-\x9F][\x80-\xBF]        # excluding surrogates
 *    |  \xF0[\x90-\xBF][\x80-\xBF]{2}     # planes 1-3
 *    | [\xF1-\xF3][\x80-\xBF]{3}          # planes 4-15
 *    |  \xF4[\x80-\x8F][\x80-\xBF]{2}     # plane 16
 *   )*\z/x;
 * \endcode
 *
 * \warning
 * Remember that QString already handles UTF-8. However, it keeps the
 * characters as UTF-16 characters in its buffers. This means asking
 * for the UTF-8 representation of a QString should always be considered
 * valid UTF-8 (although some surrogates, etc. may be wrong!)
 *
 * \param[in] string  The NUL terminated string to scan.
 *
 * \return true if the string is valid UTF-8
 */
bool is_valid_utf8(char const * str)
{
    if(str == nullptr)
    {
        // empty strings are considered valid
        return true;
    }

    // use unsigned characters so it works even if char is signed
    unsigned char const *s(reinterpret_cast<unsigned char const *>(str));
    while(*s != '\0')
    {
        if(s[0] <= 0x7F)
        {
            ++s;
        }
        else if(s[0] >= 0xC2 && s[0] <= 0xDF // non-overlong 2-byte
             && s[1] >= 0x80 && s[1] <= 0xBF)
        {
            s += 2;
        }
        else if(s[0] == 0xE0 // excluding overlongs
             && s[1] >= 0xA0 && s[1] <= 0xBF
             && s[2] >= 0x80 && s[2] <= 0xBF)
        {
            s += 3;
        }
        else if(((0xE1 <= s[0] && s[0] <= 0xEC) || s[0] == 0xEE || s[0] == 0xEF) // straight 3-byte
             && s[1] >= 0x80 && s[1] <= 0xBF
             && s[2] >= 0x80 && s[2] <= 0xBF)
        {
            s += 3;
        }
        else if(s[0] == 0xED // excluding surrogates
             && s[1] >= 0x80 && s[1] <= 0x9F
             && s[2] >= 0x80 && s[2] <= 0xBF)
        {
            s += 3;
        }
        else if(s[0] == 0xF0 // planes 1-3
             && s[1] >= 0x90 && s[1] <= 0xBF
             && s[2] >= 0x80 && s[2] <= 0xBF
             && s[3] >= 0x80 && s[3] <= 0xBF)
        {
            s += 4;
        }
        else if(s[0] >= 0xF1 && s[0] <= 0xF3 // planes 4-15
             && s[1] >= 0x80 && s[1] <= 0xBF
             && s[2] >= 0x80 && s[2] <= 0xBF
             && s[3] >= 0x80 && s[3] <= 0xBF)
        {
            s += 4;
        }
        else if(s[0] == 0xF4 // plane 16
             && s[1] >= 0x80 && s[1] <= 0x8F
             && s[2] >= 0x80 && s[2] <= 0xBF
             && s[3] >= 0x80 && s[3] <= 0xBF)
        {
            s += 4;
        }
        else
        {
            // not a supported character
            return false;
        }
    }

    return true;
}


/** \brief Check whether a string is valid UTF-16 or not.
 *
 * This function is used to verify that an input string is valid
 * UTF-16. The function checks each word (2 bytes) and if all the
 * words represent a valid UTF-16 stream it returns true, otherwise
 * it returns false.
 *
 * This function is much faster than running a full conversion if you
 * do not need the result since it does not write anything to memory.
 * Note also that this function does not throw on invalid characters
 * whereas the convertion functions do.
 *
 * The function verifies that when large characters are used, that
 * they properly use a high and then a low surrogates. If the function
 * finds a low surrogate without a high first, or if a high surrogate
 * is not followed by a low surrogate, then the function fails.
 *
 * \param[in] string  The UTF-16 string to scan.
 *
 * \return true if the string is valid UTF-16.
 */
bool is_valid_utf16(std::u16string const & str)
{
    for(std::u16string::value_type const * s(str.c_str());
        *s != L'\0';
        ++s)
    {
        surrogate_t const high_surrogate(is_surrogate(*s));
        if(high_surrogate == surrogate_t::SURROGATE_LOW)
        {
            // missing HIGH surrogate before LOW surrogate
            //
            return false;
        }
        if(high_surrogate == surrogate_t::SURROGATE_HIGH)
        {
            ++s;
            if(*s == L'\0')
            {
                // missing LOW surrogate after HIGH surrogate
                //
                return false;
            }
            surrogate_t const low_surrogate(is_surrogate(*s));
            if(low_surrogate != surrogate_t::SURROGATE_LOW)
            {
                // missing LOW surrogate after HIGH surrogate
                //
                return false;
            }
        }
    }
    return true;
}


/** \brief Check whether a string is valid UTF-8 or not.
 *
 * This function is an overload of the is_valid_utf8(char const *) with
 * an std::string.
 *
 * \param[in] str  The std::string to scan.
 *
 * \return true if the string is valid UTF-8
 */
bool is_valid_utf8(std::string const & str)
{
    return is_valid_utf8(str.c_str());
}


/** \brief Validate a Unicode character.
 *
 * This function checks the specified character. If it looks like a valid
 * Unicode character, the function returns true.
 *
 * Valid characters are between 0 and 0x10FFFF inclusive. However, the
 * code points between 0xD800 and 0xDFFF are considered invalid. They
 * are not supported in UTF-32.
 *
 * When the \p ctrl flag is set to false, then control characters are not
 * included so code points 0x00 to 0x1F and 0x7F to 0x9F are considered
 * invalid even those they are valid UTF-32 code points.
 *
 * \note
 * Many code pointers are not yet defined in Unicode. If you want to
 * test the code point itself, use the get_unicode_character() function
 * and use the unicode_character::is_defined() function instead.
 *
 * \param[in] wc  The character to validate.
 * \param[in] ctrl  Whether the character can be a control or not.
 *
 * \return true if wc is considered valid.
 */
bool is_valid_unicode(char32_t wc, bool ctrl)
{
    if(ctrl)
    {
        return wc < 0x110000 && (wc < 0x00D800 || wc > 0x00DFFF);
    }

    return  wc <  0x110000
        &&  wc >= 0x000020
        && (wc <  0x00007F || wc > 0x00009F)
        && (wc <  0x00D800 || wc > 0x00DFFF);
}


/** \brief Validate a string as Unicode characters.
 *
 * This function checks that all the characters in a string are comprised
 * only of Unicode characters (code bytes 0x01 to 0x10FFFF, since 0x00 is
 * viewed as the end of the string, it is not included as valid).
 *
 * When the ctrl parameter is set to true, controls are accepted. Otherwise
 * codes between 0x00-0x1F and 0x7F-0x9F are refused.
 *
 * \note
 * Code between 0xD800 and 0xDFFF inclusive are viewed as invalid Unicode
 * characters.
 *
 * \param[in] str  The NUL terminated string to be validated.
 * \param[in] ctrl  Set to true to also accept controls.
 *
 * \return true if the string is empty, nullptr, or only includes ASCII
 *         characters.
 */
bool is_valid_unicode(char32_t const *str, bool ctrl)
{
    if(str != nullptr)
    {
        for(; *str != '\0'; ++str)
        {
            if(!is_valid_unicode(*str, ctrl))
            {
                return false;
            }
        }
    }

    return true;
}


/** \brief Validate a string as ASCII characters.
 *
 * This function is an overload which accepts an std::u32string as input.
 *
 * \param[in] str  The string to be validated.
 * \param[in] ctrl  Set to true to also accept controls.
 *
 * \return true if the string is empty, nullptr, or only includes ASCII
 *         characters.
 */
bool is_valid_unicode(std::u32string const & str, bool ctrl)
{
    return is_valid_unicode(str.c_str(), ctrl);
}


/** \brief Check whether a wide character represents a surrogate or not.
 *
 * This function checks whether \p wc represents a surrogate, either
 * the low, the high or not a surrogate. The function returns a
 * surrogate_t enumeration:
 *
 * \li SURROGATE_NO -- not a surrogate
 * \li SURROGATE_HIGH -- a high surrogate (0xD800 to 0xDBFF)
 * \li SURROGATE_LOW -- a low surrogate (0xDC00 to 0xDFFF)
 *
 * \param[in] wc  The wide character to be checked.
 *
 * \return The surrogate category.
 */
surrogate_t is_surrogate(char32_t wc)
{
    wc &= 0xFFFFFC00;
    if(wc == 0xD800)
    {
        return surrogate_t::SURROGATE_HIGH;
    }
    if(wc == 0xDC00)
    {
        return surrogate_t::SURROGATE_LOW;
    }
    return surrogate_t::SURROGATE_NO;
}


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
    if(str == nullptr
    || len < 2)
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
    std::string result;

    char mb[MBS_MIN_BUFFER_LENGTH];
    std::u32string::size_type const max(str.length());
    result.reserve(max * 2);  // TODO: calculate correct resulting string size?
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
    std::string result;

    char mb[MBS_MIN_BUFFER_LENGTH];
    std::u16string::size_type const max(str.length());
    result.reserve(max * 2);  // TODO: calculate correct resulting string size?
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
            surrogate_t const high_surrogate(is_surrogate(wc));
            if(high_surrogate != surrogate_t::SURROGATE_NO)
            {
                // large character, verify that the two surrogates are correct
                //
                if(high_surrogate != surrogate_t::SURROGATE_HIGH)
                {
                    // 0xDC00 to 0xDFFF; introducer missing
                    //
                    throw libutf8_exception_decoding("to_u8string(): found a high UTF-16 surrogate without the low surrogate.");
                }
                ++idx;
                if(idx >= max)
                {
                    // must be followed by a code between 0xDC00 and 0xDFFF
                    //
                    throw libutf8_exception_decoding("to_u8string(): the high UTF-16 surrogate is not followed by the low surrogate.");
                }
                surrogate_t const low_surrogate(is_surrogate(s[idx]));
                if(low_surrogate != surrogate_t::SURROGATE_LOW)
                {
                    if(low_surrogate == surrogate_t::SURROGATE_HIGH)
                    {
                        throw libutf8_exception_decoding("to_u8string(): found two high UTF-16 surrogates in a row.");
                    }
                    else
                    {
                        throw libutf8_exception_decoding("to_u8string(): found a high UTF-16 surrogate without a low surrogate afterward.");
                    }
                }

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


/** \brief Converts an std::wstring to a UTF-8 string.
 *
 * This function converts an std::wstring to UTF-8. The function first
 * determines whether `wchar_t` represents 16 or 32 bits and then
 * calls the corresponding `char16_t` or `char32_t` function.
 *
 * \param[in] str  The wide character string to convert to UTF-8.
 *
 * \return The converted string.
 */
std::string to_u8string(std::wstring const & str)
{
    switch(sizeof(wchar_t))
    {
    case 2:
        return to_u8string(std::u16string(str.begin(), str.end()));

    case 4:
        return to_u8string(std::u32string(str.begin(), str.end()));

    }

    throw libutf8_exception_unsupported("wchar_t has an unsupported size.");
}


/** \brief Converts a wchar_t character to a UTF-8 string.
 *
 * This function converts a wide character (wchar_t) to a
 * UTF-8 std::string. If the wchar_t type is 4 bytes, it gets
 * converted to a char32_t. If the wchar_t type is 2 bytes,
 * it gets converted to char16_t and the \p two parameter
 * also gets forwarded to the to_u8string(char16_t, char16_t);
 * function.
 *
 * \note
 * This means that a wchar_t of 4 bytes cannot ever be a
 * surrogate.
 *
 * \param[in] one  The wchar_t character or high surrogate.
 * \param[in] two  The low surrogate if \p one is a high surrogate and wchar_t
 *                 is 2 bytes.
 *
 * \return The converted string.
 */
std::string to_u8string(wchar_t one, wchar_t two)
{
    switch(sizeof(wchar_t))
    {
    case 2:
        return to_u8string(static_cast<char16_t>(one), static_cast<char16_t>(two));

    case 4:
        return to_u8string(static_cast<char32_t>(one));

    }

    throw libutf8_exception_unsupported("wchar_t has an unsupported size.");
}


/** \brief Converts a char16_t character to a UTF-8 string.
 *
 * This function converts a wide character (char16_t) to a
 * UTF-8 std::string. The function takes two characters in case
 * the input is a pair of surrogate. If the first character is
 * not a surrogate, then you can set the second character to
 * u'\0' since it won't be used.
 *
 * You can check whether \p one or \p two is a surrogate using
 * the is_surrogate() function.
 *
 * \warning
 * The character U'\0' does not get added to the result. In that
 * situation the function returns an empty string.
 *
 * \exception libutf8_exception_decoding
 * The input character must be a valid UTF-16 character or this exception
 * gets raised. This only happens if \p one and \p two are surrogate but
 * not a valid surrogate sequence.
 *
 * \param[in] one  The UTF-16 character or high surrogate.
 * \param[in] two  The low surrogate if \p one is a high surrogate.
 *
 * \return The converted string.
 */
std::string to_u8string(char16_t one, char16_t two)
{
    surrogate_t const a(is_surrogate(one));
    if(a == surrogate_t::SURROGATE_NO)
    {
        std::u16string s;
        s += one;
        return to_u8string(s);
    }

    if(a == surrogate_t::SURROGATE_HIGH)
    {
        surrogate_t const b(is_surrogate(two));
        if(b == surrogate_t::SURROGATE_LOW)
        {
            // the to_u8string() of the u16string will determine the valid order
            // for us
            //
            std::u16string s;
            s += one;
            s += two;
            return to_u8string(s);
        }
    }

    throw libutf8_exception_decoding("to_u8string(char16_t, char16_t): the input did not represent a valid surrogate sequence.");
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
            throw libutf8_exception_encoding(
                  "to_u8string(char32_t): the input wide character(\\U"
                + snapdev::int_to_hex(wc, false, 6)
                + ") is not a valid UTF-32 character.");
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
            throw libutf8_exception_decoding("to_u32string(): a UTF-8 character could not be extracted.");
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


/** \brief Determine the length of the UTF-16 string.
 *
 * This function counts the number of characters in the specified UTF-16
 * string. It is optimized for speed for the UTF-16 encoding.
 *
 * In UTF-16, the surrogates have to appear in the correct order. This
 * function returns -1 instead of the length if it detects an invalid
 * surrogate.
 *
 * \param[in] str  The null terminated string to compute the length in
 * characters of.
 *
 * \return The number of characters in the UTF-16 string or -1 if the string
 * is considered invalid.
 */
ssize_t u16length(std::u16string const & str)
{
    ssize_t result(0);
    for(std::u16string::value_type const * s(str.c_str());
        *s != L'\0';
        ++result, ++s)
    {
        surrogate_t const high_surrogate(is_surrogate(*s));
        if(high_surrogate == surrogate_t::SURROGATE_LOW)
        {
            // missing HIGH surrogate before LOW surrogate
            //
            return -1;
        }
        if(high_surrogate == surrogate_t::SURROGATE_HIGH)
        {
            ++s;
            if(*s == L'\0')
            {
                // missing LOW surrogate after HIGH surrogate
                //
                return -1;
            }
            surrogate_t const low_surrogate(is_surrogate(*s));
            if(low_surrogate != surrogate_t::SURROGATE_LOW)
            {
                // missing LOW surrogate after HIGH surrogate
                //
                return -1;
            }
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
 * \note
 * You may want to consider using the case_insensitive_basic_string class
 * instead if you are to compare a given string case insensitively over
 * and over again.
 *
 * \exception libutf8_exception_decoding
 * This function raises the decoding exception if one of the input strings
 * includes an invalid UTF-8 sequence of characters.
 *
 * \param[in] lhs  The left handside string to compare.
 * \param[in] rhs  The right handside string to compare.
 *
 * \return -1 if lhs < rhs, 0 if lhs == rhs, and 1 if lhs > rhs
 *
 * \sa case_insensitive_basic_string
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
