////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_buffer.cpp ----------
/*!
\file vs_img_buffer.cpp
\brief Implementation of the ImgBuffer class
\author Gareth Edwards
*/

/*
\code

// ---- create an image buffer...
	ImgBuffer *img_buffer;
	img_buffer = new ImgBuffer(3684, 3684, 3);
	img_buffer->SetQuality(60);
	img_buffer->CreateBuffer();

// ---- use ImgJpegLib to read or write
	ImgJpegLib img_jpegLib;
	img_jpegLib.Read(img_buffer, "old_image.jpg");
	img_jpegLib.Write(img_buffer, "new_image.jpg");

// ---- use ImgProcess to fill with panoramic bars, etc.
	ImgProcess img_process;
	img_process.PanoBars(img_buffer, 0.1f, 0.95f);

// ---- cleanup
	img_buffer->DestroyBuffer();

\endcode

*/

// ---- include ----
#include "../header/vs_img_buffer.h"
#include <math.h>


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////

#define VS_DEFAULT_JPEG_COMPRESSION 80

// ---------- constructor ----------
ImgBuffer::ImgBuffer()
{
	buffer  = NULL;
	width   = height = 0;
	bytes   = stride = 0;
	extent  = 0;
	pitch   = 0;
	quality = VS_DEFAULT_JPEG_COMPRESSION;
	status  = 0;
};


// ---------- Initialise ----------
ImgBuffer::ImgBuffer(unsigned int w, unsigned int h, unsigned int b)
{
	width   = w;
	height  = h;
	bytes   = b < 3 ? 3 : b > 4 ? 4 : b;
	stride  = bytes;
	pitch   = width * bytes;
	extent  = height * pitch;
	quality = VS_DEFAULT_JPEG_COMPRESSION;
	status  = 0;
	buffer  = NULL;
};


// ---------- destructor ----------
ImgBuffer::~ImgBuffer()
{
	if ( buffer != NULL ) return; // BUG - don't get this at all!
	delete [] buffer;
	buffer = NULL;
	status = 0;
};


// ---------- CreateBuffer ----------
int ImgBuffer::CreateBuffer()
{
	// TBD handle exception
	if ( buffer != NULL ) return VS_ERROR;
	if ( width == 0 || height == 0 || bytes == 0 ) return VS_ERROR;
	stride = bytes;
	pitch  = width * bytes;
	extent = height * pitch;
	try
	{
		buffer = new unsigned char[extent];
		if ( buffer == NULL)
			throw (buffer);
	}
	catch (unsigned char b)
	{
		if ( b == NULL )
			return VS_ERROR;
	}
	return VS_OK;
};


// ---------- DestroyBuffer ----------
int ImgBuffer::DestroyBuffer()
{
	if ( buffer == NULL ) return VS_ERROR;
	delete [] buffer;
	buffer = NULL;
	return VS_OK;
};


// ---------- Initialise ----------
int ImgBuffer::Initialise (unsigned int w, unsigned int h, unsigned int b)
{
	width   = w;
	height  = h;
	bytes   = b < 3 ? 3 : b > 4 ? 4 : b;
	stride  = bytes;
	pitch   = width * bytes;
	extent  = height * pitch;
	quality = VS_DEFAULT_JPEG_COMPRESSION;
	status  = 0;
	return VS_OK;
};

// ---------- Get buffer info ----------
unsigned char* ImgBuffer::GetBuffer()  {  return buffer;  }
unsigned int   ImgBuffer::GetBytes()   {  return bytes;   }
unsigned int   ImgBuffer::GetExtent()  {  return extent;  }
unsigned int   ImgBuffer::GetHeight()  {  return height;  }
unsigned int   ImgBuffer::GetPitch()   {  return pitch;   }
unsigned int   ImgBuffer::GetStride()  {  return stride;  }
unsigned int   ImgBuffer::GetStryde()  {  return pitch;   }
unsigned int   ImgBuffer::GetWidth()   {  return width;   }


// ---------- Get image info ----------
unsigned int   ImgBuffer::GetQuality() {  return quality; }
unsigned int   ImgBuffer::GetStatus()  {  return status;  }


// ---------- Set ----------
INT ImgBuffer::SetBuffer  (unsigned char *external) { buffer = external; return VS_OK; }
INT ImgBuffer::SetBytes   (unsigned int b) { bytes = b; return VS_OK; }
int ImgBuffer::SetQuality (unsigned int q) { quality = q; return VS_OK; }
int ImgBuffer::SetStatus  (unsigned int s) { status = s;  return VS_OK; }


////////////////////////////////////////////////////////////////////////////////

