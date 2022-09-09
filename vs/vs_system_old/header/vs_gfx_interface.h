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


// ---- vs system include
#include "../header/vs_inc_defs.h"
#include "../header/vs_elm_context.h"
#include "../header/vs_elm_lam.h"
#include "../header/vs_pan_context.h"
#include "../header/vs_sys_context.h"
#include "../header/vs_sys_input.h"
#include "../header/vs_sys_output.h"


// ---- vs library include
#include "../../vs_library/header/vs_img_buffer.h"
#include "../../vs_library/header/vs_img_jpeglib.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- GfxInterface class----------
	/*!
	\brief GfxInterface.
	\author Gareth Edwards
	*/
	class GfxInterface
	{

	public:

		// ---- cdtor
			GfxInterface();
			~GfxInterface();


		// ---- atlas
			INT  AddPageToAtlas();                                         // ADDS AN INSTANCE
			INT  AddPageToAtlasFromTgaFileInMemory(UINT, BYTE*);           // OK
			INT  AddPageToAtlasFromImgBuffer(UINT, ImgBuffer *, BOOL);     //          
			INT  AddPageToAtlasFromJpegFile(UINT, CHAR *);                 // NOT USED

			INT  GetAtlasPageTextureDimensions(UINT, UINT *, UINT *);
			INT  GetAtlasPageHandle(UINT);
			UINT GetAtlasPageLogicalId();

		// ---- font - vs_gfx_interface_fonts.hpp
			INT  AddFont(std::string &, std::string &, UINT, UINT);
			INT  SetFont(UINT, std::string &, std::string &, UINT, UINT);


		// ---- set lam structs - vs_gfx_interface_lams.hpp
			INT SetAmbient(VsAmbient *);
			INT SetFog(VsFog *);
			INT SetLight(UINT, VsLight *);
			INT SetMaterial(UINT, VsMaterial *);
			INT SetViewMatrix(UINT, VsMatrix *);
			INT SetViewMatrix(UINT, D3DXMATRIX *);


		// ---- set lam scene - vs_gfx_interface_lams.hpp
			INT SetAmbient(FLOAT, FLOAT, FLOAT);
			INT SetFog();
			INT SetFogEnabled(BOOL);
			INT SetLighting(BOOL);
			INT SetLightEnabled(UINT, BOOL);
			INT SetMaterial(UINT);
			INT SetMaterialEnabled(BOOL);
			INT SetViewMatrix(UINT);


		// ---- page - vs_gfx_interface_pages.hpp
			INT     PageSetup(UINT, std::string &, vs_system::ImgBuffer *);
			UCHAR * PageLock(INT);
			INT     PageUnlock(INT);


		// ---- gfx shaders - vs_gfx_interface_shaders.hpp
			INT GetGfxShaderParam(UINT, FLOAT *);
			INT SetGfxShaderParam(UINT, FLOAT *);


		// ---- vs_gfx_interface_get_and_set.hpp


		// ---- get buffer dimensions
			UINT GetDeviceStatus();
			UINT GetBackBufferWidth();
			UINT GetBackBufferHeight();


		// ---- get
			INT  GetClientAdjustedViewrect(VsFloatRectangle *);
			INT  GetClientAdjustedViewport(VsFloatRectangle *);
			INT  GetElementRenderFlag(UINT, BOOL *);
			INT  GetElementRenderMode(UINT, UINT *);
			INT  GetContextOverPanel(PanelContext **);
			FLOAT *GetGfxFloatBuffer();


		// ---- get element
			INT  GetElementContext(INT, ElmContext **);
			INT  GetElementContexts(INT, INT *, ElmContext **);
			UINT GetElementGroupId(INT);
			UINT GetElementId(INT);


		// ---- set colour
			INT  SetBackground(VsColour *);
			INT  SetBackgroundColor(BOOL, VsColour *, VsColour *);
			INT  SetTextureBorderColor(BOOL, VsColour *, VsColour *);


		// ---- set
			INT  SetAAQ(UINT);
			INT  SetBorder(VsFloatRectangle *);
			INT  SetCallbackPickPanel(INT(*callback)(INT, INT), INT);
			INT  SetCallbackPickElement(INT(*callback)(INT, INT), INT);
			INT  SetClientCursorPosition(POINT *);
			INT  SetGlobalConfigureBitmask(BYTE, std::string);
			INT  SetElementConfigureBitmask(INT, BYTE);
			INT  SetConfigure(BOOL, INT);
			INT  SetConfigure(INT);
			INT  SetBorderOver(VsFloatRectangle *);
			INT  SetFPS(UINT);
			INT  SetMargin(VsFloatRectangle *);
			INT  SetElementFlag(UINT, BOOL);
			INT  SetElementSwitch(UINT, UINT, BOOL);
			INT  SetGfxFloatBuffer(FLOAT *, INT);
			INT  SetSysContext(SysContext *);
			INT  SetSysInput(SysInput *); 
			INT  SetSysOutput(SysOutput *);
			INT  SetViewport(VsFloatRectangle *);
			INT  SetViewportMarkers(UINT, UINT, UINT);
			INT  SetViewrect(VsFloatRectangle *);


		// ---- create - display list and elements
			INT  AddDisplayList(CHAR *);
			INT  AddDisplayList(CHAR *, UINT, UINT);
			INT  AddElement(INT, CHAR *, UINT, UINT, INT(*callback)(ElmContext *));
			INT  AddElement(INT, CHAR *, UINT, UINT, UINT, INT(*callback)(ElmContext *));
			
			
		// ---- set - display list and elements
			INT  SetElementConfigured(INT, BOOL, UINT);
			INT  SetElementParameters(INT, UINT, FLOAT *);
			INT  SetDisplayListActiveState();


		// ---- get - display list
			INT  GetHandleDisplayList(CHAR *);


		// ---- get - display list
			INT  SetHandleDisplayList(CHAR *, INT);


		// ---- navigate - display list
			INT  GetChildCount(INT);
			INT  GetFirst(INT);
			INT  GetNext(INT);


		// ---- vs_gfx_interface.cpp
			INT CreateScreenSpacePickRay();
			INT CreateWorldSpacePickRay();


		// ---- render
			INT  RenderBegin();
			INT  RenderViewrectX(PanelContext *, BOOL);
			INT  RenderViewrect(PanelContext *, BOOL);
			INT  RenderViewport(INT, PanelContext *);
			INT  RenderSetupForViewport(PanelContext *);
			INT  RenderEnd();


		// ---- render to image
			INT  RenderToImageBegin();
			INT  RenderToImageViewrect(PanelContext *);
			INT  RenderToImageViewport(INT, PanelContext *);
			INT  RenderToImageEnd();
			INT  RenderToImageBufferEnd(ImgBuffer *);


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