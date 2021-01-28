#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "pnggen.h"
#include "crc.h"
#include "utility.h"

static png_config_t png_conf;
static unsigned char buff;

void png_configure(FILE* fp) {
    png_conf.header[0] =                  0x89;
    png_conf.header[1] =                  0x50;
    png_conf.header[2] =                  0x4e;
    png_conf.header[3] =                  0x47;
    png_conf.header[4] =                  0x0d;
    png_conf.header[5] =                  0x0a;
    png_conf.header[6] =                  0x1a;
    png_conf.header[7] =                  0x0a;
    
    png_conf.ihdr_chunk.length[0] =       0x00;
    png_conf.ihdr_chunk.length[1] =       0x00;
    png_conf.ihdr_chunk.length[2] =       0x00;
    png_conf.ihdr_chunk.length[3] =       0x0d;
    
    png_conf.ihdr_chunk.type[0] =         0x49;
    png_conf.ihdr_chunk.type[1] =         0x48;
    png_conf.ihdr_chunk.type[2] =         0x44;
    png_conf.ihdr_chunk.type[3] =         0x52;
    
    png_conf.plte_chunk.type[0] =         0x50;
    png_conf.plte_chunk.type[1] =         0x4c;
    png_conf.plte_chunk.type[2] =         0x54;
    png_conf.plte_chunk.type[3] =         0x45;
    
/*  png_conf.ihdr_chunk.width =      0;
    png_conf.ihdr_chunk.height =     0;
    png_conf.ihdr_chunk.color_type = 0x00;
    png_conf.ihdr_chunk.compt_type = 0x00;
    png_conf.ihdr_chunk.filt_type =  0x00;
    png_conf.ihdr_chunk.itrl_type =  0x00; */
}

void png_header(FILE* fp) {
    for(int i = 0; i < PNG_HEAD_SIZE; i++) {
        buff = png_conf.header[i];
        FWRITE(buff);
    }
}

void ihdr(FILE* fp) {
    unsigned char crc_buff[17];    
    
    for(int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++) {
        buff = png_conf.ihdr_chunk.length[i];
        FWRITE(buff);
    }
    
    for(int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++) {
        buff = png_conf.ihdr_chunk.type[i];
        crc_buff[i] = buff;
        FWRITE(buff);
    }
    
    for(int i = 0; i < SIZE_BYTES; i++) {
        switch(i) {
            case 0:
                buff = 0x00;
                crc_buff[4] = buff;
                FWRITE(buff);
                break;
            case 1:
                buff = 0x00;
                crc_buff[5] = buff;
                FWRITE(buff);
                break;
            case 2:
                buff = rand() & 0x07; /* 0x07 Max value becouse in other hand resoliton will be too large */
                crc_buff[6] = buff;
                png_conf.ihdr_chunk.width |= buff << 8; /* Writes resolution in variable. 
                                                                This esoteric bit operation here just because
                                                                in beggining we generate most significant digits */
                FWRITE(buff);
                break;
            case 3:
                buff = rand() & 0xff;
                crc_buff[7] = buff;
                png_conf.ihdr_chunk.width += buff;
                FWRITE(buff);
                break;
        }       
    }
                
    for(int i = 0; i < SIZE_BYTES; i++) {
        switch(i) {
            case 0:
                buff = 0x00;
                crc_buff[8] = buff;
                FWRITE(buff);
                break;
            case 1:
                buff = 0x00;
                crc_buff[9] = buff;
                FWRITE(buff);
                break;
            case 2:
                buff = rand() & 0x07; /* 0x07 Max value becouse in other hand resoliton will be too large */
                crc_buff[10] = buff;
                png_conf.ihdr_chunk.height |= buff << 8; /* Writes resolution in variable. 
                                                                This esoteric bit operation here just because
                                                                in beggining we generate most significant digits */
                FWRITE(buff);      
                break;
            case 3:
                buff = rand() & 0xff; 
                crc_buff[11] = buff;
                png_conf.ihdr_chunk.height += buff;
                FWRITE(buff);
                break;
        }        
    }
    
    buff = rand() % 5; /* Chose color type */
    switch(buff) { 
        case 0:
            buff = 0x00;
            crc_buff[12] = buff;
            FWRITE(buff);
            buff = rand() % 5;
            switch(buff) { /* Chose bit depth */
                case 0:
                    buff = 0x01;
                    crc_buff[13] = buff;
                    FWRITE(buff);
                    break;
                case 1:
                    buff = 0x02;
                    crc_buff[13] = buff;
                    FWRITE(buff);
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
            buff = 0x02;
            crc_buff[13] = buff;
            FWRITE(buff);
            buff = rand() % 2;
            switch(buff) {
                case 0:
                    buff = 0x08;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 1:
                    buff = 0x10;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
            }
            break;
            
        case 2:
            buff = 0x03;
            crc_buff[13] = buff;
            FWRITE(buff);
            buff = rand() % 4;
            switch(buff) {
                case 0:
                    buff = 0x01;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 1:
                    buff = 0x02;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 2:
                    buff = 0x04;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 3:
                    buff = 0x08;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
            }
            break;
            
        case 3:
            buff = 0x04;
            crc_buff[13] = buff;
            FWRITE(buff);
            buff = rand() % 2;
            switch(buff) {
                case 0:
                    buff = 0x08;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 1:
                    buff = 0x10;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
            }
            break;
            
        case 4:
            buff = 0x06;
            crc_buff[13] = buff;
            FWRITE(buff);
            buff = rand() % 2;
            switch(buff) {
                case 0:
                    buff = 0x08;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
                case 1:
                    buff = 0x10;
                    crc_buff[12] = buff;
                    FWRITE(buff);
                    break;
            }
            break;        
    }
    
    png_conf.ihdr_chunk.bit_depth = crc_buff[12];
    png_conf.ihdr_chunk.color_type = crc_buff[13];
    
    buff = 0x00; /* Compression method, png support only zero value */
    crc_buff[14] = buff;
    FWRITE(buff);
    
    buff = 0x00; /* With filter method same situation */
    crc_buff[15] = buff;
    FWRITE(buff);
    
    buff = rand() & 0x01; /* Interlace method */
    crc_buff[16] = buff;
    png_conf.ihdr_chunk.itrl_type = buff;
    FWRITE(buff);
    
    unsigned crc_code = crc(crc_buff, 17);
    FWRITE(crc_code);
}

void plte(FILE* fp) {
    switch(png_conf.ihdr_chunk.bit_depth) {
        case 0x01:
            png_conf.plte_chunk.length[0] = 0x00;
            png_conf.plte_chunk.length[1] = 0x00;
            png_conf.plte_chunk.length[2] = 0x00;
            png_conf.plte_chunk.length[3] = 0x02;
            break;
        case 0x02:
            png_conf.plte_chunk.length[0] = 0x00;
            png_conf.plte_chunk.length[1] = 0x00;
            png_conf.plte_chunk.length[2] = 0x00;
            png_conf.plte_chunk.length[3] = 0x04;
            break;
        case 0x04:
            png_conf.plte_chunk.length[0] = 0x00;
            png_conf.plte_chunk.length[1] = 0x00;
            png_conf.plte_chunk.length[2] = 0x00;
            png_conf.plte_chunk.length[3] = 0x0f;
            break;
        case 0x08:
            png_conf.plte_chunk.length[0] = 0x00;
            png_conf.plte_chunk.length[1] = 0x00;
            png_conf.plte_chunk.length[2] = 0x01;
            png_conf.plte_chunk.length[3] = 0x00;
            break;
        case 0x0f:
            png_conf.plte_chunk.length[0] = 0x00;
            png_conf.plte_chunk.length[1] = 0x10;
            png_conf.plte_chunk.length[2] = 0x00;
            png_conf.plte_chunk.length[3] = 0x00;
            break;
    }
    
    unsigned char crc_buff[CHUNK_LEN_N_TYPE_SIZE + (int)pow(2, png_conf.ihdr_chunk.bit_depth)];
    PRINTF_D(png_conf.ihdr_chunk.bit_depth);
    
    for(int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++) {
        buff = png_conf.plte_chunk.length[i];
        FWRITE(buff);
    }
    
    for(int i = 0; i < CHUNK_LEN_N_TYPE_SIZE; i++) {
        buff = png_conf.plte_chunk.type[i];
        FWRITE(buff);
    }
}

void generate_png(void) {
    FILE* fp = fopen("image.png", "wb+");
    buff = 0;
    
    png_configure(fp);
    png_header(fp);
    ihdr(fp);
    
    if(png_conf.ihdr_chunk.color_type != 0 || png_conf.ihdr_chunk.color_type != 4) { /* Generate plte chunk */
        if(png_conf.ihdr_chunk.color_type == 3) {
            plte(fp);
        } else if(rand() % 2 == 1) plte(fp); /* For color type 2 and 3 plte oprional */
    }
}
