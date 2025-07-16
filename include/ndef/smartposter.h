
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#pragma once

#include <string.h>
#include "common.h"
#include "ndef/text.h"
#include "ndef/uri.h"

// Data for an NDEF Smart Poster record.
typedef struct {
    // Whether this is beginning, end, both or middle of a message.
    // Set by `ndef_smartposter_read`, ignored by `ndef_smartposter_write`.
    ndef_pos_t        pos;
    // The URI prefix to use.
    ndef_uri_prefix_t prefix;
    // URI data.
    const char*       uri;
    // URI length.
    size_t            uri_len;
    // The title of the smart poster.
    const char*       title;
    // Title length.
    size_t            title_len;
} ndef_smartposter_t;

// Read an NDEF Smart Poster record.
// The strings within are a reference to the blob passed.
// Returns how long the record was read, or 0 on error.
size_t ndef_smartposter_read(ndef_istream_t* istream, ndef_smartposter_t* poster_out);

// Write an NDEF Smart Poster record.
// Returns how long the record was written, or 0 on error.
size_t ndef_smartposter_write(ndef_ostream_t* ostream, const ndef_smartposter_t poster, ndef_pos_t pos);
