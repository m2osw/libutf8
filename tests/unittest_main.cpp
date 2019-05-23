/*
 * File:
 *    tests/unittest_main.cpp
 *
 * License:
 *    Copyright (c) 2006-2019  Made to Order Software Corp.  All Rights Reserved
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

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include "unittest_main.h"

// advgetopt lib
//
#include "libutf8/libutf8.h"
#include "libutf8/version.h"

// C++ lib
//
#include <sstream>


namespace unittest
{
    bool            g_verbose;
}


namespace
{
    struct UnitTestCLData : Catch::ConfigData
    {
        //bool        help = false;
        int         seed = 0;
        std::string tmp = std::string();
        bool        verbose = false;
        bool        version = false;
    };
}
// namespace


int unittest_main(int argc, char * argv[])
{
    UnitTestCLData configData;

    auto cli = Catch::makeCommandLineParser(configData)
             //| Catch::clara::Help(configData.help)//, "help")
             //   //["-?"]["-h"]["--help"]
             //   //("display usage information")
             | Catch::clara::Opt(configData.seed, "seed")
                ["-S"]["--seed"]
                ("value to seed the randomizer, if not specified, randomize")
             | Catch::clara::Opt(configData.verbose)
                ["-v"]["--verbose"]
                ("make the test more verbose")
             | Catch::clara::Opt(configData.version)
                ["-V"]["--version"]
                ("print out the libutf8 library version these unit tests pertain to");

    auto result(cli.parse(Catch::clara::Args(argc, argv)));
    if(!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        exit(1);
    }

    if( configData.showHelp )
    {
        std::cout << cli << std::endl;
        exit(1);
    }

    if( configData.version )
    {
        std::cout << LIBUTF8_VERSION_STRING << std::endl;
        exit(0);
    }

    std::vector<std::string> arg_list;
    for( int i = 0; i < argc; ++i )
    {
        arg_list.push_back( argv[i] );
    }

    // by default we get a different seed each time; that really helps
    // in detecting errors! (I know, I wrote loads of tests before)
    unsigned int seed(static_cast<unsigned int>(time(NULL)));
    if( configData.seed != 0 )
    {
        seed = static_cast<unsigned int>(configData.seed);
    }
    srand(seed);
    std::cout << "libutf8[" << getpid() << "]:unittest: seed is " << seed << std::endl;

    // we can only have one of those for ALL the tests that directly
    // access the library...
    // (because the result is cached and thus cannot change)

    std::vector<char *> new_argv;
    std::for_each( arg_list.begin(), arg_list.end(), [&new_argv]( const std::string& arg )
    {
        new_argv.push_back( const_cast<char *>(arg.c_str()) );
    });

    return Catch::Session().run( static_cast<int>(new_argv.size()), &new_argv[0] );
}


int main(int argc, char * argv[])
{
    int r(1);

    try
    {
        r = unittest_main(argc, argv);
    }
    catch(std::logic_error const & e)
    {
        std::cerr << "fatal error: caught a logic error in libutf8 unit tests: " << e.what() << "\n";
    }

    return r;
}

// vim: ts=4 sw=4 et
