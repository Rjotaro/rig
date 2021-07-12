#ifndef PNGGEN_H
#define PNGGEN_H

#include <stdio.h>

typedef struct 
{
    unsigned char  r;
    unsigned char  g;
    unsigned char  b;
} pallete_t;

struct png_config 
{
    unsigned char header[8];
    
    struct 
    {
      unsigned char  bit_depth;
      unsigned char  color_type;
      unsigned char  compr_type;
      unsigned char  filt_type;
      unsigned char  itrl_type;
      unsigned char  length[4];
      unsigned char  type[4];
      unsigned short width;
      unsigned short height;
    } ihdr_chunk;
    
    struct 
    {
      unsigned char length[4];
      unsigned char type[4];
      pallete_t*    pallete;
    } plte_chunk;
};

void png_configure (FILE* fp);
void png_header (FILE* fp);

void ihdr (FILE* fp);
void plte (FILE* fp);

void generate_png (void);

#define PNG_HEAD_SIZE              8
#define CHUNK_LEN_N_TYPE_SIZE      4
#define SIZE_BYTES                 4

#define FWRITE(X) fwrite(&X, sizeof(X), 1, fp)

#endif /* PNGGEN_H */
