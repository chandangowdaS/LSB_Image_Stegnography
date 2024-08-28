#include<stdio.h>

#include <string.h>
#include "common.h"
#include "types.h"

#include "decode.h"


//opening encoded output file
Status open_encoded_output_file(DecodeInfo *decInfo){
    decInfo->fptr_dup_image = fopen(decInfo->dup_image_fname, "r");
    decInfo->fptr_secret_file = fopen(decInfo->secret_fname,"a+");
    //do error handling
    if(decInfo->fptr_dup_image == NULL){
	    perror("fopen");
	    fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->dup_image_fname);
	    return d_failure;
    }
    return d_success;
}

// read and validata decode arguments from commnd line argument(argv[])
Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo){ 
    if(strstr(argv[2],".bmp")){
	    decInfo->dup_image_fname = argv[2];
    }
    else{
	    return d_failure;
    }

    if(argv[3] == NULL){
	//decInfo->secret_fname ="output";
	    strcpy(decInfo->secret_fname,"output.txt");
    }

    else{
	    strcpy(decInfo->secret_fname,argv[3]);
	//decInfo->secret_fname= argv[3];
    }
    return d_success;
}

//decoding lsb to byte
/*
   int decode_lsb_to_byte(FILE *fptr_encoded_file)
   { int j;
   int i;
   char array[size+1];
   for(i=0;i < size;i++)
   {
   char arr[8];
   char val=0;
   fread(arr,8,1,fptr_encoded_file);// reading from encoded file 

   for (j = 0; j <=7; j++)
   {
   val=(((arr[j] & 1) <<(7-j)) | val);
   }
   array[i]= val;
   }
   array[i]='\0';
//printf("val is %c\n", val);
return val;
}*/
// decode lsb_to_size
/*
   int decode_lsb_to_size(FILE *fptr_encoded_file)
   {
   char arr[32]; 
   int val=0;
   fread(arr,32,1,fptr_encoded_file);// reading from encoded file 
   for (int j = 0; j < 32; j++) {
   val=(((arr[j] & 1)<<(31-j)) | val);
   }
   return val;
   }
 */
// decoding magic string
Status decode_magic_string(DecodeInfo *decInfo){ 
    int i,j;
    char magic_string[decInfo->magic_string_len+1];
    rewind(decInfo->fptr_dup_image);
    fseek(decInfo->fptr_dup_image, 54, SEEK_SET);

    for(i=0;i < decInfo->magic_string_len;i++){
        char arr[8];
	    char val=0;
	    fread(arr,8,1,decInfo->fptr_dup_image);// reading from encoded file 

	for (j = 0; j <=7; j++)
	{
	    val=(((arr[j] & 1) <<(7-j)) | val);
	}
	magic_string[i]= val;
	//magic_string[i]=decode_lsb_to_byte(decInfo->fptr_dup_image);
    }
    magic_string[i]='\0';
    //printf("magic string:%s",magic_string);

    if(strcmp(magic_string,decInfo->user_magic_string)==0)
    {
	return d_success;
    }
    else
    {
	return d_failure;
    }
}

// decoding secret file extension size
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    char arr[32]; 
    int val=0;
    int j;
    fread(arr,32,1,decInfo->fptr_dup_image);

    for (j = 0; j < 32; j++) {
	val=(((arr[j] & 1)<<(31-j)) | val);
    }
    decInfo->extn_size = val;
//	decInfo->extn_size = decode_lsb_to_size(decInfo->fptr_dup_image);
    return d_success;
}

// decoding secret file extn

Status decode_secret_file_extn(int extn_size,DecodeInfo *decInfo)
{ int i,j;

    char array[extn_size+1];
    for(i=0;i < extn_size;i++)
    {
	char arr[8];
	char val=0;
	fread(arr,8,1,decInfo->fptr_dup_image);// reading from encoded file 

	for (j = 0; j <=7; j++)
	{
	val=(((arr[j] & 1) <<(7-j)) | val);
	}
	array[i]=val;
//	array[i]= (char)decode_lsb_to_byte(decInfo->fptr_dup_image);
    }
    array[i]='\0';

    decInfo->secret_file_extn =array;
    printf("secret file_extn:%s\n",decInfo->secret_file_extn);
    //fwrite(array,decInfo->extn_size +1,1,decInfo->secret_file_extn);
    return d_success;
}

// decoding secret file size
Status decode_secrete_file_size(DecodeInfo *decInfo)
{
    int j;
      char arr[32]; 
      int val=0;
      fread(arr,32,1,decInfo->fptr_dup_image);// reading from encoded file 
      for (j = 0; j <= 31; j++) {
      val=(((arr[j] & 1)<<(31-j)) | val);
      }
    //decInfo->secret_file_size = decode_lsb_to_size(decInfo->fptr_dup_image);
      decInfo->secret_file_size = val;


    return d_success;
}


// decoding secret file data

Status decode_secret_file_data(int file_size,DecodeInfo *decInfo)
{ int i,j;

    char array[file_size+1];
    for(i=0;i < file_size;i++)
    {
	char arr[8];
	char val=0;
	fread(arr,8,1,decInfo->fptr_dup_image);// reading from encoded file 

	for (j = 0; j <=7; j++)
	{
	val=(((arr[j] & 1) <<(7-j)) | val);
	}
	array[i] =val;
	//array[i]= decode_lsb_to_byte(decInfo->fptr_dup_image);
    }
    array[i]='\0';

    fwrite(array,(file_size+1),1,decInfo->fptr_secret_file);

    return d_success;
}


// do decoding 

Status do_decoding(DecodeInfo *decInfo)
{
    // calling function to open the encoded output file
    if (open_encoded_output_file(decInfo) == d_failure)
    {
	printf("ERROR: %s function failed\n", "open_encoded_files" );
	return 1;
    }
    else
    {
	printf("SUCCESS: %s function completed\n", "open_encoded_files" );
    }

    //calling decode magic string function
    // read magic string from user
    printf("Enter Magic String :");
    char strkk[50];
    scanf("%s",strkk);
    strcpy(decInfo->user_magic_string,strkk);
    printf("User Magic String is %s\n",decInfo->user_magic_string);

    if(decode_magic_string(decInfo)== d_success)
    {
	printf("Decoding magic _string : Success\n");
    }
    else
    {
	printf("Decoding magic string : Failed\n");
    }

    // calling function to decode secret file extn size

    if(decode_secret_file_extn_size(decInfo)==d_success)
    {
	printf("Decoding secret file extension size Success\n");
    }
    else
    {
	printf("Decoding secret file extension size : Failed\n");
    }
    
    // calling function to decode secret file extension
    if(decode_secret_file_extn(decInfo->extn_size,decInfo)==d_success)
    {
	//intf("extension of secret file :%s \n",encInfo.extn_secret_file);
	printf("Decoding secret file extension : Success\n");
    }
    else
    {
	printf("Decoding secret file extension : Failed\n");
    }

    // renaming created output secret file with extension
    //printf("secret_fname:%s and secret file extn name:%s\n",decInfo->secret_fname,decInfo->secret_file_extn);
    //strcat( decInfo->secret_fname,decInfo->secret_file_extn);
    printf("output secret file name :%s\n",decInfo->secret_fname);

    // function to decode secret_file_size
    if(decode_secrete_file_size(decInfo)==d_success)
    {
	printf("Decoding secret file size : Success\n");
    }
    else
    {
	printf("Decoding secret file size : Failed\n");
    }
    // function to decode secrete_file _data
    if(decode_secret_file_data(decInfo->secret_file_size,decInfo)==d_success)
    {
	printf("Decoding secret file data : Success\n");
    }
    else
    {
	printf("Decoding secret file data : Failed\n");
    }

    return d_success;
}
