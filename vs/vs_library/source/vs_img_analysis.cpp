////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_analysis.cpp ----------
/*!
\file vs_img_analysis.cpp
\brief Implementation of the ImgAnalysis class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_img_analysis.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ImgAnalysis class ----------
class ImgAnalysis::PI_ImgAnalysis
{

public:

	// Constructor/destructor
	PI_ImgAnalysis() {};
	~PI_ImgAnalysis() {};


};


////////////////////////////////////////////////////////////////////////////////


// ---- constructor ----
ImgAnalysis::ImgAnalysis()
{
	pi_img_analysis = new PI_ImgAnalysis();
};


// ---- destructor ----
ImgAnalysis::~ImgAnalysis()
{
	if ( pi_img_analysis != NULL ) { delete pi_img_analysis; pi_img_analysis = NULL; }
};


////////////////////////////////////////////////////////////////////////////////


int ImgAnalysis::Exposure(
		ImgBuffer *image
	)
{
	
	// ---- local ----
	UINT column_width = image->GetWidth() / 8;


	// ---- RGB colour struct ----
	typedef struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} RGB;


	// ---- RGB colour bars LUT ----
	RGB BAR_COLOUR[8] =
	{
		{ 255, 255, 255 },  // 100% White
		{ 255, 255,   0 },  // Yellow
		{   0, 255, 255 },  // Cyan
		{   0, 255,   0 },  // Green
		{ 255,   0, 255 },  // Magenta
		{ 255,   0,   0 },  // Red
		{   0,   0, 255 },  // Blue
		{   0,   0,   0 },  // Black
	};


	// ---- FOR each row ----
	unsigned char *ptr = image->GetBuffer();
	UINT bytes    = image->GetBytes();
	UINT width    = image->GetWidth();
	UINT height   = image->GetHeight();
	for ( unsigned int y=0; y<height; y++ )
	{
	
		// ---- FOR each col ----
		for (unsigned int x=0; x<width; x++)
		{
			unsigned idx = x / column_width;
			ptr[0] = BAR_COLOUR[idx].r;
			ptr[1] = BAR_COLOUR[idx].g;
			ptr[2] = BAR_COLOUR[idx].b;
			if (bytes == 4) { ptr[3] = 0; ptr+=4; } else ptr+=3;
		}
		
	}

	// done
    return VS_OK;
};



////////////////////////////////////////////////////////////////////////////////

