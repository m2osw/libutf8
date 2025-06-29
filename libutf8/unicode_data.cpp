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
#include    "libutf8/unicode_data.h"

#include    "libutf8/exception.h"
#include    "libutf8/libutf8.h"
#include    "libutf8/unicode_data_file.h"


// C++
//
#include    <cwctype>
#include    <list>


// last include
//
#include    <snapdev/poison.h>



/** \brief Name space of the UTF-8 library.
 *
 * The library to convert UTF-8 strings to UCS-4 (Unices) or UTF-16 strings
 * (MS-Windows) and vice versa.
 */
namespace libutf8
{


namespace
{





class private_unicode_character
    : public unicode_character
{
public:
                        private_unicode_character(
                                  char32_t code
                                , detail::ucd_header * h);

protected:
    virtual detail::ucd_character *
                        ucd_character_pointer() const override;

private:
    detail::ucd_character
                        f_private_character = detail::ucd_character();
};


private_unicode_character::private_unicode_character(
          char32_t code
        , detail::ucd_header * h)
    : unicode_character(code, &f_private_character, h)
{
    f_private_character.f_code = code;
    f_private_character.f_flags = detail::UCD_FLAG_PRIVATE;
    f_private_character.f_general_category = General_Category::GC_Private_Use;
    f_private_character.f_bidi_class = Bidi_Class::BC_Left_To_Right;
}


detail::ucd_character * private_unicode_character::ucd_character_pointer() const
{
    return const_cast<detail::ucd_character *>(&f_private_character);
}



} // no name namespace






unicode_character::unicode_character(
          char32_t code
        , detail::ucd_character * c
        , detail::ucd_header * h)
    : f_code(code)
    , f_character(c)
    , f_header(h)
{
}


unicode_character::~unicode_character()
{
}


unicode_character::unicode_character(unicode_character const & rhs)
{
    // this looks weird, but it works as expected
    //
    f_character = rhs.f_character;
    f_character = ucd_character_pointer();
    f_header = rhs.f_header;
}


unicode_character & unicode_character::operator = (unicode_character const & rhs)
{
    // this looks weird, but it works as expected
    //
    f_character = rhs.f_character;
    f_character = ucd_character_pointer();
    f_header = rhs.f_header;

    return *this;
}


bool unicode_character::is_valid() const
{
    return is_valid_unicode(f_code);
}


bool unicode_character::is_defined() const
{
    return f_character->f_code != NOT_A_CHARACTER;
}


bool unicode_character::is_private() const
{
    return (f_character->f_flags & detail::UCD_FLAG_PRIVATE) != 0;
}


General_Category unicode_character::category() const
{
    return f_character->f_general_category;
}


bool unicode_character::is_letter() const
{
    return f_character->f_general_category >= General_Category::GC_Uppercase_Letter
        && f_character->f_general_category <= General_Category::GC_Other_Letter;
}


bool unicode_character::is_mark() const
{
    return f_character->f_general_category >= General_Category::GC_Nonspacing_Mark
        && f_character->f_general_category <= General_Category::GC_Enclosing_Mark;
}


bool unicode_character::is_number() const
{
    return f_character->f_general_category >= General_Category::GC_Decimal_Number
        && f_character->f_general_category <= General_Category::GC_Other_Number;
}


bool unicode_character::is_punctuation() const
{
    return f_character->f_general_category >= General_Category::GC_Connector_Punctuation
        && f_character->f_general_category <= General_Category::GC_Other_Punctuation;
}


bool unicode_character::is_symbol() const
{
    return f_character->f_general_category >= General_Category::GC_Math_Symbol
        && f_character->f_general_category <= General_Category::GC_Other_Symbol;
}


bool unicode_character::is_separator() const
{
    return f_character->f_general_category >= General_Category::GC_Space_Separator
        && f_character->f_general_category <= General_Category::GC_Paragraph_Separator;
}


bool unicode_character::is_other() const
{
    return f_character->f_general_category >= General_Category::GC_Control
        && f_character->f_general_category <= General_Category::GC_Unassigned;
}



Canonical_Combining_Class unicode_character::combining_class()
{
    return f_character->f_canonical_combining_class;
}


Bidi_Class unicode_character::bidi_class() const
{
    return f_character->f_bidi_class;
}


bool unicode_character::is_bidi_mirrored() const
{
    return (f_character->f_flags & detail::UCD_FLAG_BIDI_MIRROR) != 0;
}


Decomposition_Type unicode_character::decomposition_type() const
{
    return static_cast<Decomposition_Type>(f_character->f_decomposition_type);
}


Numeric_Type unicode_character::numeric() const
{
    if((f_character->f_flags & detail::UCD_FLAG_DIGIT) != 0)
    {
        return Numeric_Type::NT_Digit;
    }

    if((f_character->f_flags & detail::UCD_FLAG_DECIMAL) != 0)
    {
        return Numeric_Type::NT_Decimal;
    }

    if((f_character->f_flags & detail::UCD_FLAG_NUMERIC) != 0)
    {
        return Numeric_Type::NT_Numeric;
    }

    return Numeric_Type::NT_Unknown;
}


std::int64_t unicode_character::get_number(int index) const
{
    std::size_t length(0);
    char const * name(find_name(detail::Name_Type::NT_Numeric, length));
    if(name == nullptr)
    {
        return 0;
    }
    if(length != 16)
    {
        // someone tempered with the database?
        //
        throw libutf8_logic_exception("invalid \"name\" size for a number");
    }
    std::int64_t const * number(reinterpret_cast<std::int64_t const *>(name));
    return number[index];
}


std::int64_t unicode_character::nominator() const
{
    return get_number(0);
}


std::int64_t unicode_character::denominator() const
{
    return get_number(1);
}


char const * unicode_character::find_name(detail::Name_Type type, std::size_t & length) const
{
    if(f_character->f_names == 0)
    {
        throw libutf8_logic_exception("character is missing a name");
    }

    char const * name(reinterpret_cast<char const *>(f_header)
                    + f_header->f_strings + f_character->f_names);
    for(;;)
    {
        detail::Name_Type const t(static_cast<detail::Name_Type>(name[0]));
        if(t == detail::Name_Type::NT_EndOfNames)
        {
            length = 0;
            return nullptr;
        }
        length = static_cast<std::uint8_t>(name[1]);
        if(t == type)
        {
            return name + 2;
        }
        name += length + 2;
    }
}


detail::ucd_character * unicode_character::ucd_character_pointer() const
{
    return f_character;
}







} // libutf8 namespace
// vim: ts=4 sw=4 et
