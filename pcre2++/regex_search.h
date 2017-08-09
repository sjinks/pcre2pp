#ifndef PCRE2XX_REGEX_SEARCH_H
#define PCRE2XX_REGEX_SEARCH_H

#include <string>

#include "basic_regex.h"
#include "regex_constants.h"
#include "match_results.h"
#include "private/do_regex_match.h"

namespace pcre2 {

template<typename BiIter, typename Alloc, typename CharT, typename ReTraits>
inline bool regex_search(
    BiIter s,
    BiIter e,
    match_results<BiIter, Alloc>& m,
    const basic_regex<CharT, ReTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::pair<int, std::size_t*> mr = pcre2::details::do_regex_search(s, e, re, f);
    m = pcre2::details::ovector_to_match<BiIter, Alloc>(mr, s, e);
    return mr.second != nullptr;
}

template<typename BiIter, typename CharT, typename ReTraits>
inline bool regex_search(
     BiIter s,
     BiIter e,
     const basic_regex<CharT, ReTraits>& re,
     regex_constants::match_flag_type f = regex_constants::match_default
)
{
    match_results<BiIter> m;
    return regex_search(s, e, m, re, f);
}

template<typename CharT, class Alloc, class RxTraits>
inline bool regex_search(
    const CharT* s,
    match_results<const CharT*, Alloc>& m,
    const basic_regex<CharT, RxTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s, s + RxTraits::length(s), m, re, f);
}

template<typename CharT, class RxTraits>
inline bool regex_search(
    const CharT* s,
    const basic_regex<CharT, RxTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s, s + RxTraits::length(s), re, f);
}

template<typename ChTraits, typename SAlloc, typename CharT, typename RxTraits>
inline bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>& s,
    const basic_regex<CharT, RxTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s.begin(), s.end(), re, f);
}

template<typename ChTraits, typename SAlloc, typename Alloc, typename CharT, typename RxTraits>
inline bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>& s,
    match_results<typename std::basic_string<CharT, ChTraits, SAlloc>::const_iterator, Alloc>& m,
    const basic_regex<CharT, RxTraits>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s.begin(), s.end(), m, re, f);
}

template<typename ChTraits, typename SAlloc, typename Alloc, typename CharT, typename RxTraits>
bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>&&,
    match_results<typename std::basic_string<CharT, ChTraits, SAlloc>::const_iterator, Alloc>&,
    const basic_regex<CharT, RxTraits>&,
    regex_constants::match_flag_type = regex_constants::match_default
) = delete;
	
}

#endif // PCRE2XX_REGEX_SEARCH_H
