#ifndef PCRE2XX_PRIVATE_COMPILE_CONTEXT_H
#define PCRE2XX_PRIVATE_COMPILE_CONTEXT_H

#include <new>

#include "../../pcre2++/private/character_table.h"
#include "../../pcre2++/private/pcre2.h"

namespace pcre2 {

namespace details {

template<typename CharT>
union compile_context {
    compile_context()
        : c8(nullptr)
    {
    }

    ~compile_context();

    void create();

    operator pcre2_compile_context_8*() const
    {
        return this->c8;
    }

    operator pcre2_compile_context_16*() const
    {
        return this->c16;
    }

    operator pcre2_compile_context_32*() const
    {
        return this->c32;
    }

    void set_character_table(const pcre2::details::character_table<CharT>& tbl);

private:
    pcre2_compile_context_8* c8;
    pcre2_compile_context_16* c16;
    pcre2_compile_context_32* c32;
};

template<>
inline void compile_context<char>::create()
{
    if (!this->c8) {
        this->c8 = pcre2_compile_context_create_8(nullptr);
        if (!this->c8) {
            throw std::bad_alloc();
        }
    }
}

template<>
inline void compile_context<char16_t>::create()
{
    if (!this->c16) {
        this->c16 = pcre2_compile_context_create_16(nullptr);
        if (!this->c16) {
            throw std::bad_alloc();
        }
    }
}

template<>
inline void compile_context<char32_t>::create()
{
    if (!this->c32) {
        this->c32 = pcre2_compile_context_create_32(nullptr);
        if (!this->c32) {
            throw std::bad_alloc();
        }
    }
}

template<>
inline compile_context<char>::~compile_context()
{
    if (this->c8) {
        pcre2_compile_context_free_8(this->c8);
    }
}

template<>
inline compile_context<char16_t>::~compile_context()
{
    if (this->c16) {
        pcre2_compile_context_free_16(this->c16);
    }
}

template<>
inline compile_context<char32_t>::~compile_context()
{
    if (this->c32) {
        pcre2_compile_context_free_32(this->c32);
    }
}

template<>
inline void compile_context<char>::set_character_table(const pcre2::details::character_table<char>& tbl)
{
    set_character_tables(this->c8, tbl);
}

template<>
inline void compile_context<char16_t>::set_character_table(const pcre2::details::character_table<char16_t>& tbl)
{
    set_character_tables(this->c16, tbl);
}

template<>
inline void compile_context<char32_t>::set_character_table(const pcre2::details::character_table<char32_t>& tbl)
{
    set_character_tables(this->c32, tbl);
}

}

}

#endif // PCRE2XX_PRIVATE_COMPILE_CONTEXT_H
