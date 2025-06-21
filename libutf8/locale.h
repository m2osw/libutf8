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
#pragma once

/** \file
 * \brief The declarations of the locale related functions.
 *
 * This file is the declarations of the functions used to manage the locale.
 * In most cases, our tools and servers only make use of the "C" locale
 * to avoid issues. But a few things require the use of a proper locale
 * so these function are here to offer such support.
 */

// C++
//
#include    <set>
#include    <string>



namespace libutf8
{



typedef std::set<std::string>       locales_t;


locales_t const &   system_locales();
locales_t const &   icu_locales();

bool                has_system_locale(char const * name);
bool                has_system_locale(std::string const & name);

bool                has_icu_locale(char const * name);
bool                has_icu_locale(std::string const & name);



} // namespace libutf8
// vim: ts=4 sw=4 et
