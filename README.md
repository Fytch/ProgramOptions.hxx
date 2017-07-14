# ProgramOptions.hxx

[![Build Status](https://travis-ci.org/Fytch/ProgramOptions.hxx.svg?branch=master)](https://travis-ci.org/Fytch/ProgramOptions.hxx)
[![Build Status](https://ci.appveyor.com/api/projects/status/0tidhvat904wagc3/branch/master?svg=true)](https://ci.appveyor.com/project/Fytch/programoptions-hxx)
[![GitHub License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE.txt)

## Contents
- [Getting started](#getting-started)
- [Design goals](#design-goals)
- [Features](#features)
  - [Syntax](#syntax)
- [Integration](#integration)
- [Usage](#usage)
  - [Example 1 (`abbreviation`, `u32`, `available`, `get`)](#example-1-abbreviation-u32-available-get)
  - [Example 2 (`fallback`, `was_set`, `string`, `multi`)](#example-2-fallback-was_set-string-multi)
  - [Example 3 (`description`, `callback`, unnamed parameter)](#example-3-description-callback-unnamed-parameter)
  - [Example 4 (more `callback`s, more `fallback`s, `f64`, `to_vector`)](#example-4-more-callbacks-more-fallbacks-f64-to_vector)
  - [Miscellaneous functions](#miscellaneous-functions)
- [Defaults](#defaults)
- [Flags](#flags)
- [Third-party libraries](#third-party-libraries)
- [License](#license)

## Getting started
It is highly recommended that you at least briefly skim through the chapters [**Integration**](#integration) and [**Usage**](#usage) before starting.

The quickest way to get started is to download *ProgramOptions.hxx* as well as one of the samples and go from there.

### `sample.cxx`
The default choice. Using *ProgramOptions.hxx* incorrectly or failing to meet a function's preconditions will throw an exception which will, by default, terminate the program and display a useful message, explaining where exactly things went wrong. If you don't want the program to be terminated, you may provide your own `try` and `catch` blocks to deal with [`std::logic_error`s](http://en.cppreference.com/w/cpp/error/logic_error).

### `sample_noexcept.cxx`
Using this sample is only recommended if you are already somewhat familiar with *ProgramOptions.hxx*. Incorrect programs will crash without any messages unless your STL implementation does so when [`assert`ions](http://en.cppreference.com/w/cpp/error/assert) fail.

## Design goals
- **Non-intrusive**. Unlike other program option libraries, such as [*Boost.Program_options*](http://www.boost.org/doc/libs/1_63_0/doc/html/program_options.html), *ProgramOptions.hxx* requires neither additional library binaries nor integration into the build process. Just drop in the header, include it and you're all set. *ProgramOptions.hxx* doesn't force you to enable exceptions or RTTI and runs just fine with `-fno-rtti -fno-exceptions` (the latter requires you to [`#define PROGRAMOPTIONS_NO_EXCEPTIONS`](#define-programoptions_no_exceptions) prior to including the header, though).
- **Intuitive**. *ProgramOptions.hxx* is designed to feel smooth and blend in well with other modern C++11 code.
- **Correct**. Extensive unit tests and runtime checks contribute to more correct software, both on the side of the user and the developer of *ProgramOptions.hxx*.
- **Permissive**. The [MIT License](https://tldrlegal.com/license/mit-license) under which *ProgramOptions.hxx* is published grants unrestricted freedom.

## Features
![Screenshot](https://raw.githubusercontent.com/Fytch/ProgramOptions.hxx/master/assets/screenshot.png)

- Fully compliant with the [GNU Program Argument Syntax Conventions](https://www.gnu.org/software/libc/manual/html_node/Argument-Syntax.html)
- Automatic help screen generation
- Automatic error handling
- Spelling suggestions
- Colored console output (can be turned off with [`#define PROGRAMOPTIONS_NO_COLORS`](#define-programoptions_no_colors))

### Syntax
*ProgramOptions.hxx* adheres to the following rules:

- **Short options** (`-g`) consist of a single hyphen and an identifying character. The character must be graphic ([`isgraph`](http://en.cppreference.com/w/cpp/string/byte/isgraph)).
  - To pass an **argument** to the option, these syntaxes are allowed: `-O3`, `-O=3`, `-O 3`
  - Multiple short options may be grouped; `-alt` is equal to `-a -l -t`. This is only allowed if all options (including the last) don't require an argument.
- **Long options** (`--version`) consist of two leading hyphens followed by an identifier. The identifier may consist of alphanumeric ([`isalnum`](http://en.cppreference.com/w/cpp/string/byte/isalnum)) characters, hyphens, and underscores but must not start with a hyphen.<sup>1</sup>
  - To pass an **argument** to the option, these syntaxes are allowed: `--optimization=3`, `--optimization 3`
- **Non-option arguments** (`file.txt`), also referred to as *operands* in POSIX lingo or *positional arguments*, are everything except options.
- **Short options**, **long options** and **non-option arguments** can be interleaved at will. Their relative position is retained, i.e. *ProgramOptions.hxx* does not reorder them.
- A single hyphen `-` is parsed as a **non-option argument**.
- Two hyphens `--` terminate the option input. Any following arguments are treated as **non-option arguments**, even if they begin with a hyphen.

<sup>1</sup> Note that `-version` (with only a single hyphen) would be interpreted as the option `-v` with the argument `ersion`.

## Integration
*ProgramOptions.hxx* is very easy to integrate. After downloading the header file, all that it takes is a simple:
```cpp
#include "ProgramOptions.hxx"
```
Don't forget to compile with C++11 enabled, i.e. with `-std=c++11`.

## Usage
Using *ProgramOptions.hxx* is straightforward; we'll explain it by means of practical examples. All examples shown here and more can be found in the [/examples](examples) directory, all of which are well-documented.

### Example 1 (`abbreviation`, `u32`, `available`, `get`)
The following snippet is the complete source code for a simple program expecting an integer optimization level.
```cpp
#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
    po::parser parser;
    parser[ "optimization" ]    // corresponds to --optimization
        .abbreviation( 'O' )    // corresponds to -O
        .type( po::u32 );       // expects an unsigned 32-bit integer

    parser( argc, argv );       // parses the command line arguments

    auto&& O = parser[ "optimization" ];
    if( !O.available() )
        std::cout << "no optimization level set!\n";
    else
        std::cout << "optimization level set to " << O.get().u32 << '\n';
}
```
And in action:
```
$ ./optimization
no optimization level set!
$ ./optimization -O2
optimization level set to 2
$ ./optimization -O=0xFF
optimization level set to 255
$ ./optimization -O3 --optimization 1e2
optimization level set to 100
```
### Example 2 (`fallback`, `was_set`, `string`, `multi`)
Let's expand on the previous code. We want it to assume a certain value for the option `optimization` even if the user sets none. This can be achieved through the `.fallback(...)` method. After parsing, the method `.was_set()` tells us whether the option was actually set by the user or fell back on the default value.

Furthermore, we want to implement the option `-I` to let the user specify include paths. Paths should not be converted to any arithmetic type so we simply set the type to `po::string`.

By calling the method `.multi()` we're telling the library to store *all* values, not just the last one. The number of arguments can be retrieved by calling the `.size()` or the `.count()` method. The individual values may be read by means of the iterators returned by `.begin()` and `.end()`. Bear in mind that these iterators point to instances of `po::value`s, so you still need to refer to the member `.string`. One way of avoiding this is to directly use the iterators provided by `.begin<po::string>()` and `.end<po::string>()`. These random-access iterators behave as if they pointed to instances of `std::string`s. For more information, refer to [Example 4](#reading-multi-options).

```cpp
#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
    po::parser parser;
    parser[ "optimization" ]
        .abbreviation( 'O' )
        .type( po::u32 )
        .fallback( 0 );         // if --optimization is not explicitly specified, assume 0

    parser[ "include-path" ]    // corresponds to --include-path
        .abbreviation( 'I' )    // corresponds to -I
        .type( po::string )     // expects a string
        .multi();               // allows multiple arguments for the same option

    parser( argc, argv );

    auto&& O = parser[ "optimization" ];
    // .was_set() reports whether the option was specified by the user or relied on the predefined fallback value.
    std::cout << "optimization level (" << ( O.was_set() ? "manual" : "auto" ) << ") = " << O.get().u32 << '\n';

    auto&& I = parser[ "include-path" ];
    // .size() and .count() return the number of given arguments. Without .multi(), their return value is always <= 1.
    std::cout << "include paths (" << I.size() << "):\n";
    // Here, the non-template .begin() / .end() methods were used. Their value type is po::value,
    // which is not a value in itself but contains the desired values as members, i.e. i.string.
    for( auto&& i : I )
        std::cout << '\t' << i.string << '\n';
}
```
In action:
```
$ ./include -I/usr/include/foo -I "/usr/include/bar" -O3
optimization level (manual) = 3
include paths (2):
        /usr/include/foo
        /usr/include/bar
```

### Example 3 (`description`, `callback`, unnamed parameter)
Up until now, we were missing the infamous `--help` command. While *ProgramOptions.hxx* will take over the tedious work of neatly formatting and displaying the options, it doesn't add a `--help` command automatically. That's up to us and so is adding an apt description for every available option. We may do so by use of the `.description(...)` method.

But how do we accomplish printing the options whenever there's a `--help` command? This is where callbacks come into play. Callbacks are functions that we supply to *ProgramOptions.hxx* to call. After we handed them over, we don't need to worry about invoking them as that's entirely *ProgramOptions.hxx*' job. In the code below, we pass a [lambda](http://en.cppreference.com/w/cpp/language/lambda) whose sole purpose is to print the options. Whenever the corresponding option occurs (`--help` in this case), the callback is invoked.

The *unnamed parameter* `""` is used to process *non-option arguments*. Consider the command line: `gcc -O2 a.c b.c` Here, unlike `-O2`, `a.c` and `b.c` do not belong to an option and neither do they start with a hyphen. They are non-option arguments but they are important nevertheless. In *ProgramOptions.hxx*, you treat the unnamed parameter like any other option. They only differ in their [default settings](#defaults).

Note that, in order to pass arguments starting with a hyphen to the unnamed parameter, you'll have to pass `--` first, signifying that all further arguments are non-option arguments and that they should be passed right to the unnamed parameter without attempting to interpret them.
```cpp
#include <ProgramOptions.hxx>
#include <iostream>

int main( int argc, char** argv ) {
    po::parser parser;
    parser[ "optimization" ]
        .abbreviation( 'O' )
        .description( "set the optimization level (default: -O0)" )
        .type( po::u32 )
        .fallback( 0 );

    parser[ "include-path" ]
        .abbreviation( 'I' )
        .description( "add an include path" )
        .type( po::string )
        .multi();

    parser[ "help" ]            // corresponds to --help
        .abbreviation( '?' )    // corresponds to -?
        .description( "print this help screen" )
        .callback( [ & ]{ std::cout << parser << '\n'; } );
                                // callbacks get invoked when the option occurs

    parser[ "" ]                // the unnamed parameter is used for non-option arguments, i.e. gcc a.c b.c
        // .type( po::string )  // redundant; default for the unnamed parameter
        // .multi()             // redundant; default for the unnamed parameter
        .callback( [ & ]( std::string const& x ){ std::cout << "processed \'" << x << "\' successfully!\n"; } );
                                // as .get_type() == po::string, the callback may take a std::string

    // parsing returns false if at least one error has occurred
    if( !parser( argc, argv ) ) {
        std::cerr << "errors occurred; aborting\n";
        return -1;
    }
    // we don't want to print anything else if the help screen has been displayed
    if( parser[ "help" ].size() )
        return 0;

    std::cout << "processed files: " << parser[ "" ].size() << '\n';

    auto&& O = parser[ "optimization" ];
    std::cout << "optimization level (" << ( O.was_set() ? "manual" : "auto" ) << ") = " << O.get().u32 << '\n';

    auto&& I = parser[ "include-path" ];
    std::cout << "include paths (" << I.size() << "):\n";
    for( auto&& i : I )
        std::cout << '\t' << i.string << '\n';
}
```
How the help screen appears:
```
$ ./files --help
Usage:
  files.exe [arguments...] [options]
Available options:
  -I, --include-path  add an include path
  -?, --help          print this help screen
  -O, --optimization  set the optimization level (default: -O0)
```
In action:
```
$ ./files -I ./include foo.cxx bar.cxx -O3 -- --qux.cxx
processed 'foo.cxx' successfully!
processed 'bar.cxx' successfully!
processed '--qux.cxx' successfully!
processed files: 3
optimization level (manual) = 3
include paths (1):
        ./include
```
### Example 4 (more `callback`s, more `fallback`s, `f64`, `to_vector`)
In this example, we will employ already known mechanics but lay the focus on their versatility.

#### Let's start with callbacks:
- Multiple callbacks are invoked in order of their addition.
- For an option of type `po::f64`, the possible parameters of a callback are: <*none*>, `std::string`, `po::f64_t`, or any type that is implicitly constructible from these.
- When using C++14, using `auto&&` as a callback's parameter is also possible.

#### About `.fallback(...)`:
- They can be provided in any form that would also suffice when parsed, e.g. as a `std::string`.
- If `.multi()` is set, `.fallback(...)` can take an arbitrary number of values.

#### About `.type(...)`:
- `po::void_`: No value, the option either occurred or not
- `po::string`: Unaltered string
- `po::i32` / `po::i64`: Signed integer; supports hexadecimal (*0x*), binary (*0b*) and positive exponents (*e0*)
- `po::u32` / `po::u64`: Unsigned integer; same as with signed integers
- `po::f32` / `po::f64`: Floating point value; supports exponents (*e0*), infinities (*inf*) and NaNs (*nan*)

#### Reading `.multi()` options:
- `.begin()` and `.end()`: Iterators that point to `po::value`s; thus you have to choose the right member when dereferencing, e.g. `.begin()->i32`
- `.begin< po::i32 >()` and `.end< po::i32 >()`: Iterators that point to values of the specified type
- `.get( i )` (and `.size()`): Reference to `po::value`; there's also a `.get_or( i, v )` method, returning the second parameter if the index is out of range
- `.to_vector< po::i32 >()`: Returns a `std::vector` with elements of the specified type

```cpp
#include <ProgramOptions.hxx>
#include <iostream>
#include <numeric>

int main( int argc, char** argv ) {
    po::parser parser;

    auto&& x = parser[ "" ]
        .type( po::f64 )            // expects 64-bit floating point numbers
        .multi()                    // allows multiple arguments
        .fallback( -8, "+.5e2" )    // if no arguments were provided, assume these as default
        .callback( [ & ]{ std::cout << "successfully parsed "; } )
        .callback( [ & ]( std::string const& x ){ std::cout << x; } )
        .callback( [ & ]{ std::cout << " which equals "; } )
        .callback( [ & ]( po::f64_t x ){ std::cout << x << '\n'; } );

    parser( argc, argv );

    std::cout << "( + ";
    for( auto&& i : x.to_vector< po::f64 >() )  // unnecessary copy; for demonstration purposes only
        std::cout << i << ' ';
    std::cout << ") = " << std::accumulate( x.begin< po::f64 >(), x.end< po::f64 >(), po::f64_t{} ) << '\n';
}
```
In action:
```
$ ./sum
( + -8 50 ) = 42
$ ./sum 39.5 2.5
successfully parsed 39.5 which equals 39.5
successfully parsed 2.5 which equals 2.5
( + 39.5 2.5 ) = 42
$ ./sum 1e3 -1e0 -1e1 -2e2
successfully parsed 1e3 which equals 1000
successfully parsed -1e0 which equals -1
successfully parsed -1e1 which equals -10
successfully parsed -2e2 which equals -200
( + 1000 -1 -10 -200 ) = 789
$ ./sum inf -1
successfully parsed inf which equals inf
successfully parsed -1 which equals -1
( + inf -1 ) = inf
$ ./sum 12 NaN
successfully parsed 12 which equals 12
successfully parsed NaN which equals nan
( + 12 nan ) = nan
```
### Miscellaneous functions

#### `void po::parser::silent()`
Suppresses all communication via `stderr`. Without this flag, *ProgramOptions.hxx* notifies the user in case of warnings or errors occurring while parsing. For instance, if an option requires an argument of type `i32` and it couldn't be parsed, overflowed or wasn't provided at all, *ProgramOptions.hxx* would print an error saying that the option's argument is invalid and that it hence was ignored.

## Defaults
This small table helps clarifying the defaults for the different kinds of options.

|Name                 |`"long-option"`      |`"x"`                |`""` *(unnamed parameter)*|
|---------------------|---------------------|---------------------|---------------------|
|`.abbreviation`      |`'\0'` *(none)*      |`'x'`                |:x: *(disallowed)*   |
|`.type`              |`po::void_`          |`po::void_`          |`po::string`         |
|`.single` / `.multi` |`.single`            |`.single`            |`.multi`             |

## Flags
All flags have to be `#define`d before including *ProgramOptions.hxx*. Different translation units may include *ProgramOptions.hxx* using different flags.

### `#define PROGRAMOPTIONS_NO_EXCEPTIONS`
Disables all exceptions and thus allows compilation with `-fno-exceptions`. However, incorrect use of the library and unmet preconditions entail `abort()` via `assert(...)`. This flag is implied by `NDEBUG`.

:exclamation: This flag must not vary across different translation units in order to not violate C++' [one definition rule (ODR)](http://en.cppreference.com/w/cpp/language/definition).

### `#define NDEBUG`
Disables all runtime checks and all exceptions. Incorrect use of the library and unmet preconditions will lead to undefined behaviour. Implies `#define PROGRAMOPTIONS_NO_EXCEPTIONS`.

:exclamation: This flag must not vary across different translation units in order to not violate C++' [one definition rule (ODR)](http://en.cppreference.com/w/cpp/language/definition).

### `#define PROGRAMOPTIONS_NO_COLORS`
Disables colored output. On Windows, *ProgramOptions.hxx* uses the WinAPI (i.e. [`SetConsoleTextAttribute`](https://msdn.microsoft.com/en-us/library/windows/desktop/ms686047)) to achieve colored console output whereas it uses [ANSI escape codes](http://bluesock.org/~willg/dev/ansi.html) anywhere else.

:exclamation: This flag must not vary across different translation units in order to not violate C++' [one definition rule (ODR)](http://en.cppreference.com/w/cpp/language/definition).

## Third-party libraries
- [**Catch**](https://github.com/philsquared/Catch) for unit testing.

## License
*ProgramOptions.hxx* is licensed under the [MIT License](https://tldrlegal.com/license/mit-license). See the enclosed [LICENSE.txt](LICENSE.txt) for more information.
