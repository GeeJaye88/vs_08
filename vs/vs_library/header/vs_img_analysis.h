////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_analysis.h ----------
/*!
\file vs_img_analysis.h
\brief Interface for the ImgAnalysis class.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_EXPOSURE
#define VS_SYSTEM_IMG_EXPOSURE


// ---- include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../header/vs_img_buffer.h"


// ---- namespace ----
namespace vs_system
{

struct ImgParametricAoi
{
	FLOAT left   = 0;
	FLOAT top    = 0;
	FLOAT right  = 0;
	FLOAT bottom = 0;
};


class ImgAnalysis
{

public:

	// ---- constructor/destructor ----
	ImgAnalysis();
	~ImgAnalysis();

	INT Exposure(ImgBuffer*);

	INT Aoi(
			UINT,
			UINT,
			ImgParametricAoi *
		);

	// ---- private implmentation ----

	class  PI_ImgAnalysis;
	PI_ImgAnalysis *pi_img_analysis;


}; // ImgAnalysis class


} // vs_system namespace


#endif // VS_SYSTEM_IMG_EXPOSURE


////////////////////////////////////////////////////////////////////////////////
