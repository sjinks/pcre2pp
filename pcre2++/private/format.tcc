#ifndef PCRE2XX_MATCH_RESULTS_H
#error "Please include pcre2++/match_results.h instead"
#endif

#include <algorithm>

template<typename BiIter, typename Alloc>
template<typename OutIter>
OutIter pcre2::match_results<BiIter, Alloc>::format(OutIter out, const char_type* fmt_first, const char_type* fmt_last, match_flag_type f) const
{
    if (f & pcre2::regex_constants::format_sed) {
        return this->format_sed(out, fmt_first, fmt_last);
    }

    return this->format_default(out, fmt_first, fmt_last);
}

template<typename BiIter, typename Alloc>
template<typename OutIter>
OutIter pcre2::match_results<BiIter, Alloc>::format_sed(OutIter out, const char_type* fmt_first, const char_type* fmt_last) const
{
    auto output = [this, &out](std::size_t idx)
    {
        auto& s = (*this)[idx];
        if (s.matched) {
            out = std::copy(s.first, s.second, out);
        }
    };

    while (fmt_first != fmt_last) {
        char_type c = *fmt_first;

        if ('&' == c) {
            output(0);
            ++fmt_first;
        }
        else if ('\\' == c) {
            ++fmt_first;
            if (fmt_first != fmt_last) {
                c = *fmt_first;
                if (c >= '0' && c <= '9') {
                    output(c - '0');
                    ++fmt_first;
                }
                else if ('&' == c || '\\' == c) {
                    *out = c;
                    ++out;
                    ++fmt_first;
                }
                else {
                    *out = '\\';
                }
            }
            else {
                *out = '\\';
            }
        }
        else {
            *out = *fmt_first;
            ++out;
            ++fmt_first;
        }
    }

    return out;
}

template<typename BiIter, typename Alloc>
template<typename OutIter>
OutIter pcre2::match_results<BiIter, Alloc>::format_default(OutIter out, const char_type* fmt_first, const char_type* fmt_last) const
{
    auto output = [this, &out](std::size_t idx)
    {
        auto& s = (*this)[idx];
        if (s.matched) {
            out = std::copy(s.first, s.second, out);
        }
    };

    while (true) {
        auto next = std::find(fmt_first, fmt_last, '$');
        if (next == fmt_last) {
            break;
        }

        out = std::copy(fmt_first, next, out);
        ++next;

        if (next == fmt_last) {
            *out = '$';
            ++out;
        }
        else if (*next == '$') {
            *out = '$';
            ++out;
            ++next;
        }
        else if (*next == '&') {
            ++next;
            output(0);
        }
        else if (*next == '`') {
            ++next;
            auto& pfx = this->get_prefix();
            if (pfx.matched) {
                out = std::copy(pfx.first, pfx.second, out);
            }
        }
        else if (*next == '\'') {
            ++next;
            auto& sfx = this->get_suffix();
            if (sfx.matched) {
                out = std::copy(sfx.first, sfx.second, out);
            }
        }
        else if (*next >= '0' && *next <= '9') {
            std::size_t idx = *next - '0';
            ++next;
            if (next != fmt_last && *next >= '0' && *next <= '9') {
                idx = idx * 10 + (*next - '0');
                ++next;
            }

            if (idx < this->size()) {
                output(idx);
            }
        }
        else {
            *out = '$';
            ++out;
        }

        fmt_first = next;
    }

    out = std::copy(fmt_first, fmt_last, out);
    return out;
}
