////////////////////////////////////////////////////////////////////////////////

// ---------- r2_img_col_process.h ----------
/*!
\file r2_img_col_process.h
\brief
\author Gareth Edwards
*/


#pragma once

// ---- include
#include <windows.h>
#include "../header/vs_img_col_process.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ArchTime class ----------

class ImgColProcess::PI_ImgColProcess
{

public:

	// ---- c/d tor
		PI_ImgColProcess()
		{
			;
		};
		~PI_ImgColProcess()
		{
			;
		};


	// ---- methods
		INT Initialise();
		INT CopyRGBtoRGBA(ImgColProcessBuffer *);


	// ---- properties
		FLOAT _brightness = 1;
		FLOAT _contrast   = 1;
		FLOAT _gamma      = 1;
		FLOAT _saturation = 1;
		BYTE  _colour[256][3];


};


// ---------- Initialise ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgColProcess::PI_ImgColProcess::Initialise()
{
	BYTE r = 0;
	for (UINT i = 0; i < 256; i++)
	{
		r = (BYTE)( (FLOAT)i);
		_colour[i][0] = r > 255 ? 255 : r;
	}

	return VS_OK;
}


// ---------- Colour ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgColProcess::PI_ImgColProcess::CopyRGBtoRGBA(ImgColProcessBuffer *icpb)
{

	// ---- return error ?
		if (icpb->src_bpp != 3) return VS_ERROR;
		if (icpb->dest_bpp != 4) return VS_ERROR;


	// ---- shortcut
		DWORD  w           = icpb->width;
		DWORD  h           = icpb->height;
		DWORD  src_bpp     = icpb->src_bpp;
		DWORD  dest_bpp    = icpb->dest_bpp;
		BYTE  *src_data    = icpb->src_data;
		BYTE  *dest_data   = icpb->dest_data;


	// ---- process source into destination
		UINT total = h * w;
		for (UINT t = 0; t<total; t++)
		{
			*(dest_data)     = *(src_data + 0);
			*(dest_data + 1) = *(src_data + 1);
			*(dest_data + 2) = *(src_data + 2);
			*(dest_data + 3) = 0;
			dest_data += dest_bpp;
			src_data  += src_bpp;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ImgColProcess::ImgColProcess()
{
	pi_img_col_process = new PI_ImgColProcess();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgColProcess::~ImgColProcess()
{
	if (pi_img_col_process != NULL )
	{
		delete pi_img_col_process;
		pi_img_col_process = NULL;
	}
}


// ---------- Get ----------

FLOAT ImgColProcess::GetBrightness() { return pi_img_col_process->_brightness; }
FLOAT ImgColProcess::GetContrast()   { return pi_img_col_process->_contrast;   }
FLOAT ImgColProcess::GetGamma()      { return pi_img_col_process->_gamma;      }
FLOAT ImgColProcess::GetSaturation() { return pi_img_col_process->_saturation; }


// ---------- Set ----------

INT ImgColProcess::SetBrightness(FLOAT v) { pi_img_col_process->_brightness = v; return VS_OK; }
INT ImgColProcess::SetContrast(FLOAT v)   { pi_img_col_process->_contrast = v;   return VS_OK; }
INT ImgColProcess::SetGamma(FLOAT v)      { pi_img_col_process->_gamma = v;      return VS_OK; }
INT ImgColProcess::SetSaturation(FLOAT v) { pi_img_col_process->_saturation = v; return VS_OK; }


// ---------- CopRGBtoRGBA ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgColProcess::CopyRGBtoRGBA(ImgColProcessBuffer *icpb)
{
	INT result = pi_img_col_process->CopyRGBtoRGBA(icpb);
	return result;
}
