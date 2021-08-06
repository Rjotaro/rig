#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "pnggen.h"
#include "crc.h"
#include "debug.h"

static unsigned char buff;
static struct png_config png_cfg =
{
  .header[0] =                  0x89,
  .header[1] =                  0x50,
  .header[2] =                  0x4e,
  .header[3] =                  0x47,
  .header[4] =                  0x0d,
  .header[5] =                  0x0a,
  .header[6] =                  0x1a,
  .header[7] =                  0x0a,

  .ihdr_chunk.length[0] =       0x00,
  .ihdr_chunk.length[1] =       0x00,
  .ihdr_chunk.length[2] =       0x00,
  .ihdr_chunk.length[3] =       0x0d,

  .ihdr_chunk.type[0] =         0x49,
  .ihdr_chunk.type[1] =         0x48,
  .ihdr_chunk.type[2] =         0x44,
  .ihdr_chunk.type[3] =         0x52,

  .plte_chunk.type[0] =         0x50,
  .plte_chunk.type[1] =         0x4c,
  .plte_chunk.type[2] =         0x54,
  .plte_chunk.type[3] =         0x45
};

void
png_header (FILE* fp)
{
  for (int i = 0; i < PNG_HEAD_SIZE; i++)
    FWRITE (png_cfg.header[i]);
}

void
ihdr (FILE* fp)
{
  unsigned char crc_buff[17];

  for (int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++)
    FWRITE (png_cfg.ihdr_chunk.length[i]);

  for (int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++)
    {
      crc_buff[i] = png_cfg.ihdr_chunk.type[i];
      FWRITE (png_cfg.ihdr_chunk.type[i]);
    }

  for (int i = 0; i < SIZE_BYTES; i++)
    {
      switch(i)
        {
        case 0:
          crc_buff[4] = 0x00;
          FWRITE (crc_buff[4]);
          break;

        case 1:
          crc_buff[5] = 0x00;
          FWRITE (crc_buff[5]);
          break;

        case 2:
          /* 0x07 Max value becouse in other hand resoliton will be too large. */
          crc_buff[6] = rand () & 0x07;
          /* Writes resolution in variable.
             This esoteric bit operation here just because
             in beggining we generate most significant digits. */
          png_cfg.ihdr_chunk.width |= crc_buff[6] << 8;
          FWRITE (crc_buff[6]);
          break;

        case 3:
          crc_buff[7] = rand () & 0xff;
          png_cfg.ihdr_chunk.width += crc_buff[7];
          FWRITE (crc_buff[7]);
          break;
        }
    }

  for (int i = 0; i < SIZE_BYTES; i++)
    {
      switch(i)
        {
        case 0:
          crc_buff[8] = 0x00;
          FWRITE (crc_buff[8]);
          break;

        case 1:
          crc_buff[9] = 0x00;
          FWRITE (crc_buff[9]);
          break;

        case 2:
          /* 0x07 Max value becouse in other hand resoliton will be too large. */
          crc_buff[10] = rand () & 0x07;
          /* Writes resolution in variable. 
             This esoteric bit operation here just because
             in beggining we generate most significant digits. */
          png_cfg.ihdr_chunk.height |= crc_buff[10] << 8;
          FWRITE (buff);
          break;

        case 3:
          crc_buff[11] = rand () & 0xff;
          png_cfg.ihdr_chunk.height += crc_buff[11];
          FWRITE (crc_buff[11]);
          break;
        }
    }

    switch (rand () % 5) /* Chose color type. */
      {
      case 0:
        crc_buff[12] = 0x00;
        FWRITE (crc_buff[12]);

        switch (rand () % 5) /* Chose bit depth. */
          {
          case 0:
            crc_buff[13] = 0x01;
            FWRITE(crc_buff[13]);
            break;

          case 1:
            crc_buff[13] = 0x02;
            FWRITE(crc_buff[13]);
            break;

          case 2:
            buff = 0x04;
            crc_buff[13] = buff;
            FWRITE(buff);
            break;

          case 3:
            buff = 0x08;
            crc_buff[13] = buff;
            FWRITE(buff);
            break;

          case 4:
            buff = 0x10;
            crc_buff[13] = buff;
            FWRITE(buff);
            break;
          }

        break;

      case 1:
        crc_buff[13] = 0x02;
        FWRITE (crc_buff[13]);

        switch (rand() % 2)
          {
          case 0:
            crc_buff[12] = 0x08;
            FWRITE(crc_buff[12]);
            break;

          case 1:
            crc_buff[12] = 0x10;
            FWRITE(crc_buff[12]);
            break;
          }

        break;

      case 2:
        crc_buff[13] = 0x03;
        FWRITE (crc_buff[13]);

        switch (rand () % 4)
          {
          case 0:
            crc_buff[12] = 0x01;
            FWRITE (crc_buff[12]);
            break;

          case 1:
            crc_buff[12] = 0x02;
            FWRITE (crc_buff[12]);
            break;

          case 2:
            crc_buff[12] = 0x04;
            FWRITE (crc_buff[12]);
            break;

          case 3:
            crc_buff[12] = 0x08;
            FWRITE (crc_buff[12]);
            break;
          }

          break;

      case 3:
        crc_buff[13] = 0x04;
        FWRITE (crc_buff[13]);

        switch(rand() % 2)
          {
          case 0:
            crc_buff[12] = 0x08;
            FWRITE (crc_buff[12]);
            break;

          case 1:
            crc_buff[12] = 0x10;
            FWRITE(crc_buff[12]);
            break;

          }

        break;

      case 4:
        crc_buff[13] = 0x06;
        FWRITE (crc_buff[13]);

        switch(rand() % 2)
          {
          case 0:
            crc_buff[12] = 0x08;
            FWRITE(crc_buff[12]);
            break;

          case 1:
            crc_buff[12] = 0x10;
            FWRITE(crc_buff[12]);
            break;
          }

        break;
      }

  png_cfg.ihdr_chunk.bit_depth  = crc_buff[12];
  png_cfg.ihdr_chunk.color_type = crc_buff[13];

  crc_buff[14] = 0x00; /* Compression method, png support only zero value */
  FWRITE (crc_buff[14]);

  crc_buff[15] = 0x00; /* With filter method same situation */
  FWRITE (crc_buff[15]);

  crc_buff[16] = rand() & 0x01; /* Interlace method */
  png_cfg.ihdr_chunk.itrl_type = crc_buff[16];
  FWRITE (crc_buff[16]);

  unsigned crc_code = crc (crc_buff, 17);
  FWRITE (crc_code);
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
generate_png (void)
{
  FILE* fp = fopen ("image.png", "wb+");
  buff = 0;

  png_header (fp);
  ihdr (fp);

  if (png_cfg.ihdr_chunk.color_type != 0 || png_cfg.ihdr_chunk.color_type != 4)
    { /* Generate plte chunk */
      if (png_cfg.ihdr_chunk.color_type == 3)
        plte(fp);
      else if (rand() % 2 == 1)
        plte(fp); /* For color type 2 and 3 plte oprional */
    }
}
