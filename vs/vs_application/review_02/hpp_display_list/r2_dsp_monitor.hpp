////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_monitor.hpp ----------
/*!
\file r2_dl_monitor.hpp
\brief application include files.
\author Gareth Edwards
*/


#include "../header/r2_model.h"
using namespace review_02;


// ---------- SetupMonitorDisplayList ----------
/*!
\brief Setup camera monitor display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupMonitorDisplayList()
{

	// Local
	INT result = 0;

	// Initialise location/layout parameters
	FLOAT border_width = 0.01f;

	// Initialise row location
	FLOAT r[2] = { 0.45f, 0.1f };

	// Initialise # of buttons
	const UINT number_of_buttons = 16;

	// Initialise column location
	FLOAT button_width = (1 - 2 * border_width) / (FLOAT)number_of_buttons;
	FLOAT c[number_of_buttons][2];
	for (INT i = 0; i < number_of_buttons; i++)
	{
		c[i][0] = button_width * (FLOAT)i + border_width;
		c[i][1] = button_width;
	}

	//	========== BUILD LUT ==========

	// System
	using namespace vs_system;

	// Initialise element count and lut
	UINT element_count = 1;
	UINT element_version_lut[2] =
	{
		ElmContext::VERSION_BACKGROUND,
		ElmContext::VERSION_OUTLINE
	};

	// Template button
	vs_system::ElmContext::ElmButton temp =
	{
		"Mon", "", 0, 0,
		ElmContext::TYPE_STAMP, ElmContext::BEHAVIOUR_NONE, 0, 0,
		0, 0, 0, 0,
		VS_ATLAS_PAGE_GROUP_TWO, 0, 0, 0, 4, 1, 1
	};

	// Initialise button list AND rember last button is base line ...
	vs_system::ElmContext::ElmButton b[number_of_buttons];
	for (INT i = 0; i < number_of_buttons; i++)
	{
		b[i] = temp;
		b[i].x = c[i][0];
		b[i].width = c[i][1];
		b[i].y = r[0];
		b[i].height = r[1];
		b[i].id = i;
	}

	//	========== USE BUTTON LUT TO CREATE DISPLAY LIST ==========

	// Application
	using namespace review_02;

	FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];

	// Add buttons to display list
	for (UINT i = 0; i<number_of_buttons; i++)
	{

		// IF type == stamp THEN amplify # of elements
		element_count = b[i].type == vs_system::ElmContext::TYPE_ICON ? 2 : 1;

		// FOR each element in a button
		for (UINT e = 0; e<element_count; e++)
		{
			// Add to display list
			INT button_handle = gfx->AddElement(
				pi_model->handle_dspl_monitor,
					b[i].name,
					b[i].group_id,
					b[i].id,
					&pi_model->obj_config->Callback_Button_Configure
				);

			// Initialise
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx->GetElementContext(button_handle, &elm);
			if (get_element_result != NULL)
			{
				result = elm->SetFontName("Title");
				result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
				result = elm->SetText(b[i].text);
				result = elm->SetBehaviour(b[i].behaviour);
				result = elm->SetVersion(element_version_lut[e]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);

				result = pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT
					);
				result = pi_model->obj_config->ButtonSetLocation(
						param,
						b[i].x, b[i].y,
						b[i].width, b[i].height
					);
				result = pi_model->obj_config->ButtonSetTexture(
						param,
						VS_ATLAS_PAGE_GROUP_TWO,
						b[i].bg_col, b[i].bg_row,
						b[i].icon_col, b[i].icon_row,
						b[i].dim_col, b[i].dim_row,
						0.01f
					);

				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}
		}
	}

	// Done
	return VS_OK;
}


// ---------- UpdateMonitorDisplayList ----------
/*!
\brief Update camera monitor display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateMonitorDisplayList(FLOAT *next_lut, FLOAT *prev_lut)
{
	FLOAT r[16][2];
	for (INT i = 0; i < 16; i++)
	{
		FLOAT height = *(next_lut + i) - *(prev_lut + i);
		r[i][0] = 0.5f - height * 2;
		r[i][1] = height * 2;
	}

	// Local
	INT result = 0;
	vs_system::ElmContext *element_context = NULL;

	// Number of elements in display list
	INT number_of_config_elements = gfx->GetChildCount(pi_model->handle_dspl_monitor);

	// FOR each monitor element
	INT elm = gfx->GetFirst(pi_model->handle_dspl_monitor);
	while (elm != 0)
	{
		// IF context THEN
		result = gfx->GetElementContext(elm, &element_context);
		if (result != NULL)
		{
			// Update OUTLINE && BACKGROUND
			UINT element_version = element_context->GetVersion();
			if (
					element_version == vs_system::ElmContext::VERSION_OUTLINE ||
					element_version == vs_system::ElmContext::VERSION_BACKGROUND
				)
			{
				INT element_id = element_context->GetId();
				element_id = element_id < 0 ? 0 : (element_id > 15 ? 15 : element_id);
				FLOAT *param = element_context->GetFloatParameter();
				*(param + 2) = r[element_id][0]; // y
				*(param + 4) = r[element_id][1]; // height
			}
		}

		// Next
		elm = gfx->GetNext(elm);
	}

	// ---- set config bitmask so that monitor bars are recreated.... ----
	result = gfx->SetGlobalConfigureBitmask(
		VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
		"Mon"
		);

	return VS_OK;
}


// ---------- SetupMonitorControlDisplayList ----------
/*!
\brief Setup camera monitor control display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupMonitorControlDisplayList()
{
	// Local
	INT result = 0;

	// Initialise location/layout parameters
	FLOAT border_width = 0.01f;
	FLOAT border_space = 0.005f;

	// Initialise row location/width
	FLOAT r[2] = { border_width, 0.1f - border_space * 2 };

	// Initialise # of buttons
	const UINT number_of_buttons = 2;

	// Initialise column location
	FLOAT button_width = 0.1f;
	FLOAT c[number_of_buttons][2];
	for (INT i = 0; i < number_of_buttons; i++)
	{
		c[i][0] = button_width * (FLOAT)i + border_space + border_width;
		c[i][1] = button_width - border_space * 2;
	}

	//	========== BUILD LUT ==========


	// ---- system
		using namespace vs_system;


	// ---- initialise element count and lut
		UINT element_count = 1;
		UINT element_version_lut[3] =
		{
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_ICON,
			vs_system::ElmContext::VERSION_OUTLINE
		};


	// ---- template button
		vs_system::ElmContext::ElmButton temp =
		{
			"Start", "", MONITOR_BUTTON_GROUP_ID, 1,
			ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_SWITCH, 0, 0,
			0, 0, 0, 0,
			VS_ATLAS_PAGE_GROUP_TWO, 0, 0, 4, 7, 1, 1
		};


	// ---- initialise button list AND remember last button is base line ...
		vs_system::ElmContext::ElmButton b[number_of_buttons];
		for (INT i = 0; i < number_of_buttons; i++)
		{
			b[i]          = temp;
			b[i].x        = c[i][0];
			b[i].width    = c[i][1];
			b[i].y        = r[0];
			b[i].height   = r[1];
			b[i].id       = i == 0 ? 1 : 2;
			b[i].icon_col = i == 0 ? 4 : 2;
			b[i].status   = i == 0 ? TRUE : FALSE;
		}


	//	========== USE BUTTON LUT TO CREATE DISPLAY LIST ==========

	// Application
	using namespace review_02;

	FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];

	// Add buttons to display list
	for (UINT i = 0; i<number_of_buttons; i++)
	{
		// IF type == stamp THEN amplify # of elements
		element_count = 3;

		// FOR each element in a button
		for (UINT e = 0; e<element_count; e++)
		{
			// Add to display list
			INT button_handle = gfx->AddElement(
					pi_model->handle_dspl_monitor_control,
					b[i].name,
					b[i].group_id,
					b[i].id,
					&pi_model->obj_config->Callback_Button_Configure
				);

			// Initialiase
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx->GetElementContext(button_handle, &elm);
			if (get_element_result != NULL)
			{
				result = elm->SetFontName("Title");
				result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
				result = elm->SetText(b[i].text);
				result = elm->SetBehaviour(b[i].behaviour);
				result = elm->SetSwitchedOn(b[i].status);
				result = elm->SetVersion(element_version_lut[e]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);

				result = pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT_HEIGHT
					);
				result = pi_model->obj_config->ButtonSetLocation(
						param,
						b[i].x, b[i].y,
						b[i].width, b[i].height
					);
				result = pi_model->obj_config->ButtonSetTexture(
						param,
						VS_ATLAS_PAGE_GROUP_TWO,
						b[i].bg_col, b[i].bg_row,
						b[i].icon_col, b[i].icon_row,
						b[i].dim_col, b[i].dim_row,
						0.01f
					);

				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}
		}
	}

	// Done
	return VS_OK;
}
