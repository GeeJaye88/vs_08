////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model.cpp ----------
/*!
\file s3_model.cpp
\brief Implementation of the Model class
\author Gareth Edwards
*/

#include "../header/s3_model.h"
#include "../header/s3_surface_03.h"


using namespace surface_03;


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

	// ---- objects managed by app
		model::Surface_03 *surface_03 = NULL;

	// ---- display list handles
		INT handle_scene = NULL;

	// ---- time
		DOUBLE time_elapsed_ms_last = 0;
		UINT   fps = 0;

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


	// ---- NOT SHARED ----

	// ---- camera
		kamera_control_mode   = vs_system::AppKamera::Mode::ROTATE;

	// ---- target
		target_object_id      = CameraTarget::NONE;
		target_object_id_prev = CameraTarget::NONE_RUNTIME;

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

}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::Model()
{
	// allocate and construct private properties
	pi_model = new PI_Model();
	s_pi_model = pi_model;
}


// ---------- Destructor ----------
/*!
\brief Destructor
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

	// ---- project
		result = ProjectSetup();


	// ---- SHARED ----
		#include "../../surface_shared/h/model_tint_setup_incl.h"


	// ======== KeyPad - REQUIRED ========

	// ---- set param group
		SetParamGroup(PARAM_GROUP_ACQUIRE);

	// ---- init kpad
		Kpad_UpdateByParameter((INT)this, GetParamGroup());


	// ---- seed
		srand(2001);

	// --- add process pages to atlas
		INT tex_id_1 = pi_model->gfx->AddPageToAtlas();

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupGraphics ----------
/*!
\brief Setup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupGraphics()
{

	// ---- local
		INT result = VS_OK;


	// ---- gfx 
		result = pi_model->gfx->SetupGraphics();


	// ---- toolbar
		#include "../../surface_shared/hpp_toolbar/model_toolbar_incl.hpp"


	// ---- project
		result = ProjectSetupGraphics();


	// ---- update as per tab body selected
		UINT body_id = pi_model->tint_setng->GetSelectedBodyId();
		TintBodyUpdateVisibleObjects(body_id);


	// ---- init MEASURE
		TintBodyUpdateKamera(TINT_BODY_MEASURE_LID + 1);


	// ---- init landing tab ACQUIRE
		TintBodyUpdateKamera(TINT_BODY_ACQUIRE_LID);


	return result;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(vs_system::PanelContext *panel_gfx_context, CHAR *fred)
{

	// ---- local
		INT result = 0;


	// ---- get handle of current display list
		INT handle_display_list = GetPanelDisplayListHandle(panel_gfx_context);
		if ( handle_display_list == NULL ) return VS_ERROR;


	// ---- display panel ?
		BOOL display_status = TRUE;
		result = panel_gfx_context->GetDisplayStatus(&display_status);
		if ( !display_status ) return VS_ERROR;


	// ---- kamera
		result = SetMsgCameraPersistantControl();


	// ---- set fov
		FLOAT *perspective = panel_gfx_context->GetPerspective();
		perspective[0] =   40.0f;  // v fov
		perspective[1] =    0.1f;  // near
		perspective[2] = 1000.0f;  // far


	// ---- accelerate
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- setup and render view rectangle background
		result = gfx->SetMargin(panel_gfx_context->GetMargin());
		result = gfx->SetViewrect(panel_gfx_context->GetViewrect());
		result = gfx->RenderViewrect(panel_gfx_context, FALSE);


	// ---- setup and render view port AND selected display list
		result = gfx->SetBorder(panel_gfx_context->GetBorderRect());
		result = gfx->SetViewport(panel_gfx_context->GetViewport());
		if ( handle_display_list != VS_ERROR )
		{
			result = gfx->RenderViewport(handle_display_list, panel_gfx_context);
		}


	// ---- dx fonts
		#if defined(CD3DXFONT)
			result = pi_model->surface_03->Display3DText(pi_model->app_kamera[0]);
		#endif


	// ---- setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_gfx_context, TRUE);

	return result;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

// ---- local object *
GfxDevice        *props_gfx_device = pi_props->gfx_device;
IDirect3DDevice9 *d3d9_device      = props_gfx_device->d3d9_device;


*/
INT Model::CleanupGraphics()
{

	INT result = pi_model->gfx->CleanupGraphics();

	// ---- project
		result = ProjectSetupGraphics();

	return result;
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


////////////////////////////////////////////////////////////////////////////////


// ---- stuff
#include "../hpp/s3_model_kamera.hpp"
#include "../hpp/s3_model_img_process.hpp"


// ---- panels layout methods
#include "../../surface_shared/hpp/model_panel_get_display_list_handle.hpp"
#include "../hpp/s3_model_panels_set_layout.hpp"


// ---- parameter methods
#include "../hpp_param/s3_model_param_interface.hpp"


// ---- project methods
#include "../hpp/s3_model_project_framework.hpp"


// ======== Kamera methods - REQUIRED ========

#include "../../surface_shared/hpp/model_kamera_methods.hpp"


// ======== Toolbar methods - REQUIRED ========

#include "../../surface_shared/hpp_toolbar/model_toolbar_enum.hpp"
#include "../../surface_shared/hpp_toolbar/model_toolbar.hpp"


// ======== Tint methods - REQUIRED ========


// ---- surface
#include "../hpp_tint/s3_tint_enum.hpp"
#include "../hpp_tint/s3_tint_callback.hpp"
#include "../hpp_tint/s3_tint_get.hpp"
#include "../hpp_tint/s3_tint_select.hpp"
#include "../hpp_tint/s3_tint_setup.hpp"


// ---- keypad
#include "../../surface_shared/hpp_kpad/model_kpad_interface.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_callback.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_get.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_select.hpp"
#include "../../surface_shared/hpp_kpad/model_kpad_setup.hpp"


// ---- keypad -> surface tint
#include "../hpp_param/s3_model_kpad_set_with_tint_update.hpp"


////////////////////////////////////////////////////////////////////////////////

