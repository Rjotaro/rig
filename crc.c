#include "crc.h"

/* Honestly stoled from https://www.w3.org/TR/PNG-CRCAppendix.html */

void make_crc_table(void) {
    unsigned c;
    int n, k;

    for(n = 0; n < 256; n++) {
        c = (unsigned) n;
        
        for(k = 0; k < 8; k++) {
            if(c & 1) c = 0xedb88320L ^ (c >> 1);
            else c = c >> 1;
        }
        
        crc_table[n] = c;
    }
    
    crc_table_computed = 1;
}

unsigned update_crc(unsigned crc, unsigned char *buf, int len) {
    unsigned long c = crc;
    int n;

    if(!crc_table_computed) make_crc_table();
    
    for(n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    
    return c;
}

unsigned crc(unsigned char *buf, int len) {
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}


