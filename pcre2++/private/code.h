#ifndef PCRE2XX_PRIVATE_CODE_H
#define PCRE2XX_PRIVATE_CODE_H

#include "private/pcre2.h"
#include "private/compile_context.h"
#include "private/wrappers.h"
#include "regex_constants.h"

namespace pcre2 {
namespace details {

template<typename CharT>
union code {
    code()
        : c8(nullptr)
    {
    }

    ~code()
    {
        code_free(this->get<CharT>());
    }

    void compile(const CharT* re, std::size_t len, pcre2::regex_constants::syntax_option_type f, const compile_context<CharT>& ctx)
    {
        int error_code;
        PCRE2_SIZE error_offset;
        auto res = pcre2::details::compile(const_cast<CharT*>(re), len, f, error_code, error_offset, ctx.get());
        if (res != nullptr) {
            // Not the best code
            // It relies upon the fact that all pointers are of the same size
            // and because this is a union, they are at the same address
            this->c8 = reinterpret_cast<pcre2_code_8*>(res);
            return;
        }

        throw_regex_error<CharT>(error_code);
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

}
}

#endif // PCRE2XX_PRIVATE_CODE_H
