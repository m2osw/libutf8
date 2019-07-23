/*    libutf8/libutf8.h -- convert between wchar_t and UTF-8 encodings
 *    Copyright (C) 2000-2019  Made to Order Software Corporation
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
#pragma once

/** \file
 * \brief The declarations of the UTF-8 library.
 *
 * This file is the declarations of the UTF-8 library which are just a few
 * functions used to convert a string from one format to another.
 */

// C++ lib
//
#include <string>



namespace libutf8
{


enum class bom_t
{
    BOM_NONE,
    BOM_UTF8,
    BOM_UTF16_LE,
    BOM_UTF16_BE,
    BOM_UTF32_LE,
    BOM_UTF32_BE
};


enum class surrogate_t
{
    SURROGATE_NO,
    SURROGATE_HIGH,
    SURROGATE_LOW
};


constexpr char32_t  BOM_CHAR = U'\U0000FEFF';


bool                is_valid_ascii(char c, bool ctrl = true);
bool                is_valid_ascii(char const * str, bool ctrl = true);
bool                is_valid_ascii(std::string const & str, bool ctrl = true);
bool                is_valid_utf8(char const * str);
bool                is_valid_utf8(std::string const & str);
bool                is_valid_unicode(char32_t const wc, bool ctrl = true);
bool                is_valid_unicode(char32_t const * str, bool ctrl = true);
bool                is_valid_unicode(std::u32string const & str, bool ctrl = true);
surrogate_t         is_surrogate(char32_t wc);
bom_t               start_with_bom(char const * str, size_t len);
std::string         to_u8string(std::u32string const & str);
std::string         to_u8string(std::u16string const & str);
std::string         to_u8string(std::wstring const & str);
std::string         to_u8string(wchar_t one, wchar_t two = L'\0');
std::string         to_u8string(char16_t one, char16_t two = u'\0');
std::string         to_u8string(char32_t wc);
std::u16string      to_u16string(std::string const & str);
std::u32string      to_u32string(std::string const & str);
size_t              u8length(std::string const & str);
int                 u8casecmp(std::string const & lhs, std::string const & rhs);



} // libutf8 namespace
// vim: ts=4 sw=4 et
