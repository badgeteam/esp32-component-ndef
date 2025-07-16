
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#pragma once

#include <string.h>
#include "common.h"

// Data for an NDEF text record.
typedef struct {
    // Whether this is beginning, end, both or middle of a message.
    // Set by `ndef_text_read`, ignored by `ndef_text_write`.
    ndef_pos_t  pos;
    // The language code, e.g. "en" for English.
    const char* lang;
    // Language code byte length.
    size_t      lang_len;
    // Text data.
    const char* text;
    // Text byte length.
    size_t      text_len;
} ndef_text_t;

// Read an NDEF text record.
// The strings within are a reference to the blob passed.
// Returns how long the record was read, or 0 on error.
size_t ndef_text_read(ndef_istream_t* istream, ndef_text_t* text_out);
// Write an NDEF text record.
bool   ndef_text_write(ndef_ostream_t* data_out, ndef_text_t text, ndef_pos_t pos);

// Make and write an NDEF text from a string.
static inline bool ndef_text_write_str(ndef_ostream_t* data_out, const char* lang, size_t lang_len, const char* text,
                                       size_t text_len, ndef_pos_t pos) {
    ndef_text_t data = {
        .lang     = lang,
        .lang_len = lang_len,
        .text     = text,
        .text_len = text_len,
    };
    return ndef_text_write(data_out, data, pos);
}

// Make and write an NDEF text from a C-string.
static inline bool ndef_text_write_cstr(ndef_ostream_t* data_out, const char* lang, const char* text, ndef_pos_t pos) {
    ndef_text_t data = {
        .lang     = lang,
        .lang_len = lang ? strlen(lang) : 0,
        .text     = text,
        .text_len = strlen(text),
    };
    return ndef_text_write(data_out, data, pos);
}
