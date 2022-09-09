////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_props.h ----------
/*!
\file vs_img_props.h
\brief Interface and implementation for the ImgProps class.
\author Gareth Edwards
\note 2017
*/

#pragma once


// ---- system include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{

	// ---------- ImgProps class ----------
	class ImgProps
	{

	public:

		// ---- groups ----
		enum {
			GFX_SHADER  = 1
		};

		// ---- property id----
		enum {
			MODE        = 1,
			BRIGHTNESS  = 2,
			CONTRAST    = 3,
			SATURATION  = 4,
			RED         = 5,
			GREEN       = 6,
			BLUE        = 7,
			INCREMENT   = 8,
		};


	public:

		// ---- con/destructor
			ImgProps();
			virtual ~ImgProps();

		// ---- framework
			INT Setup(vs_system::AppDependency *);
			INT Cleanup();

		// ---- file io
			INT Read(CHAR *);
			INT Write(CHAR *);

		// ---- set
			INT SetGfxShaderMode(UINT);

		// ---- value io
			INT GetProperty(UINT, UINT, UINT, INT *);     // INT facade
			INT GetProperty(UINT, UINT, UINT, FLOAT *);
			INT SetProperty(UINT, UINT, UINT, FLOAT);


	private:

		// private implementation
		class PI_ImgProps; PI_ImgProps *_pi_img_props;


	};

}
