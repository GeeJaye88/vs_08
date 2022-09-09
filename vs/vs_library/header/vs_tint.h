////////////////////////////////////////////////////////////////////////////////

// ---------- vs_tint.h ----------
/*!
\file vs_tint.h
\brief Interface for the T(abbed) int(erface) classes
\author Gareth Edwards
*/

#pragma once


// ---- library ----
#include "../header/vs_obj_config.h"


// ---- system ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"
#include "../../vs_system/header/vs_gfx_interface.h"


namespace vs_system
{


// ---- requires the following within scope
//      UINT noi;
//      TintItem li[#]
//
#define TINT_SET_START_NOI()                  noi = -1;
#define TINT_ADD_ITEM(name)                   li[++noi].Set(name, gid, noi);
#define TINT_ADD_ITEMI(name, i)               li[++noi].Set(name, gid, i);
#define TINT_ADD_ICON()                       li[++noi].Set("",   gid, noi);
#define TINT_ADD_ICONI(i)                     li[++noi].Set("",   gid, i);
#define TINT_SET_TARC(t, a, r, c)             li[noi].SetType(t); li[noi].SetAtlas(a, r, c);
#define TINT_SET_TARCX(t, a, r, c, x)         li[noi].SetType(t); li[noi].SetAtlas(a, r, c); li[noi].SetXOff(x);;
#define TINT_SET_TARCXL(t, a, r, c, x, l)     li[noi].SetType(t); li[noi].SetAtlas(a, r, c); li[noi].SetXOff(x); li[noi].SetLF(l);
#define TINT_SET_TARCXLI(t, a, r, c, x, l, i) li[noi].SetType(t); li[noi].SetAtlas(a, r, c); li[noi].SetXOff(x); li[noi].SetLF(l); li[noi].SetLFI(i);
#define TINT_SET_TOTAL_NOI()                  noi++;


// ---------- TintItem ----------
class TintItem
{

public:

	// --- cdtor
		TintItem();
		~TintItem() {};


	// ---- set property
		INT SetXOff(FLOAT o);
		INT SetElemId(UINT e_id);
		INT SetColumnLF(UINT cf);
		INT SetGroupId(UINT g_id);
		INT SetIcon(UINT i_id);
		INT SetLF(BOOL lf);
		INT SetLFI(FLOAT li);
		INT SetText(CHAR *c);
		INT SetType(UINT t);
		INT SetStatus(BOOL s);


	// ---- set atlas id, row and column
		INT SetAtlas(UINT, UINT, UINT);             // atlas id, row and column
		INT Set(CHAR *, UINT, UINT, UINT, UINT);    // item name, head and body, group id and id
		INT Set(CHAR *, UINT, UINT);                // item name, group id and id
		INT Set(CHAR *, UINT);                      // item with element id


	// ---- description
		CHAR  text[32];
		UINT  group_id;   // group logical id
		DWORD  elem_id;    // element logical id


	// ---- details
		UINT  type;        // ITEM_, see below... 
		UINT  icon;        // ICON_, see below... OR row * # icon (per row) + col
		UINT  status;      // 0/1
		FLOAT x_off;       // 0 (false) or 1, 2, .... # spaces
	

	// ---- atlas
		UINT  ap_group;    // atlas page group
		UINT  ap_row;      // atlas page row
		UINT  ap_col;      // atlas page col


	// ---- line conditions
		BOOL  column_feed;


	// ---- line conditions
		BOOL  line_feed;   // lf
		FLOAT line_incr;   // lf scalar increment (e.g. SPACE_HEIGHT * line_incr)

};


// ---------- TintCallback ----------
struct TintCallback
{
	INT(*callback_build_list)      (TintItem[]);
	INT(*callback_get_icon_info)   (INT);
	INT(*callback_update_model)    (INT, INT);
	INT(*callback_update_element)  (INT, INT);
	INT(*callback_add_actions)     (INT, INT, FLOAT *);
	INT(*callback_add_text)        (INT, INT, BOOL *, BOOL*, CHAR *);
	INT(*callback_add_value)       (INT, INT, BOOL *, FLOAT *, CHAR *);
};


// ---------- TintElemDsplInfo ----------
struct TintElemDsplInfo
{
	CHAR group_lid = 0;
	CHAR lid       = 0;
	INT  handle    = 0 ;
	vs_system::ElmContext *context = NULL;
};


// ---------- Tint ----------

class Tint
{

public:

	enum
	{
		TAB_ATLAS_PAGE_ID  = 1,
		TAB_ATLAS_PAGE_COL = 2,
		TAB_ATLAS_PAGE_ROW = 3,

		ICON_ATLAS_PAGE_ID = 4,

		TINT_XOFFSET       = 12,
		HEAD_YOFFSET       = 14,
		BODY_YOFFSET       = 15,

		COL_HEIGHT         = 16,
		COL_WIDTH          = 17,

		SPACE_WIDTH        = 4,
		SPACE_HEIGHT       = 5,
		SPACE_MARGIN       = 11,

		TEXT_WIDTH         = 6,
		TEXT_HEIGHT        = 7,
		ICON_WIDTH         = 8,
		ICON_HEIGHT        = 9,
		ITEM_SCALAR        = 10,

		ICONS_PER_ROW      = 8,

		ITEM_NULL          = 0,
		ITEM_TEXT          = 1,
		ITEM_CLUNK         = 2,
		ITEM_CLICK         = 4,
		ITEM_RADIO         = 8,
		ITEM_SWITCH        = 16,
		ITEM_TABS          = 32,
		ITEM_RADIO_LOCAL   = 64,

	};


	// ---- cdtor
		Tint();
		~Tint();


	// ---- init
		INT Initialise(
				vs_system::GfxInterface *obj,
				vs_system::ObjConfig *gfx,
				FLOAT(*get_config_param)(INT)
			);


	// ---- add
		INT   AddHead(CHAR *head_name,UINT  head_lid);
		INT   AddBody(INT handle_ass_obj,CHAR  *body_name,UINT body_id,TintCallback *tint_callback);
		INT   AddEnclose(INT handle_display_list);


	// ---- build
		INT   BuildBody(
					INT          handle_ass_obj,
					CHAR        *body_name,
					UINT         body_id,
					INT          index,
					TintCallback *tint_callback
				);
		INT   BuildDisplayList(
					CHAR        *head_or_body_name,
					INT          handle,
					TintItem     list_of_items[],
					INT          number_of_items,
					BOOL         is_head
				);
		INT   BuildDisplayList(
					INT          handle,
					vs_system::ElmContext::ElmButton button_list[],
					UINT         button_count,
					UINT         element_count,
					BOOL         is_head
				);


	// ---- update
		INT   UpdateStatus(INT   elem_id,BOOL  status);
		INT   UpdateRadio(INT elem_id);
		INT   UpdateAllText();
		INT   UpdateText(INT   group_id_not_used, INT   elem_id);


	// ---- head access
		INT   GetHeadDsplElmCount();
		INT   GetHeadDsplElmHandles(
					INT *elem_handles
				);


	// ---- body access
		INT   GetBodyDsplElmCount(const UINT body_id);
		INT   GetBodyDsplElmHandles(
					const UINT body_id,
					INT *elem_handles
				);
		INT GetBodyDsplElmContext(
					UINT  body_lid,                       // [in]
					INT   num_element_lid,                // [in]
					CHAR  element_lid[],                  // [in]
					INT   element_id_index[],             // [out] 
					vs_system::ElmContext *element_id_context[] // [out] 
				);
		INT GetBodyElemDsplInfo(
					UINT  body_lid,                       // [in] 
					INT   num_element_lid,                // [in] 
					CHAR  element_lid[],                  // [in] 
					CHAR  group_lid[],                    // [in] 
					TintElemDsplInfo dspl_element_info[], // [out] 
					UINT hash_element_lid_to_dspl_index[] // [out] 
				);


	// ---- change display multi-active state (on/off)
		INT ElemDsplTextTidy(
				INT id,
				CHAR *label,
				FLOAT value,
				CHAR *text
			);
		INT ElemDsplSwitch(
				UINT  body_lid,
				UINT  param_lid,
				UINT  elem_lid,
				BOOL *multi_state
			);


	// ---- change display state
		INT ElemDsplInfoSwitch(
					INT  element_dspl_count,
					TintElemDsplInfo element_dspl_info[],
					INT  group_lid,
					INT  element_lid,
					BOOL state
				);
		INT ElemDsplInfoSetText(
				INT   element_dspl_count,
				vs_system::TintElemDsplInfo element_dspl_info[],
				INT   group_lid,
				INT   element_lid,
				CHAR *text
			);


	// ---- get
		INT   GetBodyHandle(const CHAR *body_name);
		INT   GetBodyHandle(const UINT body_id);
		FLOAT GetBodyHeight(const CHAR *body_name);
		INT   GetBodyId(CHAR *body_name);
		INT   GetBodyIndex(const UINT body_id);
		CHAR *GetBodyFontName();
		INT   GetBodyName(const INT body_handle,CHAR *body_name);
		CHAR *GetHeadFontName();
		INT   GetHeadHandle();
		FLOAT GetHeadHeight();
		INT   GetHeadName(CHAR *head_name);
		INT   GetSelectedBodyId();
		INT   GetTabOffset(FLOAT *x,FLOAT *y);


	// ---- select
		INT   SelectBody(const INT body_id);


	// ---- set
		INT   SetCallbackGetBuildInfo(FLOAT(*func)(INT));
		INT   SetColumns(BOOL use, UINT num);
		INT   SetSelectedBody(const UINT id);
		INT   SetBodyFont(CHAR *name);
		INT   SetHeadFont(CHAR *name);
		INT   SetTabOffset(const FLOAT x,const FLOAT y);

		INT   SetDisplayListsConfigure();


private:


	// ---- injected
		vs_system::GfxInterface *_inj_gfx_interface = NULL;
		vs_system::ObjConfig    *_inj_obj_config    = NULL;


	// ---- tab
		FLOAT _x_tab_off = 0, _y_tab_off = 0;


public:

	// ---- max
		static const INT MAX_BODY       = 16;
		static const INT MAX_BODY_ITEM  = 256;
		static const INT MAX_BODY_ELM   = MAX_BODY_ITEM * 4;


private:


	// --- selected
		UINT  _selected_body_id = 0;


	// ---- columns
		BOOL _use_columns = 0;
		UINT _num_columns = 3;


	// ---- build
		CHAR *_build_font_name;


	// ---- head
		INT   _head_handle = 0;
		FLOAT _head_height = 0;
		CHAR  _head_name[VS_MAXCHARNAME];
		CHAR  _head_font_name[VS_MAXCHARNAME];
		INT   _head_item_elm_handle[MAX_BODY_ITEM];
		INT   _head_dspl_elm_handle[MAX_BODY_ELM];
		UINT  _head_dspl_elm_count = 0;


	// ---- body
		INT   _body_handle          [MAX_BODY];
		INT   _body_handle_ass_obj  [MAX_BODY];
		INT   _body_item_elm_handle [MAX_BODY][MAX_BODY_ITEM];
		INT   _body_dspl_elm_handle [MAX_BODY][MAX_BODY_ELM];
		CHAR  _body_name            [MAX_BODY][VS_MAXCHARNAME];
		CHAR  _body_font_name       [VS_MAXCHARNAME];
		UINT  _body_id              [MAX_BODY];
		FLOAT _body_height          [MAX_BODY];
		UINT  _body_dspl_elm_count  [MAX_BODY];


	// ---- colour
		FLOAT _r_min = 0.10f, _g_min = 0.30f, _b_min = 1.0f;
		FLOAT _r_max = 0.05f, _g_max = 0.15f, _b_max = 0.5f;


	// ---- callback
		FLOAT (*_callback_get_config_param)(INT);
		TintCallback _callback_for_body[MAX_BODY];

};


}