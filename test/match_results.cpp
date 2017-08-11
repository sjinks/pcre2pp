#include <gtest/gtest.h>
#include <string>
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

/**
 * @see http://pubs.opengroup.org/onlinepubs/9699919799/utilities/sed.html#tag_20_116_13_03
 */
TEST(MatchResults, format_sed)
{
    std::string email("Something <someone@example.com> anything");
    pcre2::regex re("<([^@]++)@([^>]++)>");
    pcre2::smatch m;
    bool f = pcre2::regex_search(email, m, re);

    EXPECT_TRUE(f);
    EXPECT_EQ(std::string("someone"), m[1].str());
    EXPECT_EQ(std::string("example.com"), m[2].str());

    std::string actual;

    // An <ampersand> ( '&' ) appearing in the replacement shall be replaced by the string matching the BRE.
    actual = m.format("!&!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!<someone@example.com>!"), actual);

    // The special meaning of '&' in this context can be suppressed by preceding it by a <backslash>.
    actual = m.format("!&\\&!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!<someone@example.com>&!"), actual);

    // The characters "\n", where n is a digit, shall be replaced by the text matched by the corresponding back-reference expression.
    actual = m.format("!\\1 \\2!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!someone example.com!"), actual);

    // If the corresponding back-reference expression does not match, then the characters "\n" shall be replaced by the empty string.
    actual = m.format("!\\3!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!!"), actual);

    // The special meaning of "\n" where n is a digit in this context, can be suppressed by preceding it by a <backslash>.
    actual = m.format("!\\\\1 \\2!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!\\1 example.com!"), actual);

    /* */
    actual = m.format("!\\x \\2!", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("!\\x example.com!"), actual);

    /* Corner case - backslash is the last character */
    actual = m.format("\\1 \\", pcre2::regex_constants::format_sed);
    EXPECT_EQ(std::string("someone \\"), actual);
}

TEST(MatchResults, format_default)
{
    {
        // http://en.cppreference.com/w/cpp/regex/match_results/format
        std::string s = "for a good time, call 867-5309";
        pcre2::regex phone_regex("\\d{3}-\\d{4}");
        pcre2::smatch phone_match;
        bool f = pcre2::regex_search(s, phone_match, phone_regex);

        EXPECT_TRUE(f);

        std::string actual = phone_match.format(
            "<$`>"    // $` means characters before the match
            "[$&]"    // $& means the matched characters
            "<$'>"    // $' means characters following the match
        );

        EXPECT_EQ(std::string("<for a good time, call >[867-5309]<>"), actual);
    }

    {
        std::string s = "867-5309 is the phone to call";
        pcre2::regex phone_regex("\\d{3}-\\d{4}");
        pcre2::smatch phone_match;
        bool f = pcre2::regex_search(s, phone_match, phone_regex);

        EXPECT_TRUE(f);

        std::string actual = phone_match.format(
            "<$`>"    // $` means characters before the match
            "[$&]"    // $& means the matched characters
            "<$'>"    // $' means characters following the match
        );

        EXPECT_EQ(std::string("<>[867-5309]< is the phone to call>"), actual);
    }

    {
        std::string s = "IP: 127.0.0.1 localhost";
        pcre2::regex re("([0-9]++)\\.([0-9]++)\\.([0-9]++)\\.([0-9]++)(:?)");
        pcre2::smatch m;
        std::string actual;
        bool f = pcre2::regex_search(s, m, re);

        EXPECT_TRUE(f);

        actual = m.format("$`");
        EXPECT_EQ(std::string("IP: "), actual);

        actual = m.format("$'");
        EXPECT_EQ(std::string(" localhost"), actual);

        actual = m.format("$&");
        EXPECT_EQ(std::string("127.0.0.1"), actual);

        actual = m.format("$0");
        EXPECT_EQ(std::string("127.0.0.1"), actual);

        actual = m.format("$00");
        EXPECT_EQ(std::string("127.0.0.1"), actual);

        actual = m.format(std::string("$x $$ $1 $5 $6 $4 $"));
        EXPECT_EQ(std::string("$x $ 127   1 $"), actual);
    }
}
