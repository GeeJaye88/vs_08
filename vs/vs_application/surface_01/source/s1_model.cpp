////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model.cpp ----------
/*!
\file s1_model.cpp
\brief Implementation of the Model class
\author Gareth Edwards
*/

#include "../header/s1_model.h"
#include "../header/s1_define.h"

using namespace surface_01;


// ---- alternative model setups
//
// #define S1_EXPLODED_CUBE
// #define S1_WAVEFRONT_OBJ
//
// note: if neither defined then choice defaults to StarSystem
//

// ---------- Private Implementation of PI_Model class ----------
class Model::PI_Model
{

public:


	// ---- cdtor
		PI_Model();
		~PI_Model();


	// ---- SHARED ----
	
		#include "../../surface_shared/h/model_pi_class_incl.h"


	// ---- NOT SHARED ----


	// ---- parameter & methods included for facade copy
		vs_system::Parameter parameter;
		#include "vs/vs_library/header/vs_parameter_methods.h"


	// ---- SOLAR SYSTEM METHODS ----


	// ---- cube methods
		INT SetupExplodedCubeDisplay();


	// ---- solar system
		INT InitStarSystemObjectsGfx(SystemObject *);
		INT InitStarSystemObjects(SystemObject *);
		INT SetupStarSystemDisplay(SystemObject *);


	// ---- wavestore
		INT SetupWavefrontDisplay();


	// ---- STAR SYSTEM PROPERTIES ----


	// ---- object type
		//
		// See 
		//
		//
		enum ObjectGeometryVersion
		{
			SPHERE   = 1,
			CUBE     = 2,
			DOTOBJ   = 3,
			MAX      = 3
		};
		INT object_geometry_version = ObjectGeometryVersion::SPHERE;


	// ---- objects - managed locally
		SystemObject *system_object  = NULL;
		SystemParam  *system_param   = NULL;
		SystemIO     *system_io      = NULL;


	// ---- user atlas page
		INT user_atlas_page_logical_id = 0;


	// ---- image capture
		UINT   img_capture_event_id   = 666;
		DOUBLE img_capture_elapsed_ms = 0;


	// ---- default matrix
		UINT gfx_view_matrix_index = 1;


	// ---- solar system target planet
		CHAR *target_planet_name[12] = 
			{
				"Sun",
				"Mercury",
				"Venus",
				"Earth",
				"Mars",
				"Ceres 1",
				"Jupiter",
				"Saturn",
				"Uranus",
				"Neptune",
				"Pluto",
				"Moon",
			};


	// ---- solar system geometry ( in draw sequence )
		INT recursive_depth_count;
		INT object_index;
		INT mtl_index;

		INT handle_starfield_dsplst;
		INT handle_starfield[SystemParam::MAX_SYS_OBJECT];

		INT handle_asteroid_dsplst;
		INT handle_asteroid[SystemParam::MAX_SYS_OBJECT];

		INT handle_sphere_dsplst;
		INT handle_sphere[SystemParam::MAX_SYS_OBJECT];

		INT handle_cube_dsplst;
		INT handle_cube[SystemParam::MAX_SYS_OBJECT];

		INT handle_dotobj_dsplst;
		INT handle_dotobj[SystemParam::MAX_SYS_OBJECT];

		INT handle_orbit_dsplst;
		INT handle_orbit[SystemParam::MAX_SYS_OBJECT];

		INT handle_ring_dsplst;
		INT handle_ring[SystemParam::MAX_SYS_OBJECT];

		INT handle_halo_dsplst;
		INT handle_halo[SystemParam::MAX_SYS_OBJECT];

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::PI_Model::PI_Model()
{

	// ---- SHARED ----

		#include "../../surface_shared/h/model_pi_ctor_incl.h"


	// ---- camera
		kamera_control_mode     = vs_system::AppKamera::Mode::XYZ;
		kamera_scalar_increment = 1.0f;


	// ---- target
		target_object_id      = CameraTarget::NONE;
		target_object_id_prev = CameraTarget::NONE_RUNTIME;


	// ---- NOT SHARED ----


	// ---- managed by app	


	// ---- allocate and construct object config
		system_object = new SystemObject(0,"Star System"); 
		system_param  = new SystemParam();
		system_io     = new SystemIO();


	// ---- init geometry
		recursive_depth_count = 0;
		object_index = 0;
		mtl_index = 0;

		handle_asteroid_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_asteroid[i] = 0;

		handle_cube_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_cube[i] = 0;

		handle_dotobj_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_dotobj[i] = 0;

		handle_halo_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_halo[i]  = 0;
	
		handle_orbit_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_orbit[i] = 0;

		handle_ring_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_ring[i] = 0;

		handle_sphere_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_sphere[i] = 0;

		handle_starfield_dsplst = 0;
		for (UINT i = 0; i < SystemParam::MAX_SYS_OBJECT; i++) handle_starfield[i] = 0;

}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::PI_Model::~PI_Model()
{

	// ---- SHARED ----
		#include "../../surface_shared/h/model_pi_dtor_incl.h"


	// ---- NOT SHARED ----


	// ---- managed by app
		if ( system_object != NULL ) { delete system_object;system_object = NULL; }
		if ( system_param  != NULL ) { delete system_param;system_param   = NULL; }
		if ( system_io     != NULL ) { delete system_io;system_io         = NULL; }

}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
Model::Model()
{
	pi_model = new PI_Model();
	s_pi_model = pi_model;
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
Model::~Model()
{
	if ( pi_model != NULL )
	{
		delete pi_model;
		s_pi_model = pi_model = NULL;
	}
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;


	// ---- store dependency pointers
		pi_model->app_dependency = app_dep;
		pi_model->gfx            = app_dep->GetGfxInterface();
		pi_model->sys_context    = app_dep->GetSysContext();
		pi_model->sys_output     = app_dep->GetSysOutput();


	// ---- allocate and construct objects

		result = ReadImgProps();


	// ======== PROJECT ========


	// ---- needed by exploded cube and wavestore obj
		result = InitAtlasPage();
		result = InitWavefront();

	// ---- always needed ( for tabbed interface!!! )
		result = InitStarSystem();
	

	// ---- SHARED ----
		#include "../../surface_shared/h/model_tint_setup_incl.h"


	// ======== KeyPad - REQUIRED ========

	// ---- set param group
		SetParamGroup(PARAM_GROUP_CAMERA);

	// ---- init kpad
		Kpad_UpdateByParameter((INT)this, GetParamGroup());


	// ---- seed
		srand(2001);

	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupGraphics()
{

	// ---- local
		INT result = 0;


	// ---- setup
		result = pi_model->gfx->SetupGraphics();

			
	// ---- toolbar
		#include "../../surface_shared/hpp_toolbar/model_toolbar_incl.hpp"


	// ======== PROJECT ========

		#if defined(S1_EXPLODED_CUBE)
			result = InitExplodedCubeScene();
			result = InitExplodedCubeObject();
		#elif defined(S1_WAVEFRONT_OBJ)
			result = InitWavefrontScene();
			result = InitWavefrontObject();
		#else
			result = InitStarSystemScene();
			result = InitStarSystemObjects();
		#endif


	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display()
{

	// ---- local
		INT result = 0;


	// ---- camera
		result = SetMsgCameraPersistantControl();


	// ======== PROJECT ========

		#if defined(S1_EXPLODED_CUBE)
			result = SetupExplodedCubeDisplay();
		#elif defined(S1_WAVEFRONT_OBJ)
			result = SetupWavefrontDisplay();
		#else
			result = SetupStarSystemDisplay();
		#endif

	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(
		vs_system::PanelContext *panel_gfx_context,
		CHAR *fred  // not used
	)
{

	// ---- local
		INT result = 0;
	

	// ---- display panel ? - ( _shared/hpp/model_panel_get_display_list_handle.hpp )
		BOOL display_status = TRUE;
		result = panel_gfx_context->GetDisplayStatus(&display_status);
		if (!display_status) return VS_ERROR;


	// ---- get handle of current display list
		INT handle_display_list = GetPanelDisplayListHandle(panel_gfx_context);
		if ( handle_display_list == NULL ) return VS_ERROR;


	// ---- exclude unwanted geometry versions
		if ( IsObjectGeometry(handle_display_list)  )
		{
			if ( ! IsSelectedObjectGeometry(handle_display_list) )
			{
				return VS_OK;
			}
		}


	// ---- exclude unwanted geometry versions
		/*
		switch ( pi_model->object_geometry_version )
		{
			case PI_Model::CUBE:
				if ( handle_display_list == pi_model->handle_sphere_dsplst || handle_display_list == pi_model->handle_dotobj_dsplst )
					return VS_OK;
				break;
			case PI_Model::DOTOBJ:
				if ( handle_display_list == pi_model->handle_sphere_dsplst || handle_display_list == pi_model->handle_cube_dsplst )
					return VS_OK;
				break;
			case PI_Model::SPHERE:
				if ( handle_display_list == pi_model->handle_cube_dsplst || handle_display_list == pi_model->handle_dotobj_dsplst )
					return VS_OK;
				break;
		}
		*/

	// ---- camera
		FLOAT *perspective = panel_gfx_context->GetPerspective();
		perspective[0] =     40.00;  // v fov
		perspective[1] =     0.01f;  // near
		perspective[2] = 100000.0f;  // far


	// ---- accelerate
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- setup and render view rectangle background
		result = gfx->SetMargin(panel_gfx_context->GetMargin());
		result = gfx->SetViewrect(panel_gfx_context->GetViewrect());
		result = gfx->RenderViewrect(panel_gfx_context, FALSE);


	// ---- setup and render view port AND selected display list
		result = gfx->SetBorder(panel_gfx_context->GetBorderRect());
		result = gfx->SetViewport(panel_gfx_context->GetViewport());
		result = gfx->RenderViewport(handle_display_list, panel_gfx_context);


	// ---- setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_gfx_context, TRUE);

	return result;
}


/*

vs_system::ElmContext *elm_context = NULL;
result = pi_model->gfx->GetElementContext(handle_display_list, &elm_context);

INT handle_dsplst[3] = { pi_model->handle_sphere_dsplst, pi_model->handle_cube_dsplst , pi_model->handle_dotobj_dsplst };

INT handle = NULL;
switch ( pi_model->object_geometry_version )
{
	case PI_Model::ObjectGeometryVersion::CUBE:
		handle = pi_model->handle_cube_dsplst;
		break;
	case PI_Model::ObjectGeometryVersion::DOTOBJ:
		handle = pi_model->handle_dotobj_dsplst;
		break;
	case PI_Model::ObjectGeometryVersion::SPHERE:
		handle = pi_model->handle_sphere_dsplst;
		break;
	default:
		break;
}

INT match = 999;
for (UINT i = 0; i < 3; i++)
{
	if ( handle == handle_dsplst[i] )
	{
		match = i;
	}
}
if  (match != 999 && match != pi_model->object_geometry_version - 1 )
	return VS_OK;

*/

// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::CleanupGraphics()
{
	INT hr = pi_model->gfx->CleanupGraphics();
	return hr;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Get PIMP stuff ----------

vs_system::AppDependency * Model::GetAppDependency()
{
	return pi_model->app_dependency;
}

INT Model::GetSystemParam(SystemParam **sp)
{
	*sp = pi_model->system_param;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- stuff
#include "../hpp/s1_model_atlas_page.hpp"
#include "../hpp/s1_model_kamera.hpp"
#include "../hpp/s1_model_img_props.hpp"
#include "../hpp/s1_model_img_capture.hpp"
#include "../hpp/s1_model_my_star.hpp"


// ---- panels layout methods
#include "../../surface_shared/hpp/model_panel_get_display_list_handle.hpp"
#include "../hpp/s1_model_panels_set_layout.hpp"


// ---- parameter methods
#include "../hpp_param/s1_model_param_interface.hpp"
#include "../hpp_param/s1_model_param_setup.hpp"


// ---- object methods
#include "../hpp/s1_model_obj_exploded_cube.hpp"
#include "../hpp/s1_model_obj_wavefront.hpp"
#include "../hpp/s1_model_obj_star_system.hpp"


// ======== Kamera methods - REQUIRED ========

#include "../../surface_shared/hpp/model_kamera_methods.hpp"


// ======== Toolbar methods - REQUIRED ========

#include "../../surface_shared/hpp_toolbar/model_toolbar_enum.hpp"
#include "../../surface_shared/hpp_toolbar/model_toolbar.hpp"


// ======== Tint methods - REQUIRED ========


// ---- surface
#include "../hpp_tint/s1_tint_enum.hpp"
#include "../hpp_tint/s1_tint_select.hpp"
#include "../hpp_tint/s1_tint_setup.hpp"
#include "../hpp_tint/s1_tint_callback.hpp"
#include "../hpp_tint/s1_tint_get.hpp"


// ---- keypad

#include "../../surface_shared/hpp_kpad/model_kpad_interface.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_callback.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_get.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_select.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_setup.hpp"


// ---- keypad -> surface tint
#include "../hpp_param/s1_model_kpad_set_with_tint_update.hpp"


////////////////////////////////////////////////////////////////////////////////


