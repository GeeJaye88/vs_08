////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_jpeglib.cpp ----------
/*!
\file vs_img_jpeglib.cpp
\brief Implementation of the ImgJpegLib class
\author Gareth Edwards
\note jpegLIB tainted
*/

// ---- include ----
#include "../header/vs_img_jpeglib.h"


// ---- namespace ----
using namespace std;


// ---- include ----
#include <stdio.h>
#include <csetjmp>
#include <cstdio>
#include <cstdlib>
#include <sstream>


// ---- include ----
#include <jpeglib.h>
#include <jerror.h>


////////////////////////////////////////////////////////////////////////////////


//#define OVERIDE_DEFAULT_JPEGLIB_ERROR


#ifdef OVERIDE_DEFAULT_JPEGLIB_ERROR

// ---------- Define an error manager struct ----------
struct jpegErrorManager
{
    // "public" fields
    struct jpeg_error_mgr pub;

    // For return to caller
    jmp_buf setjmp_buffer;
}; 


// ---------- Define an error handling routine ----------
char jpegLastErrorMsg[JMSG_LENGTH_MAX];
void jpegErrorExit (j_common_ptr cinfo)
{
    // cinfo->err actually points to a jpegErrorManager struct
    jpegErrorManager* myerr = (jpegErrorManager*) cinfo->err;
    // note : *(cinfo->err) is now equivalent to myerr->pub

    // output_message is a method to print an error message
    // (* (cinfo->err->output_message) ) (cinfo);    

    // Create the message
    ( *(cinfo->err->format_message) ) (cinfo, jpegLastErrorMsg);

    // Jump to the setjmp point
    longjmp(myerr->setjmp_buffer, 1);

}

# endif


////////////////////////////////////////////////////////////////////////////////


// ---------- GetHeaderInfo ----------
int vs_system::ImgJpegLib::GetHeaderInfo(ImgBuffer *image, char *filename)
{
	// IF no image THEN return error
	if ( !image ) return VS_ERROR;

	// IF file not opened THEN return error
	FILE* file = fopen(filename, "rb");
	if ( !file ) return VS_ERROR;

	// Initialize the JPEG decompression object
	struct jpeg_decompress_struct cinfo;

#	ifdef OVERIDE_DEFAULT_JPEGLIB_ERROR

	// register it using jpeg_std_errorJPEG error handling
	jpegErrorManager jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = jpegErrorExit;

    // Establish the setjmp return context for my_error_exit to use
    if (setjmp(jerr.setjmp_buffer))
	{
        // Error while decompressing JPEG file - see jpegLastErrorMsg
        jpeg_destroy_decompress(&cinfo);
    }

#	else

	// Set up the normal JPEG error handling
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr); 

#	endif

	// Initialize the JPEG decompression object
	jpeg_create_decompress(&cinfo);

	// Specify data source
	jpeg_stdio_src(&cinfo, file);

	// Read the JPEG header, the TRUE means we reject tables-only JPEG file
	jpeg_read_header(&cinfo, TRUE);

	// Initialise image
	image->Initialise(cinfo.image_width,cinfo.image_height,cinfo.num_components);

	// Release JPEG decompression object
	jpeg_destroy_decompress(&cinfo);

	// Close the input file
	fclose(file);

	// Done
	return VS_OK;
};


// ---------- Read ----------
int vs_system::ImgJpegLib::Read(ImgBuffer *image, char *filename)
{
	// IF no image THEN return error
	if ( !image ) return VS_ERROR;

	// IF file not opened THEN return error
	FILE* file = fopen(filename, "rb");
	if ( !file ) return VS_ERROR;

	// Initialize the JPEG decompression object
	struct jpeg_decompress_struct cinfo;

//#define OVERIDE_JPEG_LIB_ERROR

#	ifdef OVERIDE_JPEG_LIB_ERROR

	// Set up the overide JPEG error handling
	jpegErrorManager jerr;
    cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = jpegErrorExit;

    // Establish the setjmp return context for my_error_exit to use
    if (setjmp(jerr.setjmp_buffer))
	{
        // Error while decompressing JPEG file - see jpegLastErrorMsg
        jpeg_destroy_decompress(&cinfo);
    }

#	else

	// Set up the normal JPEG error handling
	jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr); 

#	endif

	// Initialize the JPEG decompression object
	jpeg_create_decompress(&cinfo);

	// Specify data source
	jpeg_stdio_src(&cinfo, file);

	// Read the JPEG header, the TRUE means we reject tables-only JPEG file
	jpeg_read_header(&cinfo, TRUE);

	// Start the decompression
	jpeg_start_decompress(&cinfo);

	// IF buffer AND same THEN continue
	int image_ok = 1;
	if (image->GetBuffer() != NULL &&
		image->GetWidth()  == cinfo.output_width &&
		image->GetHeight() == cinfo.output_height &&
		image->GetBytes()  == cinfo.num_components)
	{

		// Read the decompression results
		JSAMPROW rowptr[1];
		unsigned char *buffer = image->GetBuffer();
		while ( cinfo.output_scanline < cinfo.output_height ) 
		{
			rowptr[0] = buffer + 3 * cinfo.output_width * cinfo.output_scanline; 
			JDIMENSION D = jpeg_read_scanlines(&cinfo, rowptr, 1);
		}

		// Finish decompressing
		jpeg_finish_decompress(&cinfo);
	}

	// ELSE error
	else image_ok = 0;

	// Release JPEG decompression object
	jpeg_destroy_decompress(&cinfo);

	// Close the input file
	fclose(file);

	// Done
	return image_ok;
};


// ---------- WriteToMemory ----------
int vs_system::ImgJpegLib::WriteToMemory(ImgBuffer *image, unsigned char * out_buffer, unsigned long * length)
{
	// IF no image THEN return error
	if (!image) return VS_ERROR;

	// IF no image THEN return error
	if (image->GetBuffer() == NULL) return VS_ERROR;
	if (image->GetWidth()  < 1) return VS_ERROR;
	if (image->GetHeight() < 1) return VS_ERROR;
	if (image->GetBytes() != 3) return VS_ERROR;

	// IF open test file THEN create bars and write
	if ( !out_buffer ) return VS_ERROR;

	// Struct contains the JPEG compression parameters
	struct jpeg_compress_struct cinfo;

	// Struct represents a JPEG error handler
	struct jpeg_error_mgr jerr;

	// Set up the error handler
	cinfo.err = jpeg_std_error( &jerr );

	// Initialize the JPEG compression object
	jpeg_create_compress(&cinfo);

	// Use the library-supplied code to send compressed data to a memory stream
	jpeg_mem_dest(&cinfo, &out_buffer, length);

	// Supply a description of the input image
	cinfo.image_width		= image->GetWidth();
	cinfo.image_height		= image->GetHeight();
	cinfo.input_components	= image->GetBytes();
	cinfo.in_color_space	= JCS_RGB;

	// Use the library's routine to set default compression parameters
	jpeg_set_defaults( &cinfo );
	
	// Set info quality ( TRUE == limit to baseline-JPEG values )
	jpeg_set_quality(&cinfo, image->GetQuality(), FALSE);

	// Start Compression - TRUE ensures that we will write a complete interchange-JPEG file
	jpeg_start_compress( &cinfo, TRUE );
	
	// Write the compression results
	JSAMPROW row_pointer[1];
	unsigned char *in_buffer = image->GetBuffer();
	int row_stride = image->GetWidth() * image->GetBytes();

	while (cinfo.next_scanline < cinfo.image_height) 
	{
		row_pointer[0] = &in_buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// Finish compression
	jpeg_finish_compress( &cinfo );

	
	// Release JPEG compression object
	jpeg_destroy_compress( &cinfo );

	// OK
	return VS_OK;
};


// ---------- WriteToDisk ----------
int vs_system::ImgJpegLib::WriteToDisk(ImgBuffer *image, char *filename)
{
	// IF no image THEN return error
	if ( !image ) return VS_ERROR;

	// IF no image THEN return error
	if ( image->GetBuffer() == NULL ) return VS_ERROR;
	if ( image->GetWidth()  < 1 ) return VS_ERROR;
	if ( image->GetHeight() < 1 ) return VS_ERROR;
	if ( image->GetBytes() != 3 ) return VS_ERROR;

	// IF open test file THEN write
	FILE* hFile=fopen(filename, "wb");
	if ( !hFile ) return VS_ERROR;

	// Struct contains the JPEG compression parameters
	struct jpeg_compress_struct cinfo;

	// Struct represents a JPEG error handler
	struct jpeg_error_mgr jerr;

	// Set up the error handler
	cinfo.err = jpeg_std_error(&jerr);

	// Initialize the JPEG compression object
	jpeg_create_compress(&cinfo);

	// Use the library-supplied code to send compressed data to a stdio stream
	jpeg_stdio_dest(&cinfo, hFile);

	// Supply a description of the input image
	cinfo.image_width       = image->GetWidth();
	cinfo.image_height		= image->GetHeight();
	cinfo.input_components	= image->GetBytes();
	cinfo.in_color_space	= JCS_RGB;

	// Use the library's routine to set default compression parameters
	jpeg_set_defaults(&cinfo);

	// Set info quality ( TRUE == limit to baseline-JPEG values )
	jpeg_set_quality(&cinfo, image->GetQuality(), FALSE);

	// Start Compression - TRUE ensures that we will write a complete interchange-JPEG file
	jpeg_start_compress(&cinfo, TRUE);

	// Write the compression results
	JSAMPROW row_pointer[1];
	unsigned char *buffer = image->GetBuffer();
	int row_stride = image->GetWidth() * image->GetBytes();
	while ( cinfo.next_scanline < cinfo.image_height )
	{
		row_pointer[0] = &buffer[cinfo.next_scanline * row_stride];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	// Finish compression
	jpeg_finish_compress(&cinfo);

	// Close File
	fclose(hFile);

	// Release JPEG compression object
	jpeg_destroy_compress(&cinfo);

	// OK
	return VS_OK;
};