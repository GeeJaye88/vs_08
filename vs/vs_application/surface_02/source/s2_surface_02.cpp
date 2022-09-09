////////////////////////////////////////////////////////////////////////////////

// ---------- s2_surface_02.cpp ----------
/*!
\file s2_surface_02.cpp
\brief Implementation of a sub-application class
\author Gareth Edwards
\note
*/

#include "../header/s2_surface_02.h"


using namespace model;


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_surface_02/s2_pi.hpp"
#include "../hpp_surface_02/s2_pi_configuration.hpp"
#include "../hpp_surface_02/s2_param_interface.hpp"
#include "../hpp_surface_02/s2_scene_setup.hpp"
#include "../hpp_surface_02/s2_param_setup.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Surface_02::Surface_02()
{

	// ---- allocate and construct pimp
		pi_surface_02 = new PI_Surface_02();


	// ---- local
		INT result = VS_OK;
		parameter_top = 0;


	// ---- setup parameters
		result = SetupParamStore();


	// ---- leave as group zero
		result = parameter.SetParamGroup(surface_02::Model::PARAM_GROUP_NULL);

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Surface_02::~Surface_02()
{
	delete pi_surface_02;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- framework ----------


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;

	// ---- store dependency pointers
		pi_surface_02->app_dependency = app_dep;
		pi_surface_02->gfx            = app_dep->GetGfxInterface();
		pi_surface_02->sys_context    = app_dep->GetSysContext();
		pi_surface_02->sys_output     = app_dep->GetSysOutput();

	return VS_OK;
}


// ---------- framework / SetupGraphics ----------
/*!
\brief SetupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetupGraphics(vs_system::AppKamera *kamera)
{

	// ---- local
		INT result = VS_OK;


	// ---- store param group
		INT param_group = parameter.GetParamGroup();


	// ---- test: setup size
		vs_system::GfxInterface *gfx = pi_surface_02->gfx;


	// ---- param
		INT offset = 0;
		result = SetupParamAcquire(&offset);
		result = SetupParamSurface(&offset);
		result = SetupParamWeld(&offset);
		result = SetupParamLights(&offset);
		result = SetupParamFunction(&offset);


	// ---- scene
		result = SetupExploreCamera(SceneSetupMode::DEFAULT, kamera);
		result = SetupAcquireLightParam();
		result = SetupGraphicsLighting(model::Surface_02::SceneSetupMode::ACQUIRE);
		result = SetupExploreMaterials(model::Surface_02::SceneSetupMode::EXPLORE);
		result = SetupExploreMaterials(model::Surface_02::SceneSetupMode::DEFAULT);


	// ---- restore group
		parameter.SetParamGroup(param_group);


	// ---- setup "the" display list
		result = SetupDisplayList();


	// ---- setup objects
		for (int i = 0; i < PI_Surface_02::MAX_GFX_OBJ_LIST; i++)
		{
			PI_Surface_02::GfxObjectItem *gfx_obj =
				&pi_surface_02->gfx_object_list[i];

			if ( gfx_obj->setup != NULL )
			{
				result = gfx_obj->setup(
							gfx_obj->index,
							this
						);
			}
		}

		
		// ---- state
		pi_surface_02->runtime = FALSE;

	return VS_OK;
}


// ---------- framework / Display ----------
/*!
\brief SetupDisplay
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetupDisplay(
		vs_system::AppKamera *kamera
	)
{

	// ---- local
		INT result = 0;
		VsVector3 translate;
		FLOAT rx, ry, rz;


	// ---- get stuff
		kamera->GetTranslate(&translate);
		kamera->GetRotate(&rx, &ry, &rz);


	// ---- push
		pi_surface_02->matrix_stack->Push();


	// ---- display objects
		for (int i = 0; i < PI_Surface_02::MAX_GFX_OBJ_LIST; i++)
		{

			PI_Surface_02::GfxObjectItem *gfx_obj =
				&pi_surface_02->gfx_object_list[i];

			if ( gfx_obj->setup != NULL )
			{

				if  ( gfx_obj->display != ObjectExpSpot_Display )
				{
					pi_surface_02->matrix_stack->Push();
					pi_surface_02->matrix_stack->Rotate(rz, 'Z');
					pi_surface_02->matrix_stack->Rotate(rx, 'X');
					pi_surface_02->matrix_stack->Rotate(ry, 'Y');
					pi_surface_02->matrix_stack->Translate(
									translate.x,
									translate.y,
									translate.z
								);

					result = gfx_obj->display(
								gfx_obj->index,
								this,
								gfx_obj->visible
							);

					pi_surface_02->matrix_stack->Pop();
				}
				else
				{
					result = gfx_obj->display(
								gfx_obj->index,
								this,
								gfx_obj->visible
							);

				}

			}

		}

	// ---- pop
		pi_surface_02->matrix_stack->Pop();


	return VS_OK;
}


// ---------- framework / CleanupGraphics ----------
/*!
\brief CleanupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::CleanupGraphics()
{
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- sub-framework ----------

#include "../hpp_surface_02/s2_sub_framework.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- objects ----------

#include "../hpp_obj/s2_obj_heightfield.hpp"

#include "../hpp_obj/s2_obj_acq_lgt_axis.hpp"
#include "../hpp_obj/s2_obj_acq_lgt_spot.hpp"

#include "../hpp_obj/s2_obj_surf_axis.hpp"
#include "../hpp_obj/s2_obj_surf_base.hpp"
#include "../hpp_obj/s2_obj_surf_edge.hpp"
#include "../hpp_obj/s2_obj_surf_mesh.hpp"
#include "../hpp_obj/s2_obj_surf_quad.hpp"

#include "../hpp_obj/s2_obj_exp_lgt_axis.hpp"
#include "../hpp_obj/s2_obj_exp_lgt_spot.hpp"

#include "../hpp_obj/s2_obj_loi_spot.hpp"
#include "../hpp_obj/s2_obj_loi_rect.hpp"
#include "../hpp_obj/s2_obj_loi_rule.hpp"
#include "../hpp_obj/s2_obj_loi_axis.hpp"


////////////////////////////////////////////////////////////////////////////////
