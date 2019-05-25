/*    libutf8.cpp -- convert between wchar_t and UTF-8 encodings
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

/** \file
 * \brief Implementation of the UTF-8 functions.
 *
 * This file is the implementation of the UTF-8 functions of the libutf8
 * library. It simply is a set of functions to convert between different
 * character sets in a lossless manner. At this point it supports UTF-8,
 * UCS-4, and UTF-16 formats.
 *
 * Contrary to many of the system functions, these functions do not take
 * anything from the system in account (the locale can be anything, it does
 * not change the exact behavior of these functions.)
 *
 * Also similar functionality is found on Unices and MS-Windows, it was
 * simpler to just implement these few functions than to try to have a
 * converter that is sure not to use a locale and this way we can use
 * standard strings (std::string and std::wstring) instead of having to
 * call C functions.
 */

// self
//
#include "libutf8/iterator.h"

// libutf8 lib
//
#include "libutf8/base.h"

// C++ lib
//
#include <iostream>



namespace libutf8
{



utf8_iterator::utf8_iterator(std::string const & str)
    : f_str(str)
{
}


utf8_iterator & utf8_iterator::operator ++ ()
{
    increment();
    return *this;
}


utf8_iterator utf8_iterator::operator ++ (int) // post-increment
{
    utf8_iterator it(*this);
    increment();
    return it;
}


utf8_iterator & utf8_iterator::operator -- ()
{
    decrement();
    return *this;
}


utf8_iterator utf8_iterator::operator -- (int) // post-decrement
{
    utf8_iterator it(*this);
    decrement();
    return it;
}


char32_t utf8_iterator::operator * () const
{
    if(f_pos >= f_str.length())
    {
        return EOF;
    }
    char const * s(f_str.c_str() + f_pos);
    char32_t wc(L'\0');
    size_t len(f_str.length() - f_pos);
    if(mbstowc(wc, s, len) < 0)
    {
        f_good = false;
    }
    return wc;
}


bool utf8_iterator::operator == (std::string::iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str.begin()) == f_pos;
}


bool utf8_iterator::operator != (std::string::iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str.begin()) != f_pos;
}


bool utf8_iterator::operator == (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str.cbegin()) == f_pos;
}


bool utf8_iterator::operator != (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str.cbegin()) != f_pos;
}


bool operator == (std::string::iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str.begin()) == rhs.f_pos;
}


bool operator != (std::string::iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str.begin()) != rhs.f_pos;
}


bool operator == (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str.cbegin()) == rhs.f_pos;
}


bool operator != (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str.cbegin()) != rhs.f_pos;
}


void utf8_iterator::increment()
{
    auto skip = [&]()
    {
        for(unsigned char b(0)
            ; f_pos < f_str.length()
                && (b = static_cast<unsigned char>(f_str[f_pos]),
                            (b >= 0x80 && b <= 0xBF) || b >= 0xF5)
            ; ++f_pos);
        f_good = false;
    };

    if(f_pos >= f_str.length())
    {
        return;
    }

    // increment is easy we can just get the current character and we know
    // the size of the character in UTF-8
    //
    unsigned char c(static_cast<unsigned char>(f_str[f_pos]));

    if(c < 0x80)
    {
        ++f_pos;
    }
    else if(c <= 0xBF || c >= 0xF5)
    {
        // ?! invalid UTF-8 ?!
        //
        skip();
    }
    else if(c >= 0xF0)
    {
        f_pos += 4;
        if(c == 0xF4 && f_pos - 3 < f_str.length())
        {
            c = static_cast<unsigned char>(f_str[f_pos - 3]);
            if(c >= 0x90)
            {
                f_pos -= 3;
                skip();
            }
        }
    }
    else if(c >= 0xE0)
    {
        f_pos += 3;
    }
    else /*if(c >= 0xC0)*/    // always true so we don't have to check
    {
        f_pos += 2;
    }
    if(f_pos > f_str.length())
    {
        f_pos = f_str.length();
        f_good = false;
    }
}


void utf8_iterator::decrement()
{
    if(f_pos == 0)
    {
        return;
    }

    // decrement requires us to search for the previous starting byte
    // which means we need to scan the string
    //
    while(f_pos > 0)
    {
        --f_pos;
        unsigned char c(static_cast<unsigned char>(f_str[f_pos]));
        if(c < 0x80
        || c >= 0xC0)
        {
            break;
        }
    }
}


std::string::size_type utf8_iterator::operator - (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(f_str.cbegin() + f_pos - it);
}


std::string::size_type operator - (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str.cbegin() - rhs.f_pos);
}


bool utf8_iterator::good() const
{
    return f_good;
}


bool utf8_iterator::bad() const
{
    return !f_good;
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
