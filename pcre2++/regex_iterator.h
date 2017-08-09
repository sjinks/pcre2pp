#ifndef PCRE2XX_REGEX_ITERATOR_H
#define PCRE2XX_REGEX_ITERATOR_H

#include <cstddef>
#include <iterator>
#include <regex>
#include <string>

#include "regex_constants.h"
#include "regex_search.h"

namespace pcre2 {

template<typename BiIter, typename CharT = typename std::iterator_traits<BiIter>::value_type, typename ReTraits = std::regex_traits<CharT> >
class regex_iterator {
public:
    using regex_type        = basic_regex<CharT, ReTraits>;
    using value_type        = match_results<BiIter>;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;
    using iterator_category = std::forward_iterator_tag;

    regex_iterator()
        : m_re(nullptr), m_f(), m_match()
    {
    }

    regex_iterator(BiIter s, BiIter e, const regex_type& re, regex_constants::match_flag_type f = regex_constants::match_default)
        : m_begin(s), m_end(e), m_re(&re), m_f(f), m_match()
    {
        if (!regex_search(this->m_begin, this->m_end, this->m_match, re, f)) {
            *this = regex_iterator();
        }
    }

    regex_iterator(BiIter, BiIter, const regex_type&&, regex_constants::match_flag_type f = regex_constants::match_default) = delete;

    regex_iterator(const regex_iterator& other) = default;
    regex_iterator& operator=(const regex_iterator& rhs) = default;

    bool operator==(const regex_iterator& rhs) const
    {
        return
            (this->m_match.empty() && rhs.m_match.empty())
         || (
                this->m_begin == rhs.m_begin
             && this->m_end == rhs.m_end
             && this->m_re == rhs.m_re
             && this->m_f == rhs.m_f
             && this->m_match[0] == rhs.m_match[0]
            )
        ;
    }

    bool operator!=(const regex_iterator& rhs) const
    {
        return !this->operator==(rhs);
    }

    const value_type& operator*() const
    {
        return this->m_match;
    }

    const value_type* operator->() const
    {
        return &this->m_match;
    }

    regex_iterator& operator++()
    {
        if (this->m_match[0].matched) {
            auto start     = this->m_match[0].second;
            auto pfx_start = this->m_match[0].second;
            auto flags     = this->m_f;

            if (this->m_match[0].first == this->m_match[0].second) {
                if (start == this->m_end) {
                    this->m_match = value_type();
                    return *this;
                }
                else {
                    flags |= regex_constants::match_not_null | regex_constants::match_continuous;
                    if (regex_search(start, this->m_end, this->m_match, *this->m_re, flags)) {
                        assert(this->m_match.ready());
                        auto& prefix   = this->m_match.get_prefix();
                        prefix.first   = pfx_start;
                        prefix.matched = prefix.first != prefix.second;
                        this->m_match.m_begin = this->m_begin;
                        return *this;
                    }
                    else {
                        ++start;
                    }
                }
            }

            this->m_f &= ~regex_constants::match_not_bol;
            if (regex_search(start, this->m_end, this->m_match, *this->m_re, this->m_f)) {
                assert(this->m_match.ready());
                auto& prefix   = this->m_match.get_prefix();
                prefix.first   = pfx_start;
                prefix.matched = prefix.first != prefix.second;
                this->m_match.m_begin = this->m_begin;
            }
            else {
                this->m_match = value_type();
            }
        }

        return *this;
    }

    regex_iterator operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    BiIter m_begin;
    BiIter m_end;
    const regex_type* m_re;
    regex_constants::match_flag_type m_f;
    match_results<BiIter> m_match;
};

using cregex_iterator   = regex_iterator<const char*>;
using c16regex_iterator = regex_iterator<const char16_t*>;
using c32regex_iterator = regex_iterator<const char32_t*>;

using sregex_iterator   = regex_iterator<std::string::const_iterator>;
using s16regex_iterator = regex_iterator<std::u16string::const_iterator>;
using s32regex_iterator = regex_iterator<std::u32string::const_iterator>;

}

#endif // PCRE2XX_REGEX_ITERATOR_H
