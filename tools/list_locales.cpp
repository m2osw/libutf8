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
 * \brief List system or ICU locales.
 *
 * This tool retrieves the list of locales known by the system (via
 * `locale -a`) or the Unicode library and displays their names.
 *
 * \sa https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/classicu_1_1NumberFormat.html#a9f8438ad4c5a43003f1a13a03b3a1885
 */


// libutf8
//
#include    <libutf8/locale.h>


// C++
//
#include    <algorithm>
#include    <cstring>
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>



void usage()
{
    std::cout << "Usage: list-locales [-h|--help] [-s|--system] [-u|--icu]\n"
        << "where the options mean:\n"
        << "  -h | --help     print out this help screen\n"
        << "  -s | --system   list the system defined locales (like locale -a)\n"
        << "  -u | --icu      list the ICU defined locales\n"
        << '\n'
        << "If neither -s or -u is defined, then -s is assumed.\n";
}


int main(int argc, char * argv[])
{
    bool show_system(false);
    bool show_icu(false);

    for(int i(1); i < argc; ++i)
    {
        if(strcmp(argv[i], "-h") == 0
        || strcmp(argv[i], "--help") == 0)
        {
            usage();
            exit(2);
        }
        if(strcmp(argv[i], "-s") == 0
        || strcmp(argv[i], "--system") == 0)
        {
            show_system = true;
            continue;
        }
        if(strcmp(argv[i], "-u") == 0
        || strcmp(argv[i], "--icu") == 0)
        {
            show_icu = true;
            continue;
        }
        std::cerr << "error: unknown command line option \""
            << argv[i]
            << "\".\n";
        return 1;
    }

    if(!show_system && !show_icu)
    {
        show_system = true;
    }

    if(show_system && show_icu)
    {
        // when both are specified, we want to show them "combined"
        // which means show each local as either defined in the system
        // only, ICU only, or both
        //
        libutf8::locales_t const & system_locales(libutf8::system_locales());
        libutf8::locales_t const & icu_locales(libutf8::icu_locales());
        if(system_locales.empty() || icu_locales.empty())
        {
            std::cerr << "error: could not retrieve both lists properly, at least one is empty.\n";
            return 1;
        }

        // the following loop assumes that the sets are sorted alphabetically
        //
        auto system_it(system_locales.cbegin());
        auto icu_it(icu_locales.cbegin());
        for(;;)
        {
            std::string const system_name(system_it == system_locales.end() ? std::string() : *system_it);
            std::string const icu_name(icu_it == icu_locales.end() ? std::string() : *icu_it);
            if(system_name == icu_name)
            {
                if(system_name.empty())
                {
                    break;
                }
                std::cout << "  both: " << system_name << '\n';
                ++system_it;
                ++icu_it;
            }
            else if(system_name < icu_name || icu_name.empty())
            {
                std::cout << "system: " << system_name << '\n';
                ++system_it;
            }
            else // if(system_name > icu_name || system_name.empty())
            {
                std::cout << "   icu: " << icu_name << '\n';
                ++icu_it;
            }
        }
    }
    else if(show_system)
    {
        libutf8::locales_t const & locales(libutf8::system_locales());
        if(locales.empty())
        {
            std::cerr << "error: could not retrieve the list of system locales.\n";
            return 1;
        }
        std::for_each(locales.begin(), locales.end(), [](std::string const & name)
            {
                std::cout << name << '\n';
            });
    }
    else if(show_icu)
    {
        libutf8::locales_t const & locales(libutf8::icu_locales());
        if(locales.empty())
        {
            std::cerr << "error: could not retrieve the list of ICU locales.\n";
            return 1;
        }
        std::for_each(locales.begin(), locales.end(), [](std::string const & name)
            {
                std::cout << name << '\n';
            });
    }
    else
    {
        std::cerr << "error: unimplemented command.\n";
        return 1;
    }

    return 0;
}


// vim: ts=4 sw=4 et
