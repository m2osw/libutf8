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
#pragma once

/** \file
 * \brief The declarations of the UTF-8 exceptions.
 *
 * This file is the declarations of the exceptions used by the UTF-8 library.
 */

// libexcept
//
#include    <libexcept/exception.h>



namespace libutf8
{



DECLARE_LOGIC_ERROR(logic_exception);

DECLARE_MAIN_EXCEPTION(libutf8_exception);

DECLARE_EXCEPTION(libutf8_exception, decoding);
DECLARE_EXCEPTION(libutf8_exception, encoding);
DECLARE_EXCEPTION(libutf8_exception, invalid_parameter);
DECLARE_EXCEPTION(libutf8_exception, io);
DECLARE_EXCEPTION(libutf8_exception, missing);
DECLARE_EXCEPTION(libutf8_exception, overflow);
DECLARE_EXCEPTION(libutf8_exception, twice);
DECLARE_EXCEPTION(libutf8_exception, unsupported);



} // libutf8 namespace
// vim: ts=4 sw=4 et
