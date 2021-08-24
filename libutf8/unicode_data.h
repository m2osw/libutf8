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
 * \brief The declarations of the UTF-8 library.
 *
 * This file is the declarations of the UTF-8 library which are just a few
 * functions used to convert a string from one format to another.
 */

// self
//
#include    <libutf8/libutf8.h>


// C++ lib
//
#include    <memory>



namespace libutf8
{


namespace detail
{
class unicode_data_impl;
class parser_impl;
class ucd_header;
class ucd_character;
enum class Name_Type : std::uint8_t;
} // detail namespace



enum class General_Category : uint8_t
{
    GC_Unknown_Category = 0,

    // GC_Letter = 1 to 6           // L
    GC_Uppercase_Letter = 1,        // Lu
    GC_Lowercase_Letter = 2,        // Ll
    GC_TitleCase_Letter = 3,        // Lt
    GC_Cased_Letter = 4,            // LC
    GC_Modified_Letter = 5,         // Lm
    GC_Other_Letter = 6,            // Lo

    // GC_Mark = 7 to 9             // M
    GC_Nonspacing_Mark = 7,         // Mn
    GC_Spacing_Mark = 8,            // Mc
    GC_Enclosing_Mark = 9,          // Me

    // GC_Number = 10 to 12         // N
    GC_Decimal_Number = 10,         // Nd
    GC_Letter_Number = 11,          // Nl
    GC_Other_Number = 12,           // No

    // GC_Punctuation = 13 to 19    // P
    GC_Connector_Punctuation = 13,  // Pc
    GC_Dash_Punctuation = 14,       // Pd
    GC_Open_Punctuation = 15,       // Ps
    GC_Close_Punctuation = 16,      // Pe
    GC_Initial_Punctuation = 17,    // Pi
    GC_Final_Punctuation = 18,      // Pf
    GC_Other_Punctuation = 19,      // Po

    // GC_Symbol = 20 to 23         // S
    GC_Math_Symbol = 20,            // Sm
    GC_Current_Symbol = 21,         // Sc
    GC_Modifier_Symbol = 22,        // Sk
    GC_Other_Symbol = 23,           // So

    // GC_Separator = 24 to 26      // Z
    GC_Space_Separator = 24,        // Zs
    GC_Line_Separator = 25,         // Zl
    GC_Paragraph_Separator = 26,    // Zp

    // GC_Other = 27 to 31          // C
    GC_Control = 27,                // Cc
    GC_Format = 28,                 // Cf
    GC_Surrogate = 29,              // Cs
    GC_Private_Use = 30,            // Co
    GC_Unassigned = 31,             // Cn
};


enum class Canonical_Combining_Class : uint8_t
{
    CCC_Not_Reordered = 0,

    // Fixed position classes
    CCC_Overlay = 1,
    CCC_Han_Reading = 6,
    CCC_Nukta = 7,
    CCC_Kana_Voicing = 8,
    CCC_Virama = 9,
    CCC_Ccc10 = 10,     // first CCC
    // ... not specifically defined ...
    CCC_Ccc199 = 199,   // last CCC

    // Other classes
    CCC_Attached_Below_Left = 200,
    CCC_Attached_Below = 202,
    CCC_Attached_Above = 214,
    CCC_Attached_Above_Right = 216,
    CCC_Below_Left = 218,
    CCC_Below = 220,
    CCC_Below_Right = 222,
    CCC_Left = 224,
    CCC_Right = 226,
    CCC_Above_Left = 228,
    CCC_Above = 230,
    CCC_Above_Right = 232,
    CCC_Double_Below = 233,
    CCC_Double_Above = 234,
    CCC_Iota_Subscript = 240
};


enum class Bidi_Class : uint8_t
{
    BC_Unknown = 0,

    // Strong Types
    BC_Left_To_Right = 10,              // L
    BC_Right_To_Left = 11,              // R
    BC_Arabic_Letter = 12,              // AL

    // Weak Types
    BC_European_Number = 20,            // EN
    BC_European_Separator = 21,         // ES
    BC_European_Terminator = 22,        // ET
    BC_Arabic_Number = 23,              // AN
    BC_Common_Separator = 24,           // CS
    BC_Nonspacing_Mark = 25,            // NSM
    BC_Boundary_Neutral = 26,           // BN

    // Neutral Types
    BC_Paragraph_Separator = 30,        // B
    BC_Segment_Separator = 31,          // S
    BC_White_Space = 32,                // WS
    BC_Other_Neutral = 33,              // ON

    // Explicit Formatting Types
    BC_Left_To_Right_Embedding = 40,    // LRE
    BC_Left_To_Right_Override = 41,     // LRO
    BC_Right_To_Left_Embedding = 42,    // RLE
    BC_Right_To_Left_Override = 43,     // RLO
    BC_Pop_Directional_Format = 44,     // PDF
    BC_Left_To_Right_Isolate = 45,      // LRI
    BC_Right_To_Left_Isolate = 46,      // RLI
    BC_First_Strong_Isolate = 47,       // FSI
    BC_Pop_Directional_Isolate = 48,    // PDI
};


enum class Decomposition_Type : uint8_t
{
    DT_unknown = 0,
    DT_none = 1,
    DT_canonical = 2,

    DT_font = 10,
    DT_noBreak = 11,
    DT_initial = 12,
    DT_medial = 13,
    DT_final = 14,
    DT_isolated = 15,
    DT_circle = 16,
    DT_super = 17,
    DT_sub = 18,
    DT_vertical = 19,
    DT_wide = 20,
    DT_narrow = 21,
    DT_small = 22,
    DT_square = 23,
    DT_fraction = 24,
    DT_compat = 25,
};


enum class Numeric_Type : uint8_t
{
    NT_Unknown = 0, // a.k.a. this is not marked as a number

    NT_Digit = 1,           // the Digit type should be viewed as equivalent to Decimal
    NT_Decimal = 2,
    NT_Numeric = 3,
};





class unicode_character
{
public:
    typedef std::shared_ptr<unicode_character>
                        pointer_t;

                        unicode_character(
                              char32_t code
                            , detail::ucd_character * c
                            , detail::ucd_header * h);
    virtual             ~unicode_character();
                        unicode_character(unicode_character const & rhs);
    unicode_character & operator = (unicode_character const & rhs);

    bool                is_valid() const;       // valid code point as far as Unicode (UTF-32) is concerned
    bool                is_defined() const;     // whether this is a Unicode defined character or not
    bool                is_private() const;     // whether this code point is reserved for private use

    General_Category    category() const;
    bool                is_letter() const;
    bool                is_mark() const;
    bool                is_number() const;
    bool                is_punctuation() const;
    bool                is_symbol() const;
    bool                is_separator() const;
    bool                is_other() const;

    Canonical_Combining_Class
                        combining_class();
    Bidi_Class          bidi_class() const;
    Decomposition_Type  decomposition_type() const;

    Numeric_Type        numeric() const;
    std::int64_t        nominator() const;
    std::int64_t        denominator() const;

protected:
    virtual detail::ucd_character *
                        ucd_character_pointer() const;

private:
    std::int64_t        get_number(int index) const;
    char const *        find_name(detail::Name_Type type, std::size_t & length) const;

    char32_t            f_code = NOT_A_CHARACTER;
    detail::ucd_character *
                        f_character = nullptr;
    detail::ucd_header *f_header = nullptr;
};




class unicode_data
{
public:
    typedef std::shared_ptr<unicode_data>
                        pointer_t;

    static pointer_t    get_instance();

    // input file information
    //
    time_t              last_generated();
    void                set_cache(bool cache = true);
    bool                get_cache() const;
    char const *        version() const;
    std::string const   version_string() const;

    // access character data
    //
    unicode_character::pointer_t
                        character(char32_t wc);

private:
    typedef std::shared_ptr<detail::unicode_data_impl>
                        unicode_data_impl_pointer_t;

    unicode_data_impl_pointer_t
                        f_impl = unicode_data_impl_pointer_t();
};


class ucd_parser
{
public:
                        ucd_parser(
                              std::string const & input_dir
                            , std::string const & output_filename);

    void                generate();

private:
    typedef std::shared_ptr<detail::parser_impl>
                        parser_impl_pointer_t;

    parser_impl_pointer_t
                        f_impl = parser_impl_pointer_t();
};




} // libutf8 namespace
// vim: ts=4 sw=4 et
