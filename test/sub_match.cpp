#include <gtest/gtest.h>
#include <string>
#include "pcre2++/pcre2pp.h"

TEST(SubMatch, construct)
{
    pcre2::csub_match match;

    EXPECT_FALSE(match.matched);
    EXPECT_EQ(0, match.length());
    EXPECT_TRUE(match.str().empty());

    std::string s(match);
    EXPECT_TRUE(s.empty());

    EXPECT_TRUE(match.compare("something") < 0);
    EXPECT_TRUE(match.compare(std::string("something")) < 0);
    EXPECT_TRUE(match.compare(match) == 0);

    EXPECT_TRUE(match == match);
}
