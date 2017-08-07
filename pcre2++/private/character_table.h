#ifndef PCRE2XX_PRIVATE_CHARACTER_TABLE_H
#define PCRE2XX_PRIVATE_CHARACTER_TABLE_H

#include "../../pcre2++/private/pcre2.h"

namespace pcre2 {
namespace details {

template<typename CharT>
class character_table {
public:
    character_table();

    operator const unsigned char*() const
    {
        return this->m_tbl;
    }

private:
    const unsigned char* m_tbl;
};

template<>
inline character_table<char>::character_table()
{
    this->m_tbl = pcre2_maketables_8(nullptr);
}

template<>
inline character_table<char16_t>::character_table()
{
    this->m_tbl = pcre2_maketables_16(nullptr);
}

template<>
inline character_table<char32_t>::character_table()
{
    this->m_tbl = pcre2_maketables_32(nullptr);
}

}
}

#endif // PCRE2XX_PRIVATE_CHARACTER_TABLE_H
