#include <gtest/gtest.h>
#include <iterator>
#include <regex>
#include <string>
#include "pcre2++/pcre2pp.h"

TEST(RegexIterator, cppref)
{
    const std::string s = "Quick brown fox.";

    pcre2::regex words_regex("[^\\s]+");
    auto words_begin = pcre2::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end   = pcre2::sregex_iterator();

    ASSERT_EQ(3, std::distance(words_begin, words_end));
    pcre2::sregex_iterator i = words_begin;
    pcre2::smatch match;

    match = *i;
    ASSERT_NE(words_end, i);
    EXPECT_EQ(std::string("Quick"),  match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string("brown"),  match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string("fox."),  match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    EXPECT_EQ(words_end, i);
}

TEST(RegexIterator, empty_match)
{
    const std::string s = "cacbcab";

    pcre2::regex words_regex("a?b?");
    auto words_begin = pcre2::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end   = pcre2::sregex_iterator();

    ASSERT_EQ(7, std::distance(words_begin, words_end));
    pcre2::sregex_iterator i = words_begin;
    pcre2::smatch match;

    match = *i;
    ASSERT_NE(words_end, i);
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string("a"), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string("b"), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string("ab"), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    ASSERT_NE(words_end, i);
    match = *i;
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());

    ++i;
    EXPECT_EQ(words_end, i);
}

TEST(RegexIterator, empty_match2)
{
    const std::string s = "bc";

    pcre2::regex words_regex("a?|c");
    std::regex wr("a?|c");

    auto words_begin = pcre2::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end   = pcre2::sregex_iterator();
    auto wb          = std::sregex_iterator(s.begin(), s.end(), wr);
    auto we          = std::sregex_iterator();

    ASSERT_EQ(4, std::distance(words_begin, words_end));
    ASSERT_EQ(std::distance(wb, we), std::distance(words_begin, words_end));

    pcre2::sregex_iterator i = words_begin;
    std::sregex_iterator j   = wb;
    pcre2::smatch match;
    std::smatch m;

    match = *i;
    m     = *j;
    ASSERT_NE(words_end, i);
    ASSERT_NE(we, j);
    EXPECT_EQ(m.str(), match.str());
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());
    EXPECT_FALSE(match.prefix().matched);
    EXPECT_EQ(m.prefix().matched, match.prefix().matched);

    ++i;
    ++j;
    ASSERT_NE(words_end, i);
    ASSERT_NE(we, j);
    match = *i;
    m     = *j;
    EXPECT_EQ(m.str(), match.str());
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());
    EXPECT_TRUE(match.prefix().matched);
    EXPECT_EQ(m.prefix().matched, match.prefix().matched);

    ++i;
    ++j;
    ASSERT_NE(words_end, i);
    ASSERT_NE(we, j);
    match = *i;
    m     = *j;
    EXPECT_EQ(m.str(), match.str());
    EXPECT_EQ(std::string("c"), match.str());
    EXPECT_EQ(match.str(), i->str());
    EXPECT_FALSE(match.prefix().matched);
    EXPECT_EQ(m.prefix().matched, match.prefix().matched);

    ++i;
    ++j;
    ASSERT_NE(words_end, i);
    ASSERT_NE(we, j);
    match = *i;
    m     = *j;
    EXPECT_EQ(m.str(), match.str());
    EXPECT_EQ(std::string(), match.str());
    EXPECT_EQ(match.str(), i->str());
    EXPECT_FALSE(match.prefix().matched);
    EXPECT_EQ(m.prefix().matched, match.prefix().matched);

    ++i;
    ++j;
    EXPECT_EQ(words_end, i);
    EXPECT_EQ(we, j);
}

TEST(RegexIterator, no_match)
{
    const std::string s = "string";

    pcre2::regex words_regex("\\s+");
    auto words_begin = pcre2::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end   = pcre2::sregex_iterator();

    ASSERT_EQ(0, std::distance(words_begin, words_end));
    ASSERT_EQ(words_begin, words_end);
}

TEST(RegexIterator, compare)
{
    const std::string s = "Quick brown fox.";

    pcre2::regex words_regex("[^\\s]+");
    auto words_begin = pcre2::sregex_iterator(s.begin(), s.end(), words_regex);
    auto words_end   = pcre2::sregex_iterator();

    ASSERT_NE(words_begin, words_end);

    pcre2::sregex_iterator i = words_begin;
    ASSERT_EQ(words_begin, i);

    pcre2::sregex_iterator j = i++;
    ASSERT_EQ(words_begin, j);
}
