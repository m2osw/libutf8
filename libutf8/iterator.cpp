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
 * \brief Implementation of the UTF-8 functions.
 *
 * This file is the implementation of the UTF-8 functions of the libutf8
 * library. It simply is a set of functions to convert between different
 * character sets in a lossless manner. At this point it supports UTF-8,
 * UCS-4, and UTF-16 formats.
 *
 * Contrary to many of the system functions, these functions do not take
 * anything from the system in account (the locale can be anything, it does
 * not change the exact behavior of these functions).
 *
 * Also similar functionality is found on Unices and MS-Windows, it was
 * simpler to just implement these few functions than to try to have a
 * converter that is sure not to use a locale and this way we can use
 * standard strings (std::string and std::wstring) instead of having to
 * call C functions.
 */

// self
//
#include    "libutf8/iterator.h"

#include    "libutf8/base.h"
#include    "libutf8/libutf8.h"


// C++
//
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>



namespace libutf8
{



utf8_iterator::utf8_iterator(std::string const & str, bool end)
    : f_str(&str)
    , f_pos(end ? str.length() : 0)
    , f_start_pos(f_pos)
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


/** \brief Read the current character.
 *
 * This function reads the current character and returns it as a char32_t
 * (i.e. UTF-32).
 *
 * When the iterator is at the end of the input string (it == str.end()),
 * then the function returns libutf8::EOS (-1).
 *
 * When the current character is valid, the value is any number from 0 to
 * 0x10FFFF except for UTF-16 surrogate values (0xD800 to 0xDFFF).
 *
 * When the current character is invalid (bad UTF-8 encoding, although
 * extended UTF-8 is accepted here), then the function returns
 * libutf8::NOT_A_CHARACTER (-2). Further, the good flag is also set to
 * false, which means good() returns false and bad() returns true.
 *
 * \code
 *     for(libutf8::utf8_iterator it(s); it != s.end(); ++it)
 *     {
 *         char32_t c(*it);
 *
 *         // here you can choose:
 *         if(c == libutf8::NOT_A_CHARACTER)
 *         {
 *             // handle error -- current character is not valid UTF-8
 *             break;
 *         }
 *         // -- or --
 *         if(it.bad())
 *         {
 *             // handle error -- current character is not valid UTF-8
 *             break;
 *         }
 *     }
 * \endcode
 *
 * Since this function returns EOS when the iterator is at the end of
 * the string, you can also stop the iteration process like so:
 *
 * \code
 *     libutf8::utf8_iterator it(s);
 *     for(;;)
 *     {
 *         char32_t c(*it);
 *         if(c == libutf8::EOS)
 *         {
 *             // success, all characters were valid
 *             break;
 *         }
 *         ...handle other cases as above...
 *     }
 * \endcode
 *
 * \return EOS if at the end of the string, the current character as a
 * char32_t value or NOT_A_CHARACTER if the current character encoding is
 * wrong.
 *
 * \sa good()
 * \sa bad()
 */
char32_t utf8_iterator::operator * () const
{
    if(f_pos >= f_str->length())
    {
        return EOS;
    }
    char const * s(f_str->c_str() + f_pos);
    char32_t wc(NOT_A_CHARACTER);
    size_t len(f_str->length() - f_pos);
    if(mbstowc(wc, s, len) < 0)
    {
        f_good = false;
    }
    return wc;
}


bool utf8_iterator::operator == (utf8_iterator const & rhs) const
{
    return f_pos == rhs.f_pos;
}


bool utf8_iterator::operator != (utf8_iterator const & rhs) const
{
    return f_pos != rhs.f_pos;
}


bool utf8_iterator::operator == (std::string::iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str->begin()) == f_pos;
}


bool utf8_iterator::operator != (std::string::iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str->begin()) != f_pos;
}


bool utf8_iterator::operator == (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str->cbegin()) == f_pos;
}


bool utf8_iterator::operator != (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(it - f_str->cbegin()) != f_pos;
}


bool operator == (std::string::iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str->begin()) == rhs.f_pos;
}


bool operator != (std::string::iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str->begin()) != rhs.f_pos;
}


bool operator == (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str->cbegin()) == rhs.f_pos;
}


bool operator != (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str->cbegin()) != rhs.f_pos;
}


void utf8_iterator::increment()
{
    auto skip = [&]()
    {
        for(unsigned char b(0)
            ; f_pos < f_str->length()
                && (b = static_cast<unsigned char>(f_str[0][f_pos]),
                            (b >= 0x80 && b <= 0xBF) || b >= 0xF5)
            ; ++f_pos);
        f_good = false;
    };

    if(f_pos >= f_str->length())
    {
        return;
    }

    // increment is easy we can just get the current character and we know
    // the size of the character in UTF-8
    //
    unsigned char c(static_cast<unsigned char>(f_str[0][f_pos]));

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
        if(c == 0xF4 && f_pos - 3 < f_str->length())
        {
            c = static_cast<unsigned char>(f_str[0][f_pos - 3]);
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
    if(f_pos > f_str->length())
    {
        f_pos = f_str->length();
        f_good = false;
    }
}


/** \brief Decrement the iterator.
 *
 * If the iterator is not already at position 0, decrement it to the previous
 * UTF-8 character. This means skipping to the first UTF-8 byte.
 *
 * \note
 * Contrary to the increment(), this function does not set the good flag to
 * true or false whether it is at the start or there is an invalid character.
 */
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
        unsigned char c(static_cast<unsigned char>(f_str[0][f_pos]));
        if(c < 0x80
        || c >= 0xC0)
        {
            break;
        }
    }
}


/** \brief Compute the distance between two iterators.
 *
 * This function computers the distance between two libutf8 iterators.
 *
 * The right hand side iterator must be from the same string as the
 * lhs string.
 *
 * \return The distance between the two iterators.
 */
utf8_iterator::difference_type utf8_iterator::operator - (utf8_iterator const & rhs) const
{
    return f_pos - rhs.f_pos;
}


/** \brief Compute the distance between two iterators.
 *
 * This operator computes the difference between this iterator and the
 * specified \p it iterator.
 *
 * \param[in] it  The iterator to calculate the distance from.
 *
 * \return The distance between the two iterators.
 */
utf8_iterator::difference_type utf8_iterator::operator - (std::string::const_iterator it) const
{
    return static_cast<std::string::size_type>(f_str->cbegin() + f_pos - it);
}


/** \brief Compute the distance between two iterators.
 *
 * This operator computes the difference between the two specified iterators
 * \p it and \p rhs.
 *
 * \param[in] it  The iterator to calculate the distance from.
 * \param[in] rhs  The iterator to calculate the distance to.
 *
 * \return The distance between the two specified iterators.
 */
utf8_iterator::difference_type operator - (std::string::const_iterator it, utf8_iterator const & rhs)
{
    return static_cast<std::string::size_type>(it - rhs.f_str->cbegin() - rhs.f_pos);
}


/** \brief Restart  the iterator.
 *
 * The iterator started at 0 or the end of the string, then you moved it
 * using the `++` or `--` operators. Later you may want to re-parse the
 * string from the start or end of the string.
 *
 * This function resets the position back to 0 or the end as defined on
 * the constructor.
 */
void utf8_iterator::rewind()
{
    f_pos = f_start_pos;
}


/** \brief Clear the errors.
 *
 * The iterator is considered good by default. If you try to retreive
 * a character after the end of the string being iterated or the
 * bytes do not represent an invalid UTF-8 character.
 *
 * \sa good()
 * \sa bad()
 */
void utf8_iterator::clear()
{
    f_good = true;
}


/** \brief Check whether the iterator did not run in an error.
 *
 * The iterator remains good as long as the input characters are valid
 * and the end of the string is not reached. After either event, this
 * function returns false.
 *
 * You can clear this flag by calling the clear() function.
 *
 * \return true if no errors were encountered so far.
 *
 * \sa clear()
 * \sa bad()
 */
bool utf8_iterator::good() const
{
    return f_good;
}


/** \brief Check whether the iterator ran in an error.
 *
 * This function returns true if an invalid character or the end of the
 * string was found.
 *
 * \return true if an error condition was encountered.
 *
 * \sa clear()
 * \sa good()
 */
bool utf8_iterator::bad() const
{
    return !f_good;
}



} // libutf8 namespace
// vim: ts=4 sw=4 et
