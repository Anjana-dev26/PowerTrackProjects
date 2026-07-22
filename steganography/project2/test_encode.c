/*
------------------------------------------------------------------------------
Project Title  : Image Steganography using LSB Technique
Description    :

This program performs Image Steganography using the Least Significant Bit(LSB) technique.

How Encoding Works:
-------------------
- The program reads the source BMP image.
- It copies the 54-byte BMP header directly to the output file.
- It encodes:
    1. Magic string ("#*") to identify stego image
    2. Secret file extension size
    3. Secret file extension
    4. Secret file size
    5. Secret file data
- Each bit of secret data is stored in the Least Significant Bit (LSB)
  of image pixels.
- Remaining image data is copied as it is.

How Decoding Works:
-------------------
- The program skips the 54-byte BMP header.
- It reads and verifies the magic string.
- It extracts:
    1. File extension size
    2. File extension
    3. Secret file size
    4. Secret file data
- The extracted data is written into an output file.

Command Line Usage:
-------------------
For Encoding:
    ./stego -e source.bmp secret.txt [stego.bmp]

For Decoding:
    ./stego -d stego.bmp [output.txt]
------------------------------------------------------------------------------
*/

#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "decode.h"

int main(int argc, char *argv[])
{
    /* validate cla */
    EncodeInfo E1;
    /* Check whether user selected encode or decode */
    int res = check_operation_type(argv);
    /* If selected encoding (-e) */
    if(res == e_encode)
    {
        printf("encoding is selected\n");
        /* Validate command line arguments */
        if(read_and_validate_encode_args(argv, &E1) == e_success)
        {
            printf("Read and validate encode args is success\n");
            /* Perform encoding process */
            if(do_encoding(&E1) == e_success)
            {
                printf("Encoding is success\n");
            }
            else
            {
                printf("Encoding is failure\n");
                return 0;
            }
        }
        else
        {
            printf("Read and validate encode arg is failure\n");
            return 0;
        }
    }
    /* If selected decoding (-d) */
    else if(res == e_decode)
    {
        DecodeInfo D1;
        printf("Decoding is selected\n");

        if(read_and_validate_decode_args(argv, &D1) == e_success)
        {
            if(do_decoding(&D1) == e_success)
                printf("Decoding success\n");
            else
                printf("Decoding failed\n");
        }
        else
        {
            printf("Invalid decode arguments\n");
        }
    }
    /* If invalid option */
    else
    {
        printf("Invalid option\n");
        printf("Usage : For encoding : ./a.out -e beautiful.bmp secret.txt [stego.bmp]\n");
        printf("Usage : For encoding : ./a.out -d stego.bmp [output.txt]\n");
    }
    return 0;
}

OperationType check_operation_type(char *argv[])
{
    /* Compare second argument with "-e" */
    if( strcmp(argv[1], "-e") == 0)
    {
        return e_encode;
    }
    /* Compare second argument with "-d" */
    else if( strcmp(argv[1], "-d") == 0 )
    {
        return e_decode;
    }
    /* Invalid option */
    else
    {
        return e_unsupported;
    }
}
