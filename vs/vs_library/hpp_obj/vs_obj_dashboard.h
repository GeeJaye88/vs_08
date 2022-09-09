////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_dashboard.h ----------
/*!
\file vs_obj_dashboard.h
\brief
\author Gareth Edwards
*/


#pragma once

	
// ---- dashboard ----
enum {

	// ---- dial ----
	DBZ_DIAL_MAX_PARAMETERS = 48, // VS_MAX_ELM_FLOAT_PARAMETER


	// ---- common param ----
	DBZ_CONFIG       = 0,
	DBZ_VISIBLE      = 1,
	DBZ_ART          = 2,
	DBZ_TRANSLATE_X  = 3,
	DBZ_TRANSLATE_Y  = 4,
	DBZ_TRANSLATE_Z  = 5,
	DBZ_SCALE_X      = 6,
	DBZ_SCALE_Y      = 7,
	DBZ_SCALE_Z      = 8,
	DBZ_ROTATE_X     = 9,
	DBZ_ROTATE_Y     = 10,
	DBZ_ROTATE_Z     = 11,


	// ---- art ----
	DBZ_ART_CENTRE   = 1,
	DBZ_ART_LEFT     = 2,
	DBZ_ART_RIGHT    = 3,


	// ---- config ----
	DBZ_CONFIG_EDGES  = 1,
	DBZ_CONFIG_SHAPE  = 2,
	DBZ_CONFIG_AREA   = 4,
	DBZ_CONFIG_BG     = 8,

	DBZ_DIAL_CONFIG_BITMAP_EDGE_MIN = 1,
	DBZ_DIAL_CONFIG_BITMAP_EDGE_MAX = 2,
	DBZ_DIAL_CONFIG_BITMAP_BARS     = 4,
	DBZ_DIAL_CONFIG_BITMAP_SHAPE    = 8,


	// ---- dial param ----
	DBZ_DIAL_ANGLE_MIN       = 18,
	DBZ_DIAL_ANGLE_MAX       = 19,
	DBZ_DIAL_RADIUS_MIN      = 16,
	DBZ_DIAL_RADIUS_MAX      = 17,
	DBZ_DIAL_BLIP            = 33,
	DBZ_DIAL_BLIP_EDGE_SIZE  = 34,
	DBZ_DIAL_BLIP_RED_MAX    = 35,
	DBZ_DIAL_BLIP_GREEN_MAX  = 36,
	DBZ_DIAL_BLIP_BLUE_MAX   = 37,
	DBZ_DIAL_BLIP_RED_MIN    = 38,
	DBZ_DIAL_BLIP_GREEN_MIN  = 39,
	DBZ_DIAL_BLIP_BLUE_MIN   = 40,
	DBZ_DIAL_EDGE_SIZE       = 20,
	DBZ_DIAL_EDGE_RED_MAX    = 21,
	DBZ_DIAL_EDGE_GREEN_MAX  = 22,
	DBZ_DIAL_EDGE_BLUE_MAX   = 23,
	DBZ_DIAL_EDGE_RED_MIN    = 24,
	DBZ_DIAL_EDGE_GREEN_MIN  = 25,
	DBZ_DIAL_EDGE_BLUE_MIN   = 26,
	DBZ_DIAL_SHAPE_RED_MAX   = 27,
	DBZ_DIAL_SHAPE_GREEN_MAX = 28,
	DBZ_DIAL_SHAPE_BLUE_MAX  = 29,
	DBZ_DIAL_SHAPE_RED_MIN   = 30,
	DBZ_DIAL_SHAPE_GREEN_MIN = 31,
	DBZ_DIAL_SHAPE_BLUE_MIN  = 32,


	// ---- icon param ----
	DBZ_ICON_APG             = 16,
	DBZ_ICON_TEX_ROW         = 17,
	DBZ_ICON_TEX_COLUMN      = 18,
	DBZ_ICON_TEX_WIDTH       = 19,
	DBZ_ICON_TEX_HEIGHT      = 20,
	DBZ_ICON_TEX_ADJUST      = 21,


	// ---- needle param ----
	DBZ_NEEDLE_RED_MAX       = 16,
	DBZ_NEEDLE_GREEN_MAX     = 17,
	DBZ_NEEDLE_BLUE_MAX      = 18,
	DBZ_NEEDLE_RED_MIN       = 19,
	DBZ_NEEDLE_GREEN_MIN     = 20,
	DBZ_NEEDLE_BLUE_MIN      = 21,


	// ---- rect param ----
	DBZ_RECT_QUAD_NUM_EDGES  = 16,
	DBZ_RECT_QUAD_RADIUS_MAX = 17,
	DBZ_RECT_QUAD_RADIUS_MIN = 18,
	DBZ_RECT_WIDTH           = 19,
	DBZ_RECT_HEIGHT          = 20,
	DBZ_RECT_RED_MAX         = 21,
	DBZ_RECT_GREEN_MAX       = 22,
	DBZ_RECT_BLUE_MAX        = 23,
	DBZ_RECT_RED_MIN         = 24,
	DBZ_RECT_GREEN_MIN       = 25,
	DBZ_RECT_BLUE_MIN        = 26,


	// ---- bar graph param ----
	DBZ_GRAPH_BAR_NUM           = 16,
	DBZ_GRAPH_BAR_WIDTH_EDGE    = 17,
	DBZ_GRAPH_BAR_WIDTH_SPACE   = 18,
	DBZ_GRAPH_BAR_WIDTH_MARGIN  = 19,
	DBZ_GRAPH_BAR_YOFFSET       = 20,
	DBZ_GRAPH_BAR_XAREA_MIN     = 21,
	DBZ_GRAPH_BAR_XAREA_MAX     = 22,
	DBZ_GRAPH_BAR_WIDTH         = 25,
	DBZ_GRAPH_BAR_HEIGHT        = 26,
	DBZ_GRAPH_BAR_EDGE_RED      = 28,
	DBZ_GRAPH_BAR_EDGE_GREEN    = 29, 
	DBZ_GRAPH_BAR_EDGE_BLUE     = 30,
	DBZ_GRAPH_BAR_EDGE_FADE     = 31,
	DBZ_GRAPH_BAR_SHAPE_RED     = 32,
	DBZ_GRAPH_BAR_SHAPE_GREEN   = 33,
	DBZ_GRAPH_BAR_SHAPE_BLUE    = 34,
	DBZ_GRAPH_BAR_SHAPE_FADE    = 35,
	DBZ_GRAPH_BAR_AREA_RED      = 36,
	DBZ_GRAPH_BAR_AREA_GREEN    = 37,
	DBZ_GRAPH_BAR_AREA_BLUE     = 38,
	DBZ_GRAPH_BAR_AREA_FADE     = 39,
	DBZ_GRAPH_BAR_BG_RED        = 40,
	DBZ_GRAPH_BAR_BG_GREEN      = 41,
	DBZ_GRAPH_BAR_BG_BLUE       = 42,
	DBZ_GRAPH_BAR_BG_FADE       = 43,


	// ---- text config  ----
	DBZ_TEXT_CAPTION_ALIGNED_CENTRE = 1,
	DBZ_TEXT_CAPTION_ALIGNED_LEFT   = 2,
	DBZ_TEXT_LABEL_ALIGNED_CENTRE   = 3,
	DBZ_TEXT_LABEL_ALIGNED_LEFT     = 4,
	DBZ_TEXT_NORMAL_ALIGNED_CENTRE  = 5,
	DBZ_TEXT_NORMAL_ALIGNED_LEFT    = 6,

};


// ---- dashboard  ----
INT Dbz_SetDefault(FLOAT[]);
INT Dbz_SetArt(FLOAT[], FLOAT);
INT Dbz_SetConfig(FLOAT[], FLOAT);
INT Dbz_SetVisible(FLOAT[], FLOAT);
INT Dbz_SetScale(FLOAT[], FLOAT, FLOAT);
INT Dbz_SetRotate(FLOAT[], FLOAT);
INT Dbz_SetTranslate(FLOAT[], FLOAT, FLOAT);


// ---- dial ----
static INT Dbz_Callback_Configure_Dial(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_Dial(vs_system::ElmContext*);
static INT Dbz_Callback_GetIndexBuffer_Dial(vs_system::ElmContext*);
	INT Dbz_SetDialDefault(FLOAT[]);
	INT Dbz_SetDialAngle(FLOAT[], FLOAT, FLOAT);
	INT Dbz_SetDialRadii(FLOAT[], FLOAT, FLOAT);
	INT Dbz_SetDialBlipCount(FLOAT[], FLOAT);
	INT Dbz_SetDialBlipEdgeSize(FLOAT[], FLOAT);	
	INT Dbz_SetDialEdgeSize(FLOAT[],FLOAT);
	INT Dbz_SetDialEdgeRGBMin(FLOAT[],FLOAT, FLOAT, FLOAT);
	INT Dbz_SetDialEdgeRGBMax(FLOAT[], FLOAT, FLOAT, FLOAT);
	INT Dbz_SetDialShapeRGBMin(FLOAT[], FLOAT, FLOAT, FLOAT);
	INT Dbz_SetDialShapeRGBMax(FLOAT[], FLOAT, FLOAT, FLOAT);


// ---- icon ----
static INT Dbz_Callback_Configure_Icon(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_Icon(vs_system::ElmContext*);
	INT Dbz_SetIconTexture(
				FLOAT[],
				FLOAT, // atlas page group
				FLOAT, // row
				FLOAT, // column
				FLOAT, // width
				FLOAT, // height
				FLOAT  // tolerance
			);


// ---- text ----
static INT Dbz_Callback_Configure_Text(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_Text(vs_system::ElmContext*);


// ---- needle ----
static INT Dbz_Callback_Configure_Needle(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_Needle(vs_system::ElmContext*);
	INT Dbz_SetNeedleRGBMin(
			FLOAT[],
			FLOAT, // red
			FLOAT, // green
			FLOAT  // blue
		);
	INT Dbz_SetNeedleRGBMax(
			FLOAT[],
			FLOAT, // red
			FLOAT, // green
			FLOAT  // blue
		);


// ---- rect ----
static INT Dbz_Callback_Configure_Rect(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_Rect(vs_system::ElmContext*);
	INT Dbz_SetRectDimension(
				FLOAT[],
				FLOAT, // width
				FLOAT  // height
			);
	INT Dbz_SetRectQuad(
			FLOAT[],
				FLOAT, // num edges
				FLOAT, // radius max
				FLOAT  // radius_min
			);
	INT Dbz_SetRectRGBMin(
				FLOAT[],
				FLOAT, // red
				FLOAT, // green
				FLOAT  // blue
			);
	INT Dbz_SetRectRGBMax(
				FLOAT[],
				FLOAT, // red
				FLOAT, // green
				FLOAT  // blue
			);


// ---- bar graph ----
static INT Dbz_Callback_Configure_BarGraph(vs_system::ElmContext*);
static INT Dbz_Callback_GetVertexBuffer_BarGraph(vs_system::ElmContext*);
	INT Dbz_SetBarGraphNum(FLOAT[], FLOAT);
	INT Dbz_SetBarGraphDimension(FLOAT[], FLOAT, FLOAT);
	INT Dbz_SetBarGraphWidthEdge(FLOAT[], FLOAT);
	INT Dbz_SetBarGraphWidthSpace(FLOAT[], FLOAT);
	INT Dbz_SetBarGraphWidthMargin(FLOAT[], FLOAT);
	INT Dbz_SetBarGraphXArea(FLOAT[], FLOAT, FLOAT);
	INT Dbz_SetBarGraphYOffset(FLOAT[], FLOAT);
	INT Dbz_SetBarGraphEdgeRGBF(FLOAT[], FLOAT, FLOAT, FLOAT, FLOAT);
	INT Dbz_SetBarGraphShapeRGBF(FLOAT[], FLOAT, FLOAT, FLOAT, FLOAT);
	INT Dbz_SetBarGraphAreaRGBF(FLOAT[], FLOAT, FLOAT, FLOAT, FLOAT);
	INT Dbz_SetBarGraphBgRGBF(FLOAT[], FLOAT, FLOAT, FLOAT, FLOAT);

