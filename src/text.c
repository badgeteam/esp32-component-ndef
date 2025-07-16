
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#include "ndef/text.h"

// Read an NDEF text record.
// The strings within are a reference to the blob passed.
// Returns how long the record was read, or 0 on error.
size_t ndef_text_read(ndef_istream_t* istream, ndef_text_t* text_out) {
    ndef_record_t well_known;
    size_t        record_len = NDEF_RETURN_ON_FALSE(ndef_read_record(istream, &well_known));
    NDEF_RETURN_ON_FALSE(well_known.type_len == 1 && well_known.type[0] == 'T');
    NDEF_RETURN_ON_FALSE(well_known.payload_len >= 1);
    text_out->pos      = well_known.pos;
    text_out->lang_len = well_known.payload[0] & 0x3f;
    text_out->text_len = well_known.payload_len - text_out->lang_len - 1;
    text_out->lang     = (const char*)well_known.payload + 1;
    text_out->text     = text_out->lang + text_out->lang_len;
    return record_len;
}

// Write an NDEF text record.
bool ndef_text_write(ndef_ostream_t* data_out, ndef_text_t text, ndef_pos_t pos) {
    NDEF_RETURN_ON_FALSE(ndef_write_record(data_out, NDEF_TNF_WELL_KNOWN, "T", pos, text.lang_len + text.text_len + 1));
    NDEF_RETURN_ON_FALSE(text.lang_len <= 0x3f);
    NDEF_RETURN_ON_FALSE(ndef_ostream_push(data_out, text.lang_len));
    NDEF_RETURN_ON_FALSE(ndef_ostream_extend(data_out, (uint8_t*)text.lang, text.lang_len));
    NDEF_RETURN_ON_FALSE(ndef_ostream_extend(data_out, (uint8_t*)text.text, text.text_len));
    return true;
}
