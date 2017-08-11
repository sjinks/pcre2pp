#ifndef PCRE2XX_REGEX_TOKEN_ITERATOR_H
#define PCRE2XX_REGEX_TOKEN_ITERATOR_H

#include <initializer_list>
#include <iterator>
#include <vector>
#include "basic_regex.h"
#include "regex_iterator.h"
#include "sub_match.h"

namespace pcre2 {

template<typename BiIter, typename CharT = typename std::iterator_traits<BiIter>::value_type>
class regex_token_iterator {
public:
    using regex_type        = basic_regex<CharT>;
    using value_type        = sub_match<BiIter>;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using pointer           = const value_type*;
    using reference         = const value_type&;

    regex_token_iterator()
        : m_pos(), m_subs(), m_suffix(), m_n(0), m_result(nullptr), m_has_minus_one(false)
    {
    }

    regex_token_iterator(BiIter s, BiIter e, const regex_type& re, int submatch = 0, regex_constants::match_flag_type f = regex_constants::match_default)
        : m_pos(s, e, re, f), m_subs(1, submatch), m_n(0), m_result(), m_has_minus_one()
    {
        this->init(s, e);
    }

    regex_token_iterator(BiIter s, BiIter e, const regex_type& re, const std::vector<int>& submatches, regex_constants::match_flag_type f = regex_constants::match_default)
        : m_pos(s, e, re, f), m_subs(submatches), m_n(0), m_result(), m_has_minus_one()
    {
        this->init(s, e);
    }

    regex_token_iterator(BiIter s, BiIter e, const regex_type& re, std::initializer_list<int> submatches, regex_constants::match_flag_type f = regex_constants::match_default)
        : m_pos(s, e, re, f), m_subs(submatches), m_n(0), m_result(), m_has_minus_one()
    {
        this->init(s, e);
    }

    template<std::size_t Size>
    regex_token_iterator(BiIter s, BiIter e, const regex_type& re, const int (&submatches)[Size], regex_constants::match_flag_type f = regex_constants::match_default)
        : m_pos(s, e, re, f), m_subs(submatches, submatches + Size), m_n(0), m_result(), m_has_minus_one()
    {
        this->init(s, e);
    }


    regex_token_iterator(BiIter, BiIter, const regex_type&&, int = 0, regex_constants::match_flag_type = regex_constants::match_default) = delete;
    regex_token_iterator(BiIter, BiIter, const regex_type&&, const std::vector<int>&, regex_constants::match_flag_type = regex_constants::match_default) = delete;
    regex_token_iterator(BiIter, BiIter, const regex_type&&, std::initializer_list<int>, regex_constants::match_flag_type = regex_constants::match_default) = delete;

    template<std::size_t Size>
    regex_token_iterator(BiIter, BiIter, const regex_type&&, const int (&)[Size], regex_constants::match_flag_type = regex_constants::match_default) = delete;

    regex_token_iterator(const regex_token_iterator& other)
        : m_pos(other.m_pos), m_subs(other.m_subs), m_suffix(other.m_suffix), m_n(other.m_n), m_result(), m_has_minus_one(other.m_has_minus_one)
    {
        this->normalize();
    }

    regex_token_iterator& operator=(const regex_token_iterator& rhs)
    {
        this->m_pos           = rhs.m_pos;
        this->m_subs          = rhs.m_subs;
        this->m_n             = rhs.m_n;
        this->m_suffix        = rhs.m_suffix;
        this->m_has_minus_one = rhs.m_has_minus_one;
        this->normalize();
        return *this;
    }

    bool operator==(const regex_token_iterator& rhs) const
    {
        if (this->end_of_seq() && rhs.end_of_seq()) {
            return true;
        }

        if (this->m_suffix.matched && rhs.m_suffix.matched && this->m_suffix == rhs.m_suffix) {
            return true;
        }

        if (this->end_of_seq() || this->m_suffix.matched || rhs.end_of_seq() || rhs.m_suffix.matched) {
            return false;
        }

        return
               this->m_pos  == rhs.m_pos
            && this->m_n    == rhs.m_n
            && this->m_subs == rhs.m_subs
        ;
    }

    bool operator!=(const regex_token_iterator& rhs) const
    {
        return !(*this == rhs);
    }

    reference operator*() const
    {
        return *this->m_result;
    }

    pointer operator->() const
    {
        return this->m_result;
    }

    regex_token_iterator& operator++()
    {
        position prev = this->m_pos;
        if (this->m_suffix.matched) {
            *this = regex_token_iterator();
        }
        else if (this->m_n + 1 < this->m_subs.size()) {
            ++this->m_n;
            this->m_result = &this->current_match();
        }
        else {
            this->m_n = 0;
            ++this->m_pos;
            if (this->m_pos != position()) {
                this->m_result = &this->current_match();
            }
            else if (this->m_has_minus_one && prev->suffix().length() != 0) {
                this->m_suffix.matched = true;
                this->m_suffix.first   = prev->suffix().first;
                this->m_suffix.second  = prev->suffix().second;
                this->m_result         = &this->m_suffix;
            }
            else {
                *this = regex_token_iterator();
            }
        }

        return *this;
    }

    regex_token_iterator operator++(int)
    {
        auto tmp = *this;
        ++(*this);
        return tmp;
    }

private:
    using position = regex_iterator<BiIter, CharT>;

    position m_pos;
    std::vector<int> m_subs;
    value_type m_suffix;
    std::size_t m_n;
    pointer m_result;
    bool m_has_minus_one;

    void init(BiIter b, BiIter e)
    {
        assert(this->m_result == nullptr);
        assert(!this->m_has_minus_one);

        for (auto& it : this->m_subs) {
            if (-1 == it) {
                this->m_has_minus_one = true;
                break;
            }
        }

        if (this->m_pos != position()) {
            this->m_result = &this->current_match();
        }
        else if (this->m_has_minus_one) {
            this->m_suffix.matched = true;
            this->m_suffix.first   = b;
            this->m_suffix.second  = e;
            this->m_result         = &this->m_suffix;
        }
    }

    reference current_match() const
    {
        auto i = this->m_subs[this->m_n];
        if (i == -1) {
            return (*this->m_pos).prefix();
        }

        return (*this->m_pos)[i];
    }

    constexpr bool end_of_seq() const
    {
        return this->m_result == nullptr;
    }

    void normalize()
    {
        if (this->m_pos != position()) {
            this->m_result = &this->current_match();
        }
        else if (this->m_has_minus_one) {
            this->m_result = &this->m_suffix;
        }
        else {
            this->m_result = nullptr;
        }
    }
};

using cregex_token_iterator   = regex_token_iterator<const char*>;
using sregex_token_iterator   = regex_token_iterator<std::string::const_iterator>;
using c16regex_token_iterator = regex_token_iterator<const char16_t*>;
using s16regex_token_iterator = regex_token_iterator<std::u16string::const_iterator>;
using c32regex_token_iterator = regex_token_iterator<const char32_t*>;
using s32regex_token_iterator = regex_token_iterator<std::u32string::const_iterator>;

}

#endif // PCRE2XX_REGEX_TOKEN_ITERATOR_H
