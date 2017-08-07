#ifndef PCRE2XX_BASIC_REGEX_H
#define PCRE2XX_BASIC_REGEX_H

#include <initializer_list>
#include <locale>
#include <memory>
#include <regex>
#include <string>
#include <type_traits>

#include "../pcre2++/private/regex_private.h"
#include "../pcre2++/regex_constants.h"

namespace pcre2 {

template<typename CharT, typename Traits>
class basic_regex;

namespace details {
template<typename BiIt, typename C, typename RT>
inline std::pair<int, std::size_t*> do_regex_match(BiIt, BiIt, const pcre2::basic_regex<C, RT>&, std::uint32_t);
}

template<typename CharT, typename Traits = std::regex_traits<CharT> >
class basic_regex {
public:
    static_assert(std::is_same<CharT, typename Traits::char_type>::value, "regex and regex_traits must have the same character type");

    using value_type  = CharT;
    using traits_type = Traits;
    using string_type = typename traits_type::string_type;
    using flag_type   = regex_constants::syntax_option_type;
    using locale_type = typename traits_type::locale_type;

    /* Constructors */
    basic_regex()
        : m_flags(regex_constants::pcre2), m_loc(), d_ptr(nullptr)
    {
    }

    explicit basic_regex(const value_type* s, flag_type f = regex_constants::pcre2)
        : basic_regex(s, std::char_traits<value_type>::length(s), locale_type(), f)
    {
    }

    basic_regex(const value_type* s, std::size_t len, flag_type f = regex_constants::pcre2)
        : basic_regex(s, len, locale_type(), f)
    {
    }

    basic_regex(const basic_regex& other) = default;
    basic_regex(basic_regex&& other) noexcept = default;

    template<typename ST, typename SA>
    explicit basic_regex(const std::basic_string<value_type, ST, SA>& str, flag_type f = regex_constants::pcre2)
        : basic_regex(str.data(), str.size(), f)
    {
    }

    template<typename ForwardIt>
    basic_regex(ForwardIt first, ForwardIt last, flag_type f = regex_constants::pcre2)
        : basic_regex(&(*first), std::distance(first, last), f)
    {
    }

    basic_regex(std::initializer_list<value_type> init, flag_type f = regex_constants::pcre2)
        : basic_regex(init.begin(), init.end(), f)
    {
    }

    /* Assignment */
    basic_regex& operator=(const basic_regex& other)
    {
        return this->assign(other);
    }

    basic_regex& operator=(basic_regex&& other) noexcept
    {
        return this->assign(std::move(other));
    }

    basic_regex& operator=(const value_type* p)
    {
        return this->assign(p);
    }

    basic_regex& operator=(std::initializer_list<value_type> il)
    {
        return this->assign(il.begin(), il.end());
    }

    template<typename ST, typename SA>
    basic_regex& operator=(const std::basic_string<value_type, ST, SA>& p)
    {
        return this->assign(p);
    }

    /* Assign */
    basic_regex& assign(const basic_regex& other)
    {
        basic_regex tmp(other);
        this->swap(tmp);
        return *this;
    }

    basic_regex& assign(basic_regex&& other) noexcept
    {
        basic_regex tmp(std::move(other));
        this->swap(tmp);
        return *this;
    }

    basic_regex& assign(const value_type* ptr, flag_type f = regex_constants::pcre2)
    {
        return this->assign(string_type(ptr), f);
    }

    basic_regex& assign(const value_type* ptr, std::size_t len, flag_type f = regex_constants::pcre2)
    {
        return this->assign(string_type(ptr, len), f);
    }

    template<typename ST, typename SA>
    basic_regex& assign(const std::basic_string<value_type, ST, SA>& str, flag_type f = regex_constants::pcre2)
    {
        return this->assign(
            basic_regex(str.data(), str.size(), this->m_loc, f)
        );
    }

    template<typename InputIt>
    basic_regex& assign(InputIt first, InputIt last, flag_type f = regex_constants::pcre2)
    {
        return this->assign(string_type(first, last), f);
    }

    basic_regex& assign(std::initializer_list<value_type> l, flag_type f = regex_constants::pcre2)
    {
        return this->assign(l.begin(), l.end(), f);
    }

    /* Destructor */
    ~basic_regex()
    {
    }

    /* Observers */
    unsigned int mark_count() const
    {
        return this->d_ptr ? this->d_ptr->mark_count() : 0;
    }

    flag_type flags() const
    {
        return this->m_flags;
    }

    /* Locale */
    locale_type imbue(locale_type loc)
    {
        std::swap(this->m_loc, loc);
        this->d_ptr.reset(nullptr);
        return loc;
    }

    locale_type getloc() const
    {
        return this->m_loc;
    }

    /* Modifiers */
    void swap(basic_regex& other) noexcept
    {
        std::swap(this->m_flags, other.m_flags);
        std::swap(this->m_loc, other.m_loc);
        std::swap(this->d_ptr, other.d_ptr);
    }

private:
    flag_type m_flags;
    locale_type m_loc;
    std::unique_ptr<details::regex_private<traits_type> > d_ptr;

    basic_regex(const value_type* s, std::size_t len, locale_type loc, flag_type f)
        : m_flags(f), m_loc(loc), d_ptr(new details::regex_private<traits_type>(s, len, loc, f))
    {
    }

    template<typename BiIt, typename C, typename RT>
    friend std::pair<int, std::size_t*> pcre2::details::do_regex_match(BiIt s, BiIt e, const basic_regex<C, RT>& re, std::uint32_t flags);
};

using regex   = basic_regex<char>;
using regex16 = basic_regex<char16_t>; // FIXME: verify sizeof(char16_t) == 2
using regex32 = basic_regex<char32_t>; // FIXME: verify sizeof(char32_t) == 4

template<typename CharT, typename Traits>
inline void swap(basic_regex<CharT, Traits>& lhs, basic_regex<CharT, Traits>& rhs)
{
    lhs.swap(rhs);
}

}

#endif // PCRE2XX_BASIC_REGEX_H
