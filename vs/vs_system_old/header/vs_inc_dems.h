////////////////////////////////////////////////////////////////////////////////

// ---------- vs_inc_dems.h ----------
/*!
\file vs_inc_dems.h
\brief Structs, enum and macros
\author Gareth Edwards
*/

#pragma once


// ---- definitions -----


// ---- node error codes
#define VS_VNODEBASE    -100
#define VS_VNODE_READERROR      (VS_VNODEBASE-1)
#define VS_VNODE_UNDERFLOW      (VS_VNODEBASE-2)
#define VS_VNODE_OVERFLOW       (VS_VNODEBASE-3)
#define VS_VNODE_EMPTYFILE      (VS_VNODEBASE-4)
#define VS_VNODE_OPENFILEFAILED (VS_VNODEBASE-5)
#define VS_VNODE_READFILEFAILED (VS_VNODEBASE-6)


// ---- argv[] MAX constant ----
#define VS_MAXARGV        32                    //<! Maximum command line arguments


// ---- char[] constants (e.g. char name[VS_MAXCHARLEN]) ----
#define VS_MAXCHARNAME    32                    //<! Maximum CHAR in a name
#define VS_MAXCHARLEN     256                   //<! Maximum CHAR in process name (e.g. strcpy)
#define VS_MAXCHARLINELEN 1024                  //<! Maximum CHAR in line
#define VS_MAXCHARPATHLEN 512                   //<! Maximum CHAR in file path


// ---- MATHS constants ----
#define VS_RADIANS_PER_DEGREE	0.01745329252   //<! 
#define VS_PI                   3.14159265359   //<! 3.1415926535897932384
#define VS_RADIAN              57.29577957855   //<! 


// ---- element configuration ----
#define VS_MAX_ELM_ATLAS_PAGE_REFERENCES   4    //<! Maximum number atlas page references TBD
#define VS_MAX_ELM_FLOAT_PARAMETER        48    //<! Maximum dimension of parameter data
#define VS_MAX_ELM_FLOAT_ACTIONS           8    //<! Maximum number of parameter index/action pairs
#define VS_MAX_ELM_GFX_FLOAT_PARAMETER    16    //<! Maximum dimension of supplied data
#define VS_MAX_ELM_GFX_FLOAT_BUFFER     1024    //<! Maximum dimension of gfx float buffer
#define VS_MAX_ELM_CONFIG_PARAM           12    //<! Maximum (re)configuration values


// ---- element setup bitmask values
#define VS_ELM_SETUP_NONE        0              //<! Setup nothing
#define VS_ELM_SETUP_CONFIG      1              //<! Setup config
#define VS_ELM_SETUP_GEOMETRY    2              //<! Setup geometry
#define VS_ELM_SETUP_MAPPING     4              //<! Setup mapping
#define VS_ELM_SETUP_INDEX       8              //<! Setup index
#define VS_ELM_SETUP_ALL       255              //<! Setup all


// ---- gfx max
#define VS_MAX_LIGHTS            8
#define VS_MAX_MATERIALS       128
#define VS_MAX_SHADERS           1
#define VS_MAX_VIEW_MATRICES     8

#define VS_INDEX_BUFFER_FORMAT  32


// ---- enum ----


// ---- VS_PrimitiveType ----
/*!
\brief primitive types
\author Gareth Edwards
*/
enum VS_PrimitiveType
{
	VS_POINTLIST     = 1,
	VS_LINELIST      = 2,
	VS_LINESTRIP     = 3,
	VS_TRIANGLELIST  = 4,
	VS_TRIANGLESTRIP = 5,
	VS_TRIANGLEFAN   = 6
};


// ---- VS_AtlasPageGroup ----
/*!
\brief primitive types
\author Gareth Edwards
*/
enum VS_AtlasPageGroup
{
	VS_ATLAS_PAGE_GROUP_ONE   = 1,
	VS_ATLAS_PAGE_GROUP_TWO   = 2,
	VS_ATLAS_PAGE_GROUP_THREE = 3,
	VS_ATLAS_PAGE_GROUP_FOUR  = 4
};


// ---- VS_ProjectionType ----
/*!
\brief projection types
\author Gareth Edwards
*/
enum VS_ProjectionType
{
	VS_PERSPECTIVE = 1,
	VS_ORTHO_FRONT = 2,
	VS_ORTHO_BACK = 3,
	VS_ORTHO_TOP = 4,
	VS_ORTHO_BOTTOM = 5,
	VS_ORTHO_LEFT= 6,
	VS_ORTHO_RIGHT= 7
};


// ---- VS_VertexFormat ----
/*!
\brief vertex formats
\author Gareth Edwards
*/
enum VS_VertexFormat
{
	VS_XYZ      = 0x002, // 2
	VS_XYZRHW   = 0x004, // 4
	VS_NORMAL   = 0x010, // 16
	VS_DIFFUSE  = 0x040, // 64
	VS_SPECULAR = 0x080, // 128
	VS_TEX1     = 0x100  // 256
};


// ---- VS_SUPPLIED_PARAMETERS ----
/*!
\brief Supplied parameters array indices
supplied via context->GetGfxFloatParameter() 
\author Gareth Edwards
*/
enum VS_SuppliedParameters
{
	VS_PANEL_LEFT = 0,
	VS_PANEL_TOP = 1,
	VS_PANEL_RIGHT = 2,
	VS_PANEL_BOTTOM = 3,
	VS_BUFFER_WIDTH = 4,
	VS_BUFFER_HEIGHT = 5,
};


// ---- VS_ALIGN ----
/*!
\brief Rectalinear alignment
\author Gareth Edwards
*/
enum VS_Align
{
	VS_ALIGN_NONE = 0,
	VS_ALIGN_LEFT = 1,
	VS_ALIGN_TOP = 2,
	VS_ALIGN_RIGHT = 3,
	VS_ALIGN_BOTTOM = 4,
	VS_ALIGN_CENTRE = 6,
	VS_ALIGN_CENTRE_ON_TEXT = 7

};


// ---- VS_COLOUR_ITEM ----
/*!
\brief Colour item id
\author Gareth Edwards
*/
enum VsColourItem
{
	VS_COLOUR_BG = 0,
	VS_COLOUR_EDGE = 1,
	VS_COLOUR_BORDER = 2,
	VS_COLOUR_VIEWRECT = 3,
	VS_COLOUR_VIEWPORT = 4,
	VS_COLOUR_TEXT = 5
};


// ---- VS_COLOUR_MODE ----
/*!
\brief Colour item mode
\author Gareth Edwards
*/
enum VsColourMode
{
	VS_COLOUR_NULL = 0,
	VS_COLOUR_OFF = 1,
	VS_COLOUR_ON = 2,
	VS_COLOUR_OVER = 3
};


// ---- structs ----


// ---- VsDateTime ----
/*!
\brief Primitive type
\author Gareth Edwards
\note See MS SYSTEMTIME structure
*/
struct VsDateTime {
	WORD year;
	WORD month;
	WORD day;
	WORD hour;
	WORD minute;
	WORD second;
	WORD milliseconds;
};


// ---- d3d macros ----

// Initialises a color with the supplied red, green, blue, and 
// alpha values which must be in the range 0 through 255.
#define VS_D3DCOLOR_ARGB(a,r,g,b) \
	((D3DCOLOR)((((a)& 0xff) << 24) | (((r)& 0xff) << 16) | (((g)& 0xff) << 8) | ((b)& 0xff)))


// Initialises a color with the supplied red, green, blue, and 
// alpha values which must be in the range 0 through 255.
#define VS_D3DCOLOR_RGBA(r,g,b,a) VS_D3DCOLOR_ARGB(a,r,g,b)


// Initialises a color with the supplied red, green, blue, and 
// alpha floating-point values which must be in the range 0 through 1.0.
#define VS_D3DCOLOR_COLORVALUE(r,g,b,a) \
	VS_D3DCOLOR_RGBA((DWORD)((r)*255.f), (DWORD)((g)*255.f), (DWORD)((b)*255.f), (DWORD)((a)*255.f))
