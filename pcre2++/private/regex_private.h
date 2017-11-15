#ifndef PCRE2XX_PRIVATE_REGEX_PRIVATE_H
#define PCRE2XX_PRIVATE_REGEX_PRIVATE_H

#include "private/pcre2.h"
#include "private/code.h"
#include "private/compile_context.h"
#include "private/match_data.h"
#include "private/table_map.h"
#include "private/wrappers.h"

namespace pcre2 {
namespace details {

template<typename CharT>
class regex_private {
public:
    using value_type  = CharT;
    using string_type = std::basic_string<CharT>;
    using flag_type   = regex_constants::syntax_option_type;
    using locale_type = std::locale;

    regex_private(const value_type* s, std::size_t len, locale_type loc, flag_type f)
        : m_re(string_type(s, len)), m_loc(loc), m_flags(f), m_real_flags(), m_ctx(), m_code()
    {
        if (f & pcre2::regex_constants::collate) {
            this->m_ctx.create();
            table_map& tm = table_map::instance();
            uint8_t* tbl  = tm.get(loc.name());
            if (!tbl) {
                char* orig = std::setlocale(LC_CTYPE, loc.name().c_str());
                tbl        = maketables<value_type>();
                std::setlocale(LC_CTYPE, orig);
                tm.set(loc.name(), tbl);
            }

            this->m_ctx.set_character_table(tbl);
        }

        this->m_code.compile(s, len, f, this->m_ctx);

        if (f & pcre2::regex_constants::optimize) {
            jit_compile(this->m_code.get(), PCRE2_JIT_COMPLETE);
        }

        this->m_mdata.create_from_pattern(this->m_code);
        this->m_real_flags = pattern_info_alloptions(this->m_code.get());
    }

    unsigned int mark_count() const
    {
        return this->m_mdata.mark_count() - 1;
    }

    code<value_type>& get_code()
    {
        return this->m_code;
    }

    match_data<value_type>& get_match_data()
    {
        return this->m_mdata;
    }

    string_type& regex()
    {
        return this->m_re;
    }

    bool isUtf() const
    {
        return this->m_real_flags & PCRE2_UTF;
    }

private:
    string_type m_re;
    locale_type m_loc;
    flag_type m_flags;
    std::uint32_t m_real_flags;
    compile_context<value_type> m_ctx;
    code<value_type> m_code;
    match_data<value_type> m_mdata;
};

}
}

#endif // PCRE2XX_PRIVATE_REGEX_PRIVATE_H
