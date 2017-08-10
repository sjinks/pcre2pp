#ifndef PCRE2XX_PRIVATE_COMPILE_CONTEXT_H
#define PCRE2XX_PRIVATE_COMPILE_CONTEXT_H

#include <new>

#include "private/pcre2.h"
#include "private/wrappers.h"

namespace pcre2 {
namespace details {

template<typename CharT>
union compile_context {
    compile_context()
        : c8(nullptr)
    {
    }

    ~compile_context()
    {
        compile_context_free(this->get<CharT>());
    }

    void create();

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 1>::type* = nullptr>
    pcre2_compile_context_8* get() const
    {
        return this->c8;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 2>::type* = nullptr>
    pcre2_compile_context_16* get() const
    {
        return this->c16;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 4>::type* = nullptr>
    pcre2_compile_context_32* get() const
    {
        return this->c32;
    }

    void set_character_table(const std::uint8_t* tbl)
    {
        set_character_tables(this->get<CharT>(), tbl);
    }

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

}
}

#endif // PCRE2XX_PRIVATE_COMPILE_CONTEXT_H
