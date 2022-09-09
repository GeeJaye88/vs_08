////////////////////////////////////////////////////////////////////////////////

// ---------- s2_pi.hpp ----------
/*!
\file s2_pi.hpp
\brief private implmentation for s2_surface_02
\author Gareth Edwards
*/

// ---------- private implementation of PI_surface_02 class ----------
class Surface_02::PI_Surface_02
{

public:

	// ---- enumerated constants
		enum
		{

			// ---- gfx objects
				//
				//  4 * explore light spot (MAX_EXP_LGT_OBJ)
				//  4 * explore light rigs (MAX_EXP_LGT_OBJ)
				//  1 * surface mesh 
				//  1 * surface edge
				//  1 * surface base
				//  1 * surface axis
				//  1 * surface quad
				//  1 * explore loi spot 
				//  1 * explore loi axis
				//  1 * explore loi rect
				//  1 * explore loi rule
				//  1 * acquire light rig
				//  1 * acquire light spot
				//
				MAX_GFX_OBJ_LIST = 19,
				

			// ---- gfx object [ index, setup, display, callback, visible ] version 
				GOB_EXP_LGT_0_IDX     = 0,
				GOB_EXP_LGT_1_IDX     = 1,
				GOB_EXP_LGT_2_IDX     = 2,
				GOB_EXP_LGT_3_IDX     = 3,
				GOB_EXP_LGT_RIG_0_IDX = 4,
				GOB_EXP_LGT_RIG_1_IDX = 5,
				GOB_EXP_LGT_RIG_2_IDX = 6,
				GOB_EXP_LGT_RIG_3_IDX = 7,
				GOB_SURF_MESH_IDX     = 8,
				GOB_SURF_BASE_IDX     = 9,
				GOB_SURF_EDGE_IDX     = 10,
				GOB_SURF_AXIS_IDX     = 11,
				GOB_SURF_QUAD_IDX     = 12,
				GOB_EXP_LOI_SPOT_IDX  = 13,
				GOB_EXP_LOI_AXIS_IDX  = 14,
				GOB_EXP_LOI_RECT_IDX  = 15,
				GOB_EXP_LOI_RULE_IDX  = 16,
				GOB_ACQ_LGT_RIG_IDX   = 17,
				GOB_ACQ_LGT_SPOT_IDX  = 18,


			// ---- gfx elements
				//
				//  4 * explore light spots
				//  4 * explore light axis
				//  1 * surface mesh 
				//  1 * surface edge
				//  1 * surface base
				//  1 * surface axis
				//  1 * surface quad
				//  # * explore loi spot ( # = MAX_EXP_LOI_ )
				//  # * explore loi axis ( # = MAX_EXP_LOI_ )
				//  # * explore loi rect ( # = MAX_EXP_LOI_ )
				//  # * explore loi rule ( # = MAX_EXP_LOI_ )
				//  1 * acquire light axis
				//  1 * acquire light spot
				//
				MAX_GFX_ELM_LIST          = 13,
				MAX_GFX_ELM_HANDLES       = 16,


			// ---- gfx element [ group, item ] handle list
				ELM_EXP_LGT_SPOT_LIST_IDX = 0,
				ELM_EXP_LGT_RIG_LIST_IDX  = 1,
				ELM_SURF_BASE_LIST_IDX    = 2,
				ELM_SURF_MESH_LIST_IDX    = 3,
				ELM_SURF_EDGE_LIST_IDX    = 4,
				ELM_SURF_AXIS_LIST_IDX    = 5,
				ELM_SURF_QUAD_LIST_IDX    = 6,
				ELM_EXP_LOI_SPOT_LIST_IDX = 7,
				ELM_EXP_LOI_RECT_LIST_IDX = 8,
				ELM_EXP_LOI_RULE_LIST_IDX = 9,
				ELM_EXP_LOI_AXIS_LIST_IDX = 10,
				ELM_ACQ_LGT_AXIS_LIST_IDX = 11,
				ELM_ACQ_LGT_SPOT_LIST_IDX = 12,


			// ---- # of  things
				MAX_ACQ_IMG_BUFFER = 5,
				MAX_ACQ_LIGHT      = 4,
				MAX_ACQ_PARAM      = 6,
				MAX_EXP_LGT        = 5,  // only used in Surface_02::SetupParamLights
				MAX_EXP_LOI_       = 7,
				MAX_EXP_LGT_OBJ    = 4,
				MAX_PARAM_STORE    = 7
		};


	// ---------- methods ----------


	// ---- cdtor
		PI_Surface_02();
		~PI_Surface_02();


	// --- visibility
		INT SetObjectsVisible();
		INT SetObjectVisible(UINT, BOOL);

	// ---- targets
		INT SetupTargetLoi();


	// ---------- properties ----------


	// ---- state
		BOOL runtime = TRUE;


	// ---- objects


	// ---- injected dependencies managed by VS
		vs_system::AppDependency *app_dependency = NULL;
		vs_system::GfxInterface  *gfx            = NULL;
		vs_system::SysContext    *sys_context    = NULL;
		vs_system::SysOutput     *sys_output     = NULL;


	// ---- injected dependencies managed by application
		vs_system::ObjConfig     *obj_config     = NULL;
		vs_system::ImgProps      *img_props      = NULL;


	// ---- managed by project surface_02
		vs_system::VsMatrixStack *matrix_stack   = NULL;


	// ---- gfx objects


	// ---- gfx object item
		struct GfxObjectItem
		{
			INT  index;
			INT (*setup)(INT, Surface_02 *);
			INT (*display)(INT, Surface_02 *, BOOL);
			BOOL  visible;
		};
		

	// ---- gfx object item list
		GfxObjectItem gfx_object_list[MAX_GFX_OBJ_LIST] =
		{

			{ 0, ObjectExpSpot_Setup,     ObjectExpSpot_Display,    0 },
			{ 1, ObjectExpSpot_Setup,     ObjectExpSpot_Display,    0 },
			{ 2, ObjectExpSpot_Setup,     ObjectExpSpot_Display,    0 },
			{ 3, ObjectExpSpot_Setup,     ObjectExpSpot_Display,    0 },

			{ 0, ObjectExpLgtAxis_Setup,  ObjectExpLgtAxis_Display, 0 },
			{ 1, ObjectExpLgtAxis_Setup,  ObjectExpLgtAxis_Display, 0 },
			{ 2, ObjectExpLgtAxis_Setup,  ObjectExpLgtAxis_Display, 0 },
			{ 3, ObjectExpLgtAxis_Setup,  ObjectExpLgtAxis_Display, 0 },

			{ 0, ObjectSurfMesh_Setup,    ObjectSurfMesh_Display,   1 },
			{ 0, ObjectSurfBase_Setup,    ObjectSurfBase_Display,   1 },
			{ 0, ObjectSurfEdge_Setup,    ObjectSurfEdge_Display,   1 },
			{ 0, ObjectSurfAxis_Setup,    ObjectSurfAxis_Display,   1 },
			{ 0, ObjectSurfQuad_Setup,    ObjectSurfQuad_Display,   1 },

			{ 0, ObjectLoiSpot_Setup,     ObjectLoiSpot_Display,    1 },
			{ 0, ObjectLoiAxis_Setup,     ObjectLoiAxis_Display,    1 },
			{ 0, ObjectLoiRect_Setup,     ObjectLoiRect_Display,    1 },
			{ 0, ObjectLoiRule_Setup,     ObjectLoiRule_Display,    1 },

			{ 0, ObjectAcqAxis_Setup,     ObjectAcqAxis_Display,    1 },
			{ 0, ObjectAcqSpot_Setup,     ObjectAcqSpot_Display,    1 },

		};

		
	// ---- gfx element
		INT scene_dspl_list_handle;
		INT gfx_elem_handle[MAX_GFX_ELM_LIST][MAX_GFX_ELM_HANDLES];


	// ---- managed heightfield


	// ---- y heightfield generated during mesh get vertex
		vs_system::FloatBuffer *y_heightfield = NULL;


	// ---- demo byte heightfields

		vs_system::Heightfield *terrain_bump_map = NULL;
		INT terrain_bump_map_num_vertices = 0;

		vs_system::Heightfield *rr_bump_map[4] = { 0, 0, 0, 0 };
		UINT rr_bump_map_index = 0;
		INT  rr_bump_map_num_vertices = 0;


	// -------- SURFACE CONFIGURATION properties -------- 

		struct Surface_02_Config
		{

			// ---- note: see s2_surface_02_config_acquire.hpp


			// ---- source

				struct SourceSurfaceSetup
				{
					FLOAT  size = 6.0f;
					FLOAT  y_scalar = 1.0f;
				};
				SourceSurfaceSetup source_surface_table[7] =
				{
					{ 6,   0.10f },
					{ 8,   0.10f },
					{ 10,  0.05f },
					{ 10,  0.15f },
					{ 10,  0.05f },
					{ 23,  0.10f },
					{ 17,  1.00f },
				};


			// ---- acquire

				struct AcquireAtlasPage
				{
					UINT   ref    = 100;
					UINT   width  = 0;
					UINT   height = 0;
					UINT   bytes  = 0;
				} acquire_atlas_page;


				// dimensions
				//
				// --- note - divisions per unit marker param:
				//
				//   upto
				//   size  dpu  major minor
				//    100   1    10    5
				//    10    10   10    5
				//    5     20   20    5
				//
				// --- note - height only used in Callback_SurfAxis_GetVertexBuffer
				//     (in vs_s2_surf_axis.hpp) to calculate the size of the Y axis
				//
				//
				struct AcquireDimensions
				{
					FLOAT  size     = 6;
					FLOAT  width    = size;
					FLOAT  height   = size;
					FLOAT  depth    = size;
					FLOAT  y_scalar = 1;
				} acquire_dimensions;


				struct AcquireCamera
				{
					UINT  width    = 1024;
					UINT  height   = 1024;
					UINT  bitdepth = 8;
					UINT  rect_width  = width;
					UINT  rect_height = height;
					VsIntRectangle rect = { 0, 0, width - 1, height - 1 };
				} acquire_camera;


				// buffers
				vs_system::UnsignedShortBuffer aquire_lumina_buffer[MAX_ACQ_LIGHT];
				vs_system::ImgBuffer aquire_shader_buffer[MAX_ACQ_IMG_BUFFER];
				vs_system::ImgBuffer aquire_analysis_buffer[MAX_ACQ_IMG_BUFFER];


				// demo surface
				struct AcquireSurface
				{
					UINT version  = vs_system::ObjConfig::MESH_VERSION_CAMERA;
					UINT selected = version;
				} acquire_surface;


				// light
				struct AcquireLightParam
				{
					FLOAT intensity =  1;
					FLOAT distance  =  1;
					FLOAT elevation = 30;
					FLOAT azimuth   =  0;
					FLOAT scalar    =  1;
				} acquire_light_param;


				// ambient
				struct AcquireAmbientParam
				{
					FLOAT intensity = 0.5f;
				} acquire_ambient_param;


				struct AcquireLightEnabled
				{
					BOOL  colour = FALSE;
					BOOL  specular = FALSE;
					BOOL  state[MAX_ACQ_LIGHT] = { 1, 0, 0, 0 };
				} acquire_light_enabled;


				// lights
				UINT acquire_light_index = 0;
				vs_system::VsLight acquire_light[MAX_ACQ_LIGHT];
				VsVector3 zero = { 0.0f, 0.0f, 0.0f };
				VsVector3 acquire_location[MAX_ACQ_LIGHT] = { zero, zero, zero, zero, };


				// material
				vs_system::VsMaterial acquire_material;


				// heightfield
				struct
				{
					FLOAT *buffer = NULL;
					UINT   stride = 0;
					FLOAT  scalar = 1;
				} acquire_height_field_param;


				// y height field
				//
				// - stride = 1
				// - dimension = acquire dimension + 1
				//
				struct AquireHeightField
				{
					UINT   stride    = 0;
					UINT   width     = 0;
					UINT   extent    = 0;
					UINT   dimension = 0;
					FLOAT *array     = NULL;
					FLOAT  max       = 0;
					FLOAT  min       = 0;
					FLOAT  scalar    = 1;
				} aquire_height_field;


			// ---- explore kamera

				struct ExploreKamera
				{
					FLOAT pan  = -30;
					FLOAT tilt =  30;

					BOOL  max_scalar_selected = TRUE;
					FLOAT max_scalar = 1.00f;
					FLOAT min_scalar = 0.25f;

				} explore_kamera;


				//struct ExploreLightEnabled
				//{
				//	BOOL  colour = FALSE;
				//	BOOL  specular = FALSE;
				//	BOOL  state[MAX_EXP_LGT] = { 1, 0, 0, 0, 0 };
				//} explore_light_enabled;


				//vs_system::VsLight explore_light[MAX_EXP_LGT_OBJ];
				//VsVector3 zero = { 0.0f, 0.0f, 0.0f };
				//VsVector3 explore_location[MAX_EXP_LGT_OBJ] = { zero, zero, zero, zero, };


			// ---- 3D surface mesh
				struct
				{
					// ---- dimensions
						UINT rows      = 256;
						UINT cols      = 256;

					// ---- modes
						BOOL solid     = TRUE;
						BOOL textured  = TRUE;
						UINT display   = MeshDisplayMode::ST;
						UINT density   = MeshDensityMode::LOW;

				} surface_mesh;


			// ---- target
				UINT  target_loi_index     = 0;
				BOOL  target_loi_visible   = 0;
				BOOL  target_ruler_visible = 0;
				BOOL  target_transform     = 0;

				VsVector2 target_intersect[2];
				BOOL      target_init[MAX_EXP_LOI_];
				VsVector3 target_loi[MAX_EXP_LOI_];
				FLOAT     target_rot[MAX_EXP_LOI_];


		} _configuration;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Surface_02::PI_Surface_02::PI_Surface_02()

{
	// ---- local
		INT result = VS_OK;


	// ---- zap matrix
		matrix_stack = new vs_system::VsMatrixStack();
		matrix_stack->Initialise();


	// ---- empty display lists
		scene_dspl_list_handle = NULL;
		for (UINT i = 0; i < MAX_GFX_ELM_LIST; i++)
		{
			for (UINT j = 0; j < MAX_GFX_ELM_HANDLES; j++)
			{
				gfx_elem_handle[i][j] = NULL;
			}
		}


	// ---- loi
		result = SetupTargetLoi();

}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Surface_02::PI_Surface_02::~PI_Surface_02()
{

	if ( matrix_stack != NULL )
	{
		delete matrix_stack;
		matrix_stack = NULL;
	}

	if ( y_heightfield != NULL )
	{
		delete y_heightfield;
		y_heightfield = NULL;
	}

	if ( terrain_bump_map != NULL )
	{
		delete terrain_bump_map;
		terrain_bump_map = NULL;
	}

	for (UINT i = 0; i < 4; i++)
	{
		if ( rr_bump_map[i] != NULL )
		{
			delete rr_bump_map[i];
			rr_bump_map[i] = NULL;
		}
	}
}


// ---------- SetAllObjectsVisible ----------
/*!
\brief set all objects visible
\author Gareth Edwards
*/
INT Surface_02::PI_Surface_02::SetObjectsVisible()
{

	// ---- all on

		gfx_object_list[GOB_EXP_LGT_0_IDX].visible     = 0;
		gfx_object_list[GOB_EXP_LGT_1_IDX].visible     = 0;
		gfx_object_list[GOB_EXP_LGT_2_IDX].visible     = 0;
		gfx_object_list[GOB_EXP_LGT_3_IDX].visible     = 0;

		gfx_object_list[GOB_EXP_LGT_RIG_0_IDX].visible = 0;
		gfx_object_list[GOB_EXP_LGT_RIG_1_IDX].visible = 0;
		gfx_object_list[GOB_EXP_LGT_RIG_2_IDX].visible = 0;
		gfx_object_list[GOB_EXP_LGT_RIG_3_IDX].visible = 0;

		gfx_object_list[GOB_SURF_MESH_IDX].visible     = 1;
		gfx_object_list[GOB_SURF_BASE_IDX].visible     = 1;
		gfx_object_list[GOB_SURF_EDGE_IDX].visible     = 1;
		gfx_object_list[GOB_SURF_AXIS_IDX].visible     = 1;
		gfx_object_list[GOB_SURF_QUAD_IDX].visible     = 0;

		gfx_object_list[GOB_EXP_LOI_SPOT_IDX].visible  = 1;
		gfx_object_list[GOB_EXP_LOI_AXIS_IDX].visible  = 1;
		gfx_object_list[GOB_EXP_LOI_RECT_IDX].visible  = 1;
		gfx_object_list[GOB_EXP_LOI_RULE_IDX].visible  = 1;

		gfx_object_list[GOB_ACQ_LGT_RIG_IDX].visible   = 1;

	return VS_OK;
}


// ---------- SetObjectVisible ----------
/*!
\brief set object visibility
\author Gareth Edwards
*/
INT Surface_02::PI_Surface_02::SetObjectVisible(UINT index, BOOL visible)
{

	// ---- all on
		gfx_object_list[index].visible = visible;

	return VS_OK;
}


// ---------- SetupTargetLoi ----------
/*!
\brief initialise target LoI (Locations of Interest)
\author Gareth Edwards
*/
INT Surface_02::PI_Surface_02::SetupTargetLoi()
{
	
	// ---- default LoI ( Locations of Interest )

	// ---- initialised flag
		for (UINT i = 0; i < MAX_EXP_LOI_; i++)
		{
			_configuration.target_init[i] = FALSE;
		}

	// ---- dimensions
		FLOAT width = _configuration.acquire_dimensions.width;
		FLOAT depth = _configuration.acquire_dimensions.depth;

	// ----- location - middle + heaxgonal distribution
		VsVector3 target_loi[MAX_EXP_LOI_];
		target_loi[0] = { 0, 0, 0 };
		FLOAT radius = depth / 3;
		for (UINT i = 1; i < MAX_EXP_LOI_; i++)
		{
			FLOAT radian = (FLOAT)((i-1)*60) * (FLOAT)VS_RADIANS_PER_DEGREE;
			target_loi[i].x =  radius * (FLOAT)cos(radian);
			target_loi[i].z = -radius * (FLOAT)sin(radian);
			target_loi[i].y = 0;
			target_loi[i].y = 0;
		}
		memcpy((VOID *)_configuration.target_loi, target_loi, sizeof(target_loi));

	// ---- rotation
		for (UINT i = 0; i < MAX_EXP_LOI_; i++)
		{
			_configuration.target_rot[i] = 0;
		}


	return VS_OK;
}

