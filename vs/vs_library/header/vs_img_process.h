////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_process.h ----------
/*!
\file vs_img_process.h
\brief Interface for the ImgProcess class.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_PROCESS
#define VS_SYSTEM_IMG_PROCESS


// ---- include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"
#include "../header/vs_img_buffer.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- ImgProcess class ----------
class ImgProcess
{

public:

	// ----Constructor/Destructor ----
	ImgProcess();
	~ImgProcess();

	// General
	INT Bars(ImgBuffer*);
	INT ResizeLite(ImgBuffer*, ImgBuffer*);
	INT ResizeDown(ImgBuffer*, ImgBuffer*);
	INT Filter(
			ImgBuffer*,
			DOUBLE,
			DOUBLE *,
			UINT,
			ImgBuffer*
		);
	INT Offset(
			ImgBuffer*,
			CHAR,
			UINT,
			ImgBuffer*
		);
	INT Luminance(
			ImgBuffer*,
			FLOAT,      // Gamma
			FLOAT       // Luminance
		);

	// Swap
	INT SwapRB(ImgBuffer*);


	// Pano
	INT PanoBars(ImgBuffer*, FLOAT, FLOAT);

	INT GetPanoDimensions(
			INT, FLOAT,
			FLOAT, FLOAT, FLOAT, FLOAT,
			INT*, INT*
		);

	INT GetPanoHeight(INT, FLOAT, FLOAT);

	INT PanoFromAnnular(
			ImgBuffer *,
			FLOAT, FLOAT, FLOAT, FLOAT,
			UINT,
			ImgBuffer *
		);

	INT PanoLuminance(
			ImgBuffer*,   // Destination image
			FLOAT, FLOAT, // UV Offset
			FLOAT,        // Gamma
			FLOAT,        // Acumulation actor
			FLOAT,        // Min radius
			FLOAT,        // Max radius
			ImgBuffer*    // Source image
		);

	// Private implmentation
	class  PI_ImgProcess; PI_ImgProcess *pi_img_process;

}; // ImgProcess class


} // vs_system namespace


#endif // VS_SYSTEM_IMG_PROCESS


////////////////////////////////////////////////////////////////////////////////
