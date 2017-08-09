#ifndef PCRE2XX_MATCH_RESULTS_H
#define PCRE2XX_MATCH_RESULTS_H

#include <cassert>
#include <iterator>
#include <string>
#include <vector>

#include "regex_constants.h"
#include "sub_match.h"

namespace pcre2 {

template<typename BiIter, typename Alloc>
class match_results;

namespace details {
template<typename BiIter, typename Alloc>
inline match_results<BiIter, Alloc> ovector_to_match(const std::pair<int, std::size_t*>& ov, BiIter s, BiIter e);
}

template<typename BiIter, typename Alloc = std::allocator<sub_match<BiIter> > >
class match_results : private std::vector<sub_match<BiIter>, Alloc> {
private:
    using base_type   = std::vector<sub_match<BiIter>, Alloc>;
    using iter_traits = std::iterator_traits<BiIter>;

public:
    using value_type      = sub_match<BiIter>;
    using const_reference = const value_type&;
    using reference       = const_reference;
    using const_iterator  = typename base_type::const_iterator;
    using iterator        = const_iterator;
    using difference_type = typename iter_traits::difference_type;
    using size_type       = typename std::allocator_traits<Alloc>::size_type;
    using allocator_type  = Alloc;
    using char_type       = typename iter_traits::value_type;
    using string_type     = std::basic_string<char_type>;
    using match_flag_type = regex_constants::match_flag_type;

    explicit match_results(const Alloc& a = Alloc())
        : base_type(a)
    {
    }

    match_results(const match_results& other) = default;
    match_results(match_results&& other) noexcept = default;
    match_results& operator=(const match_results& rhs) = default;
    match_results& operator=(match_results&& rhs) = default;

    ~match_results()
    {
    }

    bool ready() const
    {
        return !base_type::empty();
    }

    size_type size() const
    {
        return base_type::empty() ? 0 : base_type::size() - 3;
    }

    size_type max_size() const
    {
        return base_type::max_size();
    }

    bool empty() const
    {
        return 0 == this->size();
    }

    difference_type length(size_type pos = 0) const
    {
        return (*this)[pos].length();
    }

    difference_type position(size_type pos = 0) const
    {
        return std::distance(this->m_begin, (*this)[pos].first);
    }

    string_type str(size_type pos = 0) const
    {
        return string_type((*this)[pos]);
    }

    const_reference operator[](size_type pos) const
    {
        assert(this->ready());
        return pos < this->size() ? base_type::operator[](pos) : this->unmatched_sub();
    }

    const_reference prefix() const
    {
        assert(this->ready());
        return !this->empty() ? this->get_prefix() : this->unmatched_sub();
    }

    const_reference suffix() const
    {
        assert(this->ready());
        return !this->empty() ? this->get_suffix() : this->unmatched_sub();
    }

    const_iterator begin() const
    {
        return base_type::begin();
    }

    const_iterator cbegin() const
    {
        return this->begin();
    }

    const_iterator end() const
    {
        return base_type::end();
    }

    const_iterator cend() const
    {
        return this->end();
    }

    template<typename OutIter>
    OutIter format(OutIter out, const char_type* fmt_first, const char_type* fmt_last, match_flag_type f = regex_constants::format_default) const;

    template<typename OutIter, typename ST, typename SA>
    OutIter format(OutIter out, const std::basic_string<char_type, ST, SA>& fmt, match_flag_type f = regex_constants::format_default) const
    {
        return this->format(out, fmt.begin(), fmt.data() + fmt.size(), f);
    }

    template<typename ST, typename SA>
    std::basic_string<char_type, ST, SA> format(const std::basic_string<char_type, ST, SA>& fmt, match_flag_type f = regex_constants::format_default) const
    {
        std::basic_string<char_type, ST, SA> result;
        this->format(std::back_inserter(result), fmt, f);
        return result;
    }

    string_type format(const char_type* fmt, match_flag_type f = regex_constants::format_default) const
    {
        string_type result;
        this->format(std::back_inserter(result), fmt, fmt + std::char_traits<char_type>::length(fmt), f);
        return result;
    }

    allocator_type get_allocator() const
    {
        return base_type::get_allocator();
    }

    void swap(match_results& other)
    {
        base_type::swap(other);
        std::swap(this->m_begin, other.m_begin);
    }

private:
    BiIter m_begin;

    const_reference unmatched_sub() const
    {
        return base_type::operator[](base_type::size() - 3);
    }

    const_reference get_prefix() const
    {
        return base_type::operator[](base_type::size() - 2);
    }

    value_type& get_prefix()
    {
        return base_type::operator[](base_type::size() - 2);
    }

    const_reference get_suffix() const
    {
        return base_type::operator[](base_type::size() - 1);
    }

    template<typename BiIter2, typename Alloc2>
    friend match_results<BiIter2, Alloc2> pcre2::details::ovector_to_match(const std::pair<int, std::size_t*>& ov, BiIter2 s, BiIter2 e);

    template<typename, typename, typename>
    friend class regex_iterator;
};

using cmatch   = match_results<const char*>;
using smatch   = match_results<std::string::const_iterator>;
using c16match = match_results<const char16_t*>;
using s16match = match_results<std::u16string::const_iterator>;
using c32match = match_results<const char32_t*>;
using s32match = match_results<std::u32string::const_iterator>;

}

#endif // PCRE2XX_MATCH_RESULTS_H
