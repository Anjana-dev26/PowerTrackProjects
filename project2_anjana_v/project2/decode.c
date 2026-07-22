#include <stdio.h>
#include <string.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get File pointers for stego and output files
 * Input: Stego Image file and Output file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_decode_files(DecodeInfo *decInfo)
{
    /* Stego Image file */
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    return e_success;
}

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* beautiful.bmp */
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
    {
        printf("Invalid input\n");
        return e_failure;
    }
    /* output file */
    if (argv[3] != NULL)
    {
        decInfo->output_fname = argv[3];
    }
    else
    {
        decInfo->output_fname = NULL;
    }
    return e_success;
}

/* Decode a byte from LSB */
Status decode_byte_from_lsb(char *data, char *image_buffer)
{
    // Initialize result byte
    *data = 0;
    /* Extract 8 bits from image LSB */
    for (int i = 0; i < 8; i++)
    {
        // Extract LSB and combine
        *data |= (image_buffer[i] & 1) << i;
    }
    return e_success;
}

/* Decode data from image */
Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char buffer[8];
    for (int i = 0; i < size; i++)
    {
        // Read 8 bytes
        fread(buffer, 8, 1, decInfo->fptr_stego_image);
        decode_byte_from_lsb(&data[i], buffer);
    }
    return e_success;
}

/* Decode size from image */
Status decode_size_from_lsb(int *size, DecodeInfo *decInfo)
{
    char buffer[32];
    /* Read 32 bytes from image */
    fread(buffer, 32, 1, decInfo->fptr_stego_image);
    *size = 0;
     /* Extract 32 bits */
    for (int i = 0; i < 32; i++)
    {
        // Reconstruct integer
        *size |= (buffer[i] & 1) << i;
    }
    return e_success;
}

/* Decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char magic_string[3];
    decode_data_from_image(magic_string, strlen(MAGIC_STRING), decInfo);
    magic_string[2] = '\0';
    if (strcmp(magic_string, MAGIC_STRING) == 0)
    {
        printf("Magic string matched\n");
        return e_success;
    }
    else
    {
        printf("Magic string mismatch\n");
        return e_failure;
    }
}

/* Decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    int extn_size;
    // Get extension length
    decode_size_from_lsb(&extn_size, decInfo);
    decode_data_from_image(decInfo->extn_secret_file, extn_size, decInfo);
    decInfo->extn_secret_file[extn_size] = '\0';
    printf("Decoded file extension: %s\n", decInfo->extn_secret_file);
    return e_success;
}

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    int size;
    // Extract size
    decode_size_from_lsb(&size, decInfo);
    decInfo->size_secret_file = size;
    printf("Decoded secret file size: %ld\n", decInfo->size_secret_file);
    return e_success;
}

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;
    /* Open output file */
    if (decInfo->output_fname == NULL)
    {
        static char fname[30];
        strcpy(fname, "output");
        strcat(fname, decInfo->extn_secret_file);
        decInfo->fptr_output = fopen(fname, "w");
    }
    else
    {
        decInfo->fptr_output = fopen(decInfo->output_fname, "w");
    }
    if (decInfo->fptr_output == NULL)
    {
        printf("Unable to open output file\n");
        return e_failure;
    }
    // Decode byte by byte
    for (long i = 0; i < decInfo->size_secret_file; i++)
    {
        decode_data_from_image(&ch, 1, decInfo);
        // Write to output
        fwrite(&ch, 1, 1, decInfo->fptr_output);
    }
    printf("Secret file data decoded successfully\n");
    return e_success;
}

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo)
{
    if (open_decode_files(decInfo) == e_success)
    {
        printf("Open files is success\n");
        /* Skip bmp header */
        fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
        if (decode_magic_string(decInfo) == e_success)
        {
            if (decode_secret_file_extn(decInfo) == e_success)
            {
                if (decode_secret_file_size(decInfo) == e_success)
                {
                    if (decode_secret_file_data(decInfo) == e_success)
                    {
                        printf("Decoding is success\n");
                    }
                    else
                    {
                        printf("Secret file data decode failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("Secret file size decode failed\n");
                    return e_failure;
                }
            }
            else
            {
                printf("Secret file extension decode failed\n");
                return e_failure;
            }
        }
        else
        {
            printf("Magic string decode failed\n");
            return e_failure;
        }
    }
    else
    {
        printf("Open files is failure\n");
        return e_failure;
    }

    return e_success;
}