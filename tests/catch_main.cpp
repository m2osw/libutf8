// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
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

// Tell catch we want it to add the runner code in this file.
#define CATCH_CONFIG_RUNNER

// self
//
#include    "catch_main.h"


// libutf8
//
#include    <libutf8/libutf8.h>
#include    <libutf8/version.h>


// libexcept
//
#include    <libexcept/exception.h>


// C++
//
#include    <sstream>


// last include
//
#include    <snapdev/poison.h>





int main(int argc, char * argv[])
{
    return SNAP_CATCH2_NAMESPACE::snap_catch2_main(
              "libutf8"
            , LIBUTF8_VERSION_STRING
            , argc
            , argv
            , []() { libexcept::set_collect_stack(libexcept::collect_stack_t::COLLECT_STACK_NO); }
        );
}


// vim: ts=4 sw=4 et
