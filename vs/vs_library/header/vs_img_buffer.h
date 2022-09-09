////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_buffer.h ----------
/*!
\file vs_img_buffer.h
\brief Interface for the ImgBuffer class.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_BUFFER
#define VS_SYSTEM_IMG_BUFFER


// ---- include ----
#include "../../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- ImgBuffer class ----------
class ImgBuffer
{

public:

	// ----Constructor/Destructor ----
	ImgBuffer();
	ImgBuffer(unsigned int, unsigned int, unsigned int);
	~ImgBuffer();

	// Housekeeping 
	int CreateBuffer();
	int DestroyBuffer();
	int Initialise(unsigned int, unsigned int, unsigned int);

	// Get buffer info
	unsigned char* GetBuffer();
	unsigned int   GetBytes();
	unsigned int   GetExtent();
	unsigned int   GetHeight();
	unsigned int   GetPitch();
	unsigned int   GetStryde();
	unsigned int   GetStride();
	unsigned int   GetWidth();

	// Get image info
	unsigned int   GetQuality();
	unsigned int   GetStatus();

	// Set
	int SetBuffer(unsigned char *);
	int SetBytes(unsigned int);
	int SetQuality(unsigned int);
	int SetStatus(unsigned int);

	int SetA(int);
	int SetA(int, int);
	int SetA(int, bool, int);

private:

	// Buffer info
	unsigned char* buffer;   // Buffer byte (unsigned char) data
	unsigned int   bytes;    // 3 (RGB) or 4 (RGBA)
	unsigned int   extent;   // Size of image in bytes (height * pitch)
	unsigned int   height;   // Pixel rows	
	unsigned int   pitch;    // Size of row in bytes (width * stride)
	unsigned int   stride;   // Alternative naming for bytes
	unsigned int   width;    // Pixel columns

	// Image info
	unsigned int   quality;  // Compression (0 to 100)
	unsigned int   status;   // Status, e.g. non zero == updated


}; // ImgBuffer class


} // vs_system namespace


#endif // VS_SYSTEM_IMG_BUFFER


////////////////////////////////////////////////////////////////////////////////
