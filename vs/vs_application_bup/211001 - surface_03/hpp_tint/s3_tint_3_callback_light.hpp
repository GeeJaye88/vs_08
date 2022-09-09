////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_3_callback_image.hpp ----------
/*!
\file s3_tint_3_callback_image.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_03;


// ---------- Class Tint_Light_Param ----------
/*!
\brief light parameters
\author Gareth Edwards
\param  vs_system::TintCallback *tint_callback - struct
\return INT - ERROR <=0 < VS_OK

\note - invoked by Tint::AddBody

\note - instance created as tint_light_param

\note - tint_light_param an object of this class is also created

*/
class Model::Tint_Light_Param
{

public:
	
	// ---- enum
		enum
		{
			MAX_ITEMS = 31
		};


	// ---- cdtor
		Tint_Light_Param::Tint_Light_Param()
		{
			;
		}
		Tint_Light_Param::~Tint_Light_Param()
		{
			;
		}


	// ---- copy
		BOOL copy_param_to_all = FALSE;
		BOOL enable_all = FALSE;
		BOOL ambient_light_on = FALSE;


	// ---- element context
		BOOL init_dspl_elm_context_list = TRUE;
		INT  element_id_index[MAX_ITEMS];
		vs_system::ElmContext *element_id_context[MAX_ITEMS];


	// ---- element id's - access indices encoded as CHAR
		CHAR element_lid_char[MAX_ITEMS] =
		{
			't',
			'u', 'v', 'w', 'x', 'y', 'z',
			'j',
			'1', '2', '3', '4', '5', '6',
			'A', 'a',
			'B', 'b',
			'C', 'c',
			'D', 'd',
			'E', 'e',
			'F', 'f',
			'G', 'g',
			'm', 'n', 'r'
		};


	// ---- group id's - match aligned with element_lid_char
		CHAR group_lid_char[MAX_ITEMS] =
		{
			'T',
			'L', 'L', 'L', 'L', 'L', 'L',
			'J',
			'G', 'G', 'G', 'G', 'G', 'G',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'M', 'N', 'R'
		};


} tint_light_param;


// ---------- Tint_Light_GetCallbacks ----------
/*!
\brief Callback - update Model object
\author Gareth Edwards
\param  vs_system::TintCallback *tint_callback - struct
\return INT - ERROR <=0 < VS_OK

\note - invoked by Tint::AddBody

	e.g.

	handle = tint->AddBody(
					(INT)this,
					"Light",
					TINT_BODY_LIGHT_LID,
					&tint_callback
				);

*/
INT Model::Tint_Light_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Light_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Light_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Light_UpdateModel;
	tint_callback->callback_update_element = Tint_Light_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Light_AddText;
	tint_callback->callback_add_value      = Tint_Light_AddValue;
	return VS_OK;
}


// ---------- Tint_Light_BuildList ----------
/*!
\brief Callback - update Model object
\author Gareth Edwards
\param  vs_system::TintItem li[] - see note below
\return INT - ERROR <=0 < VS_OK

\note - invoked by Tint::BuildBody

		TintItem list_of_items[MAX_BODY_ITEM];
		INT number_of_items = _callback_for_body[index].callback_build_list(list_of_items);


\note - line increment value = * vs_system::Tint::SPACE_HEIGHT

	see Model::TintGetBuildParam

*/
INT Model::Tint_Light_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT line_increment = 12;


	// ---- local - required for TINT_ macros
		INT  result = VS_OK;
		UINT gid = TINT_BODY_LIGHT_LID;
		UINT noi = 0;


	// ---- set param group THEN setup param access
		SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- lights setup
		BOOL lights_enabled[5];
		FLOAT *param_values = new FLOAT[param_array_size];
		for (INT group_index = 0; group_index < 5; group_index++)
		{
			result = GetParamArray(group_index, param_array_size, param_values);
			lights_enabled[group_index] = param_values[0] > 0 ? TRUE : FALSE;
		}
		delete[] param_values;
		s_pi_model->surface_03->SetExploreLightsEnabled(lights_enabled);


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set "number of items == -1"
				TINT_SET_START_NOI()

			// ---- row 0
				TINT_ADD_ITEM("Enabled")
				TINT_SET_TARCX(Tint::ITEM_TEXT ,               4, 6, 1, 0.0f)

			// ---- light - 1, 2, 3, 4 & all
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_SWITCH,    5, 3, 3)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 3, 4, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 3, 5, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 3, 6, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 3, 7, 6.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_SWITCH, 5, 5, 3, 4.6f, TRUE, line_increment)

			// ---- row 0
				TINT_ADD_ITEM("Selected")
				TINT_SET_TARCX(Tint::ITEM_TEXT ,               4, 6, 1, 0.0f)

			// ---- light - 1, 2, 3, 4 & copy
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO,     5, 3, 3)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    5, 3, 4, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    5, 3, 5, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    5, 3, 6, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    5, 3, 7, 6.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_SWITCH, 5, 5, 4, 4.6f, TRUE, line_increment /2)

			// ---- x offset
				FLOAT x_off = 1.5;
				gid = TINT_BODY_LIGHT_PARAM_LID;

			// ---- param 1
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 0)
				TINT_ADD_ITEM(char_param_array_labels[0])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment / 2)

			// ---- param 2
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 1)
				TINT_ADD_ITEM(char_param_array_labels[1])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment / 2)

			// ---- param 3
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 2)
				TINT_ADD_ITEM(char_param_array_labels[2])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment / 2)

			// ---- param 4
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 3)
				TINT_ADD_ITEM(char_param_array_labels[3])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment / 2)

			// ---- param 5
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 4)
				TINT_ADD_ITEM(char_param_array_labels[4])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment /2)

			// ---- param 6
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 5)
				TINT_ADD_ITEM(char_param_array_labels[5])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment /2)

			// ---- param 7
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 6)
				TINT_ADD_ITEM(char_param_array_labels[6])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, line_increment)

			// ---- lights
				TINT_ADD_ITEM("Colr")
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 4, 7, LAST_LINE_SPACING_0)
				TINT_ADD_ITEM("Spec")
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 4, 7, LAST_LINE_SPACING_1)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK,  5, 5, 6, LAST_LINE_SPACING_2, TRUE, line_increment)

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item id's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_light_param.element_lid_char[i];
			}


		// ---- set parameter status
			for (UINT i = 0; i < noi; i++)
			{

				CHAR group_id = tint_light_param.group_lid_char[i];
				CHAR elem_id  = tint_light_param.element_lid_char[i];

				if ( group_id == 'L' )
				{
					INT param_index = elem_id - 'u';
					if ( param_index < 5 )
					{
						if ( lights_enabled[param_index] )
						{
							li[i].SetStatus(TRUE);
						}
					}
				}
				else if ( group_id == 'J' )
				{
					strcpy(li[i].text, char_param_group_name);
				}
				else if ( group_id == 'G' )
				{
					INT param_index = elem_id - '1';
					if ( param_index < 5 )
					{
						if ( param_index == param_group_index )
						{
							li[i].SetStatus(TRUE);
						}
					}
				}
				else if ( group_id == 'P' )
				{
					INT param_index = elem_id - 'A';
					if ( param_index == param_array_index )
					{
						li[i].SetStatus(TRUE);
					}
				}
			}


		// ---- mesh param
			#include "../../surface_shared/hpp_param/param_cleanup.hpp"

		return noi;
}


// ---------- Tint_Light_GetIconInfo ----------
/*!
\brief Callback - update Model object
\author Gareth Edwards
\param  INT - tabbed interface icon properties
\return INT - ERROR <=0 < VS_OK

\note - directly invoked by:

	Tint::BuildDisplayList

		e.g.

		INT atlas_page_id = [...].callback_get_icon_info(TAB_ATLAS_PAGE_ID);
		INT atlas_col     = [...].callback_get_icon_info(TAB_ATLAS_PAGE_COL);
		INT atlas_row     = [...].callback_get_icon_info(TAB_ATLAS_PAGE_ROW);

*/
INT Model::Tint_Light_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ---------- Tint_Light_UpdateModel ----------
/*!
\brief Callback - update Model object
\author Gareth Edwards
\param  INT - Model object handle
\param  INT - element_id, e.g. 'a', 'b', etc...
\return INT - VS_ERROR <=0 < VS _OK

\note directly invoked by:

	Kontrol::TintUpdateBody
	Kontrol::KpadUpdateBody

	e.g.    tint_callbacks.callback_update_model(
				(INT)model,
					element_id
				);

*/
INT  Model::Tint_Light_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;

	// ---- get tint display list element contexts
		if ( tint_light_param.init_dspl_elm_context_list )
		{
			result = tint->GetBodyDsplElmContext(
					(UINT)TINT_BODY_LIGHT_LID,
					tint_light_param.MAX_ITEMS,
					tint_light_param.element_lid_char,
					tint_light_param.element_id_index,
					tint_light_param.element_id_context
				);
			tint_light_param.init_dspl_elm_context_list = FALSE;
		}


	// ---- set param group THEN setup param access
		SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- get lights setup
		BOOL lights_enabled[5];
		surface_03->GetExploreLightsEnabled(lights_enabled);


	// ---- select
		UINT number_of_element_ids = sizeof(tint_light_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			// ---- by element id (e.g. 'u', 'v', ... )
				if ( tint_light_param.element_lid_char[i] == (CHAR)element_id )
				{

					// ---- by group id (e.g. 'L')
						CHAR group = tint_light_param.group_lid_char[i];
						switch ( group) 
						{
					
							// ---- select a light OR all lights
								case 'L' :
									{
										
										// ---- find and set the index of the first char id
											INT first_light_id_index = 0;
											for (UINT i = 0; i < number_of_element_ids; i++)
											{
												if (tint_light_param.element_lid_char[i] == 'u' )
												{
													first_light_id_index = i;
													break;
												}
											}

										// ---- local gfx shortcut
											vs_system::GfxInterface *gfx = model->pi_model->gfx;

										// ---- lights all on ?	
											INT light_index = element_id - 'u';
											if ( light_index == 5 )
											{

												tint_light_param.enable_all = tint_light_param.enable_all ? FALSE : TRUE;

												for (INT i = 0; i < 4; i++)
												{
													tint_light_param.element_id_context[i+ first_light_id_index]->SetSwitchedOn(tint_light_param.enable_all);
													lights_enabled[i] = tint_light_param.enable_all;
													gfx->SetLightEnabled(i, tint_light_param.enable_all);
												}

											}

										// ---- light on ?
											else
											{
												BOOL enabled = lights_enabled[light_index];
												enabled = enabled ? FALSE : TRUE;

												tint_light_param.element_id_context[light_index + first_light_id_index]->SetSwitchedOn(enabled);
												lights_enabled[light_index] = enabled;

												if ( light_index < 4 )
												{
													gfx->SetLightEnabled(light_index, enabled);
												}
												else
												{
													gfx->SetAmbientEnabled(enabled);
												}

											}

										// ---- update light parameter arrays
											FLOAT *param_values = new FLOAT[param_array_size];
											for (INT i = 0; i < 5; i++)
											{
												result = GetParamArray(i, param_array_size, param_values);
												param_values[0] = (FLOAT)(lights_enabled[i] ? 1 : 0);
												result = SetParamArray(i, param_array_size, param_values);
											}
											delete[] param_values;

									}
									break;

						// ---- param group
							case 'G' :
								{

									// ---- flip copy parameter to all?
										INT light_index = element_id - '1';
										if ( light_index == 5 )
										{
											tint_light_param.copy_param_to_all = tint_light_param.copy_param_to_all ? FALSE : TRUE;
										}

									// ---- select ?
										else
										{

											// ---- set ambient_light property to avoid copy
												tint_light_param.ambient_light_on = light_index == 4 ? TRUE : FALSE;

											// ---- update param
												result = SetParamGroupIndex(light_index);
												CHAR param_id = 'A';
												do
												{
													result = Tint_Light_UpdateElement(handle_model, param_id);
													param_id++;
												} while (param_id <= 'G');

											// ---- get param group name
												std::string str_mesh_param_name;
												GetParamGroupName(str_mesh_param_name);

											// ---- relabel param group ( find and set 'j')
												for (UINT i = 0; i < number_of_element_ids; i++)
												{
													if (tint_light_param.element_lid_char[i] == 'j')
													{
														tint_light_param.element_id_context[i]->SetText(str_mesh_param_name.c_str());
													}
												}
										}
								}
								break;

						//---- parameter
							case 'P':
								{
									INT param_index = element_id - 'A';
									result = SetParamArrayIndex(param_index);

									result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
									Kpad_UpdateByParameter(handle_model, GetParamGroup());
									Kpad_Input_UpdateElement(handle_model, (INT)'P');
									result = tint->SetSelectedBody(TINT_BODY_LIGHT_LID);

								}
								break;
								
						// ---- colour
							case 'M' :
								{
									// ---- flip colourlighting
										BOOL colour_enabled = surface_03->GetExploreLightColouredEnabled();
										colour_enabled = colour_enabled ? FALSE : TRUE;
										surface_03->SetExploreLightColouredEnabled(colour_enabled);

									// ---- update explore lighting rig
										result = surface_03->ObjectExpLgtAxis_UpdateAll(surface_03);

								}
								break;

						// ---- specular
							case 'N' :
								{
									BOOL specular_enabled = surface_03->GetExploreLightSpecularEnabled();
									specular_enabled = specular_enabled ? FALSE : TRUE;
									surface_03->SetExploreLightSpecularEnabled(specular_enabled);
								}
								break;

						// ---- reset
							case 'R':
								{ 
									// ---- reset colour lighting parameters
										result = SetParamArrayDefaults(PARAM_GROUP_LIGHTS);
										for (INT i = 0; i < 6; i++)
										{
											INT param_id = i + 'A';
											result = Tint_Light_UpdateElement(handle_model, param_id);
										}

									// ---- update explore lighting rig
										result = surface_03->ObjectExpLgtAxis_UpdateAll(surface_03);

								}
								break;

							default:
								break;
						}

					// ---- cleanup parameters
						goto cleanup;
				}
		}


	// ---- param
	cleanup:

		// ---- set lighting
			surface_03->SetExploreLightsEnabled(lights_enabled);
			surface_03->SetupExploreLighting(1);
			surface_03->SetupExploreMaterials(1);

		// ---- cleanup param
			#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ---------- Tint_Light_UpdateElement.hpp ----------
/*-

\brief update an element
\author Gareth Edwards
\param  INT - Model object handle
\param  INT - element_id, e.g. 'a', 'b', etc...
\return INT - VS_ERROR <=0 < VS _OK

\note : IMPORTANT - not invoked by Tint:: !!!

\note : invoked as callback, e.g.

		tint_callbacks.callback_update_element(
				(INT)model,
				handle_element == 0 ? 0 : element_id
			);

\note : invoked directly e.g.

	result = Tint_Light_UpdateElement(handle_model, param_id);

	see:
		Model::Tint_Light_UpdateModel
		Model::Kpad_Set
		Kontrol::TintUpdateBody

\note : if Tint::UpdateText invoked, then this invokes callbacks:

		callback_add_actions ( would be Tint_Light_AddAction - but not required )
		callback_add_text    ( see below Tint_Light_AddValue )
		callback_add_value   ( see below Tint_Light_AddValue )

 */
INT  Model::Tint_Light_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set param group THEN setup param access
		SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- set selected tint body
		result = tint->SetSelectedBody(TINT_BODY_LIGHT_LID);


	// ---- element
		UINT number_of_element_ids = sizeof(tint_light_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			// ---- element_lid_char == element_id ?
				if ( tint_light_param.element_lid_char[i] == (CHAR)element_id )
				{

					// ---- param group ?
						CHAR group = tint_light_param.group_lid_char[i];
						switch ( group )
						{

							// ---- param group - relabel group 'J' using element 'j' text  
								case 'G':
								{
									result = tint->UpdateText(NULL, 'j');
								}
								break;

							// ---- param
								case 'P':
								{

									// ---- only 'A' to 'G' elements
										switch ( element_id )
										{
											case 'A':
											case 'B':
											case 'C':
											case 'D':
											case 'E':
											case 'F':
											case 'G':
											case 'H':
												{

													// ---- update 'A' to 'G' element text
														INT elem_id = tint_light_param.element_lid_char[i + 1];
														result = tint->UpdateText(NULL, elem_id);


													// ---- copy parameter to all - IF NOT ambient light
														if ( tint_light_param.copy_param_to_all && ! tint_light_param.ambient_light_on )
														{

															// ---- source value to copy....
																FLOAT copy_value = param_array_values[param_array_index];

															// ----- ...to all other parameter groups...
																for (INT pgi = 0; pgi < 4; pgi++)
																{

																	// ---- but not if NOT source group
																		if ( pgi != param_group_index )
																		{

																			// ---- get, copy & set
																				result = GetParamArrayValues(
																						pgi,
																						&param_group_id,
																						&param_array_count,
																						1,
																						param_array_values
																					);

																				param_array_values[param_array_index] = copy_value;

																				result = SetParamArrayValues(
																						pgi,
																						param_group_id,
																						param_array_count,
																						param_array_values
																					);
																		}

																}

															// ---- reset lights and surface material
																model::Surface_03 *surface_03 = s_pi_model->surface_03;
																result = surface_03->SetupExploreLighting(1);
																result = surface_03->SetupExploreMaterials(1);

														}

														// ---- update explore lighting rig
															model::Surface_03 *surface_03 = s_pi_model->surface_03;
															result = surface_03->ObjectExpLgtAxis_UpdateAll(surface_03);

												}
												break;

											default:
												break;
										}

								}
								break;

							default:
								break;

						}

					// ---- cleanup parameters
						goto cleanup;

				}
		}


	// ---- param
	cleanup:
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ---------- Tint_Acquire_AddActions ----------
/*-

\brief add actions for an element
\author Gareth Edwards
\param  INT - Model object handle
\param  INT - element_id, e.g. 'a', 'b', etc...
\return INT - VS_ERROR <=0 < VS _OK


\note : one of three Tint::UpdateText callbacks:

		callback_add_actions ( this method - but not required )
		callback_add_text    ( see below Tint_Light_AddText )
		callback_add_value   ( see below Tint_Light_AddValue )


\note : invoked as callback by Tint::UpdateText, e.g.


	// ---- get action ?
		if (_callback_for_body[selected_body_index].callback_add_actions != NULL)
		{
			FLOAT *float_param = elm_context->GetFloatParameter();
			float_param += vs_system::ObjConfig::MAX_BUTTON_PARAM_USED;
			result = _callback_for_body[selected_body_index].callback_add_actions(
				handle_ass_obj,
				elem_id,
				float_param
			);
			elm_context->SetActions(result);
		}
		
	// ---- get text ?

	// ---- get value ?

\note : sample code below...

*/
/*

INT  Model::Tint_Acquire_AddActions(
		const INT  handle_obj_config,
		const INT  element_index,
		FLOAT      action_values[]
	)
{ 

	// ---- for enum
		using namespace vs_system;

	// ---- local
		CamProps *cam_props = (CamProps *)handle_obj_config;
		BOOL edit_config = cam_props->GetEditConfig();

	// ---- local
		INT num_actions = 0;

	// ---- update ?
		switch ( element_index )
		{
			// ---- exposure : ALMHU
				case tint_acquire_param.EXP_AUTO_INDEX : 
					{

						num_actions = 3;

						FLOAT action_value =
								edit_config ? 1.0f :
									cam_props->GetActionValue(
											tint_acquire_param._update_element_index,
											CamProps::AUTO_EXPOSURE,
											CamProps::EXPOSURE_INDEX,
											CamProps::NUM_EXPOSURE_CONFIGS,
											tint_acquire_param.EXP_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 1), action_value,
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 2), action_value,
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 3), action_value
							};

						for (INT i = 0; i < num_actions*2; i++) action_values[i] = v[i];
						
					}
					break;

		etc...

*/


// ---------- Tint_Light_AddText.hpp ----------
/*-

\brief update an element
\author Gareth Edwards
\param  const INT - Model object handle
		const INT - element id,  e.g. 'a', 'b', etc...
		BOOL *    - IF TRUE THEN replace text
		BOOL *    - IF TRUE THEN add text
		CHAR *    - text
\return INT - VS_ERROR <=0 < VS _OK


\note : one of three Tint::UpdateText callbacks:

		callback_add_actions ( would be Tint_Light_AddAction - but not required )
		callback_add_text    ( this method )
		callback_add_value   ( see below Tint_Light_AddValue )


\note : invoked as callback by Tint::UpdateText, e.g.


		// ---- get action ?

		// ----local element context ok
			BOOL  rep_text  = FALSE;
			BOOL  add_text  = FALSE;
			CHAR  text[VS_MAXCHARLEN];

		// ---- get text ?
			strcpy(text, elm_context->GetName().c_str());
			if ( _callback_for_body[selected_body_index].callback_add_text != NULL )
			{
				result = _callback_for_body[selected_body_index].callback_add_text(
						handle_ass_obj,
						elem_id,
						&rep_text,
						&add_text,
						text
					);
				if ( rep_text )
				{
					std::string name;
					name.assign(text);
					elm_context->SetName(name);
				}
			}

		// ---- get value ?

 */
INT  Model::Tint_Light_AddText(
		const INT  handle_model,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- add text updated at runtime AND from Tint::UpdateText 


	// ---- init args
		*add_text     = FALSE;
		*replace_text = FALSE;
		*text         = NULL;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- param
		SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- update
		Kpad_UpdateByParameter(handle_model, GetParamGroup());


	// ---- process
		UINT number_of_element_ids = sizeof(tint_light_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

				if ( tint_light_param.element_lid_char[i] == (CHAR)element_id )
				{

						CHAR group = tint_light_param.group_lid_char[i];
						switch ( group )
						{			
					
							// ---- parameter group label
							case 'J':
								{
									*replace_text = TRUE;
									strcpy(text, param_group_name.c_str());
								} 
								break;

							// ---- parameter label
							case 'P':
								{
									
										INT id = element_id;
										switch ( element_id )
										{

											case 'A':
											case 'B':
											case 'C':
											case 'D':
											case 'E':
											case 'F':
											case 'G':
											case 'H':
												{
													id = tint_light_param.element_lid_char[i  + 1];
												}
											case 'a':
											case 'b':
											case 'c':
											case 'd':
											case 'e':
											case 'f':
											case 'g':
											case 'h':
												{

													// ---- local
														*replace_text = TRUE;
														BOOL multi_state = TRUE;
														INT  index = id - (INT)'a';


													// ---- text
														if ( strcmp(char_param_array_labels[index], "NA") != 0 )
														{

															// ---- copy & snip trailing '0's
																sprintf(text, "%s: %f", char_param_array_labels[index], param_array_values[index]);
																std::string s = tostring(text);
																strcpy(text, s.c_str());
																INT len = strlen(text);
																if (text[len - 1] == '.')
																{
																	text[len] = '0';
																	text[len + 1] = 0;
																}
														}
														else
														{
															// ---- copy
																strcpy(text, "");
																multi_state = FALSE;
														}


													// ---- set all non-multi-state to NULL behaviour
														INT elem_handles[tint->MAX_BODY_ELM];
														INT num_elem = tint->GetBodyDsplElmCount(TINT_BODY_LIGHT_LID);
														tint->GetBodyDsplElmHandles(TINT_BODY_LIGHT_LID, elem_handles);

														for (INT j = 0; j < num_elem; j++)
														{
															vs_system::ElmContext *elem_context = NULL;
															s_pi_model->gfx->GetElementContext(elem_handles[j], &elem_context);

															INT elem_group = elem_context->GetGroupId();
															if ( elem_group == TINT_BODY_LIGHT_PARAM_LID )
															{
																INT elem_id = elem_context->GetId();
																if ( elem_id == tint_light_param.element_lid_char[i - 1] )
																{
																	elem_context->SetState(multi_state);
																	elem_context->SetMultiState(multi_state);
																	elem_context->SetMultiActive(multi_state, 0);
																	INT behaviour = multi_state ?
																		vs_system::ElmContext::BEHAVIOUR_RADIO :
																			vs_system::ElmContext::BEHAVIOUR_NULL;
																	elem_context->SetBehaviour(behaviour);
																}
															}
														}

												}
												break;

											default:
												break;
										}
								}
								break;

							default:
								break;

						}

					// ---- cleanup parameters
						goto cleanup;

				}
		}


	// ---- param
	cleanup:
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ---------- Tint_Light_AddValue.hpp ----------
/*-

\brief update an element
\author Gareth Edwards
\param  const INT - Model object handle
\param  const INT - element_id, e.g. 'a', 'b', etc...
		BOOL  *   - IF TRUE THEN add value
		FLOAT *   - value
		CHAR  *   - format
\return INT - VS_ERROR <=0 < VS _OK


\note : one of three Tint::UpdateText callbacks:
		callback_add_actions ( would be Tint_Light_AddAction - but not required )
		callback_add_text    ( see below Tint_Light_AddValue )
		callback_add_value   ( this method )


\note : invoked as callback by Tint::UpdateText, e.g.


		// ---- get action ?

		// ---- get text ?
			CHAR  text[VS_MAXCHARLEN];

		// ----local element context ok
			BOOL  add_value = FALSE;
			FLOAT value = 0;

		// ---- get value ?
			CHAR suffix[VS_MAXCHARNAME];
			strcpy(suffix, "");
			if (_callback_for_body[selected_body_index].callback_add_value != NULL)
			{
				result = _callback_for_body[selected_body_index].callback_add_value(
						handle_ass_obj,
						elem_id,
						&add_value,
						&value,
						suffix
					);
			}

		// ---- format value (needs to be big)
			CHAR value_text[VS_MAXCHARLEN];
			if ( add_value )
			{

				CHAR formated_value[16];

				if ( *suffix == ' ' || *suffix == '0' )
				{
					sprintf(formated_value, "%d", (INT)value);
				}
				else
				{
					switch ( *suffix )
					{
						case '1':
							sprintf(formated_value, "%5.1f", value);
							break;
						case '2':
							sprintf(formated_value, "%5.2f", value);
							break;
						case '3':
							sprintf(formated_value, "%5.3f", value);
							break;
						default:
							sprintf(formated_value, "%5.4f", value);
							break;
					}
				}
				*suffix = ' ';

				snprintf(value_text, VS_MAXCHARLEN, "%s %s%s%s",
						elm_context->GetName().c_str(),
						add_text ? text : "",
						formated_value,
						suffix
					);

				result = elm_context->SetText(value_text);

			}
			else
			{
				snprintf(value_text, VS_MAXCHARLEN, "%s%s",
						elm_context->GetName().c_str(),
						add_text ? text : ""
					);
				result = elm_context->SetText(value_text);
			}

 */
INT  Model::Tint_Light_AddValue(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- no value
		*add_value = FALSE;
		*value     = 0;

	// ---- VS_MAXCHARLEN
		sprintf(suffix, "2");

		return VS_OK;
}


// ---------- Tint_GetElementContextList ----------
/*-
\brief  get tabbed interface element context list
\author Gareth Edwards

\param  INT    - Model object handle
\param  BOOL   - flag set TRUE after this method invoked
\param  Tint   - tabbed interface object
\param  UINT   - tabbed interface body id
\param  UINT   - tabbed interface body element count
\param  CHAR[] - tabbed interface body element id char (e.g. 'a')

\param  INT[]  - [out] tabbed interface body element id indices
\return ElmContext *[] - [out] tabbed interface body element context

\return - VS_ERROR <= 0 < VS _OK

\note - invoked by Tint::Tint_Light_UpdateModel

Function
========

Creates body element id index and context lists.

These provide allow access by index to all elements in a
display list created when a tint item (TintItrem) list
is data amplified into a display list.

The cunning part is that the index is the original tint
item char id (e.g. CHAR 'a' id == INT 97 index).

Usgae example:

	Random acccess to element context by using element char id as index
	===================================================================

	Given INT element_id, an element id (e.g. 'a')...

	UINT number_of_element_ids = sizeof(tint_light_param.element_lid_char) / sizeof(CHAR);
	for (UINT i = 0; i < number_of_element_ids; i++)
	{
		if ( tint_light_param.element_lid_char[i] == (CHAR)element_id )
		{

			....

			for (UINT i = 0; i < number_of_element_ids; i++)
			{
				if (tint_light_param.element_lid_char[i] == 'a')
				{
					tint_light_param.element_id_context[i]->SetText(str_mesh_param_name.c_str());
				}
			}
		}
	}

	Simples!

*/
/*
INT  Model::Tint_GetElementContextList(
		INT    handle_model,
		BOOL  *do_this_once,
		vs_system::Tint *tint,
		UINT   tint_body_lid,
		INT    tint_elem_count,
		CHAR   tint_element_lid_char[],

	    INT    tint_element_id_index[],
		vs_system::ElmContext  *tint_element_context[]

	)
{

	// ---- ONLY if NOT "done"
		if ( *do_this_once )
		{

			// ---- local
				INT    result = VS_OK;
				Model *model  = (Model *)handle_model;
				vs_system::GfxInterface *gfx_interface = s_pi_model->app_dependency->GetGfxInterface();


			// ---- get # of elements in a display list
				UINT elem_count = tint->GetBodyDsplElmCount(tint_body_lid);


			// ---- get display list element handles
				INT elem_handles[tint->MAX_BODY_ELM];
				result = tint->GetBodyDsplElmHandles(tint_body_lid, elem_handles);


			// ---- get display list element contexts
				vs_system::ElmContext *element_context[tint->MAX_BODY_ELM];
				result = gfx_interface->GetElementContexts(
						elem_count,
						elem_handles,
						element_context
					);
		

			// ---- set ONLY first instance of matched element id contexts
				UINT number_of_element_ids = tint_elem_count;
				UINT last_id = 0;
				for ( UINT e = 0; e<elem_count; e++ )
				{
					UINT gid = element_context[e]->GetGroupId();
					UINT  id = element_context[e]->GetId();

					if ( id != last_id )
					{
						for ( UINT i = 0; i < number_of_element_ids; i++ )
						{
							if ( id == tint_element_lid_char[i] )
							{
								tint_element_context[i]  = element_context[e];
								tint_element_id_index[i] = e;
								break;
							}
						}
					}

					last_id = id;
				}


			// ---- done...
				*do_this_once = FALSE;

		}

	return VS_OK;
}

*/