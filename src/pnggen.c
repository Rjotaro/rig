#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "pnggen.h"
#include "crc.h"
#include "debug.h"

static unsigned char buff;
static struct png_config png_cfg =
{
  .plte_chunk.type[0] =         0x50,
  .plte_chunk.type[1] =         0x4c,
  .plte_chunk.type[2] =         0x54,
  .plte_chunk.type[3] =         0x45
};

void
png_header(FILE* fp)
{
  PUTC_FP(0x89);
  PUTC_FP(0x50); // P
  PUTC_FP(0x4e); // N
  PUTC_FP(0x47); // G
  PUTC_FP(0x0d);
  PUTC_FP(0x0a);
  PUTC_FP(0x1a);
  PUTC_FP(0x0a);
}

void
ihdr(FILE* fp)
{
  /* Initialize with four bits of ihdr chunk type */
  unsigned char crc_buff[17];

  /* Write ihdr chunk length */
  PUTC_FP(0x00);
  PUTC_FP(0x00);
  PUTC_FP(0x00);
  PUTC_FP(0x0d);

  PUTC_FP_CRC(0x49, 0); // I
  PUTC_FP_CRC(0x48, 1); // H
  PUTC_FP_CRC(0x44, 2); // D
  PUTC_FP_CRC(0x52, 3); // R

  /* Widht bytes */
  PUTC_FP_CRC(0x00, 4);
  PUTC_FP_CRC(0x00, 5);
  /* 0x07 Max value because in other hand resoliton will be too large. */
  PUTC_FP_CRC(rand() & 0x07, 6);
  PUTC_FP_CRC(rand() & 0xff, 7);

  png_cfg.ihdr_chunk.width |= crc_buff[6] << 8; /* Writes resolution in variable.
                                                   This esoteric bit operation here just because
                                                   in beggining we generate most significant digits. */
  png_cfg.ihdr_chunk.width += crc_buff[7];

  /* Height bytes*/
  PUTC_FP_CRC(0x00, 8);
  PUTC_FP_CRC(0x00, 9);
  /* 0x07 Max value because in other hand resoliton will be too large. */
  PUTC_FP_CRC(rand() & 0x07, 10);
  PUTC_FP_CRC(rand() & 0xff, 11);

  png_cfg.ihdr_chunk.height |= crc_buff[10] << 8; /* Writes resolution in variable.
                                                     This esoteric bit operation here just because
                                                     in beggining we generate most significant digits. */
  png_cfg.ihdr_chunk.height += crc_buff[11];


  switch (rand() % 5) /* Chose color type. */
    {
    case 0:
      PUTC_FP_CRC(0x00, 12);
      switch (rand() % 5) /* Chose bit depth. */
        {
        case 0: PUTC_FP_CRC(0x01, 13); break;
        case 1: PUTC_FP_CRC(0x02, 13); break;
        case 2: PUTC_FP_CRC(0x04, 13); break;
        case 3: PUTC_FP_CRC(0x08, 13); break;
        case 4: PUTC_FP_CRC(0x10, 13); break;
        }
      break;

    case 1:
      PUTC_FP_CRC(0x02, 13);
      switch (rand() % 2)
        {
        case 0: PUTC_FP_CRC(0x08, 12); break;
        case 1: PUTC_FP_CRC(0x10, 12); break;
        }
      break;

    case 2:
      PUTC_FP_CRC(0x03, 13);
      switch (rand() % 4)
        {
        case 0: PUTC_FP_CRC(0x01, 12); break;
        case 1: PUTC_FP_CRC(0x02, 12); break;
        case 2: PUTC_FP_CRC(0x04, 12); break;
        case 3: PUTC_FP_CRC(0x08, 12); break;
        }
      break;

    case 3:
      PUTC_FP_CRC(0x04, 13);
      switch(rand() % 2)
        {
        case 0: PUTC_FP_CRC(0x08, 12); break;
        case 1: PUTC_FP_CRC(0x10, 12); break;
        }
      break;

    case 4:
      PUTC_FP_CRC(0x06, 13);
      switch(rand() % 2)
        {
        case 0: PUTC_FP_CRC(0x08, 12); break;
        case 1: PUTC_FP_CRC(0x10, 12); break;
        }
      break;
    }

  png_cfg.ihdr_chunk.bit_depth  = crc_buff[12];
  png_cfg.ihdr_chunk.color_type = crc_buff[13];

  PUTC_FP_CRC(0x00, 14); /* Compression method, png support only zero value */
  PUTC_FP_CRC(0x00, 15); /* With filter method same situation */
  PUTC_FP_CRC(rand() & 0x01, 16); /* Interlace method */

  png_cfg.ihdr_chunk.itrl_type = crc_buff[16];

  unsigned crc_code = crc(crc_buff, 17);
  FWRITE(crc_code);
}

void
plte (FILE* fp)
{
  switch(png_cfg.ihdr_chunk.bit_depth)
    {
    case 0x01:
      png_cfg.plte_chunk.length[0] = 0x00;
      png_cfg.plte_chunk.length[1] = 0x00;
      png_cfg.plte_chunk.length[2] = 0x00;
      png_cfg.plte_chunk.length[3] = 0x02;
      break;

    case 0x02:
      png_cfg.plte_chunk.length[0] = 0x00;
      png_cfg.plte_chunk.length[1] = 0x00;
      png_cfg.plte_chunk.length[2] = 0x00;
      png_cfg.plte_chunk.length[3] = 0x04;
      break;

    case 0x04:
      png_cfg.plte_chunk.length[0] = 0x00;
      png_cfg.plte_chunk.length[1] = 0x00;
      png_cfg.plte_chunk.length[2] = 0x00;
      png_cfg.plte_chunk.length[3] = 0x0f;
      break;

    case 0x08:
      png_cfg.plte_chunk.length[0] = 0x00;
      png_cfg.plte_chunk.length[1] = 0x00;
      png_cfg.plte_chunk.length[2] = 0x01;
      png_cfg.plte_chunk.length[3] = 0x00;
      break;

    case 0x0f:
      png_cfg.plte_chunk.length[0] = 0x00;
      png_cfg.plte_chunk.length[1] = 0x10;
      png_cfg.plte_chunk.length[2] = 0x00;
      png_cfg.plte_chunk.length[3] = 0x00;
      break;
    }

    /* unsigned char crc_buff[CHUNK_LEN_N_TYPE_SIZE + (int)pow (2, png_cfg.ihdr_chunk.bit_depth)]; */
    PRINT_D (png_cfg.ihdr_chunk.bit_depth);

    for (int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++)
      FWRITE(png_cfg.plte_chunk.length[i]);

    for (int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++)
      FWRITE(png_cfg.plte_chunk.type[i]);
}

void
generate_png(void)
{
  FILE* fp = fopen("image.png", "wb+");
  buff = 0;

  png_header(fp);
  ihdr(fp);

  if (png_cfg.ihdr_chunk.color_type != 0 || png_cfg.ihdr_chunk.color_type != 4)
    { /* Generate plte chunk */
      if (png_cfg.ihdr_chunk.color_type == 3)
        plte(fp);
      else if (rand() % 2 == 1)
        plte(fp); /* For color type 2 and 3 plte optional */
    }
}
