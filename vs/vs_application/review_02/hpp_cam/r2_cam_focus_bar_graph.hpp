////////////////////////////////////////////////////////////////////////////////


// ---------- r2_cam_focus_bar_graph.hpp ----------
/*!
\file r2_cam_focus_bar_graph.hpp
\brief application include files.
\author Gareth Edwards
*/

using namespace review_02;


// ---------- GetCamGraphs ----------
/*!
\brief Get camera graphs
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetCamGraphs(vs_system::CamGraphs **cam_graphs)
{
	*cam_graphs = pi_model->cam_graphs;
	return VS_OK;
}



// ---------- Dbz_UpdateFocusBarGraph ----------
/*!
\brief Set 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Dbz_UpdateFocusBarGraph()
{

	// ---- local ----
	INT result = VS_OK;
	vs_system::GfxInterface *gfx_interface = pi_model->app_dependency->GetGfxInterface();


	// ---- get focus float buffer ----
		vs_system::FloatBuffer *float_buffer = NULL;
		{
			result = pi_model->cam_graphs->GetFloatBuffer(
					vs_system::CamGraphs::FOCUS,
					&float_buffer
				);
		}
		UINT   cam_graph_rows    = float_buffer->GetRows();
		UINT   cam_graph_columns = float_buffer->GetColumns();
		FLOAT *cam_graph_buffer  = float_buffer->GetBuffer();


	// ---- copy cam_graph_buffer -> supplied float buffer ----
		result = gfx_interface->SetGfxFloatBuffer(cam_graph_buffer, cam_graph_columns);
		//memcpy(fb, cam_graph_buffer, cam_graph_columns * sizeof(FLOAT));


	// ---- use element context to get element parameter buffer ----
		vs_system::ElmContext *elm_con = NULL;
		result = gfx->GetElementContext(pi_model->handle_elem_pano_annu_foc_grf, &elm_con);
		FLOAT *p = elm_con->GetFloatParameter();


	// ---- use element context to update bitmask ----
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
		result = elm_con->SetBitmask(setup_bitmask);


	// ---- use gfx to invoke reconfig of "Annular Graph" ----
		result = gfx_interface->SetConfigure(
				FALSE,
				pi_model->handle_elem_pano_annu_foc_grf
			);

	return VS_OK;
}
