
// SPDX-CopyRightText: 2025 Julian Scheffers
// SPDX-License-Identifier: MIT

#pragma once

#include <string.h>
#include "common.h"

// A prefix that can be added before an NDEF URI record.
typedef enum {
    // Not prefixed
    NDEF_URI_NO_PREFIX,
    // Prefixed with `http://www.`
    NDEF_URI_PREFIX_HTTP_WWW,
    // Prefixed with `https://www.`
    NDEF_URI_PREFIX_HTTPS_WWW,
    // Prefixed with `http://`
    NDEF_URI_PREFIX_HTTP,
    // Prefixed with `https://`
    NDEF_URI_PREFIX_HTTPS,
    // Prefixed with `tel:`
    NDEF_URI_PREFIX_TEL,
    // Prefixed with `mailto:`
    NDEF_URI_PREFIX_MAILTO,
    // Prefixed with `ftp://anonymous:anonymous@`
    NDEF_URI_PREFIX_FTP_ANONYMOUS_ANONYMOUS,
    // Prefixed with `ftp://ftp.`
    NDEF_URI_PREFIX_FTP_FTP,
    // Prefixed with `ftps://`
    NDEF_URI_PREFIX_FTPS,
    // Prefixed with `sftp://`
    NDEF_URI_PREFIX_SFTP,
    // Prefixed with `smb://`
    NDEF_URI_PREFIX_SMB,
    // Prefixed with `nfs://`
    NDEF_URI_PREFIX_NFS,
    // Prefixed with `ftp://`
    NDEF_URI_PREFIX_FTP,
    // Prefixed with `dav://`
    NDEF_URI_PREFIX_DAV,
    // Prefixed with `news:`
    NDEF_URI_PREFIX_NEWS,
    // Prefixed with `telnet://`
    NDEF_URI_PREFIX_TELNET,
    // Prefixed with `imap:`
    NDEF_URI_PREFIX_IMAP,
    // Prefixed with `rtsp://`
    NDEF_URI_PREFIX_RTSP,
    // Prefixed with `urn:`
    NDEF_URI_PREFIX_URN,
    // Prefixed with `pop:`
    NDEF_URI_PREFIX_POP,
    // Prefixed with `sip:`
    NDEF_URI_PREFIX_SIP,
    // Prefixed with `sips:`
    NDEF_URI_PREFIX_SIPS,
    // Prefixed with `tftp:`
    NDEF_URI_PREFIX_TFTP,
    // Prefixed with `btspp://`
    NDEF_URI_PREFIX_BTSPP,
    // Prefixed with `btl2cap://`
    NDEF_URI_PREFIX_BTL2CAP,
    // Prefixed with `btgoep://`
    NDEF_URI_PREFIX_BTGOEP,
    // Prefixed with `tcpobex://`
    NDEF_URI_PREFIX_TCPOBEX,
    // Prefixed with `irdaobex://`
    NDEF_URI_PREFIX_IRDAOBEX,
    // Prefixed with `file://`
    NDEF_URI_PREFIX_FILE,
    // Prefixed with `urn:epc:id:`
    NDEF_URI_PREFIX_URN_EPC_ID,
    // Prefixed with `urn:epc:tag:`
    NDEF_URI_PREFIX_URN_EPC_TAG,
    // Prefixed with `urn:epc:pat:`
    NDEF_URI_PREFIX_URN_EPC_PAT,
    // Prefixed with `urn:epc:raw:`
    NDEF_URI_PREFIX_URN_EPC_RAW,
    // Prefixed with `urn:epc:`
    NDEF_URI_PREFIX_URN_EPC,
    // Prefixed with `urn:nfc:`
    NDEF_URI_PREFIX_URN_NFC,
    // Number of NDEF URI prefixes.
    NDEF_URI_NUM_PREFIX,
} ndef_uri_prefix_t;

// Table of string representations of NDEF URI prefixes.
extern const char* const ndef_uri_prefixes[];

// Data for an NDEF URI record.
typedef struct {
    // Whether this is beginning, end, both or middle of a message.
    // Set by `ndef_uri_read`, ignored by `ndef_uri_write`.
    ndef_pos_t        pos;
    // The URI prefix to use.
    ndef_uri_prefix_t prefix;
    // URI data.
    const char*       uri;
    // URI byte length.
    size_t            uri_len;
} ndef_uri_t;

// Read an NDEF URI record.
// The strings within are a reference to the blob passed.
bool       ndef_uri_read(ndef_istream_t* istream, ndef_uri_t* uri_out);
// Make an NDEF URI from a string.
ndef_uri_t ndef_uri_format(const char* uri, size_t uri_len);
// Write an NDEF URI record.
bool       ndef_uri_write(ndef_ostream_t* data_out, ndef_uri_t uri, ndef_pos_t pos);

// Make and wirite an NDEF URI from a string.
static inline bool ndef_uri_write_str(ndef_ostream_t* data_out, const char* uri, size_t uri_len, ndef_pos_t pos) {
    return ndef_uri_write(data_out, ndef_uri_format(uri, uri_len), pos);
}

// Make an NDEF URI from a C-string.
static inline ndef_uri_t ndef_uri_format_cstr(const char* uri) {
    return ndef_uri_format(uri, strlen(uri));
}

// Make and write an NDEF URI from a C-string.
static inline bool ndef_uri_write_cstr(ndef_ostream_t* data_out, const char* uri, ndef_pos_t pos) {
    return ndef_uri_write(data_out, ndef_uri_format_cstr(uri), pos);
}
