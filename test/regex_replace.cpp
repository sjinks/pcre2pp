#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "pcre2++/pcre2pp.h"

/**
 * @see http://en.cppreference.com/w/cpp/regex/regex_replace
 */
TEST(RegexReplace, cppref1)
{
    std::string text = "Quick brown fox";
    pcre2::regex vowel_re("a|e|i|o|u");
    std::stringstream str;

    pcre2::regex_replace(std::ostreambuf_iterator<char>(str), text.begin(), text.end(), vowel_re, "*");
    std::string actual = str.str();
    std::string expected("Q**ck br*wn f*x");

    EXPECT_EQ(expected, actual);
}

TEST(RegexReplace, cppref2)
{
    std::string text = "Quick brown fox";
    pcre2::regex vowel_re("a|e|i|o|u");
    std::stringstream str;

    std::string actual = pcre2::regex_replace(text, vowel_re, std::string("[$&]"));
    std::string expected("Q[u][i]ck br[o]wn f[o]x");

    EXPECT_EQ(expected, actual);
}

TEST(RegexReplace, format_first_only)
{
    pcre2::regex vowel_re("a|e|i|o|u");
    std::stringstream str;

    std::string actual = pcre2::regex_replace("Quick brown fox", vowel_re, std::string("[$&]"), pcre2::regex_constants::format_first_only);
    std::string expected("Q[u]ick brown fox");

    EXPECT_EQ(expected, actual);
}

TEST(RegexReplace, format_no_copy)
{
    pcre2::regex vowel_re("a|e|i|o|u");
    std::stringstream str;

    std::string actual = pcre2::regex_replace("Quick brown fox", vowel_re, "[$&]", pcre2::regex_constants::format_no_copy);
    std::string expected("[u][i][o][o]");

    EXPECT_EQ(expected, actual);
}

TEST(RegexReplace, no_match)
{
    pcre2::regex re("box");
    std::stringstream str;

    std::string actual = pcre2::regex_replace(std::string("Quick brown fox"), re, "[$&]");
    std::string expected("Quick brown fox");

    EXPECT_EQ(expected, actual);
}
