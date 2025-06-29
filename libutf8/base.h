// Copyright (c) 2000-2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief The declarations of the UTF-8 library base functions.
 *
 * The functions defined in this file are used to do the actual conversions.
 *
 * They may be useful to you which is why we make them available here.
 * However, these are considered low level functions and you may want
 * to restrain using them. Using the `std::string`-base functions is
 * much safer and what is expected of you.
 */

// C++
//
#include    <cstddef>


namespace libutf8
{



constexpr std::size_t       MBS_MIN_BUFFER_LENGTH = 5;
constexpr char32_t const    BOM_CHAR = U'\U0000FEFF';
constexpr char32_t const    NOT_A_CHARACTER = static_cast<char32_t>(-2);

int                     wctombs(char * mb, char32_t wc, std::size_t len);
int                     mbstowc(char32_t & wc, char const * & mb, std::size_t & len);
int                     mbstowc(char32_t & wc, char * & mb, std::size_t & len);



} // libutf8 namespace
// vim: ts=4 sw=4 et
