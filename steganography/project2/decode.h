#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include "types.h"
#include "common.h"

/* Max buffer for reading image data for decoding */
#define MAX_IMAGE_BUF_SIZE_DECODE (8)  
#define MAX_FILE_SUFFIX_DECODE 10

/* Structure to store information required for
 * decoding secret file from stego Image
 */
typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;
    uint image_capacity;
    char image_data[MAX_IMAGE_BUF_SIZE_DECODE];

    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX_DECODE];
    long size_secret_file;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

} DecodeInfo;

/* Function Prototypes */

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file pointers for stego and output files */
Status open_decode_files(DecodeInfo *decInfo);

/* Read and validate decode args */
Status read_and_validate_decode_args(char *argv[],
                                     DecodeInfo *decInfo);

/* Decode a byte from LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer);

/* Decode data from image */
Status decode_data_from_image(char *data, int size,
                              DecodeInfo *decInfo);

/* Decode size from image */
Status decode_size_from_lsb(int *size, DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

#endif