#ifndef PCRE2XX_REGEX_MATCH_H
#define PCRE2XX_REGEX_MATCH_H

#include <string>

#include "pcre2++/basic_regex.h"
#include "pcre2++/regex_constants.h"
#include "pcre2++/match_results.h"
#include "pcre2++/private/do_regex_match.h"

namespace pcre2 {

template<typename BiIter, typename Alloc, typename CharT, typename Traits>
inline bool regex_match(BiIter s, BiIter e, match_results<BiIter, Alloc>& m, const basic_regex<CharT, Traits>& re, regex_constants::match_flag_type flags = regex_constants::match_default)
{
    sub_match<BiIter> unmatched;
    unmatched.first  = e;
    unmatched.second = e;

    std::pair<int, std::size_t*> mr = pcre2::details::do_regex_match(s, e, re, flags);
    m = pcre2::details::ovector_to_match<BiIter, Alloc>(mr, s, e);
    return mr.second != nullptr;
}

template<typename BiIter, typename CharT, typename Traits>
inline bool regex_match(BiIter s, BiIter e, const basic_regex<CharT, Traits>& re, regex_constants::match_flag_type flags = regex_constants::match_default)
{
    match_results<BiIter> tmp;
    return regex_match(s, e, tmp, re, flags);
}

template<typename CharT, typename Alloc, typename Traits>
inline bool regex_match(const CharT* s, match_results<const CharT*, Alloc>& m, const basic_regex<CharT, Traits>& re, regex_constants::match_flag_type f = regex_constants::match_default)
{
    return regex_match(s, s + Traits::length(s), m, re, f);
}

template<typename ChTraits, typename ChAlloc, typename Alloc, typename CharT, typename ReTraits>
inline bool regex_match(
    const std::basic_string<CharT, ChTraits, ChAlloc>& s,
    match_results<typename std::basic_string<CharT, ChTraits, ChAlloc>::const_iterator, Alloc>& m,
    const basic_regex<CharT, ReTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default)
{
    return regex_match(s.begin(), s.end(), m, re, f);
}

template<typename ChTraits, typename ChAlloc, typename Alloc, typename CharT, typename ReTraits>
bool regex_match(
    const std::basic_string<CharT, ChTraits, ChAlloc>&&,
    match_results<typename std::basic_string<CharT, ChTraits, ChAlloc>::const_iterator, Alloc>&,
    const basic_regex<CharT, ReTraits>&,
    regex_constants::match_flag_type = regex_constants::match_default) = delete;

template<typename CharT, typename Traits>
inline bool regex_match(const CharT* s, const basic_regex<CharT, Traits>& re, regex_constants::match_flag_type f = regex_constants::match_default)
{
    return regex_match(s, s + Traits::length(s), re, f);
}

template<typename ChTraits, typename StrAlloc, typename CharT, typename ReTraits>
inline bool regex_match(
    const std::basic_string<CharT, ChTraits, StrAlloc>& s,
    const basic_regex<CharT, ReTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default)
{
    return regex_match(s.begin(), s.end(), re, f);
}

}

#endif // PCRE2XX_REGEX_MATCH_H
