// Copyright (c) 2025  Made to Order Software Corp.  All Rights Reserved
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
 * \brief List locales as seen by the Unicode libray (ICU).
 *
 * This tool retrieves the list of locales known by the Unicode library
 * and display their names.
 *
 * \sa https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1NumberFormat.html#a9f8438ad4c5a43003f1a13a03b3a1885
 */


//// libutf8
////
//#include    <libutf8/libutf8.h>
//#include    <libutf8/version.h>


// C++
//
//#include    <cstdint>
//#include    <cstring>
//#include    <fstream>
//#include    <iomanip>
#include    <iostream>
//#include    <string>
//#include    <vector>


// unicode
//
#include    <unicode/locid.h>
#include    <unicode/numfmt.h>


// last include
//
//#include    <snapdev/poison.h>



int main()
{
    int32_t num_locales(0);
    UErrorCode status = U_ZERO_ERROR;

    // Get available locales for NumberFormat
    icu::StringEnumeration * available_locales(icu::NumberFormat::getAvailableLocales());

    char const * name(nullptr);
    while((name = available_locales->next(nullptr, status)) != nullptr)
    {
        std::cout << name << '\n';
    }

    return 0;
}


// vim: ts=4 sw=4 et
