
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#include "ndef/uri.h"

// Table of string representations of NDEF URI prefixes.
const char* const ndef_uri_prefixes[] = {
    [NDEF_URI_PREFIX_HTTP_WWW]                = "http://www.",
    [NDEF_URI_PREFIX_HTTPS_WWW]               = "https://www.",
    [NDEF_URI_PREFIX_HTTP]                    = "http://",
    [NDEF_URI_PREFIX_HTTPS]                   = "https://",
    [NDEF_URI_PREFIX_TEL]                     = "tel:",
    [NDEF_URI_PREFIX_MAILTO]                  = "mailto:",
    [NDEF_URI_PREFIX_FTP_ANONYMOUS_ANONYMOUS] = "ftp://anonymous:anonymous@",
    [NDEF_URI_PREFIX_FTP_FTP]                 = "ftp://ftp.",
    [NDEF_URI_PREFIX_FTPS]                    = "ftps://",
    [NDEF_URI_PREFIX_SFTP]                    = "sftp://",
    [NDEF_URI_PREFIX_SMB]                     = "smb://",
    [NDEF_URI_PREFIX_NFS]                     = "nfs://",
    [NDEF_URI_PREFIX_FTP]                     = "ftp://",
    [NDEF_URI_PREFIX_DAV]                     = "dav://",
    [NDEF_URI_PREFIX_NEWS]                    = "news:",
    [NDEF_URI_PREFIX_TELNET]                  = "telnet://",
    [NDEF_URI_PREFIX_IMAP]                    = "imap:",
    [NDEF_URI_PREFIX_RTSP]                    = "rtsp://",
    [NDEF_URI_PREFIX_URN]                     = "urn:",
    [NDEF_URI_PREFIX_POP]                     = "pop:",
    [NDEF_URI_PREFIX_SIP]                     = "sip:",
    [NDEF_URI_PREFIX_SIPS]                    = "sips:",
    [NDEF_URI_PREFIX_TFTP]                    = "tftp:",
    [NDEF_URI_PREFIX_BTSPP]                   = "btspp://",
    [NDEF_URI_PREFIX_BTL2CAP]                 = "btl2cap://",
    [NDEF_URI_PREFIX_BTGOEP]                  = "btgoep://",
    [NDEF_URI_PREFIX_TCPOBEX]                 = "tcpobex://",
    [NDEF_URI_PREFIX_IRDAOBEX]                = "irdaobex://",
    [NDEF_URI_PREFIX_FILE]                    = "file://",
    [NDEF_URI_PREFIX_URN_EPC_ID]              = "urn:epc:id:",
    [NDEF_URI_PREFIX_URN_EPC_TAG]             = "urn:epc:tag:",
    [NDEF_URI_PREFIX_URN_EPC_PAT]             = "urn:epc:pat:",
    [NDEF_URI_PREFIX_URN_EPC_RAW]             = "urn:epc:raw:",
    [NDEF_URI_PREFIX_URN_EPC]                 = "urn:epc:",
    [NDEF_URI_PREFIX_URN_NFC]                 = "urn:nfc:",
};

// Read an NDEF URI record.
// The strings within are a reference to the blob passed.
bool ndef_uri_read(ndef_istream_t* istream, ndef_uri_t* uri_out) {
    ndef_well_known_t well_known;
    NDEF_RETURN_ON_FALSE(ndef_read_well_known(istream, &well_known));
    NDEF_RETURN_ON_FALSE(well_known.type_len == 1 && well_known.type[0] == 'U');
    uri_out->prefix = istream->data[istream->index++];
    NDEF_RETURN_ON_FALSE(uri_out->prefix < NDEF_URI_NUM_PREFIX);
    uri_out->uri     = (const char*)(well_known.payload + 1);
    uri_out->uri_len = well_known.payload_len - 1;
    uri_out->pos     = well_known.pos;
    return true;
}

// Make an NDEF URI from a string.
ndef_uri_t ndef_uri_format(const char* uri, size_t uri_len) {
    // This has a pos field, but it is ignored by `ndef_uri_write`.
    ndef_uri_t cur        = {NDEF_POS_MIDDLE, NDEF_URI_NO_PREFIX, uri, uri_len};
    size_t     prefix_len = 0;
    for (ndef_uri_prefix_t prefix = 0; prefix < NDEF_URI_NUM_PREFIX; prefix++) {
        const char* prefix_str = ndef_uri_prefixes[prefix];
        if (!prefix_str) {
            continue;
        }
        size_t len = strlen(prefix_str);
        if (len > prefix_len && uri_len >= len && strncmp(uri, prefix_str, len) == 0) {
            cur.prefix  = prefix;
            cur.uri     = uri + len;
            cur.uri_len = uri_len - len;
            prefix_len  = len;
        }
    }
    return cur;
}

// Write an NDEF URI record.
bool ndef_uri_write(ndef_ostream_t* data_out, ndef_uri_t uri, ndef_pos_t pos) {
    NDEF_RETURN_ON_FALSE(ndef_write_well_known(data_out, "U", pos, uri.uri_len + 1));
    NDEF_RETURN_ON_FALSE(ndef_ostream_push(data_out, uri.prefix));
    NDEF_RETURN_ON_FALSE(ndef_ostream_extend(data_out, (uint8_t*)uri.uri, uri.uri_len));
    return true;
}
