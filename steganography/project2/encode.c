#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    // beautiful.bmp
    if(strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2]; 
    }
    else
    {
        printf("Invalid input\n");
        return e_failure;
    }
    // secret file extension
    if(strstr(argv[3], ".txt") != NULL)
    {
        encInfo->secret_fname =argv[3];
    }
    else
    {
        printf("Invalid input\n");
        return e_failure;
    }
    // checking [stego.bmp] passed in input cmd
    if(argv[4] == NULL)
    {
        // default output image
        encInfo->stego_image_fname = "default.bmp";
    }
    else
    {
        // validate bmp or not
        encInfo->stego_image_fname =argv[4];
    }
    return e_success;
}

/* copy bmp image header*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char arr[54];
    // Go to start
    fseek(fptr_src_image, 0, SEEK_SET);
    // Read header
    fread(arr, 1, 54, fptr_src_image);
    // Write header
    fwrite(arr, 1, 54, fptr_dest_image);
    return e_success;
}

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    /* Loop through 8 bits of secret character */
    for(int i = 0; i < 8; i++)
    {
        /* Clear LSB of image byte-> Extract ith bit of secret data-> Insert that bit into LSB */
        image_buffer[i] = (image_buffer[i] & 0xFE) | ((data >> i) & 1);
    }
    return e_success;
}

Status encode_data_to_image(char *data, int size, EncodeInfo *encInfo)
{
    for(int i = 0 ; i < size; i++)
    {
        // Read 8 bytes from image
        fread(encInfo->image_data, 8, 1, encInfo->fptr_src_image);
        encode_byte_to_lsb(data[i], encInfo->image_data);
        // Write encoded bytes
        fwrite(encInfo->image_data, 8, 1, encInfo->fptr_stego_image);
    }
    return e_success;
}

Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string, strlen(magic_string), encInfo);
    return e_success;
}

Status encode_size_to_lsb(int size, EncodeInfo *encInfo)
{
    // read from input image
    char str[32];
    // fread(str, 32, 1, encInfo->fptr_stego_image);
    fread(str, 32, 1, encInfo->fptr_src_image);
    // bitwise operation
    for(int i = 0; i < 32; i++)
    {
        str[i] = (str[i] & 0xFE) | ((size >> i) & 1);
    }
    // write to output file
    fwrite(str, 32, 1, encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file extension size */
Status encode_secret_file_extn_size(int extn_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(extn_size, encInfo);
    return e_success;
}

/* Encode secret file extension string */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn, strlen(file_extn), encInfo);
    return e_success;
}

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    encode_size_to_lsb(file_size, encInfo);
    return e_success;
}

/* Encode secret file content */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    // +1 for storing the null char
    char str[encInfo->size_secret_file + 1];
    // Go to start
    rewind(encInfo->fptr_secret);
    // Read secret
    fgets(str, encInfo->size_secret_file + 1, encInfo->fptr_secret);
    // Encode data
    encode_data_to_image(str, strlen(str), encInfo);
    return e_success;
}

/* Copy remaining image bytes after encoding */
Status copy_remaining_img_data(EncodeInfo *encInfo)
{
    // Remaining bytes
    int size = encInfo->image_capacity + 54 - ftell(encInfo->fptr_src_image);
    char remaining_data[size];
    fread(remaining_data, size, 1, encInfo->fptr_src_image);
    fwrite(remaining_data, size, 1, encInfo->fptr_stego_image);
    return e_success;
}

Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("Open files is success\n");
        if(check_capacity(encInfo) == e_success)
        {
            printf("Secret data can fit in image\n");
            if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success) 
            {
                printf("Copy bmp header is success\n");
                if(encode_magic_string(MAGIC_STRING, encInfo) == e_success)
                {
                    printf("Encoded magic string successfully\n");
                    // Get secret file extension (.txt)
                    strcpy(encInfo->extn_secret_file, strstr(encInfo->secret_fname, "."));
                    printf("%s\n",encInfo->extn_secret_file);
                    if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_success) //.txt
                    {
                        printf("encoded extension size successfully\n");
                        if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_success)
                        {
                            printf("Encoded file extn successfully\n");
                            if(encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
                            {
                                printf("Encoded secret file size successfully\n");
                                if(encode_secret_file_data(encInfo) == e_success)
                                {
                                    printf("Encodede secret file data successfully\n");
                                    if(copy_remaining_img_data(encInfo) == e_success)
                                    {
                                        printf("Copied remaining data successfully\n");
                                    }
                                    else
                                    {
                                        printf("Copied remaining data failed\n");
                                        return e_failure;
                                    }
                                }
                                else
                                {
                                    printf("Encodede secret file data failed\n");
                                    return e_failure;
                                }
                            }
                            else
                            {
                                printf("Encoded secret file size failed\n");
                                return e_failure;
                            }
                        }
                        else{
                            printf("Encoded file extn failed\n");
                            return e_failure;
                        }
                    }
                    else
                    {
                        printf("encoded extension size failed\n");
                        return e_failure;
                    }
                }
                else
                {
                    printf("encoded magic string failure\n");
                    return e_failure;
                }
            }
            else
            {
                printf("copy bmp header is failure\n");
            }
        }
        else
        {
            printf("Secret data cannot fit in image\n");
            return 0;
            // return e_failure;
        }
    }
    else
    {
        printf("Open files is failure\n");
        return e_failure;
    }
    return e_success;
}

uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    return ftell(fptr);
}

Status check_capacity(EncodeInfo *encInfo)
{
    /* Get image capacity in bytes */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    /* Get secret file size */
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    /*  Check if image can store */
    if(encInfo->image_capacity > 16+32+32+32+ encInfo->size_secret_file * 8)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}
