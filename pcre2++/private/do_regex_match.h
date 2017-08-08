#ifndef PRIVATE_DO_REGEX_MATCH_H_
#define PRIVATE_DO_REGEX_MATCH_H_

#include "basic_regex.h"
#include "regex_error.h"
#include "private/regex_private.h"

namespace pcre2 {
namespace details {

template<typename BiIter, typename CharT, typename Traits>
inline std::pair<int, std::size_t*> do_regex_match(BiIter s, BiIter e, const basic_regex<CharT, Traits>& re, std::uint32_t flags)
{
    static const CharT rh[] = { CharT('\\'), CharT('A'), CharT('('), CharT('?'), CharT(':'), CharT('\0') };
    static const CharT rt[] = { CharT(')'), CharT('\\'), CharT('z'), CharT('\0') };

    if (!re.d_ptr) {
        return std::make_pair(0, nullptr);
    }

    typename Traits::string_type regex = rh + re.d_ptr->regex() + rt;

    basic_regex<CharT, Traits> full;
    full.imbue(re.getloc());
    full.assign(regex, re.flags());

    regex_private<Traits>* rp          = full.d_ptr.get();
    match_data<CharT>& md              = rp->get_match_data();
    const details::code<CharT>& code   = rp->get_code();

    int res = details::match(code.get(), const_cast<CharT*>(&(*s)), std::distance(s, e), 0, static_cast<uint32_t>(flags), md.get());
    if (res < 0) {
        if (PCRE2_ERROR_NOMATCH == res) {
            return std::make_pair(0, nullptr);
        }

        std::string error = details::code2error<CharT>(res);
        throw regex_error(res, error);
    }

    std::size_t* ovector = details::get_ovector_pointer(md.get());
    return std::make_pair(res, ovector);
}

template<typename BiIter, typename Alloc>
inline match_results<BiIter, Alloc> ovector_to_match(const std::pair<int, std::size_t*>& ov, BiIter s, BiIter e)
{
    match_results<BiIter, Alloc> result;

    sub_match<BiIter> unmatched;
    unmatched.first  = e;
    unmatched.second = e;

    result.m_begin = s;

    if (!ov.second) {
        result.reserve(3);
        result.push_back(unmatched);
        result.push_back(unmatched);
        result.push_back(unmatched);
        return result;
    }

    int cnt              = ov.first;
    std::size_t* ovector = ov.second;

    result.reserve(cnt + 3);
    for (int i=0; i<cnt; ++i) {
        sub_match<BiIter> match;
        auto offset_start = ovector[2*i];
        auto offset_end   = ovector[2*i+1];
        if (offset_start != PCRE2_UNSET) {
            auto substr_start = s + offset_start;
            auto substr_end   = s + offset_end;
            match.matched     = true;
            match.first       = substr_start;
            match.second      = substr_end;
        }
        else {
            match.matched = false;
            match.first   = e;
            match.second  = e;
        }

        result.push_back(match);
    }

    result.push_back(unmatched);
    sub_match<BiIter> prefix;
    sub_match<BiIter> suffix;
    prefix.first   = s;
    prefix.second  = s + ovector[0];
    result.push_back(prefix);
    prefix.matched = (prefix.first != prefix.second);
    suffix.first   = s + ovector[2*(cnt-1)+1];
    suffix.second  = e;
    suffix.matched = (suffix.first != suffix.second);
    result.push_back(suffix);
    return result;
}

}
}

#endif /* PRIVATE_DO_REGEX_MATCH_H_ */
