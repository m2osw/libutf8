// Copyright (c) 2000-2022  Made to Order Software Corp.  All Rights Reserved
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
 * \brief Tool used to convert the UnicodeData.txt file to C structures.
 *
 * This executable is used to convert the UnicodeData.txt to a set of
 * C structure which we can search very quickly to find Unicode characters.
 * This gives us all the necessary information to convert strings to NFKC
 * NFKD, and especially NFC and NFD.
 *
 * \sa http://www.unicode.org/reports/tr15/
 *
 * The format of the UnicodeData.txt file includes fields that are separated
 * by semi-colon. The following are the fields found in the file. The name
 * of each field is normalized as it is referenced in other parts of the
 * Unicode standard.
 *
 * \li [0] Code
 *
 * Code value in 4-digit hexadecimal format.
 *
 * \li [1] Name
 *
 * When a string value not enclosed in <angle brackets> occurs in this
 * field, it specifies the character's Name property value, which matches
 * exactly the name published in the code charts. The Name property value
 * for most ideographic characters and for Hangul syllables is derived
 * instead by various rules. See Section 4.8, Name in
 * [Unicode](https://www.unicode.org/reports/tr41/tr41-26.html#Unicode)
 * for a full specification of those rules. Strings enclosed in
 * \<angle brackets\> in this field either provide label information used in
 * the name derivation rules, or---in the case of characters which have a null
 * string as their Name property value, such as control characters---provide
 * other information about their code point type.
 *
 * \li [2] General_Category
 *
 * This is a useful breakdown into various character types which can be used
 * as a default categorization in implementations. For the property values,
 * see [General Category Values](https://www.unicode.org/reports/tr44/#General_Category_Values).
 *
 * Here is the list of categories:
 *
 * \code
 *  Lu  Uppercase_Letter
 *  Ll  Lowercase_Letter
 *  Lt  TitleCase_Letter
 *  LC  Cased_Letter
 *  Lm  Modified_Letter
 *  Lo  Other_Letter
 *  L   Letter
 *  Mn  Nonspacing_Mark
 *  Mc  Spacing_Mark
 *  Me  Enclosing_Mark
 *  M   Mark
 *  Nd  Decimal_Number
 *  Nl  Letter_Number
 *  No  Other_Number
 *  N   Number
 *  Pc  Connector_Punctuation
 *  Pd  Dash_Punctuation
 *  Ps  Open_Punctuation
 *  Pe  Close_Punctuation
 *  Pi  Initial_Punctuation
 *  Pf  Final_Punctuation
 *  Po  Other_Punctuation
 *  P   Punctuation
 *  Sm  Math_Symbol
 *  Sc  Current_Symbol
 *  Sk  Modifier_Symbol
 *  So  Other_Symbol
 *  S   Symbol
 *  Zs  Space_Separator
 *  Zl  Line_Separator
 *  Zp  Paragraph_Separator
 *  Z   Separator
 *  Cc  Control
 *  Cf  Format
 *  Cs  Surrogate
 *  Co  Private_Use
 *  Cn  Unassigned
 *  C   Other
 * \endcode
 *
 * \li [3] Canonical_Combining_Class
 *
 * The classes used for the Canonical Ordering Algorithm in the Unicode
 * Standard. This property could be considered either an enumerated
 * property or a numeric property: the principal use of the property is in
 * terms of the numeric values. For the property value names associated
 * with different numeric values, see
 * [DerivedCombiningClass.txt](https://www.unicode.org/reports/tr44/#DerivedCombiningClass.txt)
 * and [Canonical Combining Class Values](https://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values).
 *
 * The following are some explaination of the combining class numbers:
 *
 * \code
 * 0    Not_Reordered           Spacing and enclosing marks; also many vowel
 *                              and consonant signs, even if nonspacing
 * 1    Overlay                 Marks which overlay a base letter or symbol
 * 6    Han_Reading             Diacritic reading marks for CJK unified ideographs
 * 7    Nukta                   Diacritic nukta marks in Brahmi-derived scripts
 * 8    Kana_Voicing            Hiragana/Katakana voicing marks
 * 9    Virama                  Viramas
 * 10   Ccc10                   Start of fixed position classes
 * 199                          End of fixed position classes
 * 200  Attached_Below_Left     Marks attached at the bottom left
 * 202  Attached_Below          Marks attached directly below
 * 204                          Marks attached at the bottom right
 * 208                          Marks attached to the left
 * 210                          Marks attached to the right
 * 212                          Marks attached at the top left
 * 214  Attached_Above          Marks attached directly above
 * 216  Attached_Above_Right    Marks attached at the top right
 * 218  Below_Left              Distinct marks at the bottom left
 * 220  Below                   Distinct marks directly below
 * 222  Below_Right             Distinct marks at the bottom right
 * 224  Left                    Distinct marks to the left
 * 226  Right                   Distinct marks to the right
 * 228  Above_Left              Distinct marks at the top left
 * 230  Above                   Distinct marks directly above
 * 232  Above_Right             Distinct marks at the top right
 * 233  Double_Below            Distinct marks subtending two bases
 * 234  Double_Above            Distinct marks extending above two bases
 * 240  Iota_Subscript          Greek iota subscript only
 * \endcode
 *
 * \li [4] Bidi_Class
 *
 * These are the categories required by the Unicode Bidirectional Algorithm.
 * For the property values, see
 * [Bidirectional Class Values](https://www.unicode.org/reports/tr44/#Bidi_Class_Values).
 * For more information, see Unicode Standard Annex #9, "Unicode
 * Bidirectional Algorithm" [UAX9](https://www.unicode.org/reports/tr41/tr41-26.html#UAX9).
 *
 * The default property values depend on the code point, and are explained
 * in DerivedBidiClass.txt
 *
 * \code
 * Strong Types
 * L    Left_To_Right           any strong left-to-right character
 * R    Right_To_Left           any strong right-to-left (non-Arabic-type) character
 * AL   Arabic_Letter           any strong right-to-left (Arabic-type) character
 * Weak Types
 * EN   European_Number         any ASCII digit or Eastern Arabic-Indic digit
 * ES   European_Separator      plus and minus signs
 * ET   European_Terminator     a terminator in a numeric format context, includes currency signs
 * AN   Arabic_Number           any Arabic-Indic digit
 * CS   Common_Separator        commas, colons, and slashes
 * NSM  Nonspacing_Mark         any nonspacing mark
 * BN   Boundary_Neutral        most format characters, control codes, or noncharacters
 * Neutral Types
 * B    Paragraph_Separator     various newline characters
 * S    Segment_Separator       various segment-related control codes
 * WS   White_Space             spaces
 * ON   Other_Neutral           most other symbols and punctuation marks
 * Explicit Formatting Types
 * LRE  Left_To_Right_Embedding U+202A: the LR embedding control
 * LRO  Left_To_Right_Override  U+202D: the LR override control
 * RLE  Right_To_Left_Embedding U+202B: the RL embedding control
 * RLO  Right_To_Left_Override  U+202E: the RL override control
 * PDF  Pop_Directional_Format  U+202C: terminates an embedding or override control
 * LRI  Left_To_Right_Isolate   U+2066: the LR isolate control
 * RLI  Right_To_Left_Isolate   U+2067: the RL isolate control
 * FSI  First_Strong_Isolate    U+2068: the first strong isolate control
 * PDI  Pop_Directional_Isolate U+2069: terminates an isolate control
 * \endcode
 *
 * \li [5] Decomposition_Type and Decomposition_Mapping
 *
 * This field contains both values, with the type in angle brackets.
 * The decomposition mappings exactly match the decomposition mappings
 * published with the character names in the Unicode Standard. For more
 * information, see
 * [Character Decomposition Mappings](https://www.unicode.org/reports/tr44/#Character_Decomposition_Mappings).
 *
 * The compatibility equivalence is defined as a name between angle brackets:
 *
 * \code
 * <font>       Font variant (for example, a blackletter form)
 * <noBreak>    No-break version of a space or hyphen
 * <initial>    Initial presentation form (Arabic)
 * <medial>     Medial presentation form (Arabic)
 * <final>      Final presentation form (Arabic)
 * <isolated>   Isolated presentation form (Arabic)
 * <circle>     Encircled form
 * <super>      Superscript form
 * <sub>        Subscript form
 * <vertical>   Vertical layout presentation form
 * <wide>       Wide (or zenkaku) compatibility character
 * <narrow>     Narrow (or hankaku) compatibility character
 * <small>      Small variant form (CNS compatibility)
 * <square>     CJK squared font variant
 * <fraction>   Vulgar fraction form
 * <compat>     Otherwise unspecified compatibility character
 * \endcode
 *
 * Mappings without a compatibility equivalence represents a canonical
 * mapping.
 *
 * The mapping is a series of code points (i.e. `0045 0301`). The longest
 * is 18 characters.
 *
 * \li [6] Numeric_Type and Numeric_Value
 *
 * If the character has the property value `Numeric_Type=Decimal`, then
 * the `Numeric_Value` of that digit is represented with an integer value
 * (limited to the range 0..9) in fields 6, 7, and 8. Characters with
 * the property value `Numeric_Type=Decimal` are restricted to digits
 * which can be used in a decimal radix positional numeral system and
 * which are encoded in the standard in a contiguous ascending range
 * 0..9. See the discussion of decimal digits in Chapter 4, Character
 * Properties in [Unicode](https://www.unicode.org/reports/tr41/tr41-26.html#Unicode).
 *
 * \li [7] Numeric_Type and Numeric_Value
 *
 * If the character has the property value `Numeric_Type=Digit`, then the
 * `Numeric_Value` of that digit is represented with an integer value
 * (limited to the range 0..9) in fields 7 and 8, and field 6 is null.
 * This covers digits that need special handling, such as the
 * compatibility superscript digits.
 *
 * Starting with Unicode 6.3.0, no newly encoded numeric characters will
 * be given `Numeric_Type=Digit`, nor will existing characters with
 * `Numeric_Type=Numeric` be changed to `Numeric_Type=Digit`. The
 * distinction between those two types is not considered useful.
 *
 * \li [8] Numeric_Type and Numeric_Value
 *
 * If the character has the property value `Numeric_Type=Numeric`, then
 * the `Numeric_Value` of that character is represented with a positive or
 * negative integer or rational number in this field, and fields 6 and 7
 * are null. This includes fractions such as, for example, "1/5" for
 * `U+2155 VULGAR FRACTION ONE FIFTH`.
 *
 * Some characters have these properties based on values from the Unihan
 * data files. See [Numeric_Type, Han](https://www.unicode.org/reports/tr44/#Numeric_Type_Han).
 *
 * \li [9] Bidi_Mirrored
 *
 * If the character is a "mirrored" character in bidirectional text, this
 * field has the value "Y"; otherwise "N". See Section 4.7, Bidi Mirrored of
 * [Unicode](https://www.unicode.org/reports/tr41/tr41-26.html#Unicode).
 * Do not confuse this with the `Bidi_Mirroring_Glyph` property.
 *
 * \li [10] Unicode_1_Name (Obsolete as of 6.2.0)
 *
 * Old name as published in Unicode 1.0 or ISO 6429 names for control
 * functions. This field is empty unless it is significantly different
 * from the current name for the character. No longer used in code chart
 * production. See [Name_Alias](https://www.unicode.org/reports/tr44/#Name_Alias).
 *
 * See NameAliases.txt instead.
 *
 * \li [11] ISO_Comment (Obsolete as of 5.2.0)
 *
 * ISO 10646 comment field. It was used for notes that appeared in
 * parentheses in the 10646 names list, or contained an asterisk to mark
 * an Annex P note.
 *
 * As of Unicode 5.2.0, this field no longer contains any non-null values.
 *
 * \li [12] Simple_Uppercase_Mapping
 *
 * Simple uppercase mapping (single character result). If a character is
 * part of an alphabet with case distinctions, and has a simple uppercase
 * equivalent, then the uppercase equivalent is in this field. The simple
 * mappings have a single character result, where the full mappings may
 * have multi-character results. For more information, see 
 * [Case and Case Mapping](https://www.unicode.org/reports/tr44/#Casemapping).
 *
 * \li [13] Simple_Lowercase_Mapping
 *
 * Simple lowercase mapping (single character result).
 *
 * \li [14] Simple_Titlecase_Mapping
 *
 * Simple titlecase mapping (single character result).
 *
 * Note: If this field is null, then the `Simple_Titlecase_Mapping` is the
 * same as the `Simple_Uppercase_Mapping` for this character.
 *
 * \sa https://www.unicode.org/reports/tr44/#UnicodeData.txt
 * \sa https://www.unicode.org/ucd/
 * \sa https://www.unicode.org/Public/UCD/latest/
 * \sa http://www.unicode.org/Public/5.1.0/ucd/UCD.html#UnicodeData.txt (version 5.1.0)
 * \sa http://www.unicode.org/L2/L1999/UnicodeData.html (version 2)
 */


// self
//
#include    "libutf8/unicode_data_file.h"

#include    "libutf8/exception.h"


// snapdev
//
#include    <snapdev/file_contents.h>


// C++
//
#include    <fstream>
#include    <iostream>
#include    <list>
#include    <map>
#include    <sstream>
#include    <vector>


// C
//
#include    <stdlib.h>
#include    <unistd.h>


// last include
//
#include    <snapdev/poison.h>



namespace libutf8
{


namespace detail
{

/** \brief Character definition used if we can't find a code.
 *
 * Whenever you try to get a character code, if it can't be found, then
 * this entry is used instead.
 *
 * The code of the invalid character is set to libutf8::NOT_A_CHARACTER.
 */
constexpr ucd_character const       g_invalid_character = ucd_character();



class raw_character
{
public:
    typedef std::map<char32_t, raw_character>
                        map_t;
    typedef std::list<std::string>
                        name_list_t;
    typedef std::list<char32_t>
                        decomposition_t;

                        raw_character();
                        raw_character(std::string const & code);
                        raw_character(raw_character const &) = default;
    raw_character &     operator = (raw_character const &) = default;

    char32_t            code() const;

    void                set_name(std::string const & name);
    void                correct_name(std::string const & proper_name);
    void                add_abbreviation(std::string const & abbreviation);
    void                add_alternate(std::string const & alternate);
    void                add_jamo_short_name(std::string const & jamo_short_name);
    void                add_control(std::string const & control);
    void                add_figment(std::string const & figment);
    void                set_number(std::string const & number);
    void                set_age(int major_unicode, int minor_unicode);
    void                set_category(std::string const & category);
    void                set_combining_class(std::string const & combining);
    void                set_bidi_class(std::string const & bidi);
    void                set_decomposition(std::string const & decomposition);

private:
    char32_t            f_code = NOT_A_CHARACTER;
    std::string         f_name = std::string();
    std::string         f_wrong_name = std::string();
    name_list_t         f_abbreviations = name_list_t();
    name_list_t         f_alternates = name_list_t();
    name_list_t         f_jamo_short_names = name_list_t();
    name_list_t         f_figments = name_list_t();
    int64_t             f_nominator = 0;
    int64_t             f_denominator = 0;
    char                f_age[2] = { 0, 0};
    General_Category    f_general_category = General_Category::GC_Unassigned;
    Canonical_Combining_Class
                        f_canonical_combing_class = Canonical_Combining_Class::CCC_Not_Reordered;
    Bidi_Class          f_bidi_class = Bidi_Class::BC_Unknown;
    Decomposition_Type  f_decomposition_type = Decomposition_Type::DT_unknown;
    decomposition_t     f_decomposition = decomposition_t();
};


raw_character::raw_character()
{
}


raw_character::raw_character(std::string const & code)
    : f_code(std::stoi(code, nullptr, 16))
{
}


char32_t raw_character::code() const
{
    return f_code;
}


void raw_character::set_name(std::string const & name)
{
    if(!f_name.empty())
    {
        throw libutf8_logic_exception("attempting to redefine the character name");
    }
    f_name = name;
}


void raw_character::correct_name(std::string const & proper_name)
{
    if(!proper_name.empty())
    {
        if(f_name.empty())
        {
            throw libutf8_logic_exception("attempting to correct an empty character name");
        }
        f_wrong_name = f_name;
        f_name = proper_name;
    }
}


void raw_character::add_abbreviation(std::string const & abbreviation)
{
    if(!abbreviation.empty())
    {
        f_abbreviations.push_back(abbreviation);
    }
}


void raw_character::add_alternate(std::string const & alternate)
{
    if(!alternate.empty())
    {
        f_alternates.push_back(alternate);
    }
}


void raw_character::add_jamo_short_name(std::string const & jamo_short_name)
{
    if(!jamo_short_name.empty())
    {
        f_abbreviations.push_back(jamo_short_name);
    }
}


void raw_character::add_control(std::string const & control)
{
    if(!control.empty())
    {
        if(f_name == "<control>")
        {
            f_name = control;
        }
        else
        {
            f_alternates.push_back(control);
        }
    }
}


void raw_character::add_figment(std::string const & figment)
{
    if(!figment.empty())
    {
        f_figments.push_back(figment);
    }
}


void raw_character::set_number(std::string const & number)
{
    if(number.empty())
    {
        return;
    }

    if(f_denominator != 0)
    {
        throw libutf8_exception_twice("set_number() called twice");
    }

    std::string::size_type const pos(number.find('/'));
    if(pos == std::string::npos)
    {
        f_nominator = std::stoi(number, nullptr, 10);
        f_denominator = 1;
    }
    else
    {
        f_nominator = std::stoi(number.substr(0, pos), nullptr, 10);
        f_denominator = std::stoi(number.substr(pos + 1), nullptr, 10);
    }
}


void raw_character::set_age(int major_unicode, int minor_unicode)
{
    if(f_age[0] != 0 || f_age[1] != 0)
    {
        throw libutf8_exception_twice("age defined twice");
    }
    f_age[0] = major_unicode;
    f_age[1] = minor_unicode;
}


void raw_character::set_category(std::string const & category)
{
    if(category.length() != 2)
    {
        throw libutf8_exception_invalid_parameter("category name is expected to be exactly two letters.");
    }

    if(f_general_category != General_Category::GC_Unassigned)
    {
        throw libutf8_exception_twice("trying to set the general category twice.");
    }

    switch(category[0])
    {
    case 'L':
        switch(category[1])
        {
        case 'u':
            f_general_category = General_Category::GC_Uppercase_Letter;
            break;

        case 'l':
            f_general_category = General_Category::GC_Lowercase_Letter;
            break;

        case 't':
            f_general_category = General_Category::GC_TitleCase_Letter;
            break;

        case 'C':
            f_general_category = General_Category::GC_Cased_Letter;
            break;

        case 'm':
            f_general_category = General_Category::GC_Modified_Letter;
            break;

        case 'o':
            f_general_category = General_Category::GC_Other_Letter;
            break;

        }
        break;

    case 'M':
        switch(category[1])
        {
        case 'n':
            f_general_category = General_Category::GC_Nonspacing_Mark;
            break;

        case 'c':
            f_general_category = General_Category::GC_Spacing_Mark;
            break;

        case 'e':
            f_general_category = General_Category::GC_Enclosing_Mark;
            break;

        }
        break;

    case 'N':
        switch(category[1])
        {
        case 'd':
            f_general_category = General_Category::GC_Decimal_Number;
            break;

        case 'l':
            f_general_category = General_Category::GC_Letter_Number;
            break;

        case 'o':
            f_general_category = General_Category::GC_Other_Number;
            break;

        }
        break;

    case 'P':
        switch(category[1])
        {
        case 'c':
            f_general_category = General_Category::GC_Connector_Punctuation;
            break;

        case 'd':
            f_general_category = General_Category::GC_Dash_Punctuation;
            break;

        case 's':
            f_general_category = General_Category::GC_Open_Punctuation;
            break;

        case 'e':
            f_general_category = General_Category::GC_Close_Punctuation;
            break;

        case 'i':
            f_general_category = General_Category::GC_Initial_Punctuation;
            break;

        case 'f':
            f_general_category = General_Category::GC_Final_Punctuation;
            break;

        case 'o':
            f_general_category = General_Category::GC_Other_Punctuation;
            break;

        }
        break;

    case 'S':
        switch(category[1])
        {
        case 'm':
            f_general_category = General_Category::GC_Math_Symbol;
            break;

        case 'c':
            f_general_category = General_Category::GC_Current_Symbol;
            break;

        case 'k':
            f_general_category = General_Category::GC_Modifier_Symbol;
            break;

        case 'o':
            f_general_category = General_Category::GC_Other_Symbol;
            break;

        }
        break;

    case 'Z':
        switch(category[1])
        {
        case 's':
            f_general_category = General_Category::GC_Space_Separator;
            break;

        case 'l':
            f_general_category = General_Category::GC_Line_Separator;
            break;

        case 'p':
            f_general_category = General_Category::GC_Paragraph_Separator;
            break;

        }
        break;

    case 'C':
        switch(category[1])
        {
        case 'c':
            f_general_category = General_Category::GC_Control;
            break;

        case 'f':
            f_general_category = General_Category::GC_Format;
            break;

        case 's':
            f_general_category = General_Category::GC_Surrogate;
            break;

        case 'o':
            f_general_category = General_Category::GC_Private_Use;
            break;

        case 'n':
            f_general_category = General_Category::GC_Unassigned;
            break;

        }
        break;

    }

    if(f_general_category == General_Category::GC_Unassigned)
    {
        throw libutf8_exception_unsupported(
              "unknown general category \""
            + category
            + "\".");
    }
}


void raw_character::set_combining_class(std::string const & combining)
{
    // the numbers match one to one
    //
    f_canonical_combing_class = static_cast<Canonical_Combining_Class>(std::stoi(combining, nullptr, 10));
}


void raw_character::set_bidi_class(std::string const & bidi)
{
    if(bidi.empty())
    {
        throw libutf8_exception_invalid_parameter("bidi class name is expected to be at least one letter.");
    }

    if(f_bidi_class != Bidi_Class::BC_Unknown)
    {
        throw libutf8_exception_twice("trying to set the bidi class twice.");
    }

    switch(bidi[0])
    {
    case 'A':
        if(bidi.length() == 2)
        switch(bidi[1])
        {
        case 'L':
            f_bidi_class = Bidi_Class::BC_Arabic_Letter;
            break;

        case 'N':
            f_bidi_class = Bidi_Class::BC_Arabic_Number;
            break;

        }
        break;

    case 'B':
        if(bidi.length() == 1)
        {
            f_bidi_class = Bidi_Class::BC_Paragraph_Separator;
        }
        else if(bidi.length() == 2
             && bidi[1] == 'N')
        {
            f_bidi_class = Bidi_Class::BC_Boundary_Neutral;
        }
        break;

    case 'C':
        if(bidi.length() == 2
        && bidi[1] == 'S')
        {
            f_bidi_class = Bidi_Class::BC_Common_Separator;
        }
        break;

    case 'E':
        if(bidi.length() == 2)
        switch(bidi[1])
        {
        case 'N':
            f_bidi_class = Bidi_Class::BC_European_Number;
            break;

        case 'S':
            f_bidi_class = Bidi_Class::BC_European_Separator;
            break;

        case 'T':
            f_bidi_class = Bidi_Class::BC_European_Terminator;
            break;

        }
        break;

    case 'F':
        if(bidi.length() == 3
        && bidi[1] == 'S'
        && bidi[2] == 'I')
        {
            f_bidi_class = Bidi_Class::BC_First_Strong_Isolate;
        }
        break;

    case 'L':
        if(bidi.length() == 1)
        {
            f_bidi_class = Bidi_Class::BC_Left_To_Right;
        }
        else if(bidi.length() == 3
             && bidi[1] == 'R')
        {
            switch(bidi[2])
            {
            case 'E':
                f_bidi_class = Bidi_Class::BC_Left_To_Right_Embedding;
                break;

            case 'O':
                f_bidi_class = Bidi_Class::BC_Left_To_Right_Override;
                break;

            case 'I':
                f_bidi_class = Bidi_Class::BC_Left_To_Right_Isolate;
                break;

            }
        }
        break;

    case 'N':
        if(bidi.length() == 3
        && bidi[1] == 'S'
        && bidi[2] == 'M')
        {
            f_bidi_class = Bidi_Class::BC_Nonspacing_Mark;
        }
        break;

    case 'O':
        if(bidi.length() == 2
        && bidi[1] == 'N')
        {
            f_bidi_class = Bidi_Class::BC_Other_Neutral;
        }
        break;

    case 'P':
        if(bidi.length() == 3
        && bidi[1] == 'D')
        {
            switch(bidi[2])
            {
            case 'F':
                f_bidi_class = Bidi_Class::BC_Pop_Directional_Format;
                break;

            case 'I':
                f_bidi_class = Bidi_Class::BC_Pop_Directional_Isolate;
                break;

            }
        }
        break;

    case 'R':
        if(bidi.length() == 1)
        {
            f_bidi_class = Bidi_Class::BC_Right_To_Left;
        }
        else if(bidi.length() == 3
             && bidi[1] == 'L')
        {
            switch(bidi[2])
            {
            case 'E':
                f_bidi_class = Bidi_Class::BC_Right_To_Left_Embedding;
                break;

            case 'O':
                f_bidi_class = Bidi_Class::BC_Right_To_Left_Override;
                break;

            case 'I':
                f_bidi_class = Bidi_Class::BC_Right_To_Left_Isolate;
                break;

            }
        }
        break;

    case 'S':
        if(bidi.length() == 1)
        {
            f_bidi_class = Bidi_Class::BC_Segment_Separator;
        }
        break;

    case 'W':
        if(bidi.length() == 2
        && bidi[1] == 'S')
        {
            f_bidi_class = Bidi_Class::BC_White_Space;
        }
        break;

    }

    if(f_bidi_class == Bidi_Class::BC_Unknown)
    {
        throw libutf8_exception_unsupported(
              "unknown general bidi \""
            + bidi
            + "\".");
    }
}


void raw_character::set_decomposition(std::string const & decomposition)
{
    if(f_decomposition_type != Decomposition_Type::DT_none)
    {
        throw libutf8_exception_twice("set_decomposition() called twice");
    }

    if(decomposition.empty())
    {
        f_decomposition_type = Decomposition_Type::DT_none;
        return;
    }

    std::string decomp;
    if(decomposition[0] == '<')
    {
        std::string::size_type const pos(decomposition.find('>'));
        if(pos == std::string::npos)
        {
            throw libutf8_exception_invalid_parameter("a decomposition type must end with '>'.");
        }
        std::string const type(decomposition.substr(1, pos - 1));
        if(type.empty())
        {
            throw libutf8_exception_invalid_parameter("a decomposition type cannot be empty '<>'.");
        }
        switch(type[0])
        {
        case 'c':
            if(type == "circle")
            {
                f_decomposition_type = Decomposition_Type::DT_circle;
            }
            if(type == "compat")
            {
                f_decomposition_type = Decomposition_Type::DT_compat;
            }
            break;

        case 'f':
            if(type == "final")
            {
                f_decomposition_type = Decomposition_Type::DT_final;
            }
            if(type == "font")
            {
                f_decomposition_type = Decomposition_Type::DT_font;
            }
            if(type == "fraction")
            {
                f_decomposition_type = Decomposition_Type::DT_fraction;
            }
            break;

        case 'i':
            if(type == "initial")
            {
                f_decomposition_type = Decomposition_Type::DT_initial;
            }
            if(type == "isolated")
            {
                f_decomposition_type = Decomposition_Type::DT_isolated;
            }
            break;

        case 'm':
            if(type == "medial")
            {
                f_decomposition_type = Decomposition_Type::DT_medial;
            }
            break;

        case 'n':
            if(type == "narrow")
            {
                f_decomposition_type = Decomposition_Type::DT_narrow;
            }
            if(type == "noBreak")
            {
                f_decomposition_type = Decomposition_Type::DT_noBreak;
            }
            break;

        case 's':
            if(type == "small")
            {
                f_decomposition_type = Decomposition_Type::DT_small;
            }
            if(type == "square")
            {
                f_decomposition_type = Decomposition_Type::DT_square;
            }
            if(type == "sub")
            {
                f_decomposition_type = Decomposition_Type::DT_sub;
            }
            if(type == "super")
            {
                f_decomposition_type = Decomposition_Type::DT_super;
            }
            break;

        case 'v':
            if(type == "vertical")
            {
                f_decomposition_type = Decomposition_Type::DT_vertical;
            }
            break;

        case 'w':
            if(type == "wide")
            {
                f_decomposition_type = Decomposition_Type::DT_wide;
            }
            break;

        }

        decomp = decomposition.substr(pos + 1);
    }
    else
    {
        decomp = decomposition;
    }
}





class parser_impl
{
public:
    void                set_input_dir(std::string const & dir);
    void                set_output_filename(std::string const & filename);

    void                parse();

private:
    typedef std::list<std::string>
                        lines_t;
    typedef std::vector<std::string>
                        fields_t;
    struct range_t
    {
        char32_t        f_start = 0;
        char32_t        f_end = 0;
    };

    void                clear_output();
    void                create_output();
    fields_t            parse_fields(std::string const & line);
    range_t             parse_range(std::string const & code);
    void                remove_comments();
    void                read_file(std::string const & filename);
    void                read_unicode_data();
    void                convert_unicode_data();
    void                read_name_aliases();
    void                convert_name_aliases();
    void                read_jamo();
    void                convert_jamo();
    void                read_derived_age();
    void                convert_derived_age();

    std::string         f_input_dir = std::string();
    std::string         f_output_filename = std::string();
    lines_t             f_lines = lines_t();
    std::shared_ptr<snapdev::file_contents>
                        f_output = std::shared_ptr<snapdev::file_contents>();
    raw_character::map_t
                        f_characters = raw_character::map_t();
};


void parser_impl::set_input_dir(std::string const & dir)
{
    f_input_dir = dir;
}


void parser_impl::set_output_filename(std::string const & filename)
{
    f_output_filename = filename;
}


void parser_impl::parse()
{
    clear_output();
    create_output();

    read_unicode_data();
    convert_unicode_data();

    read_name_aliases();
    convert_name_aliases();

    read_jamo();
    convert_jamo();

    read_derived_age();
    convert_derived_age();
}


void parser_impl::clear_output()
{
    // remove output so make fails if the parser fails
    //
    unlink(f_output_filename.c_str());
}


void parser_impl::create_output()
{
    f_output = std::make_shared<snapdev::file_contents>(f_output_filename);
}


parser_impl::fields_t parser_impl::parse_fields(std::string const & line)
{
    fields_t fields;
    snapdev::tokenize_string(
          fields
        , line
        , ";"
        , false
        , " \t");
    return fields;
}


parser_impl::range_t parser_impl::parse_range(std::string const & field)
{
    range_t result;
    std::string::size_type const pos(field.find(".."));
    if(pos == std::string::npos)
    {
        result.f_start = stoi(field, nullptr, 16);
        result.f_end = result.f_start;
    }
    else
    {
        result.f_start = std::stoi(field.substr(0, pos), nullptr, 16);
        result.f_end = std::stoi(field.substr(pos + 2), nullptr, 16);
    }
    return result;
}



/** \brief Remove commented lines.
 *
 * All the Unicode files support comments introduced by the '#' character.
 *
 * If a '#' is found in a string, the '#' and anything after it gets removed.
 * If the resulting line is empty, it gets removed from the list.
 */
void parser_impl::remove_comments()
{
    for(auto it(f_lines.begin());
        it != f_lines.end();
        )
    {
        std::string::size_type pos(it->find('#'));
        if(pos != std::string::npos)
        {
            *it = it->substr(0, pos);
        }

        if(it->empty())
        {
            it = f_lines.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void parser_impl::read_file(std::string const & filename)
{
    snapdev::file_contents input(f_input_dir + "/" + filename);
    if(!input.read_all())
    {
        std::string const msg(
              "error: could not read input file \""
            + input.filename()
            + "\".");
        std::cerr << msg << "\n";
        throw libutf8_exception_io(msg);
    }

    f_lines.clear();
    snapdev::tokenize_string(f_lines, input.contents(), "\n", true);

    remove_comments();
}


void parser_impl::read_unicode_data()
{
    read_file("UnicodeData.txt");
}


void parser_impl::convert_unicode_data()
{
    fields_t start_range;
    for(auto & l : f_lines)
    {
        fields_t const fields(parse_fields(l));
        if(fields.size() != 15)
        {
            // all the lines are expected to include all the fields
            //
            std::string msg("error: found "
                + std::to_string(fields.size())
                + " fields instead of 15.");
            std::cerr << msg << "\n";
            throw libutf8_exception_unsupported(msg);
        }

        if(fields[1].length() >= 3
        && fields[1].front() == '<'
        && fields[1].back() == '>')
        {
            fields_t special_name;
            snapdev::tokenize_string(
                  special_name
                , fields[1].substr(1, fields[1].length() - 2)
                , ","
                , false
                , " \t");
            if(special_name.size() == 2)
            {
                if(special_name[1] == "First")
                {
                    start_range = fields;
                    continue;
                }
                if(special_name[1] == "Last")
                {
                    // got a range
                    //
                    if(start_range.empty())
                    {
                        std::string const msg(
                              "error: found an end of range without a start in \""
                            + f_input_dir
                            + "/UnicodeData.txt\" "
                            + l
                            + ".");
                        std::cerr << msg << "\n";
                        throw libutf8_exception_io(msg);
                    }
std::cerr << "TODO: range " << special_name[0] << " -> " << start_range[0] << ".." << fields[0] << " not implemented yet...\n";
                    start_range.clear();
                    continue;
                }
            }
            // others go through (as far as I know, only "<control>")
            //
            std::cout << "keeping special name [" << fields[1] << "]\n";
        }

        raw_character c(fields[0]);
        c.set_name(fields[1]);
        c.set_category(fields[2]);
        c.set_combining_class(fields[3]);
        c.set_bidi_class(fields[4]);
        f_characters[c.code()] = c;
    }
}


void parser_impl::read_name_aliases()
{
    read_file("NameAliases.txt");
}


void parser_impl::convert_name_aliases()
{
    for(auto & l : f_lines)
    {
        fields_t const fields(parse_fields(l));
        if(fields.size() != 3)
        {
            // all the lines are expected to include all the fields
            //
            std::string msg("error: found "
                + std::to_string(fields.size())
                + " fields instead of 3 in NameAliases.txt file.");
            throw libutf8_exception_unsupported(msg);
        }

        char32_t const code(std::stoi(fields[0], nullptr, 16));

        auto it(f_characters.find(code));
        if(it == f_characters.end())
        {
            std::stringstream ss;
            ss << "character U+"
               << std::hex << std::uppercase << static_cast<int32_t>(code)
               << " referenced in NameAliases.txt missing in UnicodeData.txt (line: "
               << l
               << ").";
            throw libutf8_exception_missing(ss.str());
        }

        if(fields[2] == "correction")
        {
            it->second.correct_name(fields[1]);
        }
        else if(fields[2] == "control")
        {
            it->second.add_control(fields[1]);
        }
        else if(fields[2] == "alternate")
        {
            it->second.add_alternate(fields[1]);
        }
        else if(fields[2] == "figment")
        {
            it->second.add_figment(fields[1]);
        }
        else if(fields[2] == "abbreviation")
        {
            it->second.add_abbreviation(fields[1]);
        }
        else
        {
            throw libutf8_exception_unsupported(
                  "unsupport alias type \""
                + fields[2]
                + "\".");
        }
    }
}


void parser_impl::read_jamo()
{
    read_file("Jamo.txt");
}


void parser_impl::convert_jamo()
{
    for(auto & l : f_lines)
    {
        fields_t const fields(parse_fields(l));
        if(fields.size() != 2)
        {
            // all the lines are expected to include all the fields
            //
            std::string msg("error: found "
                + std::to_string(fields.size())
                + " fields instead of 3 in Jamo.txt file.");
            throw libutf8_exception_unsupported(msg);
        }

        char32_t const code(std::stoi(fields[0], nullptr, 16));

        auto it(f_characters.find(code));
        if(it == f_characters.end())
        {
            std::stringstream ss;
            ss << "character U+"
               << std::hex << std::uppercase << static_cast<int32_t>(code)
               << " referenced in Jamo.txt missing in UnicodeData.txt";
            throw libutf8_exception_missing(ss.str());
        }

        it->second.add_jamo_short_name(fields[1]);
    }
}


void parser_impl::read_derived_age()
{
    read_file("DerivedAge.txt");
}


void parser_impl::convert_derived_age()
{
    for(auto & l : f_lines)
    {
        fields_t const fields(parse_fields(l));
        if(fields.size() != 2)
        {
            // all the lines are expected to include all the fields
            //
            std::string msg("error: found "
                + std::to_string(fields.size())
                + " fields instead of 2 in Age file.");
            throw libutf8_exception_unsupported(msg);
        }

        std::string::size_type pos(fields[1].find('.'));
        if(pos == std::string::npos)
        {
            throw libutf8_exception_unsupported("age is expected to be two numbers separated by a period");
        }

        std::string const major_str(fields[1].substr(0, pos));
        int const major_unicode(std::stoi(major_str, nullptr, 10));
        std::string const minor_str(fields[1].substr(pos + 1));
        int const minor_unicode(std::stoi(minor_str, nullptr, 10));

        range_t const range(parse_range(fields[0]));
        for(char32_t code(range.f_start); code <= range.f_end; ++code)
        {
            auto it(f_characters.find(code));
            if(it == f_characters.end())
            {
                std::stringstream ss;
                ss << "character U+"
                   << std::hex << std::uppercase << static_cast<int32_t>(code)
                   << " referenced in DerivedAge.txt missing in UnicodeData.txt";
                throw libutf8_exception_missing(ss.str());
            }
            it->second.set_age(major_unicode, minor_unicode);
        }
    }
}




} // detail namespace




ucd_parser::ucd_parser(
          std::string const & input_dir
        , std::string const & output_filename)
    : f_impl(std::make_shared<detail::parser_impl>())
{
    f_impl->set_input_dir(input_dir);
    f_impl->set_output_filename(output_filename);
}


void ucd_parser::generate()
{
    f_impl->parse();
}




} // libutf8 namespace
// vim: ts=4 sw=4 et
