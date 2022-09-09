////////////////////////////////////////////////////////////////////////////////

// ---------- r2_review_get_and_set.hpp ----------
/*!
\file r2_review_get_and_set.hpp
\brief
\author Gareth Edwards
*/


#ifndef REVIEW_02_GET_AND_SET
#define REVIEW_02_GET_AND_SET


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- GetDeviceStatus ----------
/*!
\brief Get device status
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::GetDeviceStatus()
{
	return gfx->GetDeviceStatus();
}


// ---------- GetDisplayListHandleByName ----------
/*!
\brief Get display list handle by name
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::GetDisplayListHandleByName(CHAR * name)
{
	if (!strcmp(name, "Title"))
	{
		return pi_model->handle_dspl_title_button;
	}
	else if (!strcmp(name, "Frames"))
	{
		return pi_model->handle_dspl_frame_button;
	}
	else if (!strcmp(name, "Context"))
	{
		return pi_model->handle_dspl_context_button;
	}
	else if (!strcmp(name, "Timeline"))
	{
		return pi_model->handle_dspl_timeline_button;
	}
	else if (!strcmp(name, "Clipline"))
	{
		return pi_model->handle_dspl_clipline_button;
	}
	else if (!strcmp(name, "Navigation"))
	{
		return pi_model->handle_dspl_control_button;
	}
	else if (!strcmp(name, "Shared Hidden"))
	{
		return pi_model->handle_dspl_shared_hidden;
	}

	return VS_ERROR;
}


// ---------- GetElementHandle ----------
/*!
\brief Get element handle
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::GetElementHandle(CHAR * display_name)
{

	INT display_id = 0;

	if (!strcmp(display_name, "Shared Hidden"))
	{
		return pi_model->handle_elem_shared_hidden;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetElementStuff ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetElementStuff(
		INT  handle_display_list,
		UINT group_id,
		UINT element_id,
		UINT property_id,
		UINT property_value
	)
{
	// ---- local ----
	vs_system::SysOutput *sys_output = pi_model->app_dependency->GetSysOutput();


	// ---- IF illegal handle THEN return error ----
	if (handle_display_list <= 0)
	{
		sys_output->Message("Illegal display list handle!");
		return VS_ERROR;
	}


	// ---- more local ----
	INT result = 0;
	vs_system::ElmContext *element_context = NULL;
	INT number_of_elements = gfx->GetChildCount(handle_display_list);


	// ---- FOR each element ----
	INT elm = gfx->GetFirst(handle_display_list);
	while ( elm != 0 )
	{

		// IF element context THEN
		result = gfx->GetElementContext(elm, &element_context);
		if ( result != NULL )
		{

			// IF same group id AND same element id THEN
			UINT this_group_id   = element_context->GetGroupId();
			UINT this_element_id = element_context->GetId();
			if ( this_group_id == group_id && this_element_id == element_id )
			{
				printf("Found %d %d \n", this_group_id, this_element_id);
				element_context->SetSwitchedOn(TRUE);
				;
			}
			else
				printf("      %d %d \n", this_group_id, this_element_id);
		}

		elm = gfx->GetNext(elm);
	}


	return VS_OK;
}


// ---------- SetElementProperty ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetElementProperty(
		INT  handle_display_list,
		UINT group_id,
		UINT element_id,
		UINT property_id,
		UINT property_value
	)
{
	// ---- local ----
	vs_system::SysOutput *sys_output = pi_model->app_dependency->GetSysOutput();


	// ---- IF illegal handle THEN return error ----
	if ( handle_display_list <= 0 )
	{
		sys_output->Message("Illegal display list handle!");
		return VS_ERROR;
	}


	// ---- more local ----
	INT result = 0;
	vs_system::ElmContext *element_context = NULL;
	INT number_of_elements = gfx->GetChildCount(handle_display_list);


	// ---- FOR each element ----
	INT elm = gfx->GetFirst(handle_display_list);
	while ( elm != 0 )
	{

		// IF -- element context -- THEN
		result = gfx->GetElementContext(elm, &element_context);
		if ( result != NULL )
		{

			// IF -- same group id AND same element id -- THEN
			//    note: 0 == wildchar )
			UINT this_group_id   = group_id == 0 ? 0 : element_context->GetGroupId();
			UINT this_element_id = element_id == 0 ? 0 : element_context->GetId();
			if ( this_group_id == group_id && this_element_id == element_id )
			{
				switch ( property_id )
				{
					case vs_system::ElmContext::PROPERTY_BEHAVIOUR:
						result = element_context->SetBehaviour(
								property_value
							);
						break;
					case vs_system::ElmContext::PROPERTY_BITMASK:
						result = element_context->SetBitmask(
								property_value
							);
						break;
					case vs_system::ElmContext::PROPERTY_SWITCHED_ON:
						result = element_context->SetSwitchedOn(
								(BOOL)property_value
							);
						break;
					case vs_system::ElmContext::PROPERTY_SWITCHED_ON_PREV:
						result = element_context->SetSwitchedOnPreviously(
								(BOOL)property_value
							);
						break;
					case vs_system::ElmContext::PROPERTY_SOLID:
						result = element_context->SetSolid(
								(BOOL)property_value
							);
						break;
					case vs_system::ElmContext::PROPERTY_MULTI_STATE:
						result = element_context->SetMultiState(
								(BOOL)property_value
							);
						result = element_context->SetMultiActive(
								FALSE,
								0
							);
						break;

				}
			}
		}

		// ---- next ----
		elm = gfx->GetNext(elm);
	}


	return VS_OK;
}


// ---------- SetGroupSwitchedOnOff ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetGroupSwitchedOnOff(
		INT  handle_display_list,
		UINT group_id
	)
{
	// ---- local ----
	vs_system::SysOutput *sys_output = pi_model->app_dependency->GetSysOutput();


	// ---- IF illegal handle THEN return error ----
	if ( handle_display_list <= 0 )
	{
		sys_output->Message("Illegal display list handle!");
		return VS_ERROR;
	}


	// ---- more local ----
	INT result = 0;
	vs_system::ElmContext *element_context = NULL;
	INT number_of_elements = gfx->GetChildCount(handle_display_list);


	// ---- FOR each element ----
	INT elm = gfx->GetFirst(handle_display_list);
	while ( elm != 0 )
	{

		// IF -- element context -- THEN
		result = gfx->GetElementContext(elm, &element_context);
		if ( result != NULL )
		{

			// IF -- same group id AND same element id -- THEN
			//    note: 0 == wildchar )
			UINT this_group_id = group_id == 0 ? 0 : element_context->GetGroupId();
			if ( this_group_id == group_id)
			{
				result = element_context->SetSwitchedOn(FALSE);
			}
		}

		// ---- next ----
		elm = gfx->GetNext(elm);
	}


	return VS_OK;
}


// ---------- SetConfigureAllBitmask ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetConfigureAllBitmask(BYTE setup_bitmask)
{
	gfx->SetGlobalConfigureBitmask(
			setup_bitmask,
			""
		);
	return VS_OK;
}


// ---------- SetConfigureBitmask ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetConfigureBitmask(BYTE setup_bitmask, CHAR *name)
{
	gfx->SetGlobalConfigureBitmask(
			setup_bitmask,
			name
		);
	return VS_OK;
}


#endif // REVIEW_02_GET_AND_SET


