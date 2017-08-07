#ifndef PCRE2XX_REGEX_CONSTANTS_H
#define PCRE2XX_REGEX_CONSTANTS_H

#include <cstdint>

#include "../pcre2++/private/pcre2.h"

namespace pcre2 {

namespace regex_constants {

enum syntax_option_type : std::uint64_t {};

using error_type         = int;
using match_flag_type    = std::uint64_t;

constexpr syntax_option_type pcre2      = static_cast<syntax_option_type>(0);
constexpr syntax_option_type icase      = static_cast<syntax_option_type>(PCRE2_CASELESS);
constexpr syntax_option_type nosubs     = static_cast<syntax_option_type>(PCRE2_NO_AUTO_CAPTURE);
constexpr syntax_option_type optimize   = static_cast<syntax_option_type>(UINT64_C(0x1000000000000000));
constexpr syntax_option_type collate    = static_cast<syntax_option_type>(UINT64_C(0x2000000000000000));
constexpr syntax_option_type multiline  = static_cast<syntax_option_type>(PCRE2_MULTILINE);

constexpr inline syntax_option_type operator&(syntax_option_type a, syntax_option_type b)
{
    return static_cast<syntax_option_type>(static_cast<unsigned int>(a) & static_cast<unsigned int>(b));
}

constexpr inline syntax_option_type operator|(syntax_option_type a, syntax_option_type b)
{
    return static_cast<syntax_option_type>(static_cast<unsigned int>(a) | static_cast<unsigned int>(b));
}

constexpr inline syntax_option_type operator^(syntax_option_type a, syntax_option_type b)
{
    return static_cast<syntax_option_type>(static_cast<unsigned int>(a) ^ static_cast<unsigned int>(b));
}

constexpr inline syntax_option_type operator~(syntax_option_type a)
{
    return static_cast<syntax_option_type>(~static_cast<unsigned int>(a));
}

inline syntax_option_type& operator&=(syntax_option_type& a, syntax_option_type b)
{
    return a = a & b;
}

inline syntax_option_type& operator|=(syntax_option_type& a, syntax_option_type b)
{
    return a = a | b;
}

inline syntax_option_type& operator^=(syntax_option_type& a, syntax_option_type b)
{
    return a = a ^ b;
}


constexpr error_type error_utf8_err1            = PCRE2_ERROR_UTF8_ERR1;
constexpr error_type error_utf8_err2            = PCRE2_ERROR_UTF8_ERR2;
constexpr error_type error_utf8_err3            = PCRE2_ERROR_UTF8_ERR3;
constexpr error_type error_utf8_err4            = PCRE2_ERROR_UTF8_ERR4;
constexpr error_type error_utf8_err5            = PCRE2_ERROR_UTF8_ERR5;
constexpr error_type error_utf8_err6            = PCRE2_ERROR_UTF8_ERR6;
constexpr error_type error_utf8_err7            = PCRE2_ERROR_UTF8_ERR7;
constexpr error_type error_utf8_err8            = PCRE2_ERROR_UTF8_ERR8;
constexpr error_type error_utf8_err9            = PCRE2_ERROR_UTF8_ERR9;
constexpr error_type error_utf8_err10           = PCRE2_ERROR_UTF8_ERR10;
constexpr error_type error_utf8_err11           = PCRE2_ERROR_UTF8_ERR11;
constexpr error_type error_utf8_err12           = PCRE2_ERROR_UTF8_ERR12;
constexpr error_type error_utf8_err13           = PCRE2_ERROR_UTF8_ERR13;
constexpr error_type error_utf8_err14           = PCRE2_ERROR_UTF8_ERR14;
constexpr error_type error_utf8_err15           = PCRE2_ERROR_UTF8_ERR15;
constexpr error_type error_utf8_err16           = PCRE2_ERROR_UTF8_ERR16;
constexpr error_type error_utf8_err17           = PCRE2_ERROR_UTF8_ERR17;
constexpr error_type error_utf8_err18           = PCRE2_ERROR_UTF8_ERR18;
constexpr error_type error_utf8_err19           = PCRE2_ERROR_UTF8_ERR19;
constexpr error_type error_utf8_err20           = PCRE2_ERROR_UTF8_ERR20;
constexpr error_type error_utf8_err21           = PCRE2_ERROR_UTF8_ERR21;
constexpr error_type error_utf16_err1           = PCRE2_ERROR_UTF16_ERR1;
constexpr error_type error_utf16_err2           = PCRE2_ERROR_UTF16_ERR2;
constexpr error_type error_utf16_err3           = PCRE2_ERROR_UTF16_ERR3;
constexpr error_type error_utf32_err1           = PCRE2_ERROR_UTF32_ERR1;
constexpr error_type error_utf32_err2           = PCRE2_ERROR_UTF32_ERR2;
constexpr error_type error_bad_data             = PCRE2_ERROR_BADDATA;
constexpr error_type error_mixed_tables         = PCRE2_ERROR_MIXEDTABLES;
constexpr error_type error_bad_magic            = PCRE2_ERROR_BADMAGIC;
constexpr error_type error_bad_mode             = PCRE2_ERROR_BADMODE;
constexpr error_type error_bad_offset           = PCRE2_ERROR_BADOFFSET;
constexpr error_type error_bad_option           = PCRE2_ERROR_BADOPTION;
constexpr error_type error_bad_replacement      = PCRE2_ERROR_BADREPLACEMENT;
constexpr error_type error_bad_utf_offset       = PCRE2_ERROR_BADUTFOFFSET;
constexpr error_type error_callout              = PCRE2_ERROR_CALLOUT;
constexpr error_type error_dfa_bad_restart      = PCRE2_ERROR_DFA_BADRESTART;
constexpr error_type error_dfa_recurse          = PCRE2_ERROR_DFA_RECURSE;
constexpr error_type error_dfa_ucond            = PCRE2_ERROR_DFA_UCOND;
constexpr error_type error_dfa_ufunc            = PCRE2_ERROR_DFA_UFUNC;
constexpr error_type error_dfa_uitem            = PCRE2_ERROR_DFA_UITEM;
constexpr error_type error_dfa_wssize           = PCRE2_ERROR_DFA_WSSIZE;
constexpr error_type error_internal             = PCRE2_ERROR_INTERNAL;
constexpr error_type error_jit_bad_option       = PCRE2_ERROR_JIT_BADOPTION;
constexpr error_type error_jit_stack_limit      = PCRE2_ERROR_JIT_STACKLIMIT;
constexpr error_type error_match_limit          = PCRE2_ERROR_MATCHLIMIT;
constexpr error_type error_no_memory            = PCRE2_ERROR_NOMEMORY;
constexpr error_type error_no_substring         = PCRE2_ERROR_NOSUBSTRING;
constexpr error_type error_non_unique_substring = PCRE2_ERROR_NOUNIQUESUBSTRING;
constexpr error_type error_null                 = PCRE2_ERROR_NULL;
constexpr error_type error_recurse_loop         = PCRE2_ERROR_RECURSELOOP;
constexpr error_type error_recursion_limit      = PCRE2_ERROR_RECURSIONLIMIT;
constexpr error_type error_unavailable          = PCRE2_ERROR_UNAVAILABLE;
constexpr error_type error_unset                = PCRE2_ERROR_UNSET;
constexpr error_type error_bad_offset_limit     = PCRE2_ERROR_BADOFFSETLIMIT;
constexpr error_type error_bad_rep_escape       = PCRE2_ERROR_BADREPESCAPE;
constexpr error_type error_rep_missing_brace    = PCRE2_ERROR_REPMISSINGBRACE;
constexpr error_type error_bad_subsutitution    = PCRE2_ERROR_BADSUBSTITUTION;
constexpr error_type error_bad_subs_pattern     = PCRE2_ERROR_BADSUBSPATTERN;
constexpr error_type error_too_many_replace     = PCRE2_ERROR_TOOMANYREPLACE;
constexpr error_type error_bad_serialized_data  = PCRE2_ERROR_BADSERIALIZEDDATA;


constexpr match_flag_type match_default    = 0;
constexpr match_flag_type match_not_bol    = PCRE2_NOTBOL;    // This option specifies that first character of the subject string is not the beginning of a line, so the circumflex metacharacter should not match before it. Setting this without having set PCRE2_MULTILINE at compile time causes circumflex never to match. This option affects only the behaviour of the circumflex metacharacter. It does not affect \A.
constexpr match_flag_type match_not_eol    = PCRE2_NOTEOL;    // This option specifies that the end of the subject string is not the end of a line, so the dollar metacharacter should not match it nor (except in multiline mode) a newline immediately before it. Setting this without having set PCRE2_MULTILINE at compile time causes dollar never to match. This option affects only the behaviour of the dollar metacharacter. It does not affect \Z or \z.
//constexpr match_flag_type match_not_bow    = 0;     // NOT IMPLEMENTED
//constexpr match_flag_type match_not_eow    = 0;     // NOT IMPLEMENTED
constexpr match_flag_type match_any        = 0;
constexpr match_flag_type match_not_null   = PCRE2_NOTEMPTY;  // An empty string is not considered to be a valid match if this option is set. If there are alternatives in the pattern, they are tried. If all the alternatives match the empty string, the entire match fails.
constexpr match_flag_type match_continuous = PCRE2_ANCHORED;
//constexpr match_flag_type match_prev_avail = 0      // NOT IMPLEMENTED

constexpr match_flag_type format_default   = 0;

}

}

#endif // PCRE2XX_REGEX_CONSTANTS_H
