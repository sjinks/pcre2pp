# pcre2pp

[![Build Status](https://travis-ci.org/sjinks/pcre2pp.svg?branch=master)](https://travis-ci.org/sjinks/pcre2pp)
[![codecov](https://codecov.io/gh/sjinks/pcre2pp/branch/master/graph/badge.svg)](https://codecov.io/gh/sjinks/pcre2pp)

C++ interface to PCRE2 library compatible with &lt;regex>

See:
  * [cppreference.com — Regular expressions library](http://en.cppreference.com/w/cpp/regex)
  * [Perl-compatible Regular Expressions](http://www.pcre.org/current/doc/html/)

This header-only library implements 
[std::basic_regex](http://en.cppreference.com/w/cpp/regex/basic_regex),
[std::sub_match](http://en.cppreference.com/w/cpp/regex/sub_match),
[std::match_results](http://en.cppreference.com/w/cpp/regex/match_results),
[std::regex_match](http://en.cppreference.com/w/cpp/regex/regex_match),
[std::regex_search](http://en.cppreference.com/w/cpp/regex/regex_search),
[std::regex_replace](http://en.cppreference.com/w/cpp/regex/regex_replace),
[std::regex_iterator](http://en.cppreference.com/w/cpp/regex/regex_iterator),
[std::regex_token_iterator](http://en.cppreference.com/w/cpp/regex/regex_token_iterator), and
[std::regex_error](http://en.cppreference.com/w/cpp/regex/regex_error)
interfaces for the PCRE2 library.

**Unsupported features:**
  * `regex_traits`: collation is performed by the PCRE library itself, and there is no way to affect its behavior
  * `regex_constants::syntax_option_type::ECMAScript`, `regex_constants::syntax_option_type::basic`,
  `regex_constants::syntax_option_type::extended`, `regex_constants::syntax_option_type::awk`,
  `regex_constants::syntax_option_type::grep`, `regex_constants::syntax_option_type::egrep` are not supported for
  obvious reasons: if you need another matching engine, there is no need to use PCRE :-)
  * `regex_constants::match_flag_type::match_not_bow`, `regex_constants::match_flag_type::match_not_eow`: these
  features are not supported by PCRE
  * `regex_constants::match_flag_type::match_any` is always on (its description says: "If more than one match
  is possible, then any match is an acceptable result"; I believe this is always true for PCRE)
  * `regex_constants::match_flag_type::match_prev_avail`: if I get the description right, you can just unset
  `regex_constants::match_flag_type::match_not_bol` flag
  * `pcre2::regex_constants::error_type` and `std::regex_constants::error_type` constants are different:
  PCRE2 does not officially provides constants for compilation errors (only for match errors), and therefore there is
  no portable way to match PCRE2 errors to std::regex_constants::error_type; in addition, PCRE2 returns much more
  possible errors than stdc++
  * `std::wregex`, `std::wcsub_match`, `std::wssub_match`, `std::wcmatch`, `std::wsmatch`, `std::wcregex_iterator`,
  `std::wsregex_iterator`, `std::wcregex_token_iterator`, and `std::wsregex_token_iterator` are not supported:
  the size of `wchar_t` differes across platforms, which makes it unsuitable for PCRE (however, the library does provide
  `pcre2::regex16`, `pcre2::regex32`, `pcre2::c16sub_match`, `pcre2::c32sub_match`, `pcre2::c16match`, `pcre2::c32match`,
  `pcre2::s16match`, `pcre2::s32match`, `pcre2::c16regex_iterator`, `pcre2::c32regex_iterator`, `pcre2::s16regex_iterator`,
  `pcre2::s32regex_iterator`, `pcre2::c16regex_token_iterator`, `pcre2::c32regex_token_iterator`, `pcre2::s16regex_token_iterator`,
  `pcre2::s32regex_token_iterator` for `char16_t`/`std::u16string` and `char32_t`/`std::u32string` types)
  
**NB:** `char16_t`/`char32_t` are supported only if `sizeof(char16_t) == 2` and `sizeof(char32_t) == 4` (the standard says that
`char16_t` (`char32_t`) has the same size as `std::uint_least16_t` (`std::uint_least32_t`) type, and their size may be more
than 2 (4) bytes).
  
**Differences from stdc++:**
  * `regex_constants::match_flag_type::format_sed` behaves differently than in libstdc++: as far as I can tell,
  libstdc++ does not handle [sed rules](http://pubs.opengroup.org/onlinepubs/9699919799/utilities/sed.html#tag_20_116_13_03)
  properly: there is no way to escape `&` or `\<digit>` in the format pattern. PCRE2++, however, follows the rules
  [more strictly](https://github.com/sjinks/pcre2pp/blob/master/test/match_results.cpp#L19)
  * new option: `regex_constants::utf`: causes PCRE2 to regard both the pattern and the subject strings that are
  subsequently processed as strings of UTF characters instead of single-code-unit strings
  * new option: `regex_constants::ucp`: changes the way PCRE2 processes \B, \b, \D, \d, \S, \s, \W, \w, and some of the POSIX
  character classes. By default, only ASCII characters are recognized, but when this option is set, Unicode properties are used
  instead to classify characters

The library depends upon `pcre2.h` header file (provided by `libpcre2-dev` package in Ubuntu).

8-bit features require linking in `pcre2-8` library (`libpcre2-8-0` package in Ubuntu).
16-bit features require linking in `pcre2-16` library (`libpcre2-16-0` package in Ubuntu).
32-bit features require linking in `pcre2-32` library (`libpcre2-32-0` package in Ubuntu).
