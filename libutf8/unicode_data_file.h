// Copyright (c) 2021  Made to Order Software Corporation
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
#pragma once

/** \file
 * \brief The declarations of the Unicode compiled files.
 *
 * This file includes structures used to describe the Unicode compiled
 * file. This allows us to very quickly find all the information about
 * a character.
 *
 * From the outside, you are expected to use the unicode_character
 * functions defined in the unicode_data.h header. This file is
 * considered private.
 */

// self lib
//
#include    <libutf8/unicode_data.h>


// C++ lib
//
#include    <string>



namespace libutf8
{

namespace detail
{


enum class Name_Type : std::uint8_t  // see UnicodeData.txt and NameAliases.txt
{
    NT_Name = 0xF0,
    NT_Abbreviation = 0xF1,
    NT_Jamo_Short_Name = 0xF2,  // see Jamo.txt
    NT_Alternate = 0xF3,
    NT_Control = 0xF4,
    NT_WrongName = 0xF5,        // the main name is the corrected name, this name is the invalid/incorrect name
    NT_Figment = 0xF6,
    NT_Numeric = 0xF7,          // saved as two int64_t in the strings because that's under 8kb that way

    NT_EndOfNames = 0xFF,
};



struct ucd_header
{
    char                f_magic[4] = { 'U', 'C', 'D', 'B' };
    time_t              f_timestamp = 0;                // time when this file was generated
    std::uint8_t        f_version = 0;                  // version of this file format
    std::uint8_t        f_ucd_version[3] = { 1, 1, 0 }; // version of source -- i.e. 5 2 0
    std::uint32_t       f_characters = 0;               // offset to character table
    std::uint32_t       f_strings = 0;                  // offset to string table
    std::uint32_t       f_decomposition = 0;            // offset to decomposition table
};



typedef std::uint8_t        flags_t;

constexpr flags_t           UCD_FLAG_DIGIT              = 0x01; // represents a number
constexpr flags_t           UCD_FLAG_DECIMAL            = 0x02; // represents a number
constexpr flags_t           UCD_FLAG_NUMERIC            = 0x04; // represents a number
constexpr flags_t           UCD_FLAG_BIDI_MIRROR        = 0x08; // mirror of another letter left to right vs. right to left
constexpr std::uint8_t      UCD_FLAG_CONTROL            = 0x10;
constexpr std::uint8_t      UCD_FLAG_PRIVATE            = 0x20;



struct ucd_character
{
    constexpr ucd_character()
        : f_decomposition_type(static_cast<int>(Decomposition_Type::DT_unknown))
        , f_decomposition_length(0)
        , f_decomposition_mapping(0)
    {
    }

    /* 32 */    char32_t                    f_code = NOT_A_CHARACTER;
    /* 32 */    std::uint32_t               f_names = 0;        // offset to string table
    /*  8 */    flags_t                     f_flags = 0;
    /*  8 */    General_Category            f_general_category = General_Category::GC_Unknown_Category;
    /*  8 */    Canonical_Combining_Class   f_canonical_combining_class = Canonical_Combining_Class::CCC_Not_Reordered;
    /*  8 */    Bidi_Class                  f_bidi_class = Bidi_Class::BC_Unknown;
    /*  5 */    std::uint32_t               f_decomposition_type : 5;
    /*  5 */    std::uint32_t               f_decomposition_length : 5;
    /* 22 */    std::uint32_t               f_decomposition_mapping : 22;
    /* 16 */    std::uint8_t                f_age[2] = { 1, 1 };
};

// The f_names is an offset in the string table.
//
// Each name is defined as:
//
//     struct name_t
//     {
//         Name_Type    f_type;
//         uint8_t      f_size;
//         char8_t      f_name[f_size];
//     };
//
// Names are not null terminated.
// followed by UTF-8 until the next byte representing a Name_Type, the
// last name ends with special type NT_EndOfNames.
//
// The first name is the corrected name of the character.
//
// Following are the other Name_Type names.
//
// The numeric entries are actually two 64 bit numbers (nominator and
// denominator). The size will always be 16 bytes, but the alignment
// is likely going to be "wrong" (although that should not matter much
// on Intel and ARM processors).




} // detail namespace

} // libutf8 namespace
// vim: ts=4 sw=4 et
