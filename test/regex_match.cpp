#include <gtest/gtest.h>
#include <regex>
#include <string>
#include "pcre2++/pcre2pp.h"

TEST(RegexMatch, no_match)
{
    std::string pattern("get|getvalue");
    std::string target("target");
    std::regex sr(pattern.begin(), pattern.end(), std::regex_constants::icase);
    pcre2::regex mr(pattern.begin(), pattern.end(), pcre2::regex_constants::icase);
    std::smatch sm;
    pcre2::smatch mm;

    bool match = std::regex_match(target, sm, sr);
    ASSERT_FALSE(match);

    match = pcre2::regex_match(target, mm, mr);
    ASSERT_FALSE(match);

    /*
     * If the match does not exist:
     * m.ready() == true
     * m.empty() == true
     * m.size() == 0
     */
    EXPECT_TRUE(mm.ready());
    EXPECT_TRUE(mm.empty());
    EXPECT_EQ(0, mm.size());

    EXPECT_FALSE(mm.prefix().matched);
    EXPECT_FALSE(mm.suffix().matched);

    EXPECT_EQ(sm.ready(), mm.ready());
    EXPECT_EQ(sm.size(),  mm.size());
    EXPECT_EQ(sm.empty(), mm.empty());

    // Possibly implementation-specific:
    // position = strlen(target);
    EXPECT_EQ(target.size(), mm.position());
}

TEST(RegexMatch, match)
{
    std::string pattern("(get)|(getvalue)");
    std::string target("GetValue");
    std::regex sr(pattern.begin(), pattern.end(), std::regex_constants::icase);
    pcre2::regex mr(pattern.begin(), pattern.end(), pcre2::regex_constants::icase);
    std::smatch sm;
    pcre2::smatch mm;

    bool match = std::regex_match(target, sm, sr);
    ASSERT_TRUE(match);

    match = pcre2::regex_match(target, mm, mr);
    ASSERT_TRUE(match);

    /*
     * If the match exists:
     *  m.ready()   true
     *  m.empty()   false
     *  m.size()    number of marked subexpressions plus 1, that is, 1+e.mark_count()
     *  m.prefix().first    first
     *  m.prefix().second   first
     *  m.prefix().matched  false (the match prefix is empty)
     *  m.suffix().first    last
     *  m.suffix().second   last
     *  m.suffix().matched  false (the match suffix is empty)
     *  m[0].first      first
     *  m[0].second     last
     *  m[0].matched    true (the entire sequence is matched)
     *  m[n].first      the start of the sequence that matched marked sub-expression n, or last if the subexpression did not participate in the match
     *  m[n].second     the end of the sequence that matched marked sub-expression n, or last if the subexpression did not participate in the match
     *  m[n].matched    true if sub-expression n participated in the match, false otherwise
     */
    EXPECT_TRUE(mm.ready());
    EXPECT_FALSE(mm.empty());
    EXPECT_EQ(mr.mark_count() + 1, mm.size());
    EXPECT_EQ(target.begin(), mm.prefix().first);
    EXPECT_EQ(target.begin(), mm.prefix().second);
    EXPECT_FALSE(mm.prefix().matched);
    EXPECT_EQ(target.end(), mm.suffix().first);
    EXPECT_EQ(target.end(), mm.suffix().second);
    EXPECT_FALSE(mm.suffix().matched);
    EXPECT_EQ(target.begin(), mm[0].first);
    EXPECT_EQ(target.end(), mm[0].second);
    EXPECT_TRUE(mm[0].matched);
    EXPECT_EQ(target.end(), mm[1].first);
    EXPECT_EQ(target.end(), mm[1].second);
    EXPECT_FALSE(mm[1].matched);
    EXPECT_EQ(target.begin(), mm[2].first);
    EXPECT_EQ(target.end(), mm[2].second);
    EXPECT_TRUE(mm[2].matched);

    EXPECT_EQ(sm.ready(), mm.ready());
    EXPECT_EQ(sm.empty(), mm.empty());
    EXPECT_EQ(sm.size(),  mm.size());
    EXPECT_EQ(sm.prefix().matched, mm.prefix().matched);
    EXPECT_EQ(sm.suffix().matched, mm.suffix().matched);
    EXPECT_EQ(sm[0].matched,  mm[0].matched);
    EXPECT_EQ(sm[1].matched,  mm[1].matched);
    EXPECT_EQ(sm[2].matched,  mm[2].matched);
    EXPECT_EQ(sm[0].str(),    mm[0].str());
    EXPECT_EQ(sm[1].str(),    mm[1].str());
    EXPECT_EQ(sm[2].str(),    mm[2].str());
    EXPECT_EQ(sm[0].length(), mm[0].length());
    EXPECT_EQ(sm[1].length(), mm[1].length());
    EXPECT_EQ(sm[2].length(), mm[2].length());

    EXPECT_FALSE(mm[3].matched);
    EXPECT_EQ(0, mm[3].length());

    EXPECT_EQ(sm[3].matched,  mm[3].matched);
    EXPECT_EQ(sm[3].length(), mm[3].length());

    EXPECT_EQ(mm[0].length(), mm.length(0));
    EXPECT_EQ(mm[1].length(), mm.length(1));
    EXPECT_EQ(mm[2].length(), mm.length(2));

    EXPECT_EQ(sm.position(0), mm.position(0));
    EXPECT_EQ(sm.position(1), mm.position(1));
    EXPECT_EQ(sm.position(2), mm.position(2));
}

TEST(regex_match, exception)
{
    pcre2::regex re("\\w+", pcre2::regex_constants::utf);
    std::string pattern("\xC0");
    ASSERT_THROW(pcre2::regex_match(pattern, re), pcre2::regex_error);

    try {
        pcre2::regex_match(pattern, re);
    }
    catch (const pcre2::regex_error& e) {
        EXPECT_EQ(pcre2::regex_constants::error_utf8_err1, e.code());
    }
}
