#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */
// checking operation type like endocing or decoding
OperationType check_operation_type(char *argv[])
{    
    if(strcmp(argv[1],"-e") == 0)
    { 
	return e_encode;
    }   
    else if(strcmp(argv[1],"-d") == 0)
    { 
	return e_decode;
    }   
    else
    {
	return e_unsupported;
    }
}

Status read_and_validate_encode_args(char *argv[],EncodeInfo *encInfo)
{
    if(strstr(argv[2],".bmp"))
    {
	encInfo->src_image_fname=argv[2];
    }
    else
    {
	return e_failure;
    }
    if((strstr(argv[3],".txt")) ||(strstr(argv[3],".c")) || (strstr(argv[3],".sh")))
    {
	encInfo->secret_fname=argv[3];
    }
    else
    {
	return e_failure;
    }
    if(argv[4] != NULL)
    {
	encInfo->stego_image_fname=argv[4];
    }
    else
    {
	encInfo->stego_image_fname = "stego_image.bmp";
    }
    return e_success;
}

// do encoding function

Status do_encoding(EncodeInfo *encInfo){
    // Test open_files
    if (open_files(encInfo) == e_failure){
	    printf("ERROR: %s function failed\n", "open_files" );
	    return 1;
    }
    else{
	    printf("SUCCESS: %s function completed\n", "open_files" );
    }

	// Test get_image_size_for_bmp
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("INFO: Image size = %u\n", encInfo ->image_capacity);
    
    // checking source image size is greater tha encoding data
    if(check_capacity(encInfo)==e_success)
    {
	printf("Source file size is greater than encoding data\n");
    }
    else
    {
	printf("source file size is not enough to encode data\n");
    }

    //calling copy bmp header to output bmp file
    if( copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
    {
	printf("Copying header file : Success\n");

	//calling encode magic string function
	if(encode_magic_string(MAGIC_STRING,encInfo)==e_success)
	{
	    printf("Encoding magic _string : Success\n");
	}
	else
	{
	    printf("Encoding magic string : Failed\n");
	}
	
	//taking extension from secert file name
	char *dot_pos = strrchr(encInfo->secret_fname,'.');
	strcpy(encInfo->extn_secret_file, dot_pos);

	encInfo->size_secret_file_extn = strlen(encInfo->extn_secret_file);

	if(encode_secret_file_extn_size(encInfo->size_secret_file_extn,encInfo)==e_success)
	{
	    printf("Encoding secret file extension size : Success\n");
	}
	else
	{
	    printf("Encoding secret file extension size : Failed\n");
	}

	if(encode_secret_file_extn(dot_pos,encInfo)==e_success)
	{
	    //intf("extension of secret file :%s \n",encInfo.extn_secret_file);
	    printf("Encoding secret file extension : Success\n");
	}
	else
	{
	    printf("Encoding secret file extension : Failed\n");
	}

	// getting secret file size
	encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

	// function to encode secret_file_size
	if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_success)
	{
	    printf("Encoding secret file size : Success\n");
	}
	else
	{
	    printf("Encoding secret file size : Failed\n");
	}

	//getting secret file data and encoding secret file data

	if(encode_secret_file_data(encInfo)==e_success)
	{
	    printf("Encoding secret file data : Success\n");
	}
	else
	{
	    printf("Encoding secret file data : Failed\n");
	}

	if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_success)
	{
	    printf("Copying remaining data : Success\n");
	}
	else
	{
	    printf("Copying remaining data : Failed\n");
	}

    }
    else
    {
	printf("Copying header : Failed\n");
    }   
  return e_success;
}

// checking capacity of source file is greater than encoding data
Status check_capacity(EncodeInfo * encInfo)
{
    if(encInfo->image_capacity > (54 + (encInfo->size_magic_string * 8) + 32 + (encInfo->size_secret_file_extn * 8) + 32 + (8 * encInfo->size_secret_file))){
	    return e_success;
	}
    else{
	    return e_failure;
    }
}

// copying 54bytes of bmp header to output file stego_image
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_stego_image) 
{
    char array[54];//creating array of 54 bytes
    rewind(fptr_src_image);//setting offset to 0 position as fseek already set offset to 18 positon to read width
    fread(array,54,1,fptr_src_image);//reading 54 bytes data from src_image and storing in array
    fwrite(array,54,1,fptr_stego_image);//writing data stroed in array to output file(stego_image)
    return e_success;
}
// encoding byte to lsb
//atus encode_byte_to lsb(

// encoding magic string
Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo)
{   encInfo->size_magic_string = strlen(magic_string);
    int i=0;
    while(magic_string[i] !='\0')
    {
	char arr[8];
	fread(arr,8,1,encInfo->fptr_src_image);
    char *array=encode_byte_to_lsb(magic_string[i],arr);
	/*for(int i=7;i>=0;i--)
	{
	    arr[7-i]=(((*magic_string &(1<<i))>>i) | (arr[7-i] & ~(1)));
	}*/
	fwrite(array,8,1,encInfo->fptr_stego_image);
	i++;
    }
    return e_success;
}
//enocoding secret file extension size
Status encode_secret_file_extn_size(int file_extn_size,EncodeInfo *encInfo)
{
    char arr[32];
    fread(arr,32,1,encInfo->fptr_src_image);

    char* array=encode_size_to_lsb(file_extn_size,arr);
    /*
    for(int i=31;i>=0;i--)
    {
	arr[31-i]=(((file_extn_size &(1<<i))>>i) | (arr[31-i] & ~(1)));
    }*/
    fwrite(array,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//encode secret file extension
Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encInfo)
{   
    int i=0;
    while(file_extn[i]!='\0')
    {
	char arr[8];
	fread(arr,8,1,encInfo->fptr_src_image);
        
        char* array=encode_byte_to_lsb(file_extn[i],arr);
	/*
	for(int i=7;i>=0;i--)
	{
	    arr[7-i]=(((file_extn[i] &(1<<i))>>i) | (arr[7-i] & ~(1)));
	}*/
	fwrite(array,8,1,encInfo->fptr_stego_image);
	i++;
    }
    return e_success;
}
//enocoding secret file size
Status encode_secret_file_size(int size,EncodeInfo *encInfo)
{
    char arr[32];
    fread(arr,32,1,encInfo->fptr_src_image);
    char* array=encode_size_to_lsb(size,arr);
    /*
    for(int i=31;i>=0;i--)
    {
	arr[31-i]=(((file_size &(1<<i))>>i) | (arr[31-i] & ~(1)));
    }*/
    fwrite(array,32,1,encInfo->fptr_stego_image);
    return e_success;
}

//encoding secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    rewind(encInfo->fptr_secret);
    char data_secrete[encInfo->size_secret_file];
    fread(data_secrete,encInfo->size_secret_file,1,encInfo->fptr_secret);

    int i=0;
    while(i<encInfo->size_secret_file)
    {
	char arr[8];
	fread(arr,8,1,encInfo->fptr_src_image);

        char* array=encode_byte_to_lsb(data_secrete[i],arr);
	/*
	for(int i=7;i>=0;i--)
	{
	    arr[7-i]=(((data_secrete[i] &(1<<i))>>i) | (arr[7-i] & ~(1)));
	}*/
	fwrite(array,8,1,encInfo->fptr_stego_image);
	i++;
    }
    return e_success;

}
// copying remaining data from source image file to output file(stego_image)
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_stego_image){ 
    char arr;
    while(fread(&arr,1,1,fptr_src_image)){
	    fwrite(&arr,1,1,fptr_stego_image);
    }
    return e_success;
}

// byte to lsb
char* encode_byte_to_lsb(char data, char *image_buffer){

	for(int i=7;i>=0;i--){
	    image_buffer[7-i]=(((data &(1<<i))>>i) | (image_buffer[7-i] & ~(1)));
	}
	return image_buffer;

}

// size _ to _lsb
char* encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i=31;i>=0;i--)
    {
	image_buffer[31-i]=(((data &(1<<i))>>i) | (image_buffer[31-i] & ~(1)));
    }
    return image_buffer;
}


/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image){
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
// gettinf serecet file size
uint get_file_size(FILE *fptr_secret){
    fseek(fptr_secret,0,SEEK_END);
    return ftell(fptr_secret)-1;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo){
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

    return e_success;
}
