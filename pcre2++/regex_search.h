#ifndef PCRE2XX_REGEX_SEARCH_H
#define PCRE2XX_REGEX_SEARCH_H

#include <string>

#include "basic_regex.h"
#include "regex_constants.h"
#include "match_results.h"
#include "private/do_regex_match.h"

namespace pcre2 {

template<typename BiIter, typename Alloc, typename CharT>
inline bool regex_search(
    BiIter s,
    BiIter e,
    match_results<BiIter, Alloc>& m,
    const basic_regex<CharT>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::pair<int, std::size_t*> mr = pcre2::details::do_regex_search(s, e, re, f);
    m = pcre2::details::ovector_to_match<BiIter, Alloc>(mr, s, e);
    return mr.second != nullptr;
}

template<typename BiIter, typename CharT>
inline bool regex_search(
     BiIter s,
     BiIter e,
     const basic_regex<CharT>& re,
     regex_constants::match_flag_type f = regex_constants::match_default
)
{
    match_results<BiIter> m;
    return regex_search(s, e, m, re, f);
}

template<typename CharT, typename Alloc>
inline bool regex_search(
    const CharT* s,
    match_results<const CharT*, Alloc>& m,
    const basic_regex<CharT>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s, s + std::char_traits<CharT>::length(s), m, re, f);
}

template<typename CharT>
inline bool regex_search(
    const CharT* s,
    const basic_regex<CharT>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s, s + std::char_traits<CharT>::length(s), re, f);
}

template<typename ChTraits, typename SAlloc, typename CharT>
inline bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>& s,
    const basic_regex<CharT>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s.begin(), s.end(), re, f);
}

template<typename ChTraits, typename SAlloc, typename Alloc, typename CharT>
inline bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>& s,
    match_results<typename std::basic_string<CharT, ChTraits, SAlloc>::const_iterator, Alloc>& m,
    const basic_regex<CharT>& re,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_search(s.begin(), s.end(), m, re, f);
}

template<typename ChTraits, typename SAlloc, typename Alloc, typename CharT>
bool regex_search(
    const std::basic_string<CharT, ChTraits, SAlloc>&&,
    match_results<typename std::basic_string<CharT, ChTraits, SAlloc>::const_iterator, Alloc>&,
    const basic_regex<CharT>&,
    regex_constants::match_flag_type = regex_constants::match_default
) = delete;
	
}

#endif // PCRE2XX_REGEX_SEARCH_H
