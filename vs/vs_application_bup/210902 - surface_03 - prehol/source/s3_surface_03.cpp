////////////////////////////////////////////////////////////////////////////////

// ---------- s3_portal_01.cpp ----------
/*!
\file s3_portal_01.cpp
\brief Implementation of a sub-application class
\author Gareth Edwards
\note
*/

#include "../header/s3_surface_03.h"


using namespace model;


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_surface_03/s3_pi.hpp"
#include "../hpp_surface_03/s3_pi_configuration.hpp"
#include "../hpp_surface_03/s3_param_interface.hpp"
#include "../hpp_surface_03/s3_scene_setup.hpp"
#include "../hpp_surface_03/s3_param_setup.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Surface_03::Surface_03()
{

	// ---- allocate and construct pimp
		pi_surface_03 = new PI_Surface_03();


	// ---- local
		INT result = VS_OK;
		INT offset = 0;


	// --- sub-framework setup
		result = SetupParamAcquire(&offset);
		result = SetupParamSurface(&offset);
		result = SetupParamWeld(&offset);
		result = SetupParamLights(&offset);
		result = SetupParamFunction(&offset);


	// ---- leave as zero
		result = parameter.SetParamGroup(surface_03::Model::PARAM_GROUP_NULL);


	/*

	// ----- init
		UINT   mesh_section_num_samples = 257;
		FLOAT *mesh_section_y_samples = new FLOAT[mesh_section_num_samples];
		for (UINT i = 0; i < mesh_section_num_samples; i++)
		{
			mesh_section_y_samples[i] = (FLOAT)i;
		}

	// ---- store
		LONG l[8];
		*(l + 0) = (LONG)mesh_section_y_samples;
		*(l + 1) = (LONG)mesh_section_num_samples;
		*(l + 2) = 0;

	// ---- unstore
		LONG  *ld = l;
		FLOAT *s_mesh_section_y_samples = (FLOAT*)*(ld + 0);
		UINT   s_mesh_section_num_samples = (INT)*(ld + 1);

		char msg[128];
		sprintf(msg, " s_mesh_section_num_samples %d\n", s_mesh_section_num_samples);
		OutputDebugString(msg);

		for (UINT i = 0; i < s_mesh_section_num_samples; i++)
		{
			float y = s_mesh_section_y_samples[i];
			char msg[128];
			sprintf(msg, " %6.2f\n", y);
			OutputDebugString(msg);
		}

		float fred = 2;
		*/
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Surface_03::~Surface_03()
{
	delete pi_surface_03;
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
INT Surface_03::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;

	// ---- store dependency pointers
		pi_surface_03->app_dependency = app_dep;
		pi_surface_03->gfx            = app_dep->GetGfxInterface();
		pi_surface_03->sys_context    = app_dep->GetSysContext();
		pi_surface_03->sys_output     = app_dep->GetSysOutput();

	return VS_OK;
}


// ---------- framework / SetupGraphics ----------
/*!
\brief SetupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupGraphics(vs_system::AppKamera *kamera)
{

	// ---- local
		INT result = VS_OK;


	// ---- store param group
		INT param_group = parameter.GetParamGroup();


	// ---- scene
		result = SetupExploreCamera(0, kamera);
		result = SetupAcquireLight();
		result = SetupExploreLighting(1);
		result = SetupExploreMaterials(0);
		result = SetupExploreMaterials(1);


	// ---- restore group
		parameter.SetParamGroup(param_group);


	// ---- setup "the" display list
		result = SetupDisplayList();


	// ---- setup objects
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		for (int i = 0; i < PI_Surface_03::MAX_GFX_OBJ_LIST; i++)
		{
			PI_Surface_03::GfxObjectItem *gfx_obj =
				&pi_surface_03->gfx_object_list[i];

			if ( gfx_obj->setup != NULL )
			{
				result = gfx_obj->setup(
							gfx_obj->index,
							this
						);
			}
		}


	// ---- state
		pi_surface_03->runtime = FALSE;

	return VS_OK;
}


// ---------- framework / Display ----------
/*!
\brief SetupDisplay
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupDisplay(
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
		pi_surface_03->matrix_stack->Push();


	// ---- display objects
		for (int i = 0; i < PI_Surface_03::MAX_GFX_OBJ_LIST; i++)
		{

			PI_Surface_03::GfxObjectItem *gfx_obj =
				&pi_surface_03->gfx_object_list[i];

			if ( gfx_obj->setup != NULL )
			{

				if  ( gfx_obj->display != ObjectExpSpot_Display )
				{
					pi_surface_03->matrix_stack->Push();
					pi_surface_03->matrix_stack->Rotate(rz, 'Z');
					pi_surface_03->matrix_stack->Rotate(rx, 'X');
					pi_surface_03->matrix_stack->Rotate(ry, 'Y');
					pi_surface_03->matrix_stack->Translate(
									translate.x,
									translate.y,
									translate.z
								);

					result = gfx_obj->display(
								gfx_obj->index,
								this,
								gfx_obj->visible
							);

					pi_surface_03->matrix_stack->Pop();
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
		pi_surface_03->matrix_stack->Pop();


	return VS_OK;
}


// ---------- framework / CleanupGraphics ----------
/*!
\brief CleanupGraphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::CleanupGraphics()
{
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- sub-framework ----------


// ---------- Display3DText ----------
/*!
\brief display dx 3D text
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::Display3DText(vs_system::AppKamera *kamera)
{
	
	// ---- local
		INT result = 0;
		if ( result == 0 ) return VS_OK;
		VsVector3 translate;
		FLOAT rx, ry, rz;

	// ---- get stuff
		kamera->GetTranslate(&translate);
		kamera->GetRotate(&rx, &ry, &rz);

	// ---- push
		pi_surface_03->matrix_stack->Push();

		// ---- transform
			pi_surface_03->matrix_stack->Rotate(rz, 'Z');
			pi_surface_03->matrix_stack->Rotate(rx, 'X');
			pi_surface_03->matrix_stack->Rotate(ry, 'Y');
			pi_surface_03->matrix_stack->Translate(translate.x, translate.y, translate.z);


		// ---- dx fonts
			#if defined(CD3DXFONT)


				CD3DFont *dx_font = pi_surface_03->gfx->GetDxFont(7);

				SIZE textFS;
				dx_font->GetTextExtent("Gareth", &textFS);

				DWORD color = D3DCOLOR_COLORVALUE(1, 0, 0, 0.5f);
				dx_font->Render3DText("Gareth", color, D3DFONT_FILTERED | D3DFONT_TWOSIDED);

			#endif	return VS_OK;

	// ---- pop
		pi_surface_03->matrix_stack->Pop();

	return VS_OK;
}


// ---------- SetObjConfig ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetObjConfig(vs_system::ObjConfig *oc)
{
	pi_surface_03->obj_config = oc;
	return VS_OK;
}


// ---------- SetImgProps ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetImgProps(vs_system::ImgProps *ip)
{
	pi_surface_03->img_props = ip;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupDisplayList ----------
/*!
\brief
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::SetupDisplayList()
{

	if ( pi_surface_03->runtime )
	{
		vs_system::GfxInterface *gfx = pi_surface_03->gfx;
		pi_surface_03->scene_dspl_list_handle = gfx->AddDisplayList("Scene", 1, 1);
	}

	return VS_OK;
}


// ---------- GetDisplayListHandle ----------
/*!
\brief
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::GetDisplayListHandle(INT *handle, CHAR *name)
{
	*handle = pi_surface_03->scene_dspl_list_handle;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


VOID Surface_03::SetRRBumpMapIndex(UINT index)
{
	pi_surface_03->rr_bump_map_index = index;
}

UINT Surface_03::GetRRBumpMapIndex()
{
	return pi_surface_03->rr_bump_map_index;
}

INT Surface_03::GetRRBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_03->rr_bump_map[pi_surface_03->rr_bump_map_index]->heightfield_data;
	*extent = pi_surface_03->rr_bump_map[pi_surface_03->rr_bump_map_index]->heightfield_extent;
	return VS_OK;
}

INT Surface_03::GetTerainBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_03->terrain_bump_map->heightfield_data;
	*extent = pi_surface_03->terrain_bump_map->heightfield_extent;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Surface_03::SetVisibleObjects(INT body_id, Surface_03* p_s_03)
{

	// --- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- lambda


	// ---- ACQUIRE VISIBILITY
		auto set_aquire = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX, 0);

			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, 0);

			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LGT_RIG_3_IDX, 0);
		};


	// ---- ANALYSIS VISIBILITY
		auto set_explore = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX, pimp->_configuration.target_visible);
			
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, pimp->_configuration.target_ruler_visible);
			pimp->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, pimp->_configuration.target_ruler_visible);

			pimp->SetObjectVisible(PI_Surface_03::GOB_ACQ_LGT_RIG_IDX, 0);
		};


	// ---- all on
		pimp->SetObjectsVisible();

	// ---- select off
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
			case TINT_BODY_CALIBRATE_LID:
			case TINT_BODY_FUNCTION_LID:
				set_aquire();
				break;

			case TINT_BODY_LIGHT_LID:
			case TINT_BODY_EXPLORE_LID:
			case TINT_BODY_MEASURE_LID:
			case TINT_BODY_ENHANCE_LID:
				set_explore();
				break;

			case TINT_BODY_SETTINGS_LID:
				break;

			default:
				return VS_ERROR;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_obj/s3_obj_heightfield.hpp"

#include "../hpp_obj/s3_obj_acq_lgt_axis.hpp"

#include "../hpp_obj/s3_obj_surf_axis.hpp"
#include "../hpp_obj/s3_obj_surf_base.hpp"
#include "../hpp_obj/s3_obj_surf_edge.hpp"
#include "../hpp_obj/s3_obj_surf_mesh.hpp"

#include "../hpp_obj/s3_obj_exp_lgt_axis.hpp"
#include "../hpp_obj/s3_obj_exp_lgt_spot.hpp"

#include "../hpp_obj/s3_obj_loi_spot.hpp"
#include "../hpp_obj/s3_obj_loi_rect.hpp"
#include "../hpp_obj/s3_obj_loi_rule.hpp"
#include "../hpp_obj/s3_obj_loi_axis.hpp"
#include "../hpp_obj/s3_obj_loi_sect.hpp"


////////////////////////////////////////////////////////////////////////////////
