
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#include "ndef/common.h"
#include <malloc.h>
#include <string.h>

// Reserve capacity.
bool ndef_ostream_reserve(ndef_ostream_t* arr, size_t min_cap) {
    if (arr->len > min_cap) {
        min_cap = arr->len;
    }
    if (min_cap & (min_cap - 1)) {
        min_cap |= min_cap >> 1;
        min_cap |= min_cap >> 2;
        min_cap |= min_cap >> 4;
        min_cap |= min_cap >> 8;
        min_cap |= min_cap >> 16;
#if __SIZE_WIDTH__ == 64
        min_cap |= min_cap >> 32;
#endif
        min_cap++;
    }

    if (!arr->len) {
        free(arr->data);
        arr->data = NULL;
        arr->cap  = 0;
        return true;
    }

    if (arr->cap >= min_cap) {
        return true;
    }

    void* mem = realloc(arr->data, min_cap);
    if (mem) {
        arr->data = mem;
        arr->cap  = min_cap;
    }
    return mem != NULL;
}

// Append one byte.
bool ndef_ostream_push(ndef_ostream_t* arr, uint8_t value) {
    NDEF_RETURN_ON_FALSE(ndef_ostream_reserve(arr, arr->len + 1));
    arr->data[arr->len++] = value;
    return true;
}

// Append multiple bytes.
bool ndef_ostream_extend(ndef_ostream_t* arr, uint8_t* value, size_t value_len) {
    NDEF_RETURN_ON_FALSE(ndef_ostream_reserve(arr, arr->len + value_len));
    memcpy(arr->data + arr->len, value, value_len);
    arr->len += value_len;
    return true;
}

// Try to write a well-known NDEF record without its payload.
bool ndef_write_well_known(ndef_ostream_t* data_out, const char* type, ndef_pos_t pos, size_t payload_len) {
    bool    is_short = payload_len < 256;
    uint8_t flags    = pos + is_short * NDEF_FLAG_SHORT_RECORD;
    NDEF_RETURN_ON_FALSE(ndef_ostream_push(data_out, flags));
    size_t type_len = type ? strlen(type) : 0;
    NDEF_RETURN_ON_FALSE(ndef_ostream_push(data_out, type_len));
    if (is_short) {
        NDEF_RETURN_ON_FALSE(ndef_ostream_push(data_out, payload_len));
    } else {
        uint8_t len_bytes[4] = {
            (uint8_t)(payload_len >> 24),
            (uint8_t)(payload_len >> 16),
            (uint8_t)(payload_len >> 8),
            (uint8_t)(payload_len),
        };
        NDEF_RETURN_ON_FALSE(ndef_ostream_extend(data_out, len_bytes, sizeof(len_bytes)));
    }
    NDEF_RETURN_ON_FALSE(ndef_ostream_extend(data_out, (uint8_t*)type, type_len));
    return true;
}

// Try to read a well-known NDEF URI record.
bool ndef_read_well_known(ndef_istream_t* istream, ndef_well_known_t* well_known_out) {
    NDEF_RETURN_ON_FALSE(ndef_istream_available(istream) >= 3);
    uint8_t flags            = istream->data[istream->index++];
    well_known_out->pos      = flags & (NDEF_FLAG_MESSAGE_BEGIN | NDEF_FLAG_MESSAGE_END);
    well_known_out->type_len = istream->data[istream->index++];
    bool is_short            = flags & NDEF_FLAG_SHORT_RECORD;
    if (is_short) {
        NDEF_RETURN_ON_FALSE(ndef_istream_available(istream) >= 1);
        well_known_out->payload_len = istream->data[istream->index++];
    } else {
        NDEF_RETURN_ON_FALSE(ndef_istream_available(istream) >= 4);
        uint32_t tmp  = 0;
        tmp          |= (uint32_t)istream->data[istream->index++] << 24;
        tmp          |= (uint32_t)istream->data[istream->index++] << 16;
        tmp          |= (uint32_t)istream->data[istream->index++] << 8;
        tmp          |= (uint32_t)istream->data[istream->index++];

        well_known_out->payload_len = tmp;
    }
    NDEF_RETURN_ON_FALSE(ndef_istream_available(istream) >= well_known_out->type_len + well_known_out->payload_len);
    well_known_out->type     = (const char*)(istream->data + istream->index);
    istream->index          += well_known_out->type_len;
    well_known_out->payload  = istream->data + istream->index;
    return true;
}
