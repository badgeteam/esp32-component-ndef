
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#include "ndef/smartposter.h"

// Read an NDEF Smart Poster record.
// The strings within are a reference to the blob passed.
// Returns how long the record was read, or 0 on error.
size_t ndef_smartposter_read(ndef_istream_t* istream, ndef_smartposter_t* poster_out);

// Write an NDEF Smart Poster record.
// Returns how long the record was written, or 0 on error.
size_t ndef_smartposter_write(ndef_ostream_t* ostream, const ndef_smartposter_t poster, ndef_pos_t pos) {
    NDEF_RETURN_ON_FALSE(poster.uri_len);

    // Create inner records.
    ndef_ostream_t inner_ostream = NDEF_OSTREAM_NEW();

    ndef_uri_t uri = {
        .prefix  = poster.prefix,
        .uri     = poster.uri,
        .uri_len = poster.uri_len,
    };
    NDEF_RETURN_ON_FALSE(ndef_uri_write(&inner_ostream, uri, poster.title_len ? NDEF_POS_START : NDEF_POS_START_END),
                         free(inner_ostream.data););

    if (poster.title_len) {
        ndef_text_t title = {
            .lang     = NULL,
            .lang_len = 0,
            .text     = poster.title,
            .text_len = poster.title_len,
        };
        NDEF_RETURN_ON_FALSE(ndef_text_write(&inner_ostream, title, NDEF_POS_END), free(inner_ostream.data););
    }

    // Write the outer record.
    size_t header_size = NDEF_RETURN_ON_FALSE(
        ndef_write_record(ostream, NDEF_TNF_WELL_KNOWN, "Sp", pos, inner_ostream.len), free(inner_ostream.data););
    NDEF_RETURN_ON_FALSE(ndef_ostream_extend(ostream, inner_ostream.data, inner_ostream.len),
                         free(inner_ostream.data););

    free(inner_ostream.data);
    return inner_ostream.len + header_size;
}
