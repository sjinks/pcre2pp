#include <gtest/gtest.h>
#include <locale>
#include <regex>
#include <string>
#include "pcre2pp.h"

TEST(BasicRegexTest, mark_count)
{
    std::string pattern("\\w([0-9])+");
    std::u16string pattern16(u"\\w([0-9])+");
    std::u32string pattern32(U"\\w([0-9])+");
    std::regex sr(pattern);
    pcre2::regex mr(pattern);
    pcre2::regex16 mr16(pattern16);
    pcre2::regex32 mr32(pattern32);

    EXPECT_EQ(sr.mark_count(), mr.mark_count());
    EXPECT_EQ(sr.mark_count(), mr16.mark_count());
    EXPECT_EQ(sr.mark_count(), mr32.mark_count());
}

TEST(BasicRegexTest, locale)
{
    pcre2::regex mr;
    pcre2::regex16 mr16;
    pcre2::regex32 mr32;

    std::regex::locale_type loc;

    loc = mr.imbue(std::locale());
    EXPECT_TRUE(loc == mr.getloc());

    loc = mr16.imbue(std::locale());
    EXPECT_TRUE(loc == mr16.getloc());

    loc = mr32.imbue(std::locale());
    EXPECT_TRUE(loc == mr32.getloc());
}

TEST(BasicRegexTest, flags)
{
    pcre2::regex_constants::syntax_option_type exp1 = pcre2::regex_constants::icase;
    pcre2::regex_constants::syntax_option_type exp2 = pcre2::regex_constants::nosubs;
    pcre2::regex_constants::syntax_option_type exp3 = pcre2::regex_constants::multiline;

    pcre2::regex mr("test", exp1);
    pcre2::regex16 mr16(u"test", exp2);
    pcre2::regex32 mr32(U"test", exp3);

    EXPECT_EQ(exp1, mr.flags());
    EXPECT_EQ(exp2, mr16.flags());
    EXPECT_EQ(exp3, mr32.flags());
}

TEST(BasicRegexTest, flags_ops)
{
    pcre2::regex_constants::syntax_option_type t = pcre2::regex_constants::icase | pcre2::regex_constants::nosubs;
    EXPECT_EQ(PCRE2_CASELESS | PCRE2_NO_AUTO_CAPTURE, static_cast<std::uint32_t>(t));

    t = t ^ pcre2::regex_constants::nosubs;
    EXPECT_EQ(PCRE2_CASELESS, static_cast<std::uint32_t>(t));

    t = t & pcre2::regex_constants::nosubs;
    EXPECT_EQ(0, static_cast<std::uint32_t>(t));

    t |= pcre2::regex_constants::nosubs;
    EXPECT_EQ(PCRE2_NO_AUTO_CAPTURE, static_cast<std::uint32_t>(t));

    t ^= pcre2::regex_constants::icase;
    EXPECT_EQ(PCRE2_CASELESS | PCRE2_NO_AUTO_CAPTURE, static_cast<std::uint32_t>(t));

    t &= ~pcre2::regex_constants::nosubs;
    EXPECT_EQ(PCRE2_CASELESS, static_cast<std::uint32_t>(t));
}

TEST(BasicRegexTest, bad_regex)
{
    EXPECT_THROW(pcre2::regex mr("(test"), pcre2::regex_error);
    EXPECT_THROW(pcre2::regex16 mr16(u"(test"), pcre2::regex_error);
    EXPECT_THROW(pcre2::regex32 mr32(U"(test"), pcre2::regex_error);
}
