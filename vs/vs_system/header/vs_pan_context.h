////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pan_context.h ----------
/*!
\file vs_pan_context.h
\brief Interface for the Gfx(/raphics)Context class.
\author Gareth Edwards
*/

#pragma once


#include "vs_inc_maths.h"

#include "../../vs_library/header/vs_vnode.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- PanelContext class ----------
	class PanelContext
	{

	public:

		// ---- display list stuff
			enum {
				PANEL_DISPLAY_LIST_MAX   = 8,
			};

		// ---- c/dtor
			PanelContext();
			virtual ~PanelContext();


		// ---- configure
			vs_system::VNode *v;
			INT Configure(vs_system::VNode *);


		// ---- default
			VOID DefaultPerspective();
			VOID DefaultPerspective(UINT);
			VOID DefaultOrthogonal();
			VOID DefaultOrthogonal(UINT);
			VOID DefaultRotate();
			VOID DefaultTranslate();


		// ---- get
			INT GetTypeId();

			VsFloatRectangle *GetMargin();
			VsFloatRectangle *GetViewrect();
			VsFloatRectangle *GetBorderRect();
			VsFloatRectangle *GetBorderOver();
			VsFloatRectangle *GetViewport();

			FLOAT *GetPerspective();
			FLOAT *GetOrthogonal();
			FLOAT *GetZNear();
			FLOAT *GetZFar();

			CHAR  *GetView();
			FLOAT *GetRotate();
			FLOAT *GetTranslate();

			INT    GetName(CHAR *);
			INT    GetDisplayListName(UINT, CHAR *);
			INT    GetDisplayListHandle(UINT, INT *);
			INT    GetDisplayListIndex(UINT *);
			INT    GetNumberOfDisplayLists(UINT *);
			INT    GetGroupId(UINT *);
			INT    GetId(UINT *);
			UINT   GetHandle();
			INT    GetVersion(UINT *);

			BOOL   GetBackground();
			BOOL   GetBorder();
			BOOL   GetEchoOver();
			BOOL   GetUseViewportColor();
			DWORD  GetViewportD3dColor();
			DWORD  GetViewportOverD3dColor();
			INT    GetSelected(BOOL *);
			INT    GetDisplayStatus(BOOL *);
			INT    GetInteractionStatus(BOOL *);


		// ---- set display list stuff
			INT    SetDisplayListHandle(UINT, INT);
			INT    SetDisplayListState(UINT, INT);
			INT    SetDisplayListIndex(UINT);


		// ---- set
			INT    SetGroupId(UINT);
			INT    SetHandle(UINT);
			INT    SetId(UINT);
			INT    SetVersion(UINT);
			INT    SetName(CHAR *);
			INT    SetBackground(BOOL);
			INT    SetBorder(BOOL);
			INT    SetEchoOver(BOOL);
			INT    SetSelected(BOOL);
			INT    SetDisplayStatus(BOOL);
			INT    SetInteractionStatus(BOOL);


		// ---- rti get
			INT    GetRenderToImageDimensions(UINT *, UINT *);
			BOOL   GetRenderToImageBegin();
			BOOL   GetRenderToImageEnd();
			INT    GetRenderToImageFlag(BOOL *);
			INT    GetRenderToImageId();
			BOOL   GetRenderToImageStatus();


		// ---- rti set
			INT    SetRenderToImageDimensions(UINT, UINT);
			INT    SetRenderToImageBegin(BOOL);
			INT    SetRenderToImageEnd(BOOL);
			INT    SetRenderToImageFlag(BOOL);
			INT    SetRenderToImageId(INT);
			INT    SetRenderToImageStatus(BOOL);


	private:

		// ---- private implementation
			class PI_PanelContext; PI_PanelContext *pi_panel_context;

	};


}