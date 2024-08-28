#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"
#include  "decode.h"

int main(int argc, char * argv[])
{ // validation to check operation i.e encoding or decoding
    int ret = check_operation_type(argv);
    if(ret == e_encode)
    {
	printf("Selected encoding\n");
	EncodeInfo encInfo;
	//read and validate encode arguments from argv *
	if(read_and_validate_encode_args(argv,&encInfo)==e_success)
	{
	    printf("read and validate aruments function execution success\n");
	}
	else
	{
	    printf(" read and arguments function execution failed\n");
	}
	// start the encoding process
	if(do_encoding(&encInfo) == e_success)
	{
	    printf("---------------- Encoding is successfully completed ----------------\n");
	}
	else
	{
	    printf("Encoding is Failure\n");
	}

    }	
    else if(ret == e_decode)
    {
	printf("Selected decoding\n");
	 DecodeInfo decInfo;

	 // finding length of magic_string
          decInfo.magic_string_len = strlen(MAGIC_STRING);
	 //read and validate decode arguments from argv

	if(read_and_validate_decode_args(argv,&decInfo)==d_success)
	{
	    printf("read and validate decode aruments from argv : Success\n");
	}
	else
	{
	    printf("read and validate decode arugument from argv : failed\n");
	}
         // do decoding
	   if (do_decoding(&decInfo) == d_success)
	   {
	   printf(" ---------------Decoding is Completed successfully----------------\n");
	   }
	   else
	   {
	   printf(" ---------------Decoding is failed--------------------\n");
	   }

	 
    }
    else
    {
	printf(" Error : please pass -e or -d");
    }
// ----------------------decoding--------------------------------------------------------------
/*
    DecodeInfo decInfo;
    //uint img_size;
    decInfo.oup_image_fname = "stego_img.bmp";

    // calling function to open the encoded output file
    if (open_encoded_output_file(&decInfo) == d_failure)
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
    printf(" Enter Magic String :");
    scanf("%[^\n]",decInfo.user_magic_string);
    if(decode_magic_string(&encInfo,&decInfo)== d_success)
    {
	printf("Decoding magic _string : Success\n");
    }
    else
    {
	printf("Decoding magic string : Failed\nPlease try again");
    }

    // calling function to decode secret file extn size 

    if(decode_secret_file_extn_size(&decInfo)==d_success)
    {
	printf("Decoding secret file extension size uccess\n");
    }
    else
    {
	printf("Decoding secret file extension size : Failed\n");
    } 
    if(decode_secret_file_extn(decInfo.extn_size,&decInfo)==d_success)
    {
	//intf("extension of secret file :%s \n",encInfo.extn_secret_file);
	printf("Decoding secret file extension : Success\n");
    }
    else
    {
	printf("Decoding secret file extension : Failed\n");
    }

    // function to dcode secret_file_size
    if(encode_secret_file_size(&decInfo)==d_success)
    {
	printf("Decoding secret file size : Success\n");
    }
    else
    {
	printf("Decoding secret file size : Failed\n");
    }
// function to decode secrete_file _data
    if(encode_secret_file_data(decInfo.file_size,&decInfo)==e_success)
    {
	printf("Decoding secret file data : Success\n");
    }
    else
    {
	printf("Decoding secret file data : Failed\n");
    }
    */

    //------------------------------------encoding --------------------------------------------------
  /*

    // Fill with sample filenames
    //  encInfo.src_image_fname = "beautiful.bmp";
    //  encInfo.secret_fname = "secret.txt";

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    printf("ERROR: %s function failed\n", "open_files" );
    return 1;
    }
    else
    {
    printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

    //copiny bmp header to output bmp file 
    if( copy_bmp_header(encInfo.fptr_src_image,encInfo.fptr_stego_image)==e_success)
    {
    printf("Success\n");

    //calling encode magic string function
    if(encode_magic_string(MAGIC_STRING,&encInfo)==e_success)
    {
    printf("success\n");
    }
    else
    {
    printf("Failure\n");
    }
    //taking extension from secert file name
    char *dot_pos = strrchr(encInfo.secret_fname,'.');
    strcpy(encInfo.extn_secret_file, dot_pos);

    int len=strlen(dot_pos);

    if(encode_secret_file_extn_size(len,&encInfo)==e_success)
    {
    printf("success\n");
    }
    else
    {
    printf("Failure\n");
    }

    if(encode_secret_file_extn(dot_pos,&encInfo)==e_success)
    {
    //intf("extension of secret file :%s \n",encInfo.extn_secret_file);
    printf("Success\n");
    }
    else
    {
    printf("Failure\n");
    }

    // getting secret file size
    encInfo.size_secret_file = get_file_size(encInfo.fptr_secret);

    // function to encode secret_file_size
    if(encode_secret_file_size(encInfo.size_secret_file,&encInfo)==e_success)
    {
    printf("Success\n");
    }
    else
    {
    printf("Failure\n");
}

//getting secret file data and encoding secret file data

if(encode_secret_file_data(&encInfo)==e_success)
{
    printf("Success\n");
}
else
{
    printf("Failure\n");
}

if(copy_remaining_img_data(encInfo.fptr_src_image,encInfo.fptr_stego_image)==e_success)
{
    printf("copying remaining data Success\n");
}
else
{
    printf("Failure\n");
}

}
else
{
    printf("Failure\n");
}*/
return 0;
}
