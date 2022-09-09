////////////////////////////////////////////////////////////////////////////////

// ---------- vs_gfx_interface.h ----------
/*!
\file vs_gfx_interface.h
\brief Interface for the Gfx(/raphics)Interface class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include d3d9
#include <d3d9.h>
#include <d3dx9.h>

#include "../header/vs_inc_defs.h"
#include "../header/vs_sys_input.h"
#include "../header/vs_sys_output.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- GfxTest class----------
	/*!
	\brief GfxTest.
	\author Gareth Edwards
	*/
	class GfxTest
	{

	public:

		// ---- cdtor
			GfxTest();
			~GfxTest();


		// ---- atlas
			INT  AddPageToAtlasFromTgaFileInMemory(UINT, unsigned char*);


		// ---- det buffer dimensions
			UINT GetDeviceStatus();
			UINT GetBackBufferWidth();
			UINT GetBackBufferHeight();


		// ---- set colour
			INT  SetBackground(VS_Colour *);


		// ---- set
			INT  SetAAQ(UINT);
			INT  SetFPS(UINT);
			INT  SetSysInput(SysInput *);
			INT  SetSysOutput(SysOutput *);


		// ---- device framework
			INT  Initialise(HWND, BOOL, UINT, UINT);
			INT  SetupGraphics();
			INT  CleanupGraphics();
			INT  ResetDevice(UINT, UINT);


	private:

		// ---- private implementation
			class PI_Props;   PI_Props   *pi_props;
			class PI_Element; PI_Element *pi_display_list;

	};


}