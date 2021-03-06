/* charsets.c
 * Routines for handling character sets
 *
 * $Id$
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"

#include <glib.h>

#include "charsets.h"

/*
 * Wikipedia's "Character encoding" template, giving a pile of character encodings and
 * Wikipedia pages for them:
 *
 *    http://en.wikipedia.org/wiki/Template:Character_encoding
 *
 * Unicode character encoding model:
 *
 *    http://www.unicode.org/reports/tr17/
 *
 * International Components for Unicode character set mapping tables:
 *
 *    http://site.icu-project.org/charts/charset
 *
 * MSDN information on code pages:
 *
 *    http://msdn.microsoft.com/en-us/library/dd317752(v=VS.85).aspx
 *
 * ASCII-based code pages, from IBM:
 *
 *    http://www-01.ibm.com/software/globalization/cp/cp_cpgid.html
 *
 * EBCDIC code pages, from IBM:
 *
 *    http://www-03.ibm.com/systems/i/software/globalization/codepages.html
 */

/* ASCII/EBCDIC conversion tables from
 * http://www.room42.com/store/computer_center/code_tables.shtml
 */
#if 0
static guint8 ASCII_translate_EBCDIC [ 256 ] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D, 0x4D,
    0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
    0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8,
    0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
    0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xAD, 0xE0, 0xBD, 0x5F, 0x6D,
    0x7D, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
    0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xC0, 0x6A, 0xD0, 0xA1, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B
};

void
ASCII_to_EBCDIC(guint8 *buf, guint bytes)
{
    guint    i;
    guint8    *bufptr;

    bufptr = buf;

    for (i = 0; i < bytes; i++, bufptr++) {
        *bufptr = ASCII_translate_EBCDIC[*bufptr];
    }
}

guint8
ASCII_to_EBCDIC1(guint8 c)
{
    return ASCII_translate_EBCDIC[c];
}
#endif

static guint8 EBCDIC_translate_ASCII [ 256 ] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x2E, 0x2E, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x2E, 0x3F,
    0x20, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
    0x26, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
    0x2D, 0x2F, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x7C, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
    0x2E, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7A, 0x2E, 0x2E, 0x2E, 0x5B, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x2E, 0x5D, 0x2E, 0x2E,
    0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51,
    0x52, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x5C, 0x2E, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5A, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E
};

void
EBCDIC_to_ASCII(guint8 *buf, guint bytes)
{
    guint   i;
    guint8 *bufptr;

    bufptr = buf;

    for (i = 0; i < bytes; i++, bufptr++) {
        *bufptr = EBCDIC_translate_ASCII[*bufptr];
    }
}

guint8
EBCDIC_to_ASCII1(guint8 c)
{
    return EBCDIC_translate_ASCII[c];
}

/*
 * Translation tables that map the upper 128 code points in single-byte
 * "extended ASCII" character encodings to Unicode code points in the
 * Basic Multilingual Plane.
 */

/* REPLACEMENT CHARACTER */
#define UNREPL 0xFFFD

/* ISO-8859-2 (http://en.wikipedia.org/wiki/ISO/IEC_8859-2#Code_page_layout) */
const gunichar2 charset_table_iso_8859_2[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0104, 0x02d8, 0x0141, 0x00a4, 0x013d, 0x015a, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x0160, 0x015e, 0x0164, 0x0179, 0x00ad, 0x017d, 0x017b,        /*      - 0xAF */
    0x00b0, 0x0105, 0x02db, 0x0142, 0x00b4, 0x013e, 0x015b, 0x02c7,        /* 0xB0 -      */
    0x00b8, 0x0161, 0x015f, 0x0165, 0x017a, 0x02dd, 0x017e, 0x017c,        /*      - 0xBF */
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7,        /* 0xC0 -      */
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e,        /*      - 0xCF */
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df,        /*      - 0xDF */
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7,        /* 0xE0 -      */
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,        /*      - 0xEF */
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9         /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-3 */
const gunichar2 charset_table_iso_8859_3[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0126, 0x02d8, 0x00a3, 0x00a4, UNREPL, 0x0124, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x0130, 0x015e, 0x011e, 0x0134, 0x00ad, UNREPL, 0x017b,        /*      - 0xAF */
    0x00b0, 0x0127, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x0125, 0x00b7,        /* 0xB0 -      */
    0x00b8, 0x0131, 0x015f, 0x011f, 0x0135, 0x00bd, UNREPL, 0x017c,        /*      - 0xBF */
    0x00c0, 0x00c1, 0x00c2, UNREPL, 0x00c4, 0x010a, 0x0108, 0x00c7,        /* 0xC0 -      */
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    UNREPL, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x0120, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x011c, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x016c, 0x015c, 0x00df,        /*      - 0xDF */
    0x00e0, 0x00e1, 0x00e2, UNREPL, 0x00e4, 0x010b, 0x0109, 0x00e7,        /* 0xE0 -      */
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    UNREPL, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x0121, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x011d, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x016d, 0x015d, 0x02d9,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-4 */
const gunichar2 charset_table_iso_8859_4[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0104, 0x0138, 0x0156, 0x00a4, 0x0128, 0x013b, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x0160, 0x0112, 0x0122, 0x0166, 0x00ad, 0x017d, 0x00af,        /*      - 0xAF */
    0x00b0, 0x0105, 0x02db, 0x0157, 0x00b4, 0x0129, 0x013c, 0x02c7,        /* 0xB0 -      */
    0x00b8, 0x0161, 0x0113, 0x0123, 0x0167, 0x014a, 0x017e, 0x014b,        /*      - 0xBF */
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e,        /* 0xC0 -      */
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x012a,        /*      - 0xCF */
    0x0110, 0x0145, 0x014c, 0x0136, 0x00d4, 0x00d5, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x0168, 0x016a, 0x00df,        /*      - 0xDF */
    0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f,        /* 0xE0 -      */
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x012b,        /*      - 0xEF */
    0x0111, 0x0146, 0x014d, 0x0137, 0x00f4, 0x00f5, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x0169, 0x016b, 0x02d9,        /*      - 0xFF */
};

/* ISO-8859-5 (http://en.wikipedia.org/wiki/ISO/IEC_8859-5#Code_page_layout) */
const gunichar2 charset_table_iso_8859_5[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407,        /* 0xA0 -      */
    0x0408, 0x0409, 0x040a, 0x040b, 0x040c, 0x040d, 0x040e, 0x040f,        /*      - 0xAF */
    0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417,        /* 0xB0 -      */
    0x0418, 0x0419, 0x041a, 0x041b, 0x041c, 0x041d, 0x041e, 0x041f,        /*      - 0xBF */
    0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427,        /* 0xC0 -      */
    0x0428, 0x0429, 0x042a, 0x042b, 0x042c, 0x042d, 0x042e, 0x042f,        /*      - 0xCF */
    0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437,        /* 0xD0 -      */
    0x0438, 0x0439, 0x043a, 0x043b, 0x043c, 0x043d, 0x043e, 0x043f,        /*      - 0xDF */
    0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447,        /* 0xE0 -      */
    0x0448, 0x0449, 0x044a, 0x044b, 0x044c, 0x044d, 0x044e, 0x044f,        /*      - 0xEF */
    0x2116, 0x0451, 0x0452, 0x0453, 0x0454, 0x0455, 0x0456, 0x0457,        /* 0xF0 -      */
    0x0458, 0x0459, 0x045a, 0x045b, 0x045c, 0x00a7, 0x045e, 0x045f         /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-6 */
const gunichar2 charset_table_iso_8859_6[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, UNREPL, UNREPL, UNREPL, 0x00a4, UNREPL, UNREPL, UNREPL,        /* 0xA0 -      */
    UNREPL, UNREPL, UNREPL, UNREPL, 0x060c, 0x00ad, UNREPL, UNREPL,        /*      - 0xAF */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /* 0xB0 -      */
    UNREPL, UNREPL, UNREPL, 0x061b, UNREPL, UNREPL, UNREPL, 0x061f,        /*      - 0xBF */
    UNREPL, 0x0621, 0x0622, 0x0623, 0x0624, 0x0625, 0x0626, 0x0627,        /* 0xC0 -      */
    0x0628, 0x0629, 0x062a, 0x062b, 0x062c, 0x062d, 0x062e, 0x062f,        /*      - 0xCF */
    0x0630, 0x0631, 0x0632, 0x0633, 0x0634, 0x0635, 0x0636, 0x0637,        /* 0xD0 -      */
    0x0638, 0x0639, 0x063a, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /*      - 0xDF */
    0x0640, 0x0641, 0x0642, 0x0643, 0x0644, 0x0645, 0x0646, 0x0647,        /* 0xE0 -      */
    0x0648, 0x0649, 0x064a, 0x064b, 0x064c, 0x064d, 0x064e, 0x064f,        /*      - 0xEF */
    0x0650, 0x0651, 0x0652, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /* 0xF0 -      */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-7 */
const gunichar2 charset_table_iso_8859_7[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x2018, 0x2019, 0x00a3, 0x20ac, 0x20af, 0x00a6, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x00a9, 0x037a, 0x00ab, 0x00ac, 0x00ad, UNREPL, 0x2015,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x0384, 0x0385, 0x0386, 0x00b7,        /* 0xB0 -      */
    0x0388, 0x0389, 0x038a, 0x00bb, 0x038c, 0x00bd, 0x038e, 0x038f,        /*      - 0xBF */
    0x0390, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397,        /* 0xC0 -      */
    0x0398, 0x0399, 0x039a, 0x039b, 0x039c, 0x039d, 0x039e, 0x039f,        /*      - 0xCF */
    0x03a0, 0x03a1, UNREPL, 0x03a3, 0x03a4, 0x03a5, 0x03a6, 0x03a7,        /* 0xD0 -      */
    0x03a8, 0x03a9, 0x03aa, 0x03ab, 0x03ac, 0x03ad, 0x03ae, 0x03af,        /*      - 0xDF */
    0x03b0, 0x03b1, 0x03b2, 0x03b3, 0x03b4, 0x03b5, 0x03b6, 0x03b7,        /* 0xE0 -      */
    0x03b8, 0x03b9, 0x03ba, 0x03bb, 0x03bc, 0x03bd, 0x03be, 0x03bf,        /*      - 0xEF */
    0x03c0, 0x03c1, 0x03c2, 0x03c3, 0x03c4, 0x03c5, 0x03c6, 0x03c7,        /* 0xF0 -      */
    0x03c8, 0x03c9, 0x03ca, 0x03cb, 0x03cc, 0x03cd, 0x03ce, UNREPL,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-8 */
const gunichar2 charset_table_iso_8859_8[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, UNREPL, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x00a9, 0x00d7, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x00b8, 0x00b9, 0x00f7, 0x00bb, 0x00bc, 0x00bd, 0x00be, UNREPL,        /*      - 0xBF */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /* 0xC0 -      */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /*      - 0xCF */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL,        /* 0xD0 -      */
    UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, UNREPL, 0x2017,        /*      - 0xDF */
    0x05d0, 0x05d1, 0x05d2, 0x05d3, 0x05d4, 0x05d5, 0x05d6, 0x05d7,        /* 0xE0 -      */
    0x05d8, 0x05d9, 0x05da, 0x05db, 0x05dc, 0x05dd, 0x05de, 0x05df,        /*      - 0xEF */
    0x05e0, 0x05e1, 0x05e2, 0x05e3, 0x05e4, 0x05e5, 0x05e6, 0x05e7,        /* 0xF0 -      */
    0x05e8, 0x05e9, 0x05ea, UNREPL, UNREPL, 0x200e, 0x200f, UNREPL,        /*      - 0xFF */
};

/* ISO-8859-9 (http://en.wikipedia.org/wiki/ISO/IEC_8859-9#Code_page_layout) */
const gunichar2 charset_table_iso_8859_9[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x00a4, 0x00a5, 0x00a6, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x00b4, 0x00b5, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x00b8, 0x00b9, 0x00ba, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00bf,        /*      - 0xBF */
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,        /* 0xC0 -      */
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    0x011e, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0130, 0x015e, 0x00df,        /*      - 0xDF */
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,        /* 0xE0 -      */
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    0x011f, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0131, 0x015f, 0x00ff         /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-10 */
const gunichar2 charset_table_iso_8859_10[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0104, 0x0112, 0x0122, 0x012a, 0x0128, 0x0136, 0x00a7,        /* 0xA0 -      */
    0x013b, 0x0110, 0x0160, 0x0166, 0x017d, 0x00ad, 0x016a, 0x014a,        /*      - 0xAF */
    0x00b0, 0x0105, 0x0113, 0x0123, 0x012b, 0x0129, 0x0137, 0x00b7,        /* 0xB0 -      */
    0x013c, 0x0111, 0x0161, 0x0167, 0x017e, 0x2015, 0x016b, 0x014b,        /*      - 0xBF */
    0x0100, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x012e,        /* 0xC0 -      */
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x0116, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    0x00d0, 0x0145, 0x014c, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x0168,        /* 0xD0 -      */
    0x00d8, 0x0172, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,        /*      - 0xDF */
    0x0101, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x012f,        /* 0xE0 -      */
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x0117, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    0x00f0, 0x0146, 0x014d, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x0169,        /* 0xF0 -      */
    0x00f8, 0x0173, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x0138,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-11 */
const gunichar2 charset_table_iso_8859_11[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0e01, 0x0e02, 0x0e03, 0x0e04, 0x0e05, 0x0e06, 0x0e07,        /* 0xA0 -      */
    0x0e08, 0x0e09, 0x0e0a, 0x0e0b, 0x0e0c, 0x0e0d, 0x0e0e, 0x0e0f,        /*      - 0xAF */
    0x0e10, 0x0e11, 0x0e12, 0x0e13, 0x0e14, 0x0e15, 0x0e16, 0x0e17,        /* 0xB0 -      */
    0x0e18, 0x0e19, 0x0e1a, 0x0e1b, 0x0e1c, 0x0e1d, 0x0e1e, 0x0e1f,        /*      - 0xBF */
    0x0e20, 0x0e21, 0x0e22, 0x0e23, 0x0e24, 0x0e25, 0x0e26, 0x0e27,        /* 0xC0 -      */
    0x0e28, 0x0e29, 0x0e2a, 0x0e2b, 0x0e2c, 0x0e2d, 0x0e2e, 0x0e2f,        /*      - 0xCF */
    0x0e30, 0x0e31, 0x0e32, 0x0e33, 0x0e34, 0x0e35, 0x0e36, 0x0e37,        /* 0xD0 -      */
    0x0e38, 0x0e39, 0x0e3a, UNREPL, UNREPL, UNREPL, UNREPL, 0x0e3f,        /*      - 0xDF */
    0x0e40, 0x0e41, 0x0e42, 0x0e43, 0x0e44, 0x0e45, 0x0e46, 0x0e47,        /* 0xE0 -      */
    0x0e48, 0x0e49, 0x0e4a, 0x0e4b, 0x0e4c, 0x0e4d, 0x0e4e, 0x0e4f,        /*      - 0xEF */
    0x0e50, 0x0e51, 0x0e52, 0x0e53, 0x0e54, 0x0e55, 0x0e56, 0x0e57,        /* 0xF0 -      */
    0x0e58, 0x0e59, 0x0e5a, 0x0e5b, UNREPL, UNREPL, UNREPL, UNREPL,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-13 */
const gunichar2 charset_table_iso_8859_13[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x201d, 0x00a2, 0x00a3, 0x00a4, 0x201e, 0x00a6, 0x00a7,        /* 0xA0 -      */
    0x00d8, 0x00a9, 0x0156, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00c6,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x201c, 0x00b5, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x00f8, 0x00b9, 0x0157, 0x00bb, 0x00bc, 0x00bd, 0x00be, 0x00e6,        /*      - 0xBF */
    0x0104, 0x012e, 0x0100, 0x0106, 0x00c4, 0x00c5, 0x0118, 0x0112,        /* 0xC0 -      */
    0x010c, 0x00c9, 0x0179, 0x0116, 0x0122, 0x0136, 0x012a, 0x013b,        /*      - 0xCF */
    0x0160, 0x0143, 0x0145, 0x00d3, 0x014c, 0x00d5, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x0172, 0x0141, 0x015a, 0x016a, 0x00dc, 0x017b, 0x017d, 0x00df,        /*      - 0xDF */
    0x0105, 0x012f, 0x0101, 0x0107, 0x00e4, 0x00e5, 0x0119, 0x0113,        /* 0xE0 -      */
    0x010d, 0x00e9, 0x017a, 0x0117, 0x0123, 0x0137, 0x012b, 0x013c,        /*      - 0xEF */
    0x0161, 0x0144, 0x0146, 0x00f3, 0x014d, 0x00f5, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x0173, 0x0142, 0x015b, 0x016b, 0x00fc, 0x017c, 0x017e, 0x2019,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-14 */
const gunichar2 charset_table_iso_8859_14[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x1e02, 0x1e03, 0x00a3, 0x010a, 0x010b, 0x1e0a, 0x00a7,        /* 0xA0 -      */
    0x1e80, 0x00a9, 0x1e82, 0x1e0b, 0x1ef2, 0x00ad, 0x00ae, 0x0178,        /*      - 0xAF */
    0x1e1e, 0x1e1f, 0x0120, 0x0121, 0x1e40, 0x1e41, 0x00b6, 0x1e56,        /* 0xB0 -      */
    0x1e81, 0x1e57, 0x1e83, 0x1e60, 0x1ef3, 0x1e84, 0x1e85, 0x1e61,        /*      - 0xBF */
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,        /* 0xC0 -      */
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    0x0174, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x1e6a,        /* 0xD0 -      */
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x0176, 0x00df,        /*      - 0xDF */
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,        /* 0xE0 -      */
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    0x0175, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x1e6b,        /* 0xF0 -      */
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x0177, 0x00ff,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-15 */
const gunichar2 charset_table_iso_8859_15[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x00a1, 0x00a2, 0x00a3, 0x20ac, 0x00a5, 0x0160, 0x00a7,        /* 0xA0 -      */
    0x0161, 0x00a9, 0x00aa, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x00af,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x00b2, 0x00b3, 0x017d, 0x00b5, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x017e, 0x00b9, 0x00ba, 0x00bb, 0x0152, 0x0153, 0x0178, 0x00bf,        /*      - 0xBF */
    0x00c0, 0x00c1, 0x00c2, 0x00c3, 0x00c4, 0x00c5, 0x00c6, 0x00c7,        /* 0xC0 -      */
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    0x00d0, 0x00d1, 0x00d2, 0x00d3, 0x00d4, 0x00d5, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x00d8, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x00dd, 0x00de, 0x00df,        /*      - 0xDF */
    0x00e0, 0x00e1, 0x00e2, 0x00e3, 0x00e4, 0x00e5, 0x00e6, 0x00e7,        /* 0xE0 -      */
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    0x00f0, 0x00f1, 0x00f2, 0x00f3, 0x00f4, 0x00f5, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x00f8, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x00fd, 0x00fe, 0x00ff,        /*      - 0xFF */
};

/* generated by ../tools/make_charset_ISO-8859-16 */
const gunichar2 charset_table_iso_8859_16[0x80] = {
    0x0080, 0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x0087,        /* 0x80 -      */
    0x0088, 0x0089, 0x008a, 0x008b, 0x008c, 0x008d, 0x008e, 0x008f,        /*      - 0x8F */
    0x0090, 0x0091, 0x0092, 0x0093, 0x0094, 0x0095, 0x0096, 0x0097,        /* 0x90 -      */
    0x0098, 0x0099, 0x009a, 0x009b, 0x009c, 0x009d, 0x009e, 0x009f,        /*      - 0x9F */
    0x00a0, 0x0104, 0x0105, 0x0141, 0x20ac, 0x201e, 0x0160, 0x00a7,        /* 0xA0 -      */
    0x0161, 0x00a9, 0x0218, 0x00ab, 0x0179, 0x00ad, 0x017a, 0x017b,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x010c, 0x0142, 0x017d, 0x201d, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x017e, 0x010d, 0x0219, 0x00bb, 0x0152, 0x0153, 0x0178, 0x017c,        /*      - 0xBF */
    0x00c0, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0106, 0x00c6, 0x00c7,        /* 0xC0 -      */
    0x00c8, 0x00c9, 0x00ca, 0x00cb, 0x00cc, 0x00cd, 0x00ce, 0x00cf,        /*      - 0xCF */
    0x0110, 0x0143, 0x00d2, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x015a,        /* 0xD0 -      */
    0x0170, 0x00d9, 0x00da, 0x00db, 0x00dc, 0x0118, 0x021a, 0x00df,        /*      - 0xDF */
    0x00e0, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x0107, 0x00e6, 0x00e7,        /* 0xE0 -      */
    0x00e8, 0x00e9, 0x00ea, 0x00eb, 0x00ec, 0x00ed, 0x00ee, 0x00ef,        /*      - 0xEF */
    0x0111, 0x0144, 0x00f2, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x015b,        /* 0xF0 -      */
    0x0171, 0x00f9, 0x00fa, 0x00fb, 0x00fc, 0x0119, 0x021b, 0x00ff,        /*      - 0xFF */
};

/* Windows-1250 (http://en.wikipedia.org/wiki/Windows-1250) */
const gunichar2 charset_table_cp1250[0x80] = {
    0x20ac, UNREPL, 0x201a, UNREPL, 0x201e, 0x2026, 0x2020, 0x2021,        /* 0x80 -      */
    UNREPL, 0x2030, 0x0160, 0x2039, 0x015a, 0x0164, 0x017d, 0x0179,        /*      - 0x8F */
    UNREPL, 0x2018, 0x2019, 0x201c, 0x201d, 0x2022, 0x2013, 0x2014,        /* 0x90 -      */
    UNREPL, 0x2122, 0x0161, 0x203a, 0x015b, 0x0165, 0x017e, 0x017a,        /*      - 0x9F */
    0x00a0, 0x02c7, 0x02d8, 0x0141, 0x00a4, 0x0104, 0x00a6, 0x00a7,        /* 0xA0 -      */
    0x00a8, 0x00a9, 0x015e, 0x00ab, 0x00ac, 0x00ad, 0x00ae, 0x017b,        /*      - 0xAF */
    0x00b0, 0x00b1, 0x02db, 0x0142, 0x00b4, 0x00b5, 0x00b6, 0x00b7,        /* 0xB0 -      */
    0x00b8, 0x0105, 0x015f, 0x00bb, 0x013d, 0x02dd, 0x013e, 0x017c,        /*      - 0xBF */
    0x0154, 0x00c1, 0x00c2, 0x0102, 0x00c4, 0x0139, 0x0106, 0x00c7,        /* 0xC0 -      */
    0x010c, 0x00c9, 0x0118, 0x00cb, 0x011a, 0x00cd, 0x00ce, 0x010e,        /*      - 0xCF */
    0x0110, 0x0143, 0x0147, 0x00d3, 0x00d4, 0x0150, 0x00d6, 0x00d7,        /* 0xD0 -      */
    0x0158, 0x016e, 0x00da, 0x0170, 0x00dc, 0x00dd, 0x0162, 0x00df,        /*      - 0xDF */
    0x0155, 0x00e1, 0x00e2, 0x0103, 0x00e4, 0x013a, 0x0107, 0x00e7,        /* 0xE0 -      */
    0x010d, 0x00e9, 0x0119, 0x00eb, 0x011b, 0x00ed, 0x00ee, 0x010f,        /*      - 0xEF */
    0x0111, 0x0144, 0x0148, 0x00f3, 0x00f4, 0x0151, 0x00f6, 0x00f7,        /* 0xF0 -      */
    0x0159, 0x016f, 0x00fa, 0x0171, 0x00fc, 0x00fd, 0x0163, 0x02d9,        /*      - 0xFF */
};


/*
 * Editor modelines  -  http://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 4
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=4 tabstop=8 expandtab:
 * :indentSize=4:tabSize=8:noTabs=true:
 */
