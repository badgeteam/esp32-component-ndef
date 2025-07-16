
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#pragma once

#include "ndef/smartposter.h"
#include "ndef/text.h"
#include "ndef/uri.h"

// Types for a decoded NDEF record.
typedef enum {
    NDEF_DECD_TYPE_UNKNOWN,
    NDEF_DECD_TYPE_URI,
    NDEF_DECD_TYPE_TEXT,
    NDEF_DECD_TYPE_SMART_POSTER,
    NDEF_DECD_TYPE_WIFI,
} ndef_decd_type_t;

// A decoded NDEF record.
// Decodes known MIME or well-known types.
typedef struct {
    // The position of this record in the message.
    ndef_pos_t       pos;
    // The type of this record.
    ndef_decd_type_t type;
    union {
        // The decoded URI record.
        ndef_uri_t         uri;
        // The decoded text record.
        ndef_text_t        text;
        // The decoded smart poster record.
        ndef_smartposter_t smartposter;
    } data;
} ndef_decd_record_t;
