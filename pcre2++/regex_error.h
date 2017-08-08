#ifndef PCRE2XX_REGEX_ERROR_H
#define PCRE2XX_REGEX_ERROR_H

#include <stdexcept>
#include <string>

#include "private/pcre2.h"
#include "regex_constants.h"

namespace pcre2 {

namespace details {
template<typename T> union code;
}

class regex_error : public std::runtime_error {
public:
    regex_error(regex_constants::error_type ecode)
        : std::runtime_error(std::string()), m_code(ecode)
    {
    }

    virtual ~regex_error() throw() override
    {
    }

    regex_constants::error_type code() const
    {
        return this->m_code;
    }

private:
    regex_constants::error_type m_code;

public:
    regex_error(regex_constants::error_type c, const std::string& what)
        : std::runtime_error(what), m_code(c)
    {
    }

    template<typename T>
    friend union details::code;
};

}

#endif // PCRE2XX_REGEX_ERROR_H
