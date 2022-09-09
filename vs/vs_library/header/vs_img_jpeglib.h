////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_jpeglib.h ----------
/*!
\file vs_img_jpeglib.h
\brief Interface for the ImgJpegLib functions.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_JPEGLIB
#define VS_SYSTEM_IMG_JPEGLIB


// ---- include ----
#include "../header/vs_img_buffer.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- ImgJpegLib class ----------
class ImgJpegLib
{

public:

	// ----Constructor/Destructor ----
	ImgJpegLib () {};
	~ImgJpegLib () {};

	// Methods
	int GetHeaderInfo(ImgBuffer*, char *); 
	int Read(ImgBuffer *, char *);
	int WriteToMemory(ImgBuffer *, unsigned char *, unsigned long *);
	int WriteToDisk(ImgBuffer *, char *);

}; // JpegLib class


} // vs_system namespace


#endif // VS_SYSTEM_IMG_JPEGLIB

