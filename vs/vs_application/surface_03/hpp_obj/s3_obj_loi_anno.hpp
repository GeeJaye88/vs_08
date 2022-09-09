////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_loi_anno.hpp ----------
/*!
\file s3_obj_loi_anno.hpp
\brief 
\author Gareth Edwards
*/

INT anno_handle[4] = { NULL < NULL, NULL, NULL };


// ---------- ObjectLoiAnno_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAnno_Setup(INT gfx_object_index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- text stuff
		vs_system::SysContext *sys_context = pimp->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- TBD ensure font OK
		//
		// font locations 0 to 4 already used for tabbed interface stuff
		//
		std::string font_name = "Dbz_Normal";
		std::string font_normal = "Calibri";
		pimp->gfx->SetFont(5, font_name, font_normal, 18, 1);

	// ---- elem(
		for (int i = 0; i < PI_Surface_03::MAX_EXP_ANNO; i++)
		{

			// ---- create ? - or reconfig
				if ( pimp->runtime )
				{
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i] = gfx->AddElement(
							pimp->scene_dspl_list_handle,
							"",
							1,
							1,
							pimp->obj_config->Dbz_Callback_Configure_Text
						);
				}
				else
				{
					pimp->gfx->SetElementConfigureBitmask(
							pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
						&elm_context
					);
				elm_context->SetVisible(FALSE);
				hr = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
				hr = elm_context->SetPick3D(FALSE);
				hr = elm_context->SetFog(FALSE);
				hr = elm_context->SetLit(FALSE);

	}

	return VS_OK;
}


// ---------- ObjectLoiAnno_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAnno_Display(INT gfx_object_index, Surface_03 *p_s_03, BOOL visible)
{

	// ---- target ?
		UINT target_index = p_s_03->GetTargetLoiIndex();
		//if ( target_index == 999 )
		//	return false;


	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_ANNO; i++)
		{

			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
						&elm_context
					);

			// ---- visible ?
				elm_context->SetVisible(visible);
				if (visible)
				{
					;
				}
		}

	return VS_OK;
}


// ---------- ObjectLoiAnno_HideAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK  
*/
INT Surface_03::ObjectLoiAnno_HideAll(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- hide all
		for (int i = 0; i < PI_Surface_03::MAX_EXP_ANNO; i++)
		{
			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
						&elm_context
					);

			// ---- visible ?
				elm_context->SetVisible(FALSE);

		}

	return VS_OK;
}


// ---------- ObjectLoiAnno_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAnno_Update(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- values to display
		VsVector3 v;         p_s_03->GetTargetLoiForDisplay(&v);
		UINT  target_index = p_s_03->GetTargetLoiIndex();
		FLOAT y_rot        = p_s_03->GetTargetLoi(target_index, 'r');

	// ---- display arrays
		FLOAT anno_values[PI_Surface_03::MAX_EXP_ANNO]  = { y_rot, v.x, v.y, v.z };
		UINT  anno_colours[PI_Surface_03::MAX_EXP_ANNO] = { 'g', 'r', 'g', 'o' };
		CHAR *anno_text[PI_Surface_03::MAX_EXP_ANNO]    = { "R", "X", "Y", "Z" };

	// ---- display placement param
		FLOAT anno_yinc =  0.08f;
		FLOAT anno_xloc = -0.75f; // note, -0.8 is left edge
		FLOAT anno_yloc = -1.00f;

	// ---- update all
		for (int i = 0; i < PI_Surface_03::MAX_EXP_ANNO; i++)
		{
			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
						&elm_context
					);

			// ---- reconfigure
				INT hr = pimp->gfx->SetElementConfigureBitmask(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_ANNO_LIST_IDX][i],
						VS_ELM_SETUP_GEOMETRY
					);

			// ---- visible ?
				elm_context->SetVisible(TRUE);
				if ( TRUE ) 
				{

					// ---- text
						char msg[128];
						sprintf_s(msg, 128, "%s = %.3f", anno_text[i], anno_values[i]);
						elm_context->SetText(msg);

					// ---- colours
						switch (anno_colours[i])
						{
							case 'r': elm_context->SetRGBA(1.0f, 0.1f, 0.1f, 1.0f); break;
							case 'g': elm_context->SetRGBA(0.1f, 1.0f, 0.1f, 1.0f); break;
							case 'o': elm_context->SetRGBA(1.0f, 0.5f, 0.1f, 1.0f); break;
							default : elm_context->SetRGBA(0.1f, 0.1f, 1.0f, 1.0f); break;
						}
						
					// ---- font
						std::string font_name = "Dbz_Normal";
						std::string font_normal = "Courier New";
						elm_context->SetFontName(font_name);

					// ---- set db
						INT  result = 0;
						FLOAT *p = elm_context->GetFloatParameter();
						result = pimp->obj_config->Dbz_SetDefault(p);
						result = pimp->obj_config->Dbz_SetConfig(p, 0);

						result = pimp->obj_config->Dbz_SetArt(p, (FLOAT)pimp->obj_config->DBZ_ART_LEFT);
						result = pimp->obj_config->Dbz_SetVisible(p, 1);
						result = pimp->obj_config->Dbz_SetScale(p, 0.2f, 0.08f);

						result = pimp->obj_config->Dbz_SetTranslate(
							p,
							anno_xloc,
							anno_yloc + anno_yinc * (FLOAT)(i+1)
						);
				}

		}

	return VS_OK;
}
