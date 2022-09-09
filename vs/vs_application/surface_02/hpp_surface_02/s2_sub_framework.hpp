////////////////////////////////////////////////////////////////////////////////

// ---------- s2_sub_framework.hpp ----------
/*!
\file s2_sub_framework.h
\brief 
\author Gareth Edwards
*/

// ---------- sub-framework ----------


////////////////////////////////////////////////////////////////////////////////


// ---------- Display3DText ----------
/*!
\brief display dx 3D text
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::Display3DText(vs_system::AppKamera *kamera)
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
		pi_surface_02->matrix_stack->Push();

		// ---- transform
			pi_surface_02->matrix_stack->Rotate(rz, 'Z');
			pi_surface_02->matrix_stack->Rotate(rx, 'X');
			pi_surface_02->matrix_stack->Rotate(ry, 'Y');
			pi_surface_02->matrix_stack->Translate(translate.x, translate.y, translate.z);


		// ---- dx fonts
			#if defined(CD3DXFONT)


				CD3DFont *dx_font = pi_surface_02->gfx->GetDxFont(7);

				SIZE textFS;
				dx_font->GetTextExtent("Gareth", &textFS);

				DWORD color = D3DCOLOR_COLORVALUE(1, 0, 0, 0.5f);
				dx_font->Render3DText("Gareth", color, D3DFONT_FILTERED | D3DFONT_TWOSIDED);

			#endif	return VS_OK;

	// ---- pop
		pi_surface_02->matrix_stack->Pop();

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetObjConfig ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetObjConfig(vs_system::ObjConfig *oc)
{
	pi_surface_02->obj_config = oc;
	return VS_OK;
}


// ---------- SetImgProps ----------
/*!
\brief Set pointer to object configuration object
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetImgProps(vs_system::ImgProps *ip)
{
	pi_surface_02->img_props = ip;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupAcquire ----------
/*!
\brief 
\author Gareth Edwards
\param 
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetupAcquire(vs_system::AppKamera *kamera)
{

	// ---- local
		INT result = VS_OK;


	// ---- store param group
		INT param_group = parameter.GetParamGroup();


	// ---- test: setup size
		vs_system::GfxInterface *gfx = pi_surface_02->gfx;


	// ---- scene
		pi_surface_02->runtime = TRUE;
		result = SetupExploreCamera(SceneSetupMode::ALL, kamera);
		pi_surface_02->runtime = FALSE;
		result = SetupAcquireLightParam();
		result = SetupGraphicsLighting(model::Surface_02::SceneSetupMode::ACQUIRE);
		result = SetupExploreMaterials(model::Surface_02::SceneSetupMode::EXPLORE);
		result = SetupExploreMaterials(model::Surface_02::SceneSetupMode::DEFAULT);


	// ---- restore group
		parameter.SetParamGroup(param_group);


	// ---- setup objects

		for (int i = 8; i < PI_Surface_02::MAX_GFX_OBJ_LIST; i++)
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


	return VS_OK;
}


// ---------- SetupParameters ----------
/*!
\brief 
\author Gareth Edwards
\param 
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::SetupAcquireParameters()
{

	// ---- local
		INT result = VS_OK;

	// ---- initialise acquisition parameter store
		result = SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"

		SetAcquireSurfaceSize(GetAcquireSetupSourceSurfaceSize(0));
		SetAcquireSurfaceYScalar(GetAcquireSetupSourceSurfaceYScalar(0));
		parameter.CopyParamSet(&parameter_store[0]);
		SetAcquireSurfaceVersionSelected(0);

		result = SetParamGroupIndex(0);
		result = SetParamArrayIndex(0);
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

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
INT Surface_02::SetupDisplayList()
{

	if ( pi_surface_02->runtime )
	{
		vs_system::GfxInterface *gfx = pi_surface_02->gfx;
		pi_surface_02->scene_dspl_list_handle = gfx->AddDisplayList("Scene", 1, 1);
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
INT Surface_02::GetDisplayListHandle(INT *handle, CHAR *name)
{
	*handle = pi_surface_02->scene_dspl_list_handle;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Surface_02::SetVisibleObjects(INT body_id, Surface_02* surface_02)
{

	// --- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- lambda

		auto set_always_off = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_3_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_RIG_0_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_RIG_1_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_RIG_2_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LGT_RIG_3_IDX, 0);
		};

		// set by acquire
		//GOB_SURF_MESH_IDX
		//GOB_SURF_BASE_IDX
		//GOB_SURF_EDGE_IDX
		//GOB_SURF_AXIS_IDX
		//GOB_SURF_QUAD_IDX

		auto set_aquire = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_SPOT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_AXIS_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RECT_IDX, 0);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RULE_IDX, 0);
		};

		auto set_explore = [pimp]()
		{
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_SPOT_IDX, 1);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_AXIS_IDX, 1);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RECT_IDX, 1);
			pimp->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RULE_IDX, 1);
		};

		//GOB_ACQ_LGT_RIG_IDX

	// ---- all on
		pimp->SetObjectsVisible();

	// ---- always
		set_always_off();

	// ---- select off
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
			case TINT_BODY_FUNCTION_LID:
				set_aquire();
				break;
			case TINT_BODY_EXPLORE_LID:
				set_explore();
				break;

			default:
				return VS_ERROR;
		}

	return VS_OK;
}


INT Surface_02::SetVisibleMeshType(
		INT body_id,
		Surface_02* surface_02,
		BOOL mesh_3d
	)
{

	// --- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- flip
		if ( mesh_3d )
		{
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_MESH_IDX, TRUE);
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_EDGE_IDX, TRUE);
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_QUAD_IDX, FALSE);
		}
		else
		{
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_MESH_IDX, FALSE);
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_EDGE_IDX, FALSE);
			pimp->SetObjectVisible(PI_Surface_02::GOB_SURF_QUAD_IDX, TRUE);
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


VOID Surface_02::SetRRBumpMapIndex(UINT index)
{
	pi_surface_02->rr_bump_map_index = index;
}

UINT Surface_02::GetRRBumpMapIndex()
{
	return pi_surface_02->rr_bump_map_index;
}

INT Surface_02::GetRRBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_02->rr_bump_map[pi_surface_02->rr_bump_map_index]->heightfield_data;
	*extent = pi_surface_02->rr_bump_map[pi_surface_02->rr_bump_map_index]->heightfield_extent;
	return VS_OK;
}

INT Surface_02::GetTerainBumpMapData(UCHAR **data, INT *extent)
{
	*data = pi_surface_02->terrain_bump_map->heightfield_data;
	*extent = pi_surface_02->terrain_bump_map->heightfield_extent;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
