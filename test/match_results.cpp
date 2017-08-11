#include <gtest/gtest.h>
#include "pcre2++/pcre2pp.h"

TEST(MatchResults, basic)
{
    pcre2::smatch m;

    EXPECT_FALSE(m.ready());
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(0, m.size());
    EXPECT_EQ(m.begin(), m.end());
    EXPECT_EQ(m.cbegin(), m.cend());
}
