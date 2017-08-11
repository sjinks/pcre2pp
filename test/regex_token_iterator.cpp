#include <gtest/gtest.h>
#include <algorithm>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>
#include "pcre2++/pcre2pp.h"

/**
 * @see http://en.cppreference.com/w/cpp/regex/regex_token_iterator
 */
TEST(RegexTokenIterator, cppref1)
{
    // tokenization (non-matched fragments)
    // Note that regex is matched only two times: when the third value is obtained
    // the iterator is a suffix iterator.
    std::string text = "Quick brown fox.";
    pcre2::regex ws_re("\\s+");
    std::stringstream stream;
    std::copy(pcre2::sregex_token_iterator(text.begin(), text.end(), ws_re, -1), pcre2::sregex_token_iterator(), std::ostream_iterator<std::string>(stream, "\n"));

    std::string expected("Quick\nbrown\nfox.\n");
    std::string actual(stream.str());

    EXPECT_EQ(expected, actual);
}

TEST(RegexTokenIterator, cppref2)
{
    // iterating the first submatches
    std::string html = "<p><a href=\"http://google.com\">google</a> "
                       "< a HREF =\"http://cppreference.com\">cppreference</a>\n</p>";
    pcre2::regex url_re("<\\s*A\\s+[^>]*href\\s*=\\s*\"([^\"]*)\"", pcre2::regex_constants::icase);
    std::stringstream stream;
    std::copy(
        pcre2::sregex_token_iterator(html.begin(), html.end(), url_re, 1),
        pcre2::sregex_token_iterator(),
        std::ostream_iterator<std::string>(stream, "\n")
    );

    std::string expected("http://google.com\nhttp://cppreference.com\n");
    std::string actual(stream.str());

    EXPECT_EQ(expected, actual);
}

TEST(RegexTokenIterator, test1)
{
    std::string str("In the end");
    pcre2::regex re("\\b(\\w)\\w*?(\\w)\\b");
    std::regex rx("\\b(\\w)\\w*?(\\w)\\b");
    std::stringstream stream;
    std::copy(
        pcre2::sregex_token_iterator(str.begin(), str.end(), re, {1, 2, -1, 0}),
        pcre2::sregex_token_iterator(),
        std::ostream_iterator<std::string>(stream, "\n")
    );

    std::string expected("I\nn\n\nIn\nt\ne\n \nthe\ne\nd\n \nend\n");
    std::string actual(stream.str());

    EXPECT_EQ(expected, actual);

    stream.str(std::string());
    std::copy(
        std::sregex_token_iterator(str.begin(), str.end(), rx, {1, 2, -1, 0}),
        std::sregex_token_iterator(),
        std::ostream_iterator<std::string>(stream, "\n")
    );
    actual = stream.str();
    EXPECT_EQ(expected, actual);
}

TEST(RegexTokenIterator, test2)
{
    std::string str(" test");
    pcre2::regex re(" ");

    auto cur = pcre2::sregex_token_iterator(str.begin(), str.end(), re, std::vector<int>{-1});
    auto beg = cur;
    auto end = pcre2::sregex_token_iterator();

    auto it1 = cur++;
    auto it2 = cur++;

    EXPECT_EQ(end, cur);
    EXPECT_EQ(beg, it1);
    EXPECT_EQ(std::string("test"), *it2);
}

TEST(RegexTokenIterator, test3)
{
    std::string str("test");
    pcre2::regex re("\\s+");
    int m[1] = {-1};

    auto cur = pcre2::sregex_token_iterator(str.begin(), str.end(), re, m);
    auto it(cur);
    EXPECT_EQ(it, cur);
    EXPECT_EQ(std::string("test"), cur->str());
}
