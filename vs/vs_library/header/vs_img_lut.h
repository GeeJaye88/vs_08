////////////////////////////////////////////////////////////////////////////////

// ---------- r2_img_lut.h ----------
/*!
\file r2_img_lut.h
\brief Interface for the ImgLutProcess class.
\author Gareth Edwards
*/

#ifndef VS_IMG_LUT
#define VS_IMG_LUT


// ---- vs system include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_sys_output.h"


// ---------- namespace ----------
namespace vs_system
{


// ---------- ImgLutProcess class ----------

struct ImgLutProcessBuffer
{
	DWORD  lut_mode   = 0;
	DWORD  boost_mode = 0;
	DWORD  width      = 0;
	DWORD  height     = 0;
	DWORD  src_bpp    = 0;
	DWORD  dest_bpp   = 0;
	BYTE  *src_data   = NULL;
	BYTE  *dest_data  = NULL;
};


class ImgLutProcess
{

public:

	enum {
		COLOUR = 0,
		MONO   = 1,
		SEPIA  = 2,
		GREEN  = 3
	};

	// ---- c/dtor ----
	ImgLutProcess();
	virtual ~ImgLutProcess();

	INT PreProcess(ImgLutProcessBuffer*);

private:

	// ---- private implementation ----
	class PI_ImgLutProcess;
	PI_ImgLutProcess *pi_img_pre_process;

};


};


#endif // VS_IMG_LUT

