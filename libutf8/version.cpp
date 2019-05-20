/*
 * File:
 *    libutf8/version.cpp -- UTF-8 chacracter handling
 *
 * License:
 *    Copyright (c) 2000-2019  Made to Order Software Corp.  All Rights Reserved
 *
 *    https://snapwebsites.org/
 *    contact@m2osw.com
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
 * Authors:
 *    Alexis Wilke   alexis@m2osw.com
 */

/** \file
 * \brief The UTF-8 libray is used to convert C++ strings.
 *
 * This file shows the UTF-8 library version.
 *
 * The `#define` give you the library version at the time you are compiling.
 * The functions allow you to retrieve the version of a dynamically linked
 * library.
 */
#include    "libutf8/version.h"


namespace libutf8
{




/** \brief Get the major version of the library
 *
 * This function returns the version of the running library (the
 * one you are linked against at runtime).
 *
 * \return The major version.
 */
int get_major_version()
{
    return LIBUTF8_VERSION_MAJOR;
}


/** \brief Get the release version of the library.
 *
 * This function returns the release version of the running library
 * (the one you are linked against at runtime).
 *
 * \return The release version.
 */
int get_release_version()
{
    return LIBUTF8_VERSION_MINOR;
}


/** \brief Get the patch version of the library.
 *
 * This function returns the patch version of the running library
 * (the one you are linked against at runtime).
 *
 * \return The patch version.
 */
int get_patch_version()
{
    return LIBUTF8_VERSION_PATCH;
}


/** \brief Get the full version of the library as a string.
 *
 * This function returns the major, release, and patch versions of the
 * running library (the one you are linked against at runtime) in the
 * form of a string.
 *
 * The build version is not made available.
 *
 * \return The library version.
 */
char const * get_version_string()
{
    return LIBUTF8_VERSION_STRING;
}


} // libutf8 namespace
// vim: ts=4 sw=4 et
