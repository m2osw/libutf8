
<p align="center">
<img alt="advgetopt" title="Advance getopt, a C++ library to handle your command line options and configuration files seamlessly."
src="https://snapwebsites.org/sites/snapwebsites.org/files/images/advgetopt-277x277.png" width="277" height="277"/>
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

## UTF-8 Iterator

It is often that we have an `std::string` and we want to iterate the content
as UTF-8 characters. Although we could convert the string to an
`std::u32string` and then iterate the `std::u32string`, that (1) requires a
copy and (2) uses four times the amount of memory (five times if you include
the `std::string`.)

The iterator solves this problem by allowing us to iterate through the
`std::string` and getting the next or previous Unicode character without
having to use more memory. It is slightly slower than converting a string
all at once, but doing a `malloc()` to get the `std::u32string` is definitely
going to be way slower than our iterator.

## Low Level Functions

We expose the low level functions such as `mbstowc()` for edgy cases where
you may not have an `std::string`. Those functions should not be used if
at all possible because they require proper handling of the buffers passed
to them. An error to such and you could end up with a crashing bug in your
code.

# TODO

We'd like add support for UTF-16 and UCS-2.

Also conversions for many more types of strings such as all the `char *`
and also look into whether implementing an extension to the
`std::basic_string` would be possible to directly have conversions
integrated in our strings (i.e. to be able to write `str8 = str32;` and
`str32 = str8` without having to write `str8 = libutf8::to_u8string(str32)`.)


# License

The source is covered by the MIT license. The debian folder is covered
by the GPL 2.0.


# Bugs

Submit bug reports and patches on
[github](https://github.com/m2osw/libutf8/issues).


_This file is part of the [snapcpp project](https://snapwebsites.org/)._
