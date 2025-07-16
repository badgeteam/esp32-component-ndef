
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Return false if the expression is false.
#define NDEF_RETURN_ON_FALSE(expr)   \
    ({                               \
        typeof(expr) expr_ = (expr); \
        if (!(expr_)) return false;  \
        expr_;                       \
    })

// A stream of output bytes with a length and capacity.
typedef struct {
    uint8_t* data;
    size_t   len;
    size_t   cap;
} ndef_ostream_t;

// A stream of input bytes with an index.
typedef struct {
    uint8_t const* data;
    size_t         len;
    size_t         index;
} ndef_istream_t;

// NDEF type name format.
typedef enum {
    NDEF_TNF_EMPTY          = 0x00,  // Empty record
    NDEF_TNF_NFC_WELL_KNOWN = 0x01,  // NFC well-known type
    NDEF_TNF_MIME_MEDIA     = 0x02,  // MIME media type
    NDEF_TNF_ABSOLUTE_URI   = 0x03,  // Absolute URI
    NDEF_TNF_EXTERNAL_TYPE  = 0x04,  // External type
    NDEF_TNF_UNKNOWN        = 0x05,  // Unknown type
    NDEF_TNF_UNCHANGED      = 0x06,  // Unchanged type
    NDEF_TNF_RESERVED       = 0x07,  // Reserved type
} ndef_tnf_t;

// Create an input stream from some bytes.
#define NDEF_ISTREAM_NEW(data_, len_) ((ndef_istream_t){data_, len_, 0})

// Create an empty output stream.
#define NDEF_OSTREAM_NEW() ((ndef_ostream_t){NULL, 0, 0})

// NDEF flag: Message begin; must be set for first entry in a message.
#define NDEF_FLAG_MESSAGE_BEGIN    0x80
// NDEF flag: Message end; must be set for lest entry in a message.
#define NDEF_FLAG_MESSAGE_END      0x40
// NDEF flag: Short record; indicates that the payload length is 0-255 bytes.
#define NDEF_FLAG_SHORT_RECORD     0x20
// NDEF flag: Chunked record; indicates that the payload is split into multiple chunks.
#define NDEF_FLAG_CHUNKED_RECORD   0x10
// NDEF flag: ID length; indicates that the ID field is present.
#define NDEF_FLAG_ID_LENGTH        0x08
// NDEF flag: Type name format; indicates how the type is encoded.
#define NDEF_FLAG_TYPE_NAME_FORMAT 0x07

// Enum that specifies whether a record is start, end, both or none.
typedef enum {
    NDEF_POS_START     = NDEF_FLAG_MESSAGE_BEGIN,
    NDEF_POS_END       = NDEF_FLAG_MESSAGE_END,
    NDEF_POS_START_END = NDEF_FLAG_MESSAGE_BEGIN | NDEF_FLAG_MESSAGE_END,
    NDEF_POS_MIDDLE    = 0x00,
} ndef_pos_t;

// A decoded NDEF well-known message.
typedef struct {
    // Whether this is beginning, end, both or middle of a message.
    ndef_pos_t     pos;
    // Length of type.
    size_t         type_len;
    // Type string.
    const char*    type;
    // Length of payload.
    size_t         payload_len;
    // Payload data.
    const uint8_t* payload;
} ndef_well_known_t;

// Reserve additional capacity.
bool ndef_ostream_reserve(ndef_ostream_t* arr, size_t min_cap);
// Append one byte.
bool ndef_ostream_push(ndef_ostream_t* arr, uint8_t value);
// Append multiple bytes.
bool ndef_ostream_extend(ndef_ostream_t* arr, uint8_t* value, size_t value_len);

// Get the number of available bytes in `stream`.
static inline size_t ndef_istream_available(const ndef_istream_t* stream) {
    return stream->len - stream->index;
}

// Try to write a well-known NDEF record without its payload.
bool ndef_write_well_known(ndef_ostream_t* data_out, const char* type, ndef_pos_t pos, size_t payload_len);
// Try to read a well-known NDEF URI record.
bool ndef_read_well_known(ndef_istream_t* istream, ndef_well_known_t* well_known_out);
