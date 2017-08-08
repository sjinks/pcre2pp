#ifndef PCRE2XX_PRIVATE_REGEX_PRIVATE_H
#define PCRE2XX_PRIVATE_REGEX_PRIVATE_H

#include "pcre2++/private/code.h"
#include "pcre2++/private/compile_context.h"
#include "pcre2++/private/match_data.h"
#include "pcre2++/private/pcre2.h"
#include "pcre2++/private/table_map.h"

namespace pcre2 {
namespace details {

template<typename Traits>
class regex_private {
public:
    using traits_type = Traits;
    using value_type  = typename traits_type::char_type;
    using string_type = typename traits_type::string_type;
    using flag_type   = regex_constants::syntax_option_type;
    using locale_type = typename traits_type::locale_type;

    regex_private(const value_type* s, std::size_t len, locale_type loc, flag_type f)
        : m_re(string_type(s, len)), m_loc(loc), m_flags(f), m_ctx(), m_code()
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
        this->m_mdata.create_from_pattern(this->m_code);
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

private:
    string_type m_re;
    locale_type m_loc;
    flag_type m_flags;
    compile_context<value_type> m_ctx;
    code<value_type> m_code;
    match_data<value_type> m_mdata;
};

}
}

#endif // PCRE2XX_PRIVATE_REGEX_PRIVATE_H
