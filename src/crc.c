/* Honestly stoled from https://www.w3.org/TR/PNG-CRCAppendix.html */

#include <stdbool.h>
#include "crc.h"

static unsigned crc_table[256]; /* Table of CRCs of all 8-bit messages. */
static bool crc_table_computed = false; /* Flag: has the table been computed? Initially false. */

void make_crc_table (void) 
{
  unsigned c;

  for (int n = 0; n < 256; n++) 
    {
      c = (unsigned) n;
        
      for(int k = 0; k < 8; k++) 
        {
          if(c & 1) 
            c = 0xedb88320L ^ (c >> 1);
          else 
            c = c >> 1;
        }
        
      crc_table[n] = c;
    }
    
    crc_table_computed = true;
}

unsigned 
update_crc (unsigned crc, unsigned char *buf, int len) 
{
    unsigned long c = crc;

    if (!crc_table_computed)
      make_crc_table();
    
    for (int n = 0; n < len; n++)
      c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    
    return c;
}

unsigned 
crc (unsigned char *buf, int len)
{
    return update_crc(0xffffffffL, buf, len) ^ 0xffffffffL;
}


