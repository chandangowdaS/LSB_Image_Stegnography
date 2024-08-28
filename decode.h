#ifndef DECODE_H
#define DECODE_H

#include "types.h"//contains user defined datatypes




/*structure to store information needed for 
* decoding duplicate output file to get secret file data
*/

typedef struct _DecodeInfo
{
    // duplicate output file
       
    char *dup_image_fname;
   FILE *fptr_dup_image;

  // magic string
   char user_magic_string[50];
   int magic_string_len;
   //secret_file
   int extn_size;
   char *secret_file_extn;
   int secret_file_size;

   char secret_fname[100];
   FILE *fptr_secret_file;

   
} DecodeInfo;

/* Read and validate Deccode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);


/* Perform the dencoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open__encoded_output_file(DecodeInfo *decInfo);

/* decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

// secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* decode secret file extenstion */
Status decode_secret_file_extn(int extn_size, DecodeInfo *decInfo);

/* deccode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* decode secret file data*/
Status decode_secret_file_data(int file_size,DecodeInfo *decInfo);

/* decode lsb_to_byte*/
int decode_lsb_to_byte(FILE *fptr_encoded_file);

/* decode lsb_to_size*/
int decode_lsb_to_size(FILE *fptr_encoded_file);
#endif
