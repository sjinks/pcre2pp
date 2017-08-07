#ifndef PCRE2XX_SUB_MATCH_H
#define PCRE2XX_SUB_MATCH_H

#include <iterator>
#include <ostream>
#include <string>
#include <utility>

namespace pcre2 {

template<typename BidirIt>
class sub_match : public std::pair<BidirIt, BidirIt> {
public:
    using iterator        = BidirIt;
    using value_type      = typename std::iterator_traits<BidirIt>::value_type;
    using difference_type = typename std::iterator_traits<BidirIt>::difference_type;
    using string_type     = std::basic_string<value_type>;

    bool matched;

    constexpr sub_match()
        : matched(false)
    {
    }

    difference_type length() const
    {
        return this->matched ? std::distance(this->first, this->second) : 0;
    }

    operator string_type() const
    {
        return this->str();
    }

    string_type str() const
    {
        return this->matched ? string_type(this->first, this->second) : string_type();
    }

    int compare(const sub_match& s) const
    {
        return this->str().compare(s.str());
    }

    int compare(const string_type& s) const
    {
        return this->str().compare(s);
    }

    int compare(const value_type* s) const
    {
        return this->str().compare(s);
    }
};

using csub_match   = sub_match<const char*>;
using ssub_match   = sub_match<std::string::const_iterator>;
using c16sub_match = sub_match<const char16_t*>;
using s16sub_match = sub_match<std::u16string::const_iterator>;
using c32sub_match = sub_match<const char32_t*>;
using s32sub_match = sub_match<std::u32string::const_iterator>;

template<typename BiIter>
inline bool operator==(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return 0 == lhs.compare(rhs);
}

template<typename BiIter>
inline bool operator!=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return 0 != lhs.compare(rhs);
}

template<typename BiIter>
inline bool operator<(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return lhs.compare(rhs) < 0;
}

template<typename BiIter>
inline bool operator<=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<typename BiIter>
inline bool operator>=(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return lhs.compare(rhs) >= 0;
}

template<typename BiIter>
inline bool operator>(const sub_match<BiIter>& lhs, const sub_match<BiIter>& rhs)
{
    return lhs.compare(rhs) > 0;
}



template<typename BiIter, typename CT, typename CA>
using sub_match_string = std::basic_string<typename std::iterator_traits<BiIter>::value_type, CT, CA>;

template<typename BiIter, typename CT, typename CA>
inline bool operator==(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) == 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator!=(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) != 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator<(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) < 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator<=(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) <= 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator>=(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) >= 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator>(const sub_match_string<BiIter, CT, CA>& lhs, const sub_match<BiIter>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(lhs.data(), lhs.size())) > 0;
}



template<typename BiIter, typename CT, typename CA>
inline bool operator==(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) == 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator!=(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) != 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator<(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) < 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator<=(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) <= 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator>=(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) >= 0;
}

template<typename BiIter, typename CT, typename CA>
inline bool operator>(const sub_match<BiIter>& lhs, const sub_match_string<BiIter, CT, CA>& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(rhs.data(), rhs.size())) > 0;
}



template<typename BiIter>
inline bool operator==(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) == 0;
}

template<typename BiIter>
inline bool operator!=(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) != 0;
}

template<typename BiIter>
inline bool operator<(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) < 0;
}

template<typename BiIter>
inline bool operator<=(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) <= 0;
}

template<typename BiIter>
inline bool operator>=(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) >= 0;
}

template<typename BiIter>
inline bool operator>(typename std::iterator_traits<BiIter>::value_type const* lhs, const sub_match<BiIter>& rhs)
{
    return rhs.compare(lhs) > 0;
}



template<typename BiIter>
inline bool operator==(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) == 0;
}

template<typename BiIter>
inline bool operator!=(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) != 0;
}

template<typename BiIter>
inline bool operator<(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) < 0;
}

template<typename BiIter>
inline bool operator<=(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) <= 0;
}

template<typename BiIter>
inline bool operator>=(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) >= 0;
}

template<typename BiIter>
inline bool operator>(const sub_match<BiIter>& lhs, typename std::iterator_traits<BiIter>::value_type const* rhs)
{
    return lhs.compare(rhs) > 0;
}




template<typename BiIter>
inline bool operator==(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) == 0;
}

template<typename BiIter>
inline bool operator!=(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) != 0;
}

template<typename BiIter>
inline bool operator<(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) < 0;
}

template<typename BiIter>
inline bool operator<=(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) <= 0;
}

template<typename BiIter>
inline bool operator>=(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) >= 0;
}

template<typename BiIter>
inline bool operator>(typename std::iterator_traits<BiIter>::value_type const& lhs, const sub_match<BiIter> rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return rhs.compare(string_type(1, lhs)) > 0;
}



template<typename BiIter>
inline bool operator==(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) == 0;
}

template<typename BiIter>
inline bool operator!=(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) != 0;
}

template<typename BiIter>
inline bool operator<(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) < 0;
}

template<typename BiIter>
inline bool operator<=(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) <= 0;
}

template<typename BiIter>
inline bool operator>=(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) >= 0;
}

template<typename BiIter>
inline bool operator>(const sub_match<BiIter> lhs, typename std::iterator_traits<BiIter>::value_type const& rhs)
{
    using string_type = typename sub_match<BiIter>::string_type;
    return lhs.compare(string_type(1, rhs)) > 0;
}



template<typename CharT, typename Traits, typename BiIter>
inline std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const sub_match<BiIter>& m)
{
    return os << m.str();
}

}

#endif // PCRE2XX_SUB_MATCH_H
