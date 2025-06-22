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
 * \brief Implement that locale related functions.
 *
 * Somehow POSIX does not define a library function to get the list of
 * locales so instead we have to write one that uses popen() which
 * seems rather bad. Even if POSIX does not define a way to enumerate
 * your locales you should still be able to offer such in the C library.
 *
 * This implementation supports two lists: the one from the system which
 * the `locale -a` command line returns and the one from the ICU library
 * which can be different (and is not exactly a system locale).
 */

// self
//
#include    "libutf8/locale.h"

//#include    "libutf8/base.h"
#include    "libutf8/exception.h"


// snapdev
//
#include    <snapdev/raii_generic_deleter.h>


// C++
//
#include    <cstring>


// C
//
//#include    <cwctype>


// unicode
//
#include    <unicode/locid.h>
#include    <unicode/numfmt.h>


// last include
//
#include    <snapdev/poison.h>



namespace libutf8
{



namespace
{



locales_t       g_system_locales;
locales_t       g_icu_locales;



} // no name namespace



/** \brief Get a reference to the set of system locales.
 *
 * This function returns a reference to the set of system locales. If
 * the locales were not yet loaded, try to load them at the time the
 * function is called.
 *
 * If an error occurs before any locale name is retrieved, then the
 * returned list is empty and the errno variable is set. If at least
 * one locale is loaded, then it gets returned.
 *
 * Note that the list may have redundant entries such as "C" and
 * "POSIX" as two separate entries, yet they both represent the
 * exact same locale.
 *
 * \warning
 * The set is loaded once the first time the function gets called.
 * If the operating system gets updated and new locales added or
 * removed, then your software will not notice that change until
 * it restarts.
 *
 * \return the list of locales as defined by the operating system or an
 *         empty list and errno set an error.
 */
locales_t const & system_locales()
{
    if(g_system_locales.empty())
    {
        // no read yet, retrieve the list now
        //
        FILE * f(popen("locale -a", "r"));
        if(f == nullptr)
        {
            return g_system_locales; // LCOV_EXCL_LINE
        }
        snapdev::raii_file_t auto_close(f);

        // the longest locale name is much less than 128 characters
        //
        char buf[128];
        while(fgets(buf, sizeof(buf), f) != nullptr)
        {
            std::size_t const l(std::strlen(buf));
            if(l == 0)
            {
                // this should never happen
                //
                throw libutf8_logic_exception("somehow the fgets() call returned an empty string."); // LCOV_EXCL_LINE
            }
            if(buf[l - 1] == '\n')
            {
                buf[l - 1] = '\0';
            }
            g_system_locales.insert(buf);
        }
    }

    return g_system_locales;
}


/** \brief Get a reference to the set of ICU locales.
 *
 * This function returns a reference to the set of ICU locales. The
 * set is global, allowing us to load the list once and reuse the same
 * set over and over again.
 *
 * If an error occurs obtaining the set and the set is still empty,
 * then the function returns an empty set of an error in errno.
 *
 * \warning
 * The set is loaded once the first time the function gets called.
 * If the ICU library gets updated and new locales added or removed,
 * then your software will not notice that change until it restarts.
 *
 * \return A set of ICU locales. If the gathering of the locale names
 *         fails, then the function returns an empty set and sets the
 *         errno variable to the reason why it failed.
 */
locales_t const & icu_locales()
{
    if(g_icu_locales.empty())
    {
        // get available locales for NumberFormat
        //
        // the default function should return all the locales, whether they
        // include NumberFormat definitions or not
        //
        std::unique_ptr<icu::StringEnumeration> available_locales(icu::NumberFormat::getAvailableLocales());
        if(available_locales == nullptr)
        {
            errno = ENOENT;             // LCOV_EXCL_LINE
            return g_icu_locales;       // LCOV_EXCL_LINE
        }

        for(;;)
        {
            UErrorCode status(U_ZERO_ERROR);
            char const * name(available_locales->next(nullptr, status));
            if(name == nullptr)
            {
                break;
            }
            g_icu_locales.insert(name);
        }
    }

    return g_icu_locales;
}


/** \brief Check whether the named system locale exists on this system.
 *
 * This function checks the specified locale \p name. If found in the
 * set of system locales, then the function returns true. It returns
 * false in all other cases.
 *
 * When the function returns true, it means that a locale with that
 * name exists in the set of locales.
 *
 * \note
 * A system locale name may include multiple parts: the name of a
 * language, the name of a country, a format (utf8, iso88591, ...)
 * and some options (@euro).
 *
 * \param[in] name  The system locale name to check.
 *
 * \return true if the named locale exists.
 */
bool has_system_locale(char const * name)
{
    return system_locales().contains(name);
}


bool has_system_locale(std::string const & name)
{
    return has_system_locale(name.c_str());
}


/** \brief Check whether a string is valid UTF-8 or not.
 *
 * This function  some surrogates, etc. may be wrong!)
 *
 * \param[in] name  The ICU locale name to check.
 *
 * \return true if the ICU library defines that locale.
 */
bool has_icu_locale(char const * name)
{
    return icu_locales().contains(name);
}


bool has_icu_locale(std::string const & name)
{
    return has_icu_locale(name.c_str());
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
