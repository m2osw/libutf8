
<p align="center">
<img alt="advgetopt" title="Advance getopt, a C++ library to handle your command line options and configuration files seamlessly."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/libutf8.png" width="277" height="277"/>
</p>

# Introduction

The libutf8 library is a helper library to handle UTF-8 strings in C++.
Although C++11 added `char32_t` (and `char16_t`) and C++20 added
`char8_t`, the conversions are still not seamless between each type
(although it is becoming easier to handle such.)

This library proposes automated conversions between `std::string` (viewed
as UTF-8 in nearly all of our code) and `std::u32string` (a.k.a. UTF-32
strings.)

# Reasons Behind Having Our Own Library

All the libraries I've seen are either in C and very cumbersome to use or
offer an interface which depends on the current `LOCALE`. In other words,
the system default `mbstowc()` function, for example, does not always view
the input string as UTF-8. That also means there are complexities and thus
inefficiencies in determining which conversion to use.

In our case, we always have UTF-8 as input and output and at times we need
to handle the characters as UTF-32. For example, to transform the character
to uppercase, it is necessary to have a UTF-32 character.

# API

## String Conversions

The library offers to conversion functions as follow:

    libutf8::to_u8string(std::u32string const & str);
    libutf8::to_u32string(std::string const & str);

As time passes, we will add other conversions so as to support all formats
although at this point these two are the only two we need in Snap! Websites.

Here is an example of usage:

    std::string u8;

    u8 = u8"This is a UTF-8 string";

    std::w32string u32;
    u32 = libutf8::to_u32string(u8);

    std::string back;
    back = libutf8::to_u8string(u32);

Note that u8 string could be _more_ UTF-8 by including characters outside
of the ASCII range and it would still work as you would expect.

### String Length in Characters

The library offers the `u8length()` function which computes the length of
a UTF-8 string. Note that this does not verify whether the UTF-8 data is
valid. It very quickly counts the number of non-continuation bytes (i.e.
bytes between 0x80 and 0xBF inclusive.)

    std::string u8("Your UTF-8 string");
    size_t length = libutf8::u8length(u8);

### Case Insensitive Compare

In most cases, you can compare two UTF-8 strings with the normal `==`
operator. Once in a while, though, you may want to compare them case
insensitively.

Like with the iterator below, we wanted to offer a function that allows
you to compare two UTF-8 strings properly and as quickly as possible.
This meant to not have to convert the entire strings before doing the
compare because having to do so means allocating memory for both
strings just to do the compare and the conversion would convert the
entire strings instead of just what's necessary.

Out of these constraints we created  the `u8casecmp()` function. It
takes two UTF-8 strings and compares the characters one at a time.
Unless the strings are equal, only the number of characters up to
the first non-equal one, will be converted.

    std::string a("First String");
    std::string b("First Test");

    int r(libutf8::u8casecmp(a, b));
    if(r == 0)
    {
    	std::cout << "a and b are equal" << std::endl;
    }
    else if(r < 0)
    {
    	std::cout << "a comes before b" << std::endl;
    }
    else //if(r > 0)
    {
    	std::cout << "a comes after b" << std::endl;
    }

WARNING: the function does no collation, so it is not going to take the
language in account. It uses lowercase characters, as suggested by the
Unicode standard, but outside of that, the compare is binary.

## UTF-8 Iterator

It is often that we have an `std::string` representing UTF-8 and we want
to iterate the content as UTF-32 characters. Although we could convert
the string to a full `std::u32string` and then iterate through the
`std::u32string`, that (1) requires a copy and (2) uses four times
the amount of memory (five times if you include the `std::string` size...)
Note also that the copy requires a `malloc()` and later a `free()` once
done with it.

The iterator solves these problems by allowing us to iterate through the
`std::string` and getting the next or previous Unicode character without
having to use any more memory. The conversion itself is slightly slower
than converting a string all at once, but doing a `malloc()` to get the
`std::u32string` is definitely going to be way slower than our iterator
in nearly all circumstances.

The following example shows the code point of each character, one per line:

    std::string u8("This is your UTF-8 string");

    for(libutf8::utf8_iterator it(u8);
    	it != u8.end();
	++it)
    {
    	std::cout << static_cast<int>(*it) << std::endl;
    }

You can compare standard `std::string` iterators with `==` and `!=`. The
`++` and `--` operators work as expected. If you do a `++` when already
at the end, nothing happens. If you do a `--` when already at the beginning,
nothing happens.

Once you are at the end, getting the character (`*it`) returns `libutf8::EOS`.
So you can loop through until you get `libutf8::EOS` instead of checking
against the end iterator:

    std::string u8("This is your UTF-8 string");

    libutf8::utf8_iterator it(u8);
    while(*it != libutf8::EOS)
    {
    	std::cout << static_cast<int>(*it++) << std::endl;
    }

Remember that a good optimization is to avoid the post increment. It will
be faster to do:

    char32_t c = *it;
    ++it;

because you avoid a copy of the iterator (even though it's only 16 bytes...)

## Low Level Functions

We expose the low level functions such as `mbstowc()` for edgy cases where
you may not have an `std::string`. Those functions should not be used if
at all possible because they require proper handling of the buffers passed
to them. An error to such and you could end up with a crashing bug in your
code.

# TODO

## Auto-Conversions

Conversions for many more types of strings such as all the `char *`
and also look into whether implementing an extension to the
`std::basic_string` would be possible to directly have conversions
integrated in our strings (i.e. to be able to write `str8 = str32;` and
`str32 = str8` without having to write `str8 = libutf8::to_u8string(str32)`.)

## Canonicalization

Right now, we do not try to canonicalize the strings, so diacritics may
appear as standalone or combined characters. We want to implement the
necessary code to decomposed and re-composed in a normalized manner.

This is very important for comparing strings against each other for
equality (i.e. an 'a' with a grave accent is equal to an 'a' followed
by the grave accent character).

## Character Name, Type, etc.

The UnicodeData.txt file (offered by the Unicode website) lists all the
characters with their name and their types. We want to offer the user
access to that data.

We should simple have the table as a struct and return a pointer to
the corresponding character. Sort those by character number and use
a binary search to find the structure.

Some of that information is to be used for the canonicalization so it
is a must have.

UnicodeData.txt file format is defined in:
http://www.unicode.org/L2/L1999/UnicodeData.html



# License

The source is covered by the MIT license. The debian folder is covered
by the GPL 2.0.


# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/libutf8/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
