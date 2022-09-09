////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_col_process.h ----------
/*!
\file vs_img_col_process.h
\brief Interface for the ImgColProcess class.
\author Gareth Edwards
*/

#pragma once

// ---- vs system include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_sys_output.h"


// ---- namespace
namespace vs_system
{

	// ---------- ImgColProcessBuffer struct ----------
	struct ImgColProcessBuffer
	{
		DWORD  width      = 0;
		DWORD  height     = 0;
		DWORD  src_bpp    = 0;
		DWORD  dest_bpp   = 0;
		BYTE  *src_data   = NULL;
		BYTE  *dest_data  = NULL;
	};


	// ---------- ImgColProcess class ----------
	class ImgColProcess
	{

		public:

			// ---- c/dtor
				ImgColProcess();
				virtual ~ImgColProcess();

			// ---- methods
				FLOAT GetBrightness();
				FLOAT GetContrast();
				FLOAT GetGamma();
				FLOAT GetSaturation();
				INT SetBrightness(FLOAT);
				INT SetContrast(FLOAT);
				INT SetGamma(FLOAT);
				INT SetSaturation(FLOAT);
				INT CopyRGBtoRGBA(ImgColProcessBuffer*);

		private:

			// ---- private implementation
				class PI_ImgColProcess;
				PI_ImgColProcess *pi_img_col_process;

	};


};
