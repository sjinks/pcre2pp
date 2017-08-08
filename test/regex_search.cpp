#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "pcre2++/pcre2pp.h"

/// @see http://en.cppreference.com/w/cpp/regex/regex_search
TEST(RegexSearch, cppref1)
{
    const std::string lines[] = {
        "Roses are #ff0000",
        "violets are #0000ff",
        "all of my base are belong to you"
    };

    pcre2::regex color_regex(
        "#([a-f0-9]{2})"
        "([a-f0-9]{2})"
        "([a-f0-9]{2})"
    );

    EXPECT_TRUE(pcre2::regex_search(lines[0], color_regex));
    EXPECT_TRUE(pcre2::regex_search(lines[1], color_regex));
    EXPECT_FALSE(pcre2::regex_search(lines[2], color_regex));

    pcre2::smatch color_match;

    pcre2::regex_search(lines[0], color_match, color_regex);
    EXPECT_EQ(std::string("Roses are "), color_match.prefix().str());
    EXPECT_EQ(std::string("#ff0000"), color_match[0]);
    EXPECT_EQ(std::string("ff"), color_match[1]);
    EXPECT_EQ(std::string("00"), color_match[2]);
    EXPECT_EQ(std::string("00"), color_match[3]);
    EXPECT_EQ(std::string(""), color_match.suffix());

    pcre2::regex_search(lines[1], color_match, color_regex);
    EXPECT_EQ(std::string("violets are "), color_match.prefix().str());
    EXPECT_EQ(std::string("#0000ff"), color_match[0]);
    EXPECT_EQ(std::string("00"), color_match[1]);
    EXPECT_EQ(std::string("00"), color_match[2]);
    EXPECT_EQ(std::string("ff"), color_match[3]);
    EXPECT_EQ(std::string(""), color_match.suffix());
}

TEST(RegexSearch, cppref2)
{
    std::string log(R"(
        Speed: 366
        Mass: 35
        Speed: 378
        Mass: 32
        Speed: 400
        Mass: 30)"
    );

    pcre2::regex r(R"(Speed: \d*)");
    pcre2::smatch sm;
    std::stringstream str;

    EXPECT_TRUE(pcre2::regex_search(log, sm, r));
    while (pcre2::regex_search(log, sm, r)) {
        str << sm.str() << '\n';
        log = sm.suffix();
    }

    const std::string expected("Speed: 366\nSpeed: 378\nSpeed: 400\n");
    std::string actual = str.str();
    EXPECT_EQ(expected, actual);
}

TEST(RegexSearch, cppref3)
{
    pcre2::cmatch cm;
    bool m;

    m = pcre2::regex_search("this is a test", cm, pcre2::regex("test"));
    ASSERT_TRUE(m);

    EXPECT_EQ("test", cm[0]);
    EXPECT_EQ(10, cm.prefix().length());
}

TEST(RegexSearch, empty_regex)
{
    pcre2::regex re;
    bool m;

    m = pcre2::regex_search("", re);
    EXPECT_FALSE(m);

    m = pcre2::regex_search("anything", re);
    EXPECT_FALSE(m);
}
