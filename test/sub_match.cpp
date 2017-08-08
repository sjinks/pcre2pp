#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include "pcre2++/pcre2pp.h"

TEST(SubMatch, construct)
{
    pcre2::csub_match match;

    EXPECT_FALSE(match.matched);
    EXPECT_EQ(0, match.length());
    EXPECT_TRUE(match.str().empty());

    std::string s(match);
    EXPECT_TRUE(s.empty());
}

TEST(SubMatch, compare)
{
    pcre2::csub_match match;
    std::string empty;
    std::string a("a");

    EXPECT_TRUE(match.compare("something") < 0);
    EXPECT_TRUE(match.compare(std::string("something")) < 0);
    EXPECT_TRUE(match.compare(match) == 0);

    EXPECT_TRUE(match == match);
    EXPECT_TRUE(match <= match);
    EXPECT_TRUE(match >= match);
    EXPECT_FALSE(match < match);
    EXPECT_FALSE(match > match);
    EXPECT_FALSE(match != match);


    EXPECT_TRUE(match == "");
    EXPECT_TRUE(match <= "");
    EXPECT_TRUE(match >= "");
    EXPECT_FALSE(match < "");
    EXPECT_FALSE(match > "");
    EXPECT_FALSE(match != "");

    EXPECT_TRUE("" == match);
    EXPECT_TRUE("" <= match);
    EXPECT_TRUE("" >= match);
    EXPECT_FALSE("" < match);
    EXPECT_FALSE("" > match);
    EXPECT_FALSE("" != match);

    EXPECT_FALSE(match == "a");
    EXPECT_TRUE(match <= "a");
    EXPECT_FALSE(match >= "a");
    EXPECT_TRUE(match < "a");
    EXPECT_FALSE(match > "");
    EXPECT_TRUE(match != "a");

    EXPECT_FALSE("a" == match);
    EXPECT_FALSE("a" <= match);
    EXPECT_TRUE("a" >= match);
    EXPECT_FALSE("a" < match);
    EXPECT_TRUE("a" > match);
    EXPECT_TRUE("a" != match);


    EXPECT_TRUE(match == empty);
    EXPECT_TRUE(match <= empty);
    EXPECT_TRUE(match >= empty);
    EXPECT_FALSE(match < empty);
    EXPECT_FALSE(match > empty);
    EXPECT_FALSE(match != empty);

    EXPECT_TRUE(empty == match);
    EXPECT_TRUE(empty <= match);
    EXPECT_TRUE(empty >= match);
    EXPECT_FALSE(empty < match);
    EXPECT_FALSE(empty > match);
    EXPECT_FALSE(empty != match);

    EXPECT_FALSE(match == a);
    EXPECT_TRUE(match <= a);
    EXPECT_FALSE(match >= a);
    EXPECT_TRUE(match < a);
    EXPECT_FALSE(match > a);
    EXPECT_TRUE(match != a);

    EXPECT_FALSE(a == match);
    EXPECT_FALSE(a <= match);
    EXPECT_TRUE(a >= match);
    EXPECT_FALSE(a < match);
    EXPECT_TRUE(a > match);
    EXPECT_TRUE(a != match);


    EXPECT_FALSE(match == 'a');
    EXPECT_TRUE(match <= 'a');
    EXPECT_FALSE(match >= 'a');
    EXPECT_TRUE(match < 'a');
    EXPECT_FALSE(match > 'a');
    EXPECT_TRUE(match != 'a');

    EXPECT_FALSE('a' == match);
    EXPECT_FALSE('a' <= match);
    EXPECT_TRUE('a' >= match);
    EXPECT_FALSE('a' < match);
    EXPECT_TRUE('a' > match);
    EXPECT_TRUE('a' != match);
}

TEST(SubMatch, stream)
{
    std::stringstream stream;
    pcre2::csub_match match;

    stream << match;
    EXPECT_EQ(std::string(), stream.str());
}
