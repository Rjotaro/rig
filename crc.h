#ifndef CRC_H
#define CRC_H

/* Honestly stoled from https://www.w3.org/TR/PNG-CRCAppendix.html */

static unsigned crc_table[256]; /* Table of CRCs of all 8-bit messages. */
static int crc_table_computed = 0; /* Flag: has the table been computed? Initially false. */

void make_crc_table(void); /* Make the table for a fast CRC. */

/* Update a running CRC with the bytes buf[0..len-1]--the CRC
should be initialized to all 1's, and the transmitted value
is the 1's complement of the final running CRC (see the
crc() routine below)). */
unsigned update_crc(unsigned crc, unsigned char *buf, int len);

/* Return the CRC of the bytes buf[0..len-1]. */
unsigned crc(unsigned char *buf, int len);

#endif /* CRC_H */
