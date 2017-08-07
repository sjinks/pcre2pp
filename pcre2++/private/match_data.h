#ifndef PCRE2XX_PRIVATE_MATCH_DATA_H
#define PCRE2XX_PRIVATE_MATCH_DATA_H

#include <cassert>
#include <new>
#include <type_traits>

#include "../../pcre2++/private/code.h"
#include "../../pcre2++/private/pcre2.h"

namespace pcre2 {
namespace details {

template<typename CharT>
union match_data {
public:
    match_data()
        : m8(nullptr)
    {
    }

    ~match_data();

    void create_from_pattern(const code<CharT>& c)
    {
        assert(!this->m8);
        this->m8 = reinterpret_cast<pcre2_match_data_8*>(match_data_create_from_pattern(c.get()));
        if (!this->m8) {
            throw std::bad_alloc();
        }
    }

    uint32_t mark_count() const;

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 1>::type* = nullptr>
    pcre2_match_data_8* get()
    {
        return this->m8;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 2>::type* = nullptr>
    pcre2_match_data_16* get()
    {
        return this->m16;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 4>::type* = nullptr>
    pcre2_match_data_32* get()
    {
        return this->m32;
    }

private:
    pcre2_match_data_8* m8;
    pcre2_match_data_16* m16;
    pcre2_match_data_32* m32;
};

template<>
inline match_data<char>::~match_data()
{
    if (this->m8) {
        pcre2_match_data_free_8(this->m8);
    }
}

template<>
inline match_data<char16_t>::~match_data()
{
    if (this->m16) {
        pcre2_match_data_free_16(this->m16);
    }
}

template<>
inline match_data<char32_t>::~match_data()
{
    if (this->m32) {
        pcre2_match_data_free_32(this->m32);
    }
}

//template<>
//inline void match_data<char>::create_from_pattern(const code<char>& c)
//{
//    assert(!this->m8);
//    this->m8 = match_data_create_from_pattern(c.get());
//    if (!this->m8) {
//        throw std::bad_alloc();
//    }
//}
//
//template<>
//inline void match_data<char16_t>::create_from_pattern(const code<char16_t>& c)
//{
//    assert(!this->m16);
//    this->m16 = match_data_create_from_pattern(c.get());
//    if (!this->m16) {
//        throw std::bad_alloc();
//    }
//}
//
//template<>
//inline void match_data<char32_t>::create_from_pattern(const code<char32_t>& c)
//{
//    assert(!this->m32);
//    this->m32 = match_data_create_from_pattern(c.get());
//    if (!this->m32) {
//        throw std::bad_alloc();
//    }
//}

template<>
inline std::uint32_t match_data<char>::mark_count() const
{
    assert(this->m8);
    return pcre2_get_ovector_count_8(this->m8);
}

template<>
inline std::uint32_t match_data<char16_t>::mark_count() const
{
    assert(this->m16);
    return pcre2_get_ovector_count_16(this->m16);
}

template<>
inline std::uint32_t match_data<char32_t>::mark_count() const
{
    assert(this->m32);
    return pcre2_get_ovector_count_32(this->m32);
}

}
}

#endif // PCRE2XX_PRIVATE_MATCH_DATA_H
