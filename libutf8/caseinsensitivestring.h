// Copyright (c) 2013-2023  Made to Order Software Corp.  All Rights Reserved
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


// self
//
#include    <libutf8/libutf8.h>


// C++
//
#include    <string>



namespace libutf8
{



/** \brief Case insensitive string.
 *
 * This class is an overload of the string template which allows you to
 * create case insensitive strings as far as the comparison operators
 * are concerned. All the other functions still work the same way.
 *
 * This is particularly useful if you manage an std::map<> with a string as
 * the key, string which should not be case sensitive.
 *
 * The comparisons are done using the libutf8::u8casecmp() function.
 *
 * \sa u8casecmp()
 */
template<
    class _CharT,
    class _Traits = std::char_traits<_CharT>,
    class _Alloc = std::allocator<_CharT>
>
class case_insensitive_basic_string
    : public std::basic_string<_CharT, _Traits, _Alloc>
{
public:
    typedef typename std::basic_string<_CharT, _Traits, _Alloc>::size_type      size_type;

    case_insensitive_basic_string() noexcept(std::is_nothrow_default_constructible<_Alloc>::value)
        : std::basic_string<_CharT, _Traits, _Alloc>()
    {
    }

    explicit case_insensitive_basic_string(_Alloc const & __a)
        : std::basic_string<_CharT, _Traits, _Alloc>(__a)
    {
    }

    case_insensitive_basic_string(size_type __n, _CharT __c, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__n, __c, __a)
    {
    }

    // the following are for C++17 and over
    // (and then the next two constructors will not set __n)
    //
    //case_insensitive_basic_string(std::basic_string<_CharT, _Traits, _Alloc> const & __str, size_type __pos, _Alloc const & __a = _Alloc())
    //    : std::basic_string<_CharT, _Traits, _Alloc>(__str, __pos, __a)
    //{
    //}
    //
    //case_insensitive_basic_string(case_insensitive_basic_string const & __str, size_type __pos, _Alloc const & __a = _Alloc())
    //    : std::basic_string<_CharT, _Traits, _Alloc>(static_cast<std::basic_string<_CharT, _Traits, _Alloc> const &>(__str), __pos, __a)
    //{
    //}

    case_insensitive_basic_string(std::basic_string<_CharT, _Traits, _Alloc> const & __str, size_type __pos, size_type __n = std::basic_string<_CharT, _Traits, _Alloc>::npos, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__str, __pos, __n, __a)
    {
    }

    case_insensitive_basic_string(case_insensitive_basic_string const & __str, size_type __pos, size_type __n = std::basic_string<_CharT, _Traits, _Alloc>::npos, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__str, __pos, __n, __a)
    {
    }

    case_insensitive_basic_string(_CharT const * __d, size_type __n, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__d, __n, __a)
    {
    }

    case_insensitive_basic_string(_CharT const * __d, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__d, __a)
    {
    }

    template<class _InputIterator>
    case_insensitive_basic_string(_InputIterator __beg, _InputIterator __end, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__beg, __end, __a)
    {
    }

    case_insensitive_basic_string(std::basic_string<_CharT, _Traits, _Alloc> const & __str)
        : std::basic_string<_CharT, _Traits, _Alloc>(__str)
    {
    }

    case_insensitive_basic_string(case_insensitive_basic_string const & __str)
        : std::basic_string<_CharT, _Traits, _Alloc>(__str)
    {
    }

    case_insensitive_basic_string(std::basic_string<_CharT, _Traits, _Alloc> && __str) noexcept
        : std::basic_string<_CharT, _Traits, _Alloc>(__str)
    {
    }

    case_insensitive_basic_string(case_insensitive_basic_string && __str) noexcept
        : std::basic_string<_CharT, _Traits, _Alloc>(__str)
    {
    }

    case_insensitive_basic_string(std::basic_string<_CharT, _Traits, _Alloc> && __str, _Alloc const & __a)
        : std::basic_string<_CharT, _Traits, _Alloc>(__str, __a)
    {
    }

    case_insensitive_basic_string(case_insensitive_basic_string && __str, _Alloc const & __a)
        : std::basic_string<_CharT, _Traits, _Alloc>(__str, __a)
    {
    }

    case_insensitive_basic_string(std::initializer_list<_CharT> __l, _Alloc const & __a = _Alloc())
        : std::basic_string<_CharT, _Traits, _Alloc>(__l, __a)
    {
    }


    friend bool operator == (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) == 0;
    }

    friend bool operator == (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) == 0;
    }

    friend bool operator == (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) == 0;
    }

    friend bool operator == (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) == 0;
    }

    friend bool operator == (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) == 0;
    }

    friend bool operator != (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) != 0;
    }

    friend bool operator != (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) != 0;
    }

    friend bool operator != (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) != 0;
    }

    friend bool operator != (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) != 0;
    }

    friend bool operator != (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) != 0;
    }

    friend bool operator < (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) < 0;
    }

    friend bool operator < (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) < 0;
    }

    friend bool operator < (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) < 0;
    }

    friend bool operator < (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) < 0;
    }

    friend bool operator < (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) < 0;
    }

    friend bool operator <= (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) <= 0;
    }

    friend bool operator <= (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) <= 0;
    }

    friend bool operator <= (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) <= 0;
    }

    friend bool operator <= (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) <= 0;
    }

    friend bool operator <= (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) <= 0;
    }

    friend bool operator > (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) > 0;
    }

    friend bool operator > (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) > 0;
    }

    friend bool operator > (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) > 0;
    }

    friend bool operator > (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) > 0;
    }

    friend bool operator > (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) > 0;
    }

    friend bool operator >= (case_insensitive_basic_string const & lhs, std::basic_string<_CharT, _Traits, _Alloc> const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) >= 0;
    }

    friend bool operator >= (case_insensitive_basic_string const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) >= 0;
    }

    friend bool operator >= (std::basic_string<_CharT, _Traits, _Alloc> const & lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) >= 0;
    }

    friend bool operator >= (case_insensitive_basic_string const & lhs, _CharT const * rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) >= 0;
    }

    friend bool operator >= (_CharT const * lhs, case_insensitive_basic_string const & rhs)
    {
        return libutf8::u8casecmp(lhs, rhs) >= 0;
    }
};


typedef case_insensitive_basic_string<char>         case_insensitive_string;

// TODO add support for other types
//typedef case_insensitive_basic_string<wchar_t>      case_insensitive_wstring;
//typedef case_insensitive_basic_string<char16_t>     case_insensitive_u16string;
//typedef case_insensitive_basic_string<char32_t>     case_insensitive_u32string;


}
// libutf8 namespace
// vim: ts=4 sw=4 et
