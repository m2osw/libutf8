/*    libutf8/exception.h -- convert between wchar_t and UTF-8 encodings
 *    Copyright (C) 2000-2015  Made to Order Software Corporation
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
 *    Authors
 *    Alexis Wilke   alexis@m2osw.com
 */
#pragma once

/** \file
 * \brief The declarations of the UTF-8 library.
 *
 * This file is the declarations of the UTF-8 library which are just a few
 * functions used to convert a string from one format to another.
 */

#include <libexcept/exception.h>



namespace libutf8
{



class libutf8_logic_exception : public libexcept::logic_exception_t
{
public:
    libutf8_logic_exception(std::string const & msg) : logic_exception_t(msg) {}
};



class libutf8_exception : public libexcept::exception_t
{
public:
    libutf8_exception(std::string const & msg) : exception_t(msg) {}
};



class libutf8_exception_decoding : public libutf8_exception
{
public:
    libutf8_exception_decoding(std::string const & msg) : libutf8_exception(msg) {}
};



class libutf8_exception_encoding : public libutf8_exception
{
public:
    libutf8_exception_encoding(std::string const & msg) : libutf8_exception(msg) {}
};





} // libutf8 namespace
// vim: ts=4 sw=4 et
