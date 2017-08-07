#ifndef PCRE2XX_PRIVATE_PCRE2_H
#define PCRE2XX_PRIVATE_PCRE2_H

#include <codecvt>
#include <cstdint>
#include <locale>
#include <string>

#define PCRE2_CODE_UNIT_WIDTH 0
#include <pcre2.h>

namespace pcre2 {
namespace details {

static inline void compile_context_free(pcre2_compile_context_8* ctx)
{
    pcre2_compile_context_free_8(ctx);
}

static inline void compile_context_free(pcre2_compile_context_16* ctx)
{
    pcre2_compile_context_free_16(ctx);
}

static inline void compile_context_free(pcre2_compile_context_32* ctx)
{
    pcre2_compile_context_free_32(ctx);
}

static inline void set_character_tables(pcre2_compile_context_8* ctx, const std::uint8_t* tbl)
{
    pcre2_set_character_tables_8(ctx, tbl);
}

static inline void set_character_tables(pcre2_compile_context_16* ctx, const std::uint8_t* tbl)
{
    pcre2_set_character_tables_16(ctx, tbl);
}

static inline void set_character_tables(pcre2_compile_context_32* ctx, const std::uint8_t* tbl)
{
    pcre2_set_character_tables_32(ctx, tbl);
}

static inline pcre2_code_8* compile(const char* re, std::size_t len, std::uint32_t options, int& error_code, PCRE2_SIZE& error_offset, pcre2_compile_context_8* ctx)
{
    return pcre2_compile_8(
        reinterpret_cast<PCRE2_SPTR8>(re),
        static_cast<PCRE2_SIZE>(len),
        options,
        &error_code,
        &error_offset,
        ctx
    );
}

static inline pcre2_code_16* compile(char16_t* re, std::size_t len, std::uint32_t options, int& error_code, PCRE2_SIZE& error_offset, pcre2_compile_context_16* ctx)
{
    return pcre2_compile_16(
        reinterpret_cast<PCRE2_SPTR16>(re),
        static_cast<PCRE2_SIZE>(len),
        options,
        &error_code,
        &error_offset,
        ctx
    );
}

static inline pcre2_code_32* compile(char32_t* re, std::size_t len, std::uint32_t options, int& error_code, PCRE2_SIZE& error_offset, pcre2_compile_context_32* ctx)
{
    return pcre2_compile_32(
        reinterpret_cast<PCRE2_SPTR32>(re),
        static_cast<PCRE2_SIZE>(len),
        options,
        &error_code,
        &error_offset,
        ctx
    );
}

static inline std::string get_error_message(int code, char* buf, std::size_t len)
{
    int l = pcre2_get_error_message_8(code, reinterpret_cast<PCRE2_UCHAR8*>(buf), len);
    return std::string(buf, (l > 0 ? l : len));
}

static inline std::string get_error_message(int code, char16_t* buf, std::size_t len)
{
    int l = pcre2_get_error_message_16(code, reinterpret_cast<PCRE2_UCHAR16*>(buf), len);
    std::u16string err(buf, (l > 0 ? l : len));
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
    return conv.to_bytes(err);
}

static inline std::string get_error_message(int code, char32_t* buf, std::size_t len)
{
    int l = pcre2_get_error_message_32(code, reinterpret_cast<PCRE2_UCHAR32*>(buf), len);
    std::u32string err(buf, (l > 0 ? l : len));
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
    return conv.to_bytes(err);
}

static inline pcre2_match_data_8* match_data_create_from_pattern(const pcre2_code_8* re)
{
    return pcre2_match_data_create_from_pattern_8(re, nullptr);
}

static inline pcre2_match_data_16* match_data_create_from_pattern(const pcre2_code_16* re)
{
    return pcre2_match_data_create_from_pattern_16(re, nullptr);
}

static inline pcre2_match_data_32* match_data_create_from_pattern(const pcre2_code_32* re)
{
    return pcre2_match_data_create_from_pattern_32(re, nullptr);
}

static inline int match(const pcre2_code_8* re, char* subject, std::size_t subject_length, std::size_t offset, uint32_t flags, pcre2_match_data_8* md)
{
    return pcre2_match_8(
        re,
        reinterpret_cast<PCRE2_SPTR8>(subject),
        subject_length,
        offset,
        flags,
        md,
        nullptr
    );
}

static inline int match(const pcre2_code_16* re, char16_t* subject, std::size_t subject_length, std::size_t offset, uint32_t flags, pcre2_match_data_16* md)
{
    return pcre2_match_16(
        re,
        reinterpret_cast<PCRE2_SPTR16>(subject),
        subject_length,
        offset,
        flags,
        md,
        nullptr
    );
}

static inline int match(const pcre2_code_32* re, char32_t* subject, std::size_t subject_length, std::size_t offset, uint32_t flags, pcre2_match_data_32* md)
{
    return pcre2_match_32(
        re,
        reinterpret_cast<PCRE2_SPTR32>(subject),
        subject_length,
        offset,
        flags,
        md,
        nullptr
    );
}

static inline std::size_t* get_ovector_pointer(pcre2_match_data_8* md)
{
    return pcre2_get_ovector_pointer_8(md);
}

static inline std::size_t* get_ovector_pointer(pcre2_match_data_16* md)
{
    return pcre2_get_ovector_pointer_16(md);
}

static inline std::size_t* get_ovector_pointer(pcre2_match_data_32* md)
{
    return pcre2_get_ovector_pointer_32(md);
}


}
}

#endif // PCRE2XX_PRIVATE_PCRE2_H
