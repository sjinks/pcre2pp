#ifndef PCRE2XX_PRIVATE_CODE_H
#define PCRE2XX_PRIVATE_CODE_H

#include "../../pcre2++/private/compile_context.h"
#include "../../pcre2++/private/pcre2.h"
#include "../../pcre2++/regex_constants.h"
#include "../../pcre2++/regex_error.h"

namespace pcre2 {
namespace details {

template<typename CharT>
union code {
    code()
        : c8(nullptr)
    {
    }

    ~code();

    void compile(const CharT* re, std::size_t len, pcre2::regex_constants::syntax_option_type f, const compile_context<CharT>& ctx)
    {
        int error_code;
        PCRE2_SIZE error_offset;
        auto res = pcre2::details::compile(const_cast<CharT*>(re), len, f, error_code, error_offset, ctx);
        if (res != nullptr) {
            // Not the best code
            // It relies upon the fact that all pointers are of the same size
            // and because this is a union, they are at the same address
            this->c8 = reinterpret_cast<pcre2_code_8*>(res);
            return;
        }

        std::string error = code2error<CharT>(error_code);
        throw regex_error(error_code, error);
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 1>::type* = nullptr>
    pcre2_code_8* get() const
    {
        return this->c8;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 2>::type* = nullptr>
    pcre2_code_16* get() const
    {
        return this->c16;
    }

    template<typename C = CharT, typename std::enable_if<sizeof(C) == 4>::type* = nullptr>
    pcre2_code_32* get() const
    {
        return this->c32;
    }

private:
    pcre2_code_8* c8;
    pcre2_code_16* c16;
    pcre2_code_32* c32;
};

template<>
inline code<char>::~code()
{
    if (this->c8) {
        pcre2_code_free_8(this->c8);
    }
}

template<>
inline code<char16_t>::~code()
{
    if (this->c16) {
        pcre2_code_free_16(this->c16);
    }
}

template<>
inline code<char32_t>::~code()
{
    if (this->c32) {
        pcre2_code_free_32(this->c32);
    }
}

}
}

#endif // PCRE2XX_PRIVATE_CODE_H
