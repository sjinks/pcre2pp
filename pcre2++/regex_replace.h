#ifndef PCRE2XX_REGEX_REPLACE_H
#define PCRE2XX_REGEX_REPLACE_H

#include <iterator>
#include <string>
#include "basic_regex.h"
#include "regex_constants.h"
#include "regex_iterator.h"

namespace pcre2 {

template<typename OutIter, typename BiIter, typename CharT, typename ST, typename SA>
inline OutIter regex_replace(
    OutIter out,
    BiIter b,
    BiIter e,
    const basic_regex<CharT>& re,
    const std::basic_string<CharT, ST, SA>& fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    return regex_replace(out, b, e, re, fmt.c_str(), f);
}

template<typename OutIter, typename BiIter, typename CharT>
inline OutIter regex_replace(
    OutIter out,
    BiIter b,
    BiIter e,
    const basic_regex<CharT>& re,
    const CharT* fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    using Iter = regex_iterator<BiIter, CharT>;

    Iter it(b, e, re, f);
    Iter end;

    if (it == end) {
        if (!(f & regex_constants::format_no_copy)) {
            out = std::copy(b, e, out);
        }
    }
    else {
        sub_match<BiIter> last;
        auto len = std::char_traits<CharT>::length(fmt);
        for ( ; it != end; ++it) {
            if (!(f & regex_constants::format_no_copy)) {
                out = std::copy(it->prefix().first, it->prefix().second, out);
            }

            out  = it->format(out, fmt, fmt + len, f);
            last = it->suffix();

            if (f & regex_constants::format_first_only) {
                break;
            }
        }

        if (!(f & regex_constants::format_no_copy)) {
            out = std::copy(last.first, last.second, out);
        }
    }

    return out;
}

template<typename CharT, typename ST, typename SA, typename FST, typename FSA>
inline std::basic_string<CharT, ST, SA> regex_replace(
    const std::basic_string<CharT, ST, SA>& s,
    const basic_regex<CharT>& re,
    const std::basic_string<CharT, FST, FSA>& fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::basic_string<CharT, ST, SA> result;
    regex_replace(std::back_inserter(result), s.begin(), s.end(), re, fmt, f);
    return result;
}

template<typename CharT, typename ST, typename SA>
inline std::basic_string<CharT, ST, SA> regex_replace(
    const std::basic_string<CharT, ST, SA>& s,
    const basic_regex<CharT>& re,
    const CharT* fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::basic_string<CharT, ST, SA> result;
    regex_replace(std::back_inserter(result), s.begin(), s.end(), re, fmt, f);
    return result;
}

template<typename CharT, typename ST, typename SA>
inline std::basic_string<CharT> regex_replace(
    const CharT* s,
    const basic_regex<CharT>& re,
    const std::basic_string<CharT, ST, SA>& fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::basic_string<CharT> result;
    regex_replace(std::back_inserter(result), s, s + std::char_traits<CharT>::length(s), re, fmt, f);
    return result;
}

template<typename CharT>
inline std::basic_string<CharT> regex_replace(
    const CharT* s,
    const basic_regex<CharT>& re,
    const CharT* fmt,
    regex_constants::match_flag_type f = regex_constants::match_default
)
{
    std::basic_string<CharT> result;
    regex_replace(std::back_inserter(result), s, s + std::char_traits<CharT>::length(s), re, fmt, f);
    return result;
}

}

#endif // PCRE2XX_REGEX_REPLACE_H
