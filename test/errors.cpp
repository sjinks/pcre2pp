#include <gtest/gtest.h>
#include <cstring>
#include "pcre2++/pcre2pp.h"
#include "pcre2++/private/wrappers.h"
#include "pcre2++/private/code.h"
#include "pcre2++/private/compile_context.h"

TEST(Errors, test1)
{
    pcre2::details::compile_context<char> ctx8;
    pcre2::details::compile_context<char16_t> ctx16;
    pcre2::details::compile_context<char32_t> ctx32;
    pcre2::details::code<char> code8;
    pcre2::details::code<char16_t> code16;
    pcre2::details::code<char32_t> code32;

    ASSERT_NO_THROW(code8.compile("a", 1, pcre2::regex_constants::pcre2, ctx8));
    ASSERT_NO_THROW(code16.compile(u"a", 1, pcre2::regex_constants::pcre2, ctx16));
    ASSERT_NO_THROW(code32.compile(U"a", 1, pcre2::regex_constants::pcre2, ctx32));

    pcre2_code_8* i8;
    pcre2_code_16* i16;
    pcre2_code_32* i32;

    i8  = code8.get();
    i16 = reinterpret_cast<pcre2_code_16*>(i8);

    EXPECT_THROW(pcre2::details::pattern_info_alloptions(i16), pcre2::regex_error);

    i16 = code16.get();
    i32 = reinterpret_cast<pcre2_code_32*>(i16);
    EXPECT_THROW(pcre2::details::pattern_info_alloptions(i32), pcre2::regex_error);

    i32 = code32.get();
    i8  = reinterpret_cast<pcre2_code_8*>(i32);
    EXPECT_THROW(pcre2::details::pattern_info_alloptions(i8), pcre2::regex_error);
}
