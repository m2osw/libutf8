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
    struct UnitTestCLData
        //: public Catch::ConfigData
    {
        int         seed = 0;
        bool        progress = false;
        bool        version = false;
    };
}
// namespace


int unittest_main(int argc, char * argv[])
{
    UnitTestCLData configData;

    Catch::Session session;

    auto cli = session.cli()
             | Catch::clara::Opt(configData.seed, "seed")
                ["-S"]["--seed"]
                ("value to seed the randomizer, if not specified, randomize")
             | Catch::clara::Opt(configData.progress)
                ["-p"]["--progress"]
                ("print name of test section being run")
             | Catch::clara::Opt(configData.version)
                ["-V"]["--version"]
                ("print out the libutf8 library version these unit tests pertain to");

    session.cli(cli);

    auto result(session.applyCommandLine(argc, argv));
    if(result != 0)
    {
        std::cerr << "Error in command line." << std::endl;
        exit(1);
    }

    //if( configData.showHelp )
    //{
    //    session.showHelp();
    //    //std::cout << cli << std::endl;
    //    exit(1);
    //}

    if( configData.version )
    {
        std::cout << LIBUTF8_VERSION_STRING << std::endl;
        exit(0);
    }

    unittest::g_verbose = configData.progress;

    // by default we get a different seed each time; that really helps
    // in detecting errors! (I know, I wrote loads of tests before)
    //
    unsigned int seed(static_cast<unsigned int>(time(NULL)));
    if( configData.seed != 0 )
    {
        seed = static_cast<unsigned int>(configData.seed);
    }
    srand(seed);
    std::cout << "libutf8 v" << LIBUTF8_VERSION_STRING << " [" << getpid() << "]:unittest: seed is " << seed << std::endl;

    return session.run();
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
