#include <gtest/gtest.h>
#include <iostream>
#include "pcre2++/pcre2pp.h"

TEST(RegexError, basic)
{
    pcre2::regex_constants::error_type expected = pcre2::regex_constants::error_internal;
    try {
        throw pcre2::regex_error(expected);
    }
    catch (const pcre2::regex_error& e) {
        EXPECT_EQ(expected, e.code());
    }
}

TEST(RegexError, bad_regex)
{
    ASSERT_THROW(pcre2::regex("("), pcre2::regex_error);

    try {
        pcre2::regex r("(");
    }
    catch (const pcre2::regex_error& e) {
        // WARNING! PCRE2 provides no error codes for compilation errors
        // '114' was taken from pcre2_error.c and it is possible that
        // this number could change in the future
        // Error message matching this code is
        // 'missing closing parenthesis'
        EXPECT_EQ(114, e.code());
        EXPECT_EQ(std::string("missing closing parenthesis"), e.what());
    }
}
