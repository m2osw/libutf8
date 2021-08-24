// Copyright (C) 2000-2021  Made to Order Software Corporation
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
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \file
 * \brief Tool used to convert the UnicodeData.txt file to C structures.
 *
 * This executable is used to convert the UnicodeData.txt to a set of
 * C structure which we can search very quickly to find Unicode characters.
 * This gives us all the necessary information to convert strings to NFKC
 * NFKD, and especially NFC and NFD.
 *
 * \sa http://www.unicode.org/reports/tr15/
 */


// libutf8 lib
//
#include    <libutf8/unicode_data.h>


// snapdev lib
//
//#include    <snapdev/file_contents.h>


// C++ lib
//
#include    <fstream>
#include    <iostream>
#include    <string>


// C lib
//
#include    <stdlib.h>
#include    <unistd.h>


// last include
//
#include    <snapdev/poison.h>



namespace
{






} // no name namespace



void usage()
{
    std::cout << "Usage: unicode_data_parser <in> <out>\n";
    std::cout << "Where:\n";
    std::cout << "  <in>     is a path to the unicode files such as UnicodeData.txt (default: \"/usr/shared/libutf8/unicode\")\n";
    std::cout << "  <out>    is a path to the output unicode_data.ucdb file (default: a.ucdb)\n";
}


int main(int argc, char * argv[])
{
    std::string input_dir;
    std::string output_filename;

    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
            case 'h':
                usage();
                exit(1);

            default:
                std::cerr << "error: unknown command line option -"
                    << argv[i][1]
                    << "\n";
                exit(1);
                break;

            }
        }
        else
        {
            if(input_dir.empty())
            {
                input_dir = argv[i];
                if(input_dir.empty())
                {
                    std::cerr << "error: input directory name can't be empty, try \".\" for current folder.\n";
                    exit(1);
                }
            }
            else if(output_filename.empty())
            {
                output_filename = argv[i];
            }
            else
            {
                std::cerr << "error: too many filenames on the command line.\n";
                exit(1);
            }
        }
    }

    if(input_dir.empty())
    {
        input_dir = "/usr/shared/libutf8/unicode";
    }

    if(output_filename.empty())
    {
        output_filename = "a.ucdb";
    }

    libutf8::ucd_parser p(input_dir, output_filename);
    p.generate();

    return 0;
}


// vim: ts=4 sw=4 et
