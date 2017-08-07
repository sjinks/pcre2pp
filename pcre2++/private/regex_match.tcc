
#ifndef PCRE2XX_REGEX_MATCH_H
#error "Include regex_match.h instead"
#endif

#include "../../pcre2++/basic_regex.h"
#include "../../pcre2++/regex_error.h"
#include "../../pcre2++/match_results.h"
#include "../../pcre2++/private/do_regex_match.h"

template<typename BiIter, typename Alloc, typename CharT, typename Traits>
inline bool pcre2::regex_match(BiIter s, BiIter e, match_results<BiIter, Alloc>& m, const basic_regex<CharT, Traits>& re, regex_constants::match_flag_type flags)
{
    sub_match<BiIter> unmatched;
    unmatched.first  = e;
    unmatched.second = e;

    std::pair<int, std::size_t*> mr = pcre2::details::do_regex_match(s, e, re, flags);
    m = pcre2::details::ovector_to_match<BiIter, Alloc>(mr, s, e);
    return mr.second != nullptr;
}

template<typename BiIter, typename CharT, typename Traits>
inline bool pcre2::regex_match(BiIter s, BiIter e, const pcre2::basic_regex<CharT, Traits>& re, regex_constants::match_flag_type flags)
{
    match_results<BiIter> tmp;
    return regex_match(s, e, tmp, re, flags);
}
