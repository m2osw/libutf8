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


// libutf8
//
#include    <libutf8/libutf8.h>
#include    <libutf8/version.h>


// C++
//
#include    <cstdint>
#include    <cstring>
#include    <fstream>
#include    <iomanip>
#include    <iostream>
#include    <string>
#include    <vector>


// last include
//
#include    <snapdev/poison.h>



namespace
{


class show_unicode
{
public:
    enum class mode_t
    {
        MODE_STRING,
        MODE_CHARACTER,
        MODE_UTF8_FILENAME,
        MODE_UTF16_FILENAME,
        MODE_UTF32_FILENAME,

        MODE_DEFAULT // like MODE_STRING, just not set explicitly
    };

    int         parse_args(int agrc, char * argv[]);
    int         verify_args();
    int         process();

private:
    void        usage();
    int         set_mode(mode_t m);
    int         read_file();

    mode_t                      f_mode = mode_t::MODE_DEFAULT;
    std::string                 f_filename = std::string();
    std::vector<std::uint8_t>   f_input = std::vector<std::uint8_t>();
    bool                        f_valid_fffe_ffff = true;
};




int show_unicode::parse_args(int argc, char * argv[])
{
    for(int i(1); i < argc; ++i)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i], "-h") == 0
            || strcmp(argv[i], "--help") == 0)
            {
                usage();
                return 2;
            }
            if(strcmp(argv[i], "-V") == 0
            || strcmp(argv[i], "--version") == 0)
            {
                std::cout << LIBUTF8_VERSION_STRING << '\n';
                return 2;
            }
            if(strcmp(argv[i], "-C") == 0
            || strcmp(argv[i], "--unicode") == 0)
            {
                ++i;
                if(i >= argc)
                {
                    std::cerr << "error: the --character command line option must be followed by a number representing a valid Unicode characters in UTF-32.\n";
                    return 3;
                }
                char * end;
                char * s(argv[i]);
                int base(10);
                if(*s == '0')
                {
                    ++s;
                    base = 8;
                    if(*s == 'x' || *s == 'X')
                    {
                        base = 16;
                        ++s;
                    }
                }
                char32_t const wc(strtol(s, &end, base));
                if(end == nullptr
                || *end != '\0')
                {
                    std::cerr
                        << "error: expected a valid decimal, octal, or hexadecimal number; could not parse \""
                        << argv[i]
                        << "\" as a valid number.\n";
                    return 1;
                }
                if(!libutf8::is_valid_unicode(wc))
                {
                    std::cerr
                        << "error: code \"0x"
                        << std::uppercase << std::hex << std::setfill('0') << std::setw(6) << static_cast<std::uint32_t>(wc)
                        << "\" does not represent a valid Unicode character.\n";
                    return 1;
                }
                std::string const character(libutf8::to_u8string(wc));
                f_input.insert(f_input.end(), character.begin(), character.end());
                int const r(set_mode(mode_t::MODE_CHARACTER));
                if(r != 0)
                {
                    return r;
                }
                continue;
            }
            if(strcmp(argv[i], "-s") == 0
            || strcmp(argv[i], "--string") == 0)
            {
                ++i;
                if(i >= argc)
                {
                    std::cerr << "error: the --string command line option must be followed by the string to process.\n";
                    return 3;
                }
                f_input.insert(f_input.end(), argv[i], argv[i] + strlen(argv[i]));
                int const r(set_mode(mode_t::MODE_STRING));
                if(r != 0)
                {
                    return r;
                }
                continue;
            }
            if(strcmp(argv[i], "-f") == 0
            || strcmp(argv[i], "--input") == 0)
            {
                ++i;
                if(i >= argc)
                {
                    std::cerr << "error: the --input command line option must be followed by the input filename.\n";
                    return 3;
                }
                f_filename = argv[i];
                int r(set_mode(mode_t::MODE_UTF8_FILENAME));
                if(r == 0)
                {
                    r = read_file();
                }
                if(r != 0)
                {
                    return r;
                }
                continue;
            }
            if(strcmp(argv[i], "-S") == 0
            || strcmp(argv[i], "--input-utf16") == 0)
            {
                ++i;
                if(i >= argc)
                {
                    std::cerr << "error: the --input-utf16 command line option must be followed by the input filename.\n";
                    return 3;
                }
                f_filename = argv[i];
                int r(set_mode(mode_t::MODE_UTF16_FILENAME));
                if(r == 0)
                {
                    r = read_file();
                }
                if(r == 0 && f_input.size() % 2 != 0)
                {
                    std::cerr << "error: the size of \""
                        << f_filename
                        << "\" was expected to be a multiple of 2.\n";
                    return 1;
                }
                if(r == 0)
                {
                    std::u16string in(reinterpret_cast<char16_t *>(f_input.data()), f_input.size() / 2);
                    std::string u8(libutf8::to_u8string(in));
                    f_input.resize(u8.length());
                    memcpy(f_input.data(), u8.data(), u8.length());
                }
                if(r != 0)
                {
                    return r;
                }
                continue;
            }
            if(strcmp(argv[i], "-F") == 0
            || strcmp(argv[i], "--input-utf32") == 0)
            {
                ++i;
                if(i >= argc)
                {
                    std::cerr << "error: the --input-utf32 command line option must be followed by the input filename.\n";
                    return 3;
                }
                f_filename = argv[i];
                int r(set_mode(mode_t::MODE_UTF32_FILENAME));
                if(r == 0)
                {
                    r = read_file();
                }
                if(r == 0 && f_input.size() % 4 != 0)
                {
                    std::cerr << "error: the size of \""
                        << f_filename
                        << "\" was expected to be a multiple of 4.\n";
                    return 1;
                }
                if(r == 0)
                {
                    std::u32string in(reinterpret_cast<char32_t *>(f_input.data()), f_input.size() / 4);
                    std::string u8(libutf8::to_u8string(in));
                    f_input.resize(u8.length());
                    memcpy(f_input.data(), u8.data(), u8.length());
                }
                if(r != 0)
                {
                    return r;
                }
                continue;
            }
            if(strcmp(argv[i], "--valid-fffe-ffff") == 0)
            {
                f_valid_fffe_ffff = true;
                continue;
            }
            if(strcmp(argv[i], "-W") == 0
            || strcmp(argv[i], "--invalid-fffe-ffff") == 0)
            {
                f_valid_fffe_ffff = false;
                continue;
            }
            std::cerr << "error: unknown command line option \""
                << argv[i]
                << "\".\n";
            return 4;
        }
        else
        {
            f_input.insert(f_input.end(), argv[i], argv[i] + strlen(argv[i]));
        }
    }

    return 0;
}


int show_unicode::set_mode(mode_t m)
{
    if(f_mode != mode_t::MODE_DEFAULT)
    {
        std::cerr << "error: mode already set to: " << static_cast<int>(f_mode) << "\n";
        return 3;
    }
    f_mode = m;

    return 0;
}


int show_unicode::read_file()
{
    std::ifstream in(f_filename);
    if(!in.is_open())
    {
        std::cerr
            << "error: could not open input file \""
            << f_filename
            << "\".\n";
        return 1;
    }
    in.seekg(0, std::ios::end);
    std::size_t const size(in.tellg());
    in.seekg(0);
    f_input.resize(size);
    in.read(reinterpret_cast<char *>(f_input.data()), size);
    if(!in)
    {
        std::cerr
            << "error: could not read input file \""
            << f_filename
            << "\".\n";
        return 1;
    }

    return 0;
}


int show_unicode::verify_args()
{
    // the mode already generated an error no need for that here
    return 0;
}


int show_unicode::process()
{
    // first show the string as is
    //
    std::string utf8(std::string(reinterpret_cast<char *>(f_input.data()), f_input.size()));
    std::cout << "Input: \"" << utf8 << "\".\n";

    // next show the string as UTF-8 bytes
    //
    std::cout << "UTF-8:" << std::hex << std::setfill('0');
    for(auto it(f_input.begin()); it != f_input.end(); ++it)
    {
        char const * space(" ");
        if(*it >= 0x80 && *it <= 0xBF)
        {
            space = ".";
        }
        std::cout << space << std::setw(2) << static_cast<std::uint32_t>(*it);
    }
    std::cout << '\n';

    // next show the string as UTF-16 words
    //
    std::u16string utf16(libutf8::to_u16string(utf8));
    std::cout << "UTF-16:";
    for(auto it(utf16.begin()); it != utf16.end(); ++it)
    {
        std::cout << ' ' << std::setw(4) << static_cast<std::uint32_t>(*it);
    }
    std::cout << '\n';

    // next show the string as UTF-32 words
    //
    std::u32string utf32(libutf8::to_u32string(utf8));
    std::cout << "UTF-32:";
    for(auto it(utf32.begin()); it != utf32.end(); ++it)
    {
        std::cout << ' ' << std::setw(6) << static_cast<std::uint32_t>(*it);
    }
    std::cout << '\n';

    return 0;
}


void show_unicode::usage()
{
    std::cout << "Usage: show-unicode [-<opts>] [-s|--string] '<string>' | -C <value> | -f <filename>\n"
                 "Where -<opts> is one or more of:\n"
                 "  -h | --help                     print this help screen.\n"
                 "  -C | --unicode <value>          use specified value.\n"
                 "  -s | --string <string>          input string to convert (using -s or --string is optional).\n"
                 "  -f | --input <filename>         input file of UTF-8 characters.\n"
                 "  -S | --input-utf16 <filename>   input file of UTF-16 characters.\n"
                 "  -F | --input-utf32 <filename>   input file of UTF-32 characters.\n"
                 "       --valid-fffe-ffff          consider \\uFFFE and \\uFFFF as valid characters (default).\n"
                 "  -W | --invalid-fffe-ffff        consider \\uFFFE and \\uFFFF as invalid characters.\n"
                 "  -V | --version                  print out this tool's version.\n"
                 "\n";
}


} // no name namespace


int main(int argc, char * argv[])
{
    show_unicode show;
    int r(show.parse_args(argc, argv));
    if(r != 0)
    {
        return r;
    }
    r = show.verify_args();
    if(r != 0)
    {
        return r;
    }
    return show.process();
}


// vim: ts=4 sw=4 et
