////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_config.h ----------
/*!
\file vs_obj_config.h
\brief Interface for the Obj classes.
\author Gareth Edwards
*/

#pragma once


#include <random>


// ---------- library include ----------
#include "../header/vs_img_buffer.h"
#include "../header/vs_flt_buffer.h"
#include "../header/vs_cam_props.h"
#include "../header/vs_dot_obj_elem.h"
#include "../header/vs_fractal.h"
#include "../header/vs_fractal_ds.h"
#include "../header/vs_img_heightfield.h"


// ---------- system include ----------
#include "../../vs_system/header/vs_elm_context.h"


#ifdef VS_GFX_FLAG
#include "../../vs_system/header/vs_pan_context.h"
#include "../../vs_system/header/vs_gfx_interface.h"
#endif


// ---------- namespace ----------
namespace vs_system
{


////////////////////////////////////////////////////////////////////////////////


// ---- all geom id
#define VS_OBJ_GEOMETRY                  10


// ---- pano group id
#define VS_OBJ_PANO_SPHERE               11
#define VS_OBJ_PANO_CYLINDER             12
#define VS_OBJ_PANO_CYLINDER_TOP_RING    13


// ---- pano rect group id
#define VS_OBJ_PANO_RECT                 20
#define VS_OBJ_PANO_RECT_TOP             21
#define VS_OBJ_PANO_RECT_BOTTOM          22
#define VS_OBJ_PANO_RECT_FRONT           23
#define VS_OBJ_PANO_RECT_BACK            24
#define VS_OBJ_PANO_RECT_EQUI            25
#define VS_OBJ_PANO_RECT_PART            26


// ---- pano annular id
#define VS_OBJ_PANO_ANNULAR              30
#define VS_OBJ_PANO_ANNULAR_MASK         31
#define VS_OBJ_PANO_ANNULAR_AOI          32
#define VS_OBJ_PANO_ANNULAR_EXPOSURE_AOI 33
#define VS_OBJ_PANO_ANNULAR_FOCUS_AOI    34
#define VS_OBJ_PANO_ANNULAR_FOCUS_GRF    35


// ---- image element id's (group is VS_OBJ_GEOMETRY)
#define VS_OBJ_IMG                       36


// ---- button group id's
#define VS_OBJ_IR_IMG_BUTTONS            55
#define VS_OBJ_MV_IMG_BUTTONS            56
#define VS_OBJ_VM_IMG_BUTTONS            57


////////////////////////////////////////////////////////////////////////////////


// ---------- ObjConfig class ----------
class ObjConfig
{

public:

	// ---- group
		enum {
				GROUP_BASE                = 1000,
				GROUP_ELEM_PANO_GEOMETRY  = GROUP_BASE,
				//ELEM_GROUP_PANO_LID = GROUP_BASE + 20,
			};


	// ---- panel rect
		enum {
				MAX_PANEL_RECT_PARAMETERS = VS_MAX_ELM_FLOAT_PARAMETER,
				PANEL_RECT_TYPE    = 0,
				PANEL_RECT_TEX_LID = 1,
				PANEL_RECT_BORDER  = 2,
				PANEL_RECT_ASPECT  = 3,
			};


	// ---- button
		enum {
				MAX_BUTTON_PARAMETERS = VS_MAX_ELM_FLOAT_PARAMETER,
				MAX_BUTTON_PARAM_USED = 20,
				BUTTON_DPT        = 0,  // Dimensions Proportional To
				BUTTON_X          = 1,  // proportional location
				BUTTON_Y          = 2,
				BUTTON_WIDTH      = 3,  // proportional dimension
				BUTTON_HEIGHT     = 4,
				BUTTON_BG_COL     = 5,  // background
				BUTTON_BG_ROW     = 6,
				BUTTON_FG_COL     = 7,  // foreground
				BUTTON_FG_ROW     = 8,
				BUTTON_DIM_COL    = 9,  // e.g. 1 == width of a column
				BUTTON_DIM_ROW    = 10, // e.g. 1 == width of a row
				BUTTON_ADJUST     = 11, // uv tolerance
				BUTTON_APG        = 12, // atlas page
				BUTTON_LINE_SHADE = 14, // line shading mode == 0 (none), etc.
				BUTTON_AOI        = 15, // aoi on/off == 1/0
				BUTTON_LEFT       = 16, // aoi
				BUTTON_TOP        = 17,
				BUTTON_RIGHT      = 18,
				BUTTON_BOTTOM     = 19
			};


	// ---- pano
		enum {
				MAX_PANO_GEOMETRY     = 8,
				PANOSPHERE            = GROUP_ELEM_PANO_GEOMETRY + 1,
				PANOCYLINDER          = GROUP_ELEM_PANO_GEOMETRY + 2,
				PANOCYLINDER_TOP_RING = GROUP_ELEM_PANO_GEOMETRY + 3,
				PANORECT              = GROUP_ELEM_PANO_GEOMETRY + 4,
				PANORECT_FRONT        = GROUP_ELEM_PANO_GEOMETRY + 5,
				PANORECT_BACK         = GROUP_ELEM_PANO_GEOMETRY + 6,
				PANOANNULAR           = GROUP_ELEM_PANO_GEOMETRY + 7,
				PANOANNULARMASK       = GROUP_ELEM_PANO_GEOMETRY + 8
			};


	// ---- stream
		enum {
				STREAM_PANO_PTF_ID = 12
			};


	// ---- aoi zone
		enum {
				AOI_ALL     = 0,
				AOI_ANNULAR = 1,
				AOI_TOP     = 2,
				AOI_LEFT    = 3,
				AOI_BOTTOM  = 4,
				AOI_RIGHT   = 5,
				AOI_FOCUS   = 6,
			};


	// ---- aoi zone shape
		enum {
				AOI_DEFAULT  = 1,
				AOI_SQUARE   = 2,
				AOI_CIRCULAR = 3
			};


	// ---- pano PTF LUT
		enum {
				MAX_PANO_PTF_LUT = 16,
				MAX_PANO_PARAM = 2,
			};


	// ---- orbit
		enum {
				MAX_ORBIT_PARAMETERS = 9,
				ORBIT_LINES      = 0,
				ORBIT_RADIUS     = 1,
				ORBIT_RED        = 2,
				ORBIT_GREEN      = 3,
				ORBIT_BLUE       = 4,
				ORBIT_X          = 5,
				ORBIT_Y          = 6,
				ORBIT_Z          = 7,
				ORBIT_DEGREES    = 8
			};

		
	// ---- halo
		enum {
				MAX_HALO_PARAMETERS = 11,
				HALO_EDGES    = 0,
				HALO_RED      = 1,
				HALO_GREEN    = 2,
				HALO_BLUE     = 3,
				HALO_ALPHA    = 4,
				HALO_X        = 5,
				HALO_Y        = 6,
				HALO_Z        = 7,
				HALO_DEGREES  = 8,
				HALO_RAD_MIN  = 9,
				HALO_RAD_MAX  = 10,
			};


	// ---- box face parameters
		enum {
				MAX_BOX_FACE_PARAMETERS = 23,
				BOX_FACE_MODE    =  0,
				BOX_FACE_TYPE    =  1, // 0 - wireframe, 1 - solid, 2 - etc.
				BOX_FACE_AP_REF  =  2,
				BOX_FACE_RED     =  3,
				BOX_FACE_GREEN   =  4,
				BOX_FACE_BLUE    =  5,
				BOX_FACE_SCALE   =  6,
				BOX_FACE_EXPLODE =  7,
				BOX_FACE_WIDTH   =  8,
				BOX_FACE_HEIGHT  =  9,
				BOX_FACE_DEPTH   = 10,
				BOX_FACE_X       = 11,
				BOX_FACE_Y       = 12,
				BOX_FACE_Z       = 13,
				BOX_FACE_AP_SECT = 14,
				BOX_FLAG_1       = 15,
				BOX_FLAG_2       = 16,
				BOX_FLAG_3       = 17,
				BOX_FLAG_4       = 18,
				BOX_FLAG_5       = 19,
				BOX_FLAG_6       = 20,
				BOX_FLAG_7       = 21,
				BOX_FLAG_8       = 22,
		};


	// ---- box face indices
		enum {
				BOX_FACE_OUTSIDE_FRONT  = 0,
				BOX_FACE_OUTSIDE_RIGHT  = 1,
				BOX_FACE_OUTSIDE_BACK   = 2,
				BOX_FACE_OUTSIDE_LEFT   = 3,
				BOX_FACE_OUTSIDE_TOP    = 4,
				BOX_FACE_OUTSIDE_BOTTOM = 5,
				BOX_FACE_INSIDE_FRONT   = 6,
				BOX_FACE_INSIDE_RIGHT   = 7,
				BOX_FACE_INSIDE_BACK    = 8,
				BOX_FACE_INSIDE_LEFT    = 9,
				BOX_FACE_INSIDE_TOP     = 10,
				BOX_FACE_INSIDE_BOTTOM  = 11,
			};


	// ---- kandinsky
		enum {
				MAX_KANDINSKY_PARAMETERS = 10,
				KANDINSKY_TYPE   = 0,
				KANDINSKY_AP_REF = 1,
				KANDINSKY_RED    = 2,
				KANDINSKY_GREEN  = 3,
				KANDINSKY_BLUE   = 4,
				KANDINSKY_XOFF   = 4,
				KANDINSKY_YOFF   = 5,
				KANDINSKY_ZOFF   = 6,
				KANDINSKY_XSCALE = 7,
				KANDINSKY_YSCALE = 8,
				KANDINSKY_ZSCALE = 9
		};

		enum {
				KANDINSKY_2D = 2,
				KANDINSKY_3D = 3,

				KANDINSKY_GEOTYPE_POINT = 0,
				KANDINSKY_GEOTYPE_LINE  = 1,
				KANDINSKY_GEOTYPE_FACE  = 2,
		};

	// ---- mesh
		enum {
				PARAM_GROUP_SIZEETERS = 24,
				MESH_COLS     = 0,
				MESH_ROWS     = 1,
				MESH_TYPE     = 2, // 0 - wireframe, 1 - solid, 2 - etc.
				MESH_AP_REF   = 3,
				MESH_ALIGN    = 4, // 0 - xz, 1 - xy, 2 - yz
				MESH_RED      = 5,
				MESH_GREEN    = 6,
				MESH_BLUE     = 7,
				MESH_WIDTH    = 8,
				MESH_DEPTH    = 9,
				MESH_HEIGHT   = 10,
				MESH_XOFF     = 11,
				MESH_YOFF     = 12,
				MESH_ZOFF     = 13,
				MESH_PARAM_0  = 14, // mode
				MESH_PARAM_1  = 15,
				MESH_PARAM_2  = 16,
				MESH_PARAM_3  = 17,
				MESH_PARAM_4  = 18,
				MESH_PARAM_5  = 19,
				MESH_PARAM_6  = 20,
				MESH_PARAM_7  = 21,
				MESH_PARAM_8  = 22,
				MESH_SCALAR   = 23,
		};

	// ---- mesh fractal Diamond Square algorithm selected
		enum
			{
				MESH_FRACTAL_DS1 = 1,
				MESH_FRACTAL_DS2 = 2,
				MESH_FRACTAL     = MESH_FRACTAL_DS1
			};

	// ---- mesh mode
		enum
			{
				MESH_VERSION_CAMERA    = 0,
				MESH_VERSION_RR        = 1,
				MESH_VERSION_TERRAIN   = 2,
				MESH_VERSION_FUNCTION  = 3,
				MESH_VERSION_ACQUIRE   = 4,
			};

	// ---- mesh acquire & explore axis/lights
		//
		//  mesh axis & light param passed via array
		//
		enum {
				MAX_MESH_AL_PARAMETERS = 36,
				MESH_AL_MODE   = 0,  // acquire or explore
				MESH_AL_ID     = 1,  // component
				MESH_AL_WIDTH  = 2,  // x dimension
				MESH_AL_HEIGHT = 3,  // y "
				MESH_AL_DEPTH  = 4,  // z "
				MESH_AL_XOFF   = 5,  // x offset
				MESH_AL_YOFF   = 6,  // y "
				MESH_AL_ZOFF   = 7,  // z "

				MESH_AL_MODE_ACQUIRE = 1,
				MESH_AL_MODE_EXPLORE = 2,

				MESH_AL_LIGHT_OFFSET = 8, // light data param array index
				MESH_AL_LIGHT_COUNT  = 4, // number of lights
				MESH_AL_LIGHT_SIZE   = 5, // parameters per light
			};

	// ---- dot obj
		enum {
				MAX_DOTOBJ_PARAMETERS = 14,
				DOTOBJ_MODE     = 0,
				DOTOBJ_TYPE     = 1,  // 0 - wireframe, 1 - solid, 2 - etc.
				DOTOBJ_AP_REF   = 2,  // 100 +
				DOTOBJ_AP_SECT  = 3,  // 0/1 - top & 2 - bottom
				DOTOBJ_RED      = 4,  // 0 to 1
				DOTOBJ_GREEN    = 5,  //   "
				DOTOBJ_BLUE     = 6,  //   "
				DOTOBJ_SCALE    = 7,  // 0.0001 -> scale
				DOTOBJ_WIDTH    = 8,  // * scale
				DOTOBJ_HEIGHT   = 9,  //    "
				DOTOBJ_DEPTH    = 10, //    "
				DOTOBJ_XOFF     = 11, // post scaling
				DOTOBJ_YOFF     = 12, //      "
				DOTOBJ_ZOFF     = 13  //      "
			};


	// ---- grid
		enum {
				MAX_GRID_PARAMETERS = 11,
				GRID_CEL_COLS =  0,
				GRID_CEL_ROWS =  1,
				GRID_TYPE     =  2, // 0 - wireframe, 1 - solid, 2 - etc.
				GRID_AP_REF   =  3,
				GRID_ALIGN    =  4, // 0 - xz, 1 - xy, 2 - yz
				GRID_RED      =  5,
				GRID_GREEN    =  6,
				GRID_BLUE     =  7,
				GRID_WIDTH    =  8,
				GRID_DEPTH    =  9,
				GRID_HEIGHT   = 10
			};


	// ---- sphere
		enum {
				MAX_SPHERE_PARAMETERS = 14,
				SPHERE_CEL_COLS =  0,
				SPHERE_CEL_ROWS =  1,
				SPHERE_TYPE     =  2, // 0 - wireframe, 1 - solid, 2 - etc.
				SPHERE_AP_REF   =  3,
				SPHERE_AP_SECT  = 10,
				SPHERE_RADIUS   =  4,
				SPHERE_RED      =  5,
				SPHERE_GREEN    =  6,
				SPHERE_BLUE     =  7,
				SPHERE_LAT_MIN  =  8,
				SPHERE_LAT_MAX  =  9,
				SPHERE_RAD_MIN  = 11,
				SPHERE_RAD_MAX  = 12,
				SPHERE_NUMBER   = 13,
			};


	// ---- asteroid belt
		enum {
				MAX_ASTEROID_BELT_PARAMETERS = 11,
				ASTEROID_BELT_DIVISION  = 0,
				ASTEROID_BELT_RADIUS    = 4,
				ASTEROID_BELT_RED       = 5,
				ASTEROID_BELT_GREEN     = 6,
				ASTEROID_BELT_BLUE      = 7,
				ASTEROID_BELT_RAD_MIN   = 8,
				ASTEROID_BELT_RAD_MAX   = 9,
				ASTEROID_BELT_NUMBER    = 10,
			};


		// ---- ring system
		enum {
				MAX_RING_SYSTEM_PARAMETERS = 21,
				RING_SYSTEM_COLS      = 0,    // per ring
				RING_SYSTEM_ROWS      = 1,
				RING_SYSTEM_TYPE      = 2,    // 0 - wireframe, 1 - solid, 2 - etc.
				RING_SYSTEM_AP_REF    = 3,    // atlas page ref
				RING_SYSTEM_AP_SECT   = 4,    // atlas page section
				RING_SYSTEM_RED       = 5,    // tint
				RING_SYSTEM_GREEN     = 6,
				RING_SYSTEM_BLUE      = 7,
				RING_SYSTEM_NUMBER    = 8,
				RING_SYSTEM_RAD_MIN   = 9,   // five rings
				RING_SYSTEM_RAD_MAX   = 10,
				RING_SYSTEM_RAD_MIN_0 = 11,   // five rings
				RING_SYSTEM_RAD_MAX_0 = 12,
				RING_SYSTEM_RAD_MIN_1 = 13,
				RING_SYSTEM_RAD_MAX_1 = 14,
				RING_SYSTEM_RAD_MIN_2 = 15,
				RING_SYSTEM_RAD_MAX_2 = 16,
				RING_SYSTEM_RAD_MIN_3 = 17,
				RING_SYSTEM_RAD_MAX_3 = 18,
				RING_SYSTEM_RAD_MIN_4 = 19,
				RING_SYSTEM_RAD_MAX_4 = 20,
			};


		// ---- starfield
		enum {
				MAX_STARFIELD_PARAMETERS = 9,
				STARFIELD_TYPE         = 1,
				STARFIELD_RADIUS_MAX   = 2,
				STARFIELD_RADIUS_MIN   = 3,
				STARFIELD_STAR_COUNT   = 4,
				STARFIELD_STAR_RADIUS  = 5,
			};


	#ifdef VS_GFX_FLAG

	// ---- cdtor ----
		ObjConfig();
		virtual ~ObjConfig();


		INT Initialise(); // NOT USED AT PRESENT


	// ---- IO ptf
		INT ReadPanoConfig(CHAR *);
		INT WritePanoConfig();


	// ---- copy ptf
		INT CopyPanoPTF(UINT, UINT);


	// ---- active ptf panel
		INT CpyImagePanelActivePtfbyId(UINT);

		INT GetImagePanelActiveId(UINT *);
		INT GetImagePanelPreviousId(UINT *);
		INT GetImagePanelAspectRatio(FLOAT *);

		INT SetImagePanelActiveId(UINT);
		INT SetImagePanelPreviousId(UINT);
		INT SetImagePanelAspectRatio(FLOAT);


	// ---- over ptf
		INT GetImagePanelOverId(UINT *);
		INT SetImagePanelOverId(UINT);


	// ---- get ptf
		INT GetPanoParamById(UINT, UINT, INT *);
		INT GetPanoPTF(FLOAT *, FLOAT *, FLOAT *);
		INT GetPanoPTFLast(FLOAT *, FLOAT *, FLOAT *);
		INT GetPanoPTFDerived(FLOAT *, FLOAT *, FLOAT *);
		INT GetPanoPTFById(UINT, FLOAT *, FLOAT *, FLOAT *);
		INT GetPanoPTFId(UINT *);
		INT GetPanoPanOffset(FLOAT *);
		INT GetPanoAutoPan(INT *);
		INT GetPanoAngles(FLOAT *, FLOAT *);
		INT GetPanoAngleMax(FLOAT *);
		INT GetPanoAngleAtUnitRadius(FLOAT *);
		INT GetPanoAspectRatio(FLOAT *);
		INT GetPanoCamHFlip(BOOL *);
		INT GetPanoCamVFlip(BOOL *);
		INT GetPanoCamHOffset(INT *);
		INT GetPanoCamVOffset(INT *);
		INT GetPanoCamWidth(INT *);
		INT GetPanoCamHeight(INT *);
		INT GetPanoUpdated(BOOL *);
		INT GetPanoConfigPaf(CHAR *);
		INT GetPanoCylinderXRotate(FLOAT *);
		INT GetPanoCylinderScale(FLOAT *);
		INT GetPanoFovMax(FLOAT *);
		INT GetPanoPanVertexBuffer(BOOL *);
		INT GetPanoPicking(BOOL *);
	  
		INT GetPanoRadii(FLOAT *, FLOAT *);
		INT GetPanoRadiusMax(FLOAT *);
		INT GetPanoRadiusMin(FLOAT *);

		#ifdef GET_PANO_EXPOSURE
		INT GetPanoExposureAoi(FLOAT *, FLOAT *);
		INT GetPanoExposureAoiMax(FLOAT *);
		INT GetPanoExposureAoiMin(FLOAT *);
		INT GetPanoExposureAoiShape(UINT *);
		INT GetPanoExposureAoiZone(UINT *);
		#endif

		#ifdef GET_PANO_FOCUS
		INT GetPanoFocusAoi(FLOAT *, FLOAT *);
		INT GetPanoFocusAoiMax(FLOAT *);
		INT GetPanoFocusAoiMin(FLOAT *);
		INT GetPanoFocusAoiSector(FLOAT *);
		INT GetPanoFocusAoiRotate(FLOAT *);
		#endif

		INT GetPanoUVOffset(FLOAT *, FLOAT *);


	// ---- set pano
		INT SetPanoParamById(UINT, UINT, INT);
		INT SetPanoPTF(FLOAT, FLOAT, FLOAT);
		INT SetPanoPTFLast();
		INT SetPanoPTFLast(FLOAT, FLOAT, FLOAT);
		INT SetPanoPTFByIndex(UINT, FLOAT, FLOAT, FLOAT);
		INT SetPanoPTFById(UINT, FLOAT, FLOAT, FLOAT);
		INT SetPanoPtfSelectedById(UINT);
		INT SetPanoAngles(FLOAT, FLOAT);
		INT SetPanoAngleMax(FLOAT);
		INT SetPanoAspectRatio(FLOAT);
		INT SetPanoAspectRatio(FLOAT, FLOAT);
		INT SetPanoAtlasPageLid(INT);
		INT SetPanoAutoPan(INT);
		INT SetPanoCamHOffset(INT);
		INT SetPanoCamVOffset(INT);
		INT SetPanoCamWidth(INT);
		INT SetPanoCamHeight(INT);
		INT SetPanoPanVertexBuffer(BOOL);
		INT SetPanoUpdated(BOOL);
		INT SetPanoDefault();
		INT SetPanoDefault(BOOL);
		INT SetPanoDefault(UINT, BOOL);
		INT SetPanoOffset(FLOAT, FLOAT);
		INT SetPanoPicking(BOOL);

		INT SetPanoRadii(FLOAT, FLOAT);
		INT SetPanoRadiusMax(FLOAT);
		INT SetPanoRadiusMin(FLOAT);

		INT SetPanoExposureAoi(FLOAT, FLOAT);
		INT SetPanoExposureAoiMax(FLOAT);
		INT SetPanoExposureAoiMin(FLOAT);
		INT SetPanoExposureAoiZone(UINT);


		INT SetPanoFocusAoi(FLOAT, FLOAT);
		INT SetPanoFocusAoiMax(FLOAT);
		INT SetPanoFocusAoiMin(FLOAT);
		INT SetPanoFocusAoiSector(FLOAT);
		INT SetPanoFocusAoiRotate(FLOAT);


	// ---- set
		INT SetConfigureAllBitmask(BYTE);


	// ---- Configure, GetVertexBuffer and GetIndexBuffer callback methods
	

	// ---- Box
		static INT Callback_Box_Configure(vs_system::ElmContext*);
		static INT Callback_Box_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_Box_GetVertexBufferB(vs_system::ElmContext*);

	// ---- BoxFace
		static INT Callback_BoxFace_Configure(vs_system::ElmContext*);
		static INT Callback_BoxFace_GetVertexBuffer(vs_system::ElmContext*);


	// ---- Surface_03


	// ---- surface axis
		static INT Callback_SurfAxis_Configure(vs_system::ElmContext*);
		static INT Callback_SurfAxis_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_SurfAxis_GetIndexBuffer(vs_system::ElmContext*);

	// ---- surface edge ( see MeshEdge )

	// ---- surface mesh ( see Mesh)

	// ---- surface mount
		static INT Callback_SurfMount_Configure(vs_system::ElmContext*);
		static INT Callback_SurfMount_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_SurfMount_GetIndexBuffer(vs_system::ElmContext*);

	// ---- explore light axis
		static INT Callback_ExpLgtAxis_Configure(vs_system::ElmContext*);
		static INT Callback_ExpLgtAxis_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_ExpLgtAxis_GetIndexBuffer(vs_system::ElmContext*);

	// ---- explore light spot ( see Sphere )

	// ---- loi axis
		static INT Callback_LoiAxis_Configure(vs_system::ElmContext*);
		static INT Callback_LoiAxis_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_LoiAxis_GetIndexBuffer(vs_system::ElmContext*);

	// ---- loi rectangle
		static INT Callback_S3_Loi_Blade_Configure(vs_system::ElmContext*);
		static INT Callback_S3_Loi_Blade_GetVertexBuffer(vs_system::ElmContext*);

	// ---- loi ruler
		static INT Callback_S3_Loi_Ruler_Configure(vs_system::ElmContext*);
		static INT Callback_S3_Loi_Ruler_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_S3_Loi_Ruler_GetIndexBuffer(vs_system::ElmContext*);
		
	// ---- loi ruler
		static INT Callback_S3_Loi_Section_Configure(vs_system::ElmContext*);
		static INT Callback_S3_Loi_Section_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_S3_Loi_Section_GetIndexBuffer(vs_system::ElmContext*);
		
	// ---- loi spot ( see Sphere )


	// ---- acquire light axis
		static INT Callback_S2_AcqLgtAxis_Configure(vs_system::ElmContext*);
		static INT Callback_S2_AcqLgtAxis_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_S2_AcqLgtAxis_GetIndexBuffer(vs_system::ElmContext*);


	// ---- surface_03


	// ---- acquire light axis
		static INT Callback_S3_AcqLgtAxis_Configure(vs_system::ElmContext*);
		static INT Callback_S3_AcqLgtAxis_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_S3_AcqLgtAxis_GetIndexBuffer(vs_system::ElmContext*);

	// ---- acquire light spot
		static INT Callback_S3_AcqLgtSpot_Configure(vs_system::ElmContext*);
		static INT Callback_S3_AcqLgtSpot_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_S3_AcqLgtSpot_GetIndexBuffer(vs_system::ElmContext*);


	// ---- ObjGrid3D
		static INT Callback_ObjGrid3D_Configure(vs_system::ElmContext*);
		static INT Callback_ObjGrid3D_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_ObjGrid3D_GetIndexBuffer(vs_system::ElmContext*);
		static INT Callback_ObjGrid3D_UpdateVertexBuffer(
					vs_system::ElmContext*,
					FLOAT *, // points
					FLOAT *  // normals
				);


	// ---- Obj
		static INT Callback_Obj_Configure(vs_system::ElmContext*);
		static INT Callback_Obj_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_Obj_GetIndexBuffer(vs_system::ElmContext*);

			static INT Callback_DotObj_Configure(vs_system::ElmContext*);
			static INT Callback_DotObj_GetVertexBuffer(vs_system::ElmContext*);
			static INT Callback_DotObj_GetIndexBuffer(vs_system::ElmContext*);

			static INT Callback_DotObj_GetVertexBuffer_Teapot(vs_system::ElmContext*);
			static INT Callback_DotObj_GetIndexBuffer_Teapot(vs_system::ElmContext*);


	// ---- Kandinsky
		static INT Callback_Kandinsky_Configure(vs_system::ElmContext*);
		static INT Callback_Kandinsky_GetVertexBuffer(vs_system::ElmContext*);

			static UINT KandinskyGetDimension(FLOAT[]);
			static UINT KandinskyGetGeoType(FLOAT []);
			static UINT KandinskyGetNumColours(FLOAT[]);
			static UINT KandinskyGetNumElements(FLOAT[]);

			static UINT KandinskyGetNumVertices(UINT index, FLOAT k[]);

			static UINT KandinskyGetColourIndex(UINT id, FLOAT k[]);
			static UINT KandinskyGetElementIndex(UINT id, FLOAT k[]);
			static UINT KandinskyNextElementIndex(UINT index, FLOAT k[]);

			static UINT KandinskyGetNumGfxElements(UINT type, UINT num_vertices);
			static UINT KandinskyGetNumGfxVertices(UINT type, UINT num_vertices);


	// ---- Mesh
		static INT Callback_Mesh_Configure(vs_system::ElmContext*);
		static INT Callback_Mesh_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_Mesh_GetIndexBuffer(vs_system::ElmContext*);
			static INT Mesh_Normals(FLOAT *, UINT, FLOAT *, FLOAT, FLOAT);
			static INT Mesh_Normals(VsVector3 [], UINT, VsVector3[], FLOAT, FLOAT);
			static INT Mesh_Filter(FLOAT *, UINT, FLOAT *, UINT, FLOAT);
			static INT Mesh_Fractal(FLOAT *, UINT, FLOAT *);
			static INT Mesh_YClip(FLOAT *, UINT, FLOAT, FLOAT);
			static INT Mesh_YOffset(FLOAT *, UINT, FLOAT);
			static INT Mesh_YScalar(FLOAT *, UINT, FLOAT);

	// ---- Mesh Edge
		static INT Callback_Mesh_Edge_Configure(vs_system::ElmContext*);
		static INT Callback_Mesh_Edge_GetVertexBuffer(vs_system::ElmContext*);

	// ---- Mesh Quad
		static INT Callback_Mesh_Quad_Configure(vs_system::ElmContext*);
		static INT Callback_Mesh_Quad_GetVertexBuffer(vs_system::ElmContext*);


	// ---- Button
		static INT Callback_Button_Configure(vs_system::ElmContext*);
		static INT Callback_Button_GetVertexBuffer(vs_system::ElmContext*);

			INT ButtonSetDefault(FLOAT[]);
			INT ButtonSetAoi(FLOAT[], UINT, FLOAT, FLOAT, FLOAT, FLOAT);
			INT ButtonSetDPT(FLOAT[], UINT);
			INT ButtonSetLineAlpha(FLOAT[], UINT);
			INT ButtonSetLocation(FLOAT[], FLOAT, FLOAT, FLOAT, FLOAT);
			INT ButtonSetTexture(FLOAT[], UINT, UINT, UINT, UINT, UINT, UINT, UINT, FLOAT);
			INT ButtonSetTextureOff(FLOAT[]);


	// ---- Img
		static INT Callback_Img_Rect_Configure(vs_system::ElmContext*);
		static INT Callback_Img_Rect_GetVertexBuffer(vs_system::ElmContext*);

			enum {
				IMG_RECT_MAX_PARAMETERS = 4, // limit is VS_MAX_ELM_FLOAT_PARAMETER
				IMG_RECT_INDEX = 0,
				IMG_RECT_TYPE  = 1,
				IMG_RECT_MODE  = 2,
				IMG_RECT_ZOOM  = 3,
				IMG_RECT_PARA_LEFT   = 4,
				IMG_RECT_PARA_RIGHT  = 5,
				IMG_RECT_PARA_BOTTOM = 6,
				IMG_RECT_PARA_TOP    = 7,
			};

			// ---- store indices
			//
			//    have to update Model::InitialiseImg1PanelRects to match!
			//
			enum {
				IMG_PTZ_OVERVIEW_PANEL_INDEX = 0,
				IMG_PTZ_FIRST_PANEL_INDEX    = 1,
				IMG_PTZ_LAST_PANEL_INDEX     = 9,
				IMG_PTZ_SOLO_PANEL_INDEX     = 10,
				IMG_PTZ_SETTINGS_PANEL_INDEX = 11,
				IMG_PTZ_HIDDEN_PANEL_INDEX   = 12,
				IMG_PTZ_VISIBLE_PANEL_INDEX  = 13,
				IMG_PTZ_GROUP_SIZE           = 3,
			};

			// ---- frames id's
			enum {
				IMG_PTZ_OVERVIEW_FRAME_ID  = 2,
				IMG_PTZ_MULTIVIEW_FRAME_ID = 3,
				IMG_PTZ_SOLOVIEW_FRAME_ID  = 5,
				IMG_PTZ_SETTINGS_FRAME_ID  = 7
			};

			// ---- modes
			enum {
				IMG_RECT_TYPE_SOLID_PTZ_VIEW   = 1,
				IMG_RECT_TYPE_LINE_PTZ_EDGE    = 2,
				IMG_RECT_TYPE_SOLID_PTZ_CENTRE = 3,

				IMG_RECT_MODE_PTF_VIEW     = 1,
				IMG_RECT_MODE_PTF_OVERVIEW = 2,
				IMG_RECT_MODE_PTF_CENTRE   = 3,

				IMG_RECT_EXP_AOI_OFF        = 0,
				IMG_RECT_EXP_AOI_ON         = 1,

				IMG_RECT_EXP_AOI_ALL        = 0, // 1,
				IMG_RECT_EXP_AOI_CENTRE     = 1, // 2,
				IMG_RECT_EXP_AOI_PTF_ACTIVE = 2  // 3,

			};

	// ---- Sphere
		static INT Callback_Sphere_Configure(vs_system::ElmContext*);
		static INT Callback_Sphere_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_Sphere_GetIndexBuffer(vs_system::ElmContext*);


	// ---- Text3D
		static INT ObjConfig::Text3D_GetInfo(
				CHAR  *te,		// [in] text
				FLOAT  bh,		// [in] base height
				UINT  *nl,		// [out] number of lines
				FLOAT *wi,		// [out] width
				FLOAT *he		// [out] height
			);

		static INT ObjConfig::Text3D_GetLines(
				CHAR  *te,		// [in] text
				FLOAT  bh,		// [in] base height
				UINT  *nl,		// [out] number of lines
				FLOAT *lp		// [out] line pair data
			);

		static INT ObjConfig::Text3D_GetChar(
				CHAR  *ch,		// [in] char
				FLOAT  colour,  // [in] colour
				CHAR   axis,    // [in] alignment
				FLOAT  width,	// [in] char width
				FLOAT  height,  // [in] char height
				FLOAT  x,		// [in] x offset
				FLOAT  y,		// [in] y offset
				FLOAT  z,		// [in] z offset
				UINT  *nl,		// [out] number of lines
				FLOAT *lp		// [out] line xy pair data
			);


	// ---- PanelGrid (not used)
		static INT Callback_PanelGrid_Configure(vs_system::ElmContext*);
		static INT Callback_PanelGrid_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanelRect (not used)
		static INT Callback_PanelRect_Configure(vs_system::ElmContext*);
		static INT Callback_PanelRect_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanoAnnular
		static INT Callback_PanoAnnular_Configure(vs_system::ElmContext*);
		static INT Callback_PanoAnnular_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanoAnnularMask ----
		static INT Callback_PanoAnnularMask_Configure(vs_system::ElmContext*);
		static INT Callback_PanoAnnularMask_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanoAnnularAoi
		static INT Callback_PanoAnnularAoi_Configure(vs_system::ElmContext*);
		static INT Callback_PanoAnnularAoi_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanoExposureAec
		static INT Callback_PanoExposureAec_Configure(vs_system::ElmContext*);

		static INT Callback_PanoExposureAec_GetVertexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_GetAnnularVertexBuffer(
					vs_system::ElmContext*,
					UINT
				);
			static INT PanoExposureAec_GetVertexBuffer_Rect(
					vs_system::ElmContext*
				);
		
		static INT Callback_PanoExposureAec_GetIndexBuffer(vs_system::ElmContext*);
		static INT PanoExposureAec_GetAnnularIndexBuffer(
					vs_system::ElmContext*,
					UINT
				);
			static INT PanoExposureAec_GetRectIndexBuffer(
					vs_system::ElmContext*
				);


	// ---- PanoExposureAec_T_ 
		static INT Callback_PanoExposureAec_T_Configure(vs_system::ElmContext*);

		static INT Callback_PanoExposureAec_T_GetVertexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetAnnularVertexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetRectVertexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetSectorVertexBuffer(vs_system::ElmContext*);

		static INT Callback_PanoExposureAec_T_GetIndexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetAnnularIndexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetRectIndexBuffer(vs_system::ElmContext*);
			static INT PanoExposureAec_T_GetSectorIndexBuffer(vs_system::ElmContext*);


	// ---- PanoSphere 
		static INT Callback_PanoSphere_Configure(vs_system::ElmContext*);
		static INT Callback_PanoSphere_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_PanoSphere_GetIndexBuffer(vs_system::ElmContext*);


	// ---- PanoLidar 
		static INT Callback_PanoLidar_Configure(vs_system::ElmContext*);
		static INT Callback_PanoLidar_GetVertexBuffer(vs_system::ElmContext*);


	// ---- PanoCylinder
		static INT Callback_PanoCylinder_Configure(vs_system::ElmContext*);
		static INT Callback_PanoCylinder_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_PanoCylinder_GetIndexBuffer(vs_system::ElmContext*);


	// ---- PanoCylinderTopRing
		static INT Callback_PanoCylinderTopRing_Configure(vs_system::ElmContext*);
		static INT Callback_PanoCylinderTopRing_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_PanoCylinderTopRing_GetIndexBuffer(vs_system::ElmContext*);


	// ---- PanoRect
		static INT Callback_PanoRect_Configure(vs_system::ElmContext*);
		static INT Callback_PanoRect_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_PanoRect_GetIndexBuffer(vs_system::ElmContext*);


	// ---- PanoRectPart
		static INT Callback_PanoRectPart_Configure(vs_system::ElmContext*);
		static INT Callback_PanoRectPart_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_PanoRectPart_GetIndexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Asteroid ----
		static INT Callback_MyStar_Asteroid_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Asteroid_GetVertexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Tetrahedron ----
		static INT GetTetrahedronTriangles(INT, INT*, INT*);
		static INT GetTetrahedronFaceVertices(INT, FLOAT, FLOAT, FLOAT*);


	// ---- MyStar_AsteroidBelt (made of recursively subdivided tetrahedrons) ----
		static INT Callback_MyStar_Asteroid_Belt_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Asteroid_Belt_GetVertexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Orbit ----
		static INT Callback_MyStar_Orbit_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Orbit_GetVertexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Halo ----
		static INT Callback_MyStar_Halo_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Halo_GetVertexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Rings ----
		static INT Callback_MyStar_Rings_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Rings_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_MyStar_Rings_GetIndexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Ring System----
		static INT Callback_MyStar_Ring_System_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Ring_System_GetVertexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Sphere
		static INT Callback_MyStar_Sphere_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Sphere_GetVertexBuffer(vs_system::ElmContext*);
		static INT Callback_MyStar_Sphere_GetIndexBuffer(vs_system::ElmContext*);


	// ---- MyStar_Starfield ----
		static INT Callback_MyStar_Starfield_Configure(vs_system::ElmContext*);
		static INT Callback_MyStar_Starfield_GetVertexBuffer(vs_system::ElmContext*);


	#include "../hpp_obj/vs_obj_dashboard.h"

	
	// ---- camera properties
		INT SetCamProps(vs_system::CamProps *cp)
		{
			cam_props = cp;
			return VS_OK;
		};


private:


	// ---- private implmentations

		class  ObjVptf;
		static ObjVptf *pi_obj_pano;

		class  ImageRoi;
		static ImageRoi *pi_image_roi;


	// ---- graphics
		vs_system::GfxInterface *gfx;
		static vs_system::CamProps *cam_props;


	#endif   // --- VS_GFX_FLAG


};   // --- ObjConfig class



}   // --- namespace vs_system
