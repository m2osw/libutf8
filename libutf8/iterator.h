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
 * \brief The declarations of the UTF-8 library.
 *
 * This file is the declarations of the UTF-8 library which are just a few
 * functions used to convert a string from one format to another.
 */

// C++
//
#include    <string>



namespace libutf8
{


constexpr char32_t              EOS = static_cast<char32_t>(EOF);


class utf8_iterator
{
public:
    // Iterator traits
    //
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef char32_t                        value_type;
    typedef ssize_t                         difference_type;
    typedef char32_t const *                pointer;
    typedef char32_t const &                reference;

                                utf8_iterator(std::string const & str, bool end = false);

    utf8_iterator &             operator ++ ();
    utf8_iterator               operator ++ (int);
    utf8_iterator &             operator -- ();
    utf8_iterator               operator -- (int);
    value_type                  operator * () const;
    bool                        operator == (utf8_iterator const & rhs) const;
    bool                        operator != (utf8_iterator const & rhs) const;
    bool                        operator == (std::string::iterator it) const;
    bool                        operator != (std::string::iterator it) const;
    bool                        operator == (std::string::const_iterator it) const;
    bool                        operator != (std::string::const_iterator it) const;
    friend bool                 operator == (std::string::iterator it, utf8_iterator const & rhs);
    friend bool                 operator != (std::string::iterator it, utf8_iterator const & rhs);
    friend bool                 operator == (std::string::const_iterator it, utf8_iterator const & rhs);
    friend bool                 operator != (std::string::const_iterator it, utf8_iterator const & rhs);
    difference_type             operator - (utf8_iterator const & rhs) const;
    difference_type             operator - (std::string::const_iterator it) const;
    friend difference_type      operator - (std::string::const_iterator it, utf8_iterator const & rhs);

    void                        rewind();
    void                        clear();
    bool                        good() const;
    bool                        bad() const;

private:
    void                        increment();
    void                        decrement();

    std::string const *         f_str = nullptr;
    std::string::size_type      f_pos = 0;
    std::string::size_type      f_start_pos = 0;
    mutable bool                f_good = true;
};



} // libutf8 namespace
// vim: ts=4 sw=4 et
