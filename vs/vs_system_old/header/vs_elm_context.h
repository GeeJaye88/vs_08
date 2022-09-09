////////////////////////////////////////////////////////////////////////////////

// ---------- vs_elm_context.h ----------
/*!
\file vs_elm_context.h
\brief Interface for the El(e)m(ent)Context class.
\author Gareth Edwards
*/

#pragma once


#include "vs_inc_defs.h"
#include "vs_inc_dems.h"
#include "vs_inc_maths.h"
#include "vs_elm_matrix.h"

#include <string>


// ---- namespace ----
namespace vs_system
{


	// ---------- ElmContext class ----------
	class ElmContext
	{


	public:

		// ---- element behaviours
			enum {
				BEHAVIOUR_NULL      = 0,   // NO behaviour EVER
				BEHAVIOUR_NONE      = 1,   // NO behaviour BUT might be assigned behaviour
				BEHAVIOUR_CLICK     = 2,   // ON and OFF with no repeat CLICK test
				BEHAVIOUR_CLUNK     = 3,   // ON and stay ON until application re-init UNCLUNK
				BEHAVIOUR_RADIO     = 4,   // RADIO all contiguous group elements within a display list
				BEHAVIOUR_SWITCH    = 5,   // ON or OFF
				BEHAVIOUR_UNCLUNK   = 6,   // when set by application re-init to CLICK
				BEHAVIOUR_RADIO_ALL = 7,   // RADIO all group elements within a display list 
			};


		// ---- element sub-behaviours
			enum {
				SUB_BEHAVIOUR_NULL         = 0,   // NO sub-behaviour EVER
				SUB_BEHAVIOUR_NONE         = 1,   // NO sub-behaviour BUT might be assigned sub-behaviour
				SUB_BEHAVIOUR_RADIO_SWITCH = 2,   // overides RADIO & RADIO_ALL "only select if not selected"
			};


		// ---- element flags
			enum {
				ALPHA_BLENDING  = 1, // transparency alpha blending
				COLOUR_VERTEX   = 2, // interpolated colour vertices
				CULL_MODE       = 3, // cull mode, "NONE", "CW", "CCW"
				MULTI_STATE     = 4, // off/on/over/active multi states
				SOLID           = 5, // solid or wireframe rendering
				TEXTURE         = 6, // texture mapping 
				VISIBLE         = 7, // visible or hidden
				FLAG_OVERIDE    = 8, // overide element config callback flag states
			};


		// ---- element properties
			enum {
				PROPERTY_BEHAVIOUR        = 1,
				PROPERTY_BITMASK          = 2,
				PROPERTY_SWITCHED_ON      = 3,
				PROPERTY_SWITCHED_ON_PREV = 4,
				PROPERTY_SOLID            = 5,
				PROPERTY_MULTI_STATE      = 6
			};


		// ---- element alignment
			enum {
				ALIGN_NONE    = 0,
				ALIGN_LEFT    = 1,
				ALIGN_CENTRE  = 2,
				ALIGN_CENTRED_ON_TEXT = 3
			};


		// ---- element line shade modes
			enum {
				LINE_ALPHA_DEFAULT = 0,
				LINE_ALPHA_ON      = 1,
				LINE_ALPHA_OFF     = 2,
			};


		// ---- element proportional dimensions
			enum {
				VIEWPORT        = 1,
				VIEWPORT_WIDTH  = 2,
				VIEWPORT_HEIGHT = 4,
				VIEWPORT_CLIP   = 8,
				VIEWPORT_WIDTH_XLOC  = 16,
				VIEWPORT_HEIGHT_YLOC = 32,
			};


		// ---- button types
			//
			// note: buttons are groups of elements that share the same group
			// and element id, but with differening verion (see below)
			//
			enum {
				TYPE_ICON       = 2,   // 3 versions - used in vs_tint.hpp
				TYPE_TEXT       = 3,   // 1 version  - used in r2_dsp_monitor.hpp && vs_tint.hpp
				TYPE_STAMP      = 4    // 2 versions - used in r2_dsp_monitor.hpp && r2_dsp_stream.hpp
			};


			// ---- state
			enum {
				STATE_INACTIVE  = 0, // like a log !
				STATE_ACTIVE    = 1  // e.g. pickable... 
			};
			
			
			// ---- versions
			enum {
				//VERSION_NONE       = 0,
				VERSION_BACKGROUND = 0,
				VERSION_ICON       = 1,
				VERSION_TEXT       = 2,
				VERSION_OUTLINE    = 3,
				VERSION_PANEL      = 4,
			};


			// ---- x versions
			enum {
				X_VERSION_NONE       = 0,
				X_VERSION_BACKGROUND = 0,
				X_VERSION_ICON       = 1,
				X_VERSION_TEXT       = 2,
				X_VERSION_OUTLINE    = 3,
			};

		// ---- CULL modes
			enum {
				CULL_NONE = 1,
				CULL_CW   = 2,
				CULL_CCW  = 3,
			};


		// ---- element button utility structure
			struct ElmButton
			{
				CHAR  name[64];       // who - name of this element
				CHAR  text[64];       //     - displayed content
				UINT  group_id;       //     - parent logical id
				DWORD id;             //     - logical id
				UINT  type;           // NONE, ICON, TEXT or STAMP
				UINT  behaviour;
				UINT  sub_behaviour;
				UINT  status;
				FLOAT x;              // where - proportional panel ordinates
				FLOAT y;
				FLOAT width;
				FLOAT height;
				UINT  apg;
				UINT  bg_col;         // with - texture col/row indices index
				UINT  bg_row;
				UINT  icon_col;
				UINT  icon_row;
				UINT  dim_col;
				UINT  dim_row;

				FLOAT action_type[VS_MAX_ELM_FLOAT_ACTIONS];
				FLOAT action_index[VS_MAX_ELM_FLOAT_ACTIONS];

			};

	 
	#ifdef VS_GFX_FLAG

		// ---- cdtor
			ElmContext();
			virtual ~ElmContext();


		// ---- configure ----
			INT BeginConfig();
			INT EndConfig();


		// -------- set
			INT SetName(std::string);
			INT SetText(std::string);
			INT SetFontName(std::string);
			INT SetActions(UINT);
			INT SetAlign(UINT);
			INT SetBehaviour(UINT);
			INT SetBitmask(UINT);
			INT SetConfigured(BOOL);
			INT SetAlpha(FLOAT);
			INT SetRGBA(FLOAT, FLOAT, FLOAT, FLOAT);
			INT SetFontAlignment(UINT);
			INT SetFontHeight(UINT);
			INT SetGroupId(UINT);
			INT SetId(UINT);
			INT SetMaterialIndex(UINT);
			INT SetShaderIndex(UINT);
			INT SetViewMatrix(UINT);
			INT SetVersion(UINT);
			INT SetReport(BOOL);
			INT SetRuntime(BOOL);
			INT SetState(UINT);
			INT SetSubBehaviour(UINT);
			INT SetDisplay(BOOL);
			INT SetUseRGBA(BOOL);


		// ---- atlas
			//INT SetAtlasMode(UINT);
			INT SetAtlasPages(UINT);
			INT SetAtlasPage(UINT, UINT);


		// ---- callback
			INT SetIndexBufferCallback(INT(*get_index_buffer)(ElmContext *));
			INT SetVertexBufferCallback(INT(*get_vertex_buffer)(ElmContext *));
			INT IndexBufferCallback(ElmContext*);
			INT VertexBufferCallback(ElmContext*);


		// ---- data
			INT SetFloatParameter(UINT, FLOAT*);
			INT SetGfxFloatParameter(FLOAT *);    // invoked in GfxInterface::RenderViewport
			INT SetGfxFloatBuffer(FLOAT *);       // managed by user...
			INT SetLongParameter(LONG *);
			INT SetLongParameterSize(UINT);


		// ---- render flags
			INT SetAlphaBlending(BOOL);
			INT SetColourVertex(BOOL);
			INT SetFog(BOOL);
			INT SetLit(BOOL);
			INT SetOveride(BOOL);
			INT SetPick2D(BOOL);
			INT SetPick3D(BOOL);
			INT SetPickOnRadioButton(BOOL);
			INT SetShaded(BOOL);
			INT SetSwitchedOn(BOOL);
			INT SetSwitchedOnPreviously(BOOL);
			INT SetSolid(BOOL);
			INT SetTextOutline(BOOL);
			INT SetTextVertical(BOOL);
			INT SetTextured(BOOL);
			INT SetTextureMirror(BOOL);
			INT SetVisible(BOOL);

		// ---- render modes
			INT SetCullMode(UINT);


		// ---- index buffer
			#if VS_INDEX_BUFFER_FORMAT == 32
			INT SetIndexBufferPtr(DWORD *);
			#else
			INT SetIndexBufferPtr(WORD *);
			#endif
			INT SetIndexCount(UINT);


		// ---- matrix
			INT SetUseMatrix(BOOL);
			INT SetMatrix(VsMatrix *);


		// ---- multi-state texturing
			INT SetMultiActive(BOOL, UINT);
			INT SetMultiState(BOOL);
	

		// ---- vertex buffer
			INT SetVertexBufferPtr(FLOAT *);
			INT SetVertexCount(UINT);
			INT SetVertexFormat(UINT);
			INT SetVertexSize(UINT);


		// ---- primitive parameters
			INT SetPrimitiveBytes(UINT) ;
			INT SetPrimitiveCount(UINT);
			INT SetPrimitiveType(VS_PrimitiveType);


		// ---- get
			std::string GetName();
			std::string GetText();
			std::string GetFontName();
			UINT   GetActions();
			UINT   GetAlign();
			UINT   GetAtlasMode();
			UINT   GetBehaviour();
			INT    GetRGBA(FLOAT *, FLOAT *, FLOAT *, FLOAT *);
			DWORD  GetRGBA();
			UINT   GetSetupBitmask();
			UINT   GetFontAlignment();
			UINT   GetFontHeight();
			UINT   GetGroupId();
			UINT   GetId();
			UINT   GetMaterialIndex();
			UINT   GetShaderIndex();
			UINT   GetViewMatrixIndex();
			UINT   GetVersion();
			BOOL   GetRuntime();
			BOOL   GetReport();
			UINT   GetState();
			UINT   GetSubBehaviour();
			BOOL   GetDisplay();
			BOOL   GetUseRGBA();


		// ---- atlas page
			INT GetAtlasPages(UINT *);
			INT GetAtlasPage(UINT, UINT *);


		// ---- data
			UINT   GetFloatParameterSize();       // set in SetFloatParameter
			FLOAT *GetFloatParameter();
			FLOAT *GetGfxFloatParameter();    // invoked in all object methods
			FLOAT *GetGfxFloatBuffer();       // managed by user...

			UINT   GetLongParameterSize();
			LONG  *GetLongParameter();


		// ---- render flags
			BOOL   GetAlphaBlending();
			BOOL   GetColourVertex();
			BOOL   GetFog();
			BOOL   GetLit();
			BOOL   GetOveride();
			BOOL   GetPick2D();
			BOOL   GetPick3D();
			BOOL   GetPickOnRadioButton();
			BOOL   GetShaded();
			BOOL   GetSwitchedOn();
			BOOL   GetSwitchedOnPreviously();
			BOOL   GetSolid();
			BOOL   GetTextOutline();
			BOOL   GetTextVertical();
			BOOL   GetTextured();
			BOOL   GetTextureMirror();
			BOOL   GetVisible();


		// ---- render modes
			UINT   GetCullMode();


		// ---- index buffer
			#if VS_INDEX_BUFFER_FORMAT == 32
			DWORD * GetIndexBufferPtr();
			#else
			WORD * GetIndexBufferPtr();
			#endif
			UINT   GetIndexCount();


		// ---- matrix
			INT    GetUseMatrix(BOOL *);
			INT    GetMatrix(VsMatrix *);


		// ---- multi-state texturing
			BOOL   GetMultiActive();
			UINT   GetMultiNull();
			BOOL   GetMultiState();


		// ---- vertex buffer
			FLOAT *GetVertexBufferPtr();
			UINT   GetVertexCount();
			UINT   GetVertexFormat();
			UINT   GetVertexSize();


		// ---- primitive parameters
			UINT   GetPrimitiveBytes();
			UINT   GetPrimitiveCount();
			VS_PrimitiveType GetPrimitiveType();


	private:

		// ---- private implementation
			class PI_ElmContext;
			PI_ElmContext *pi_elm_context;


	#endif


	};


}