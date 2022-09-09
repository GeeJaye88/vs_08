////////////////////////////////////////////////////////////////////////////////

// ---------- r3_model.cpp ----------
/*!
\file r3_model.cpp
\brief Implementation of the Model class.
\author Gareth Edwards
\note Vanilla C++
*/

// ---------- application include ----------
#include "../header/r3_model.h"


// ---------- namespace ----------
using namespace record_03;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Model class ----------

class Model::PI_Model
{
public:

	enum
	{
		CAM_REPORT_TEXT_RECORD    = 0,
		CAM_REPORT_TEXT_LIGHT     = 1,
		CAM_REPORT_TEXT_BRIGHT    = 2,
		CAM_REPORT_TEXT_FRAMERATE = 3,
		CAM_REPORT_TEXT_EXPOSURE  = 4,
		CAM_REPORT_TEXT_GAIN      = 5,
		CAM_REPORT_TEXT_CONTRAST  = 6,
		CAM_REPORT_TEXT_EXIT      = 7
	};

	// Constructor
	PI_Model()
	{
		// Objects
		app_dependency = NULL;
		cam_modes      = NULL;
		obj_config     = NULL;
		gfx            = NULL;

		// Application Objects
		config_files = new ConfigFiles();

		// Button handles
		for (UINT i=0; i<10; i++) handle_cam_report_text[i] = 0;
		handle_cam_modes_frame_rate_text  = 0;

		// Display list handles
		handle_title_bar  = 0;
		handle_cam_report = 0;
		handle_cam_modes  = 0;
		handle_about      = 0;

		// About gfx context
		about_panel_gfx_context = NULL;
	};

	// Destructor
	~PI_Model()
	{
		// ---- ConfigFiles
		if ( config_files != NULL ) delete config_files;
	};

	// VS Objects
	vs_system::AppDependency *app_dependency;
	vs_system::CamModes      *cam_modes;
	vs_system::CamProps      *cam_props;
	vs_system::ObjConfig     *obj_config;
	vs_system::GfxInterface  *gfx;
	
	// Application Objects
	ConfigFiles *config_files;

	// Button Handles
	INT handle_cam_report_text[10];
	INT handle_cam_modes_frame_rate_text;

	// Display list handle
	INT handle_title_bar;
	INT handle_cam_report;
	INT handle_cam_modes;
	INT handle_about;

	// About gfx context
	vs_system::GfxContext *about_panel_gfx_context;
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::Model()
{
	// Private implementation
	pi_model = new PI_Model;

	// Injected dependency
	pi_model->app_dependency = NULL;

	// Shortcut to an injected dependency property
	pi_model->gfx = NULL;

	// ---- Camera modes ----
	pi_model->cam_modes = new vs_system::CamModes();

	// ---- Camera properties ----
	pi_model->cam_props = new vs_system::CamProps();

	// ---- Object configirations ----
	pi_model->obj_config = new vs_system::ObjConfig();

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Model::~Model()
{
	// Camera light modes 
	if ( pi_model->cam_modes != NULL ) delete pi_model->cam_modes;

	// Camera properties 
	if (pi_model->cam_props != NULL) delete pi_model->cam_props;

	// ObjConfig: 
	if ( pi_model->obj_config != NULL ) delete pi_model->obj_config;

	// Private implementation
	if ( pi_model != NULL ) delete pi_model;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Setup(vs_system::AppDependency *app_dependency)
{
	// Store injected dependency
	this->pi_model->app_dependency = app_dependency;

	// Note: Managed FONTS set in application

	// Store gfx
	pi_model->gfx = app_dependency->GetGfxInterface();

	// Done
	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup graphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupGraphics()
{
	INT result = pi_model->gfx->SetupGraphics();
	return result;
}


// ---------- Begin ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::DisplayBegin()
{
	// Done
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(
		vs_system::GfxContext *panel_gfx_context,
		vs_system::ImgBuffer *dest_img_buffer
	)
{
	// Local
	INT result = 0;


	// ==================== GET PANEL NAME ====================

	// Get panels name
	CHAR panel_name[VS_MAXCHARLEN];
	result = panel_gfx_context->GetName(panel_name);

	// IF panel name NOT null THEN
	if ( panel_name != NULL )
	{

		// ==================== GET DISPLAY LIST HANDLE ====================
		
		// Get display list handle
		INT display_list_handle;
		result = panel_gfx_context->GetDisplayListHandle(0, &display_list_handle);

		// RUNTIME: IF no display list handle THEN setup and do one-time initialisation
		if (display_list_handle <= 0)
		{

			// Get display list name
			CHAR display_name[VS_MAXCHARLEN];
			result = panel_gfx_context->GetDisplayListName(0, display_name);

			// ---- UI ----
			if (!strcmp(display_name, "TitleBar"))
			{
				display_list_handle = SetupTitleBar();
			} 
			else if (!strcmp(display_name, "CamReport"))
			{
				display_list_handle = SetupCamReport();
			}
			else if (!strcmp(display_name, "CamModes"))
			{
				display_list_handle = SetupCamModes();
			}
			else if (!strcmp(display_name, "About"))
			{
				display_list_handle = SetupAbout();
				pi_model->about_panel_gfx_context = panel_gfx_context;
				panel_gfx_context->SetDisplayStatus(FALSE);
			}

			// ---- Ooops ----
			else return VS_ERROR;

			// One-time initialisation
			result = panel_gfx_context->SetGroupId(1);
			result = panel_gfx_context->SetDisplayListHandle( 0, display_list_handle);
			result = panel_gfx_context->SetSelected(FALSE);
		}


		// ==================== GET PANEL DISPLAY STATUS ====================

		BOOL panel_status = TRUE;
		result = panel_gfx_context->GetDisplayStatus(&panel_status);
		if ( panel_status == FALSE ) return VS_OK;

		//CHAR display_name[VS_MAXCHARLEN];
		//result = panel_gfx_context->GetDisplayListName(0, display_name);
		//printf("%s\n", display_name);


		// ==================== RENDER ====================

		// Accelerate
		vs_system::GfxInterface  *gfx = pi_model->gfx;

		// Setup and render view rectangle background
		result = gfx->SetMargin(panel_gfx_context->GetMargin());
		result = gfx->SetViewrect(panel_gfx_context->GetViewrect());
		result = gfx->RenderViewrect(panel_gfx_context, FALSE);

		// Setup and render view port AND selected display list
		result = gfx->SetBorder(panel_gfx_context->GetBorder());
		result = gfx->SetViewport(panel_gfx_context->GetViewport());			
		result = gfx->RenderViewport(display_list_handle, panel_gfx_context);

		// Setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_gfx_context, TRUE);
	}

	// Success - display name OK
	return VS_ERROR;
}


// ---------- End ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::DisplayEnd()
{
	// Done
	return VS_OK;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::CleanupGraphics()
{
	INT result = pi_model->gfx->CleanupGraphics();
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


// ---------- SetupTitleBar ----------
/*!
\brief Setup title bar display list
\author Gareth Edwards
\return INT - Display list handle
*/
INT Model::SetupTitleBar()
{
	// Local
	INT result;

	// Add, then Setup
	pi_model->handle_title_bar = pi_model->gfx->AddDisplayList("TitleBar");
	result = SetupTitleBarDisplayList(pi_model->handle_title_bar);
	return pi_model->handle_title_bar;
}


// ---------- SetupTitleBarDisplayList ----------
/*!
\brief Setup title bar display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupTitleBarDisplayList(INT display_list_handle)
{
	// Local
	INT result = 0;

	// Initialise location/layout parameters
	FLOAT x_top = 0.10f;
	FLOAT y_top = 0.10f;
	FLOAT icon_width = 1.0f;
	FLOAT icon_height = 1.0f;

	// Initialise # of buttons
	const UINT number_of_buttons = 2;

	//	========== BUILD BUTTON LUT ==========

	// System
	using namespace vs_system;

	// Initialise element vars
	UINT element_count = 1;
	UINT element_version_lut[3] =
	{
		ElmContext::VERSION_BACKGROUND,
		ElmContext::VERSION_ICON,
		ElmContext::VERSION_OUTLINE
	};

	// Initialise button list
	vs_system::ElmContext::ElmButton b[number_of_buttons] =
	{
		{
			"", "", ABOUT_BUTTON_GROUP_ID, ABOUT_VIEW,
			ElmContext::TYPE_STAMP,
			ElmContext::BEHAVIOUR_CLICK, 0,
			0, 0, 4, 1,
			0, 0, 3, 0, 4, 1
		},
		{
			"", "",
			0, 0,
			ElmContext::TYPE_TEXT,
			ElmContext::BEHAVIOUR_NONE, 0,
			0, 0, 1, 1,
			0, 0, 1, 1, 1, 1
		},

	};

	//	========== USE BUTTON LUT TO CREATE DISPLAY LIST ==========

	// Application
	using namespace record_03;

	// Add buttons to display list
	for (UINT i = 0; i<number_of_buttons; i++)
	{
		// IF type == icon THEN amplify # of elements
		element_count = b[i].type == vs_system::ElmContext::TYPE_ICON ? 3 : 1;

		// IF type == text THEN version == text ELSE version == background
		element_version_lut[0] =
			b[i].type == vs_system::ElmContext::TYPE_STAMP ?
				vs_system::ElmContext::VERSION_STAMP :
				vs_system::ElmContext::VERSION_ICON;

		// FOR each element in a button
		for (UINT e = 0; e<element_count; e++)
		{
			// Add to display list
			INT button_handle = pi_model->gfx->AddElement(
					display_list_handle,
					b[i].name,
					b[i].group_id,
					b[i].id,
					&pi_model->obj_config->Callback_Button_Configure
				);

			// Initialiase
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = pi_model->gfx->GetElementContext(button_handle, &elm);
			if (get_element_result != NULL)
			{
				result = elm->SetFontName("CamModes");
				result = elm->SetFontAlignment(VS_ALIGN_LEFT);
				result = elm->SetText(b[i].text);
				result = elm->SetBehaviour(b[i].behaviour);
				result = elm->SetVersion(element_version_lut[e]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

				// Parameters
				FLOAT param[VS_MAX_ELM_PARAMETER_DATA] =
				{
					// SRT
					(FLOAT)vs_system::ElmContext::VIEWPORT_HEIGHT,

					// XY
					b[i].x, b[i].y,
					b[i].width, b[i].height,

					// UV
					b[i].bg_col, b[i].bg_row,
					b[i].icon_col, b[i].icon_row,
					b[i].dim_col, b[i].dim_row,

					// UV adjust
					0
				};
				result = elm->SetParameter(sizeof(param) / sizeof(FLOAT), param);
			}
		}
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////



// ---------- SetupCamReport ----------
/*!
\brief Setup cam report display list
\author Gareth Edwards
\return INT - Display list handle
*/
INT Model::SetupCamReport()
{
	// Local
	INT result;

	// Add, then Setup
	pi_model->handle_cam_report = pi_model->gfx->AddDisplayList("CamReport");
	result = SetupCamReportDisplayList(pi_model->handle_cam_report);
	return pi_model->handle_cam_report;
}


// ---------- SetupSetupCamReportDisplayList ----------
/*!
\brief Setup camera report display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCamReportDisplayList(INT display_list_handle)
{
	// Local
	INT result = 0;

	// Initialise location/layout parameters
	FLOAT x_top = 0.10f;
	FLOAT y_top = 0.10f;
	FLOAT space_width  = 0.04f;
	FLOAT space_height = 0.02f;
	FLOAT text_width   = 0.07f;
	FLOAT text_height  = 0.07f;
	FLOAT icon_width   = 0.07f;
	FLOAT icon_height  = 0.07f;

	// Initialise # of buttons
	const UINT number_of_rows = 8;

	// Initialise row column location
	FLOAT r[number_of_rows];
	FLOAT y = y_top;
	for (INT i = 0; i<number_of_rows; i++)
	{
		r[i] = y;
		y += icon_height + space_height;
	}

	// Initialise column location
	FLOAT c[2] = { x_top, x_top + icon_width + space_width };

	// System
	using namespace vs_system;

	// Initialise camera modes indices
	UINT mode[3];
	CHAR group_text[3][64];
	for (INT i=1; i<=3; i++)
	{
		UINT g = CamModes::BASE_GROUP + i;
		switch ( g )
		{
			case CamModes::LIGHT_GROUP:
			case CamModes::BRIGHT_GROUP:
			case CamModes::FRAME_RATE_GROUP:
				{
					// Get group mode index
					mode[i-1] = pi_model->cam_modes->GetCamModesIndex(g);
					result = pi_model->cam_modes->GetCamModesDescription(group_text[i - 1], g);
					
				}
				break;

			default:
				break;
		}
	}

	// Initialise button list
	vs_system::ElmContext::ElmButton b[2][number_of_rows] =
	{
		{
			{
				"", "", CAM_REPORT_GROUP, CAM_REPORT_RECORD, // Record
				ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_SWITCH, 0,
				c[0], r[0], icon_width, icon_height,
				2, 0, 6, 7, 1, 1
			},
			{
				"", "", 0, 0, // Lighting
				ElmContext::TYPE_STAMP, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[1], icon_width, icon_height,
				0, 0, (FLOAT)mode[0] - 1, 4, 1, 1
			},
			{
				"", "", 0, 0, // Brightness
				ElmContext::TYPE_STAMP, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[2], icon_width, icon_height,
				0, 0, 4 + (FLOAT)mode[1], 4, 1, 1
			},
			{
				"", "", 0, 0, // Framerate
				ElmContext::TYPE_STAMP, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[3], icon_width, icon_height,
				0, 0, 8 - (FLOAT)mode[2], 3, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[4], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[5], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[0], r[6], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"", "",
				CAM_REPORT_GROUP, CAM_REPORT_EXIT,
				ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_SWITCH, 0,
				c[0], r[7], icon_width, icon_height,
				2, 0, 7, 1, 1, 1
			},
		},

		{
			{
				"", "Record", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[0], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[1], text_width, text_height,
				0, 0, 0, 4, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[2], text_width, text_height,
				0, 0, 0, 4, 1, 1
			},
			{
				"", "", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[3], text_width, text_height,
				0, 0, 0, 3, 1, 1
			},
			{
				"A", "Exposure [NA] ms", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[4], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"B", "Gain [NA] R,  [NA] G,  [NA] B", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[5], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"C", "Contrast [NA] %", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[6], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
			{
				"D", "Exit", 0, 0,
				ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
				c[1], r[7], text_width, text_height,
				0, 0, 0, 0, 1, 1
			},
		}
	};

	//	========== USE BUTTON LUT TO CREATE DISPLAY LIST ==========

	// Application
	using namespace record_03;

	// FOR each row
	for (UINT r = 0; r<number_of_rows; r++)
	{
		// FOR each column
		for (UINT c = 0; c<2; c++)
		{
			// Initialise element count and lut
			UINT element_count = 1;
			UINT element_version_lut[3];
			FLOAT uv_adjust = 0.0f;

			// IF type == text THEN version == text ELSE version == background
			if (b[c][r].type == ElmContext::TYPE_TEXT)
			{
				element_version_lut[0] = ElmContext::VERSION_TEXT;
				element_count = 1;
			}
			else
			{
				if (b[c][r].behaviour == ElmContext::BEHAVIOUR_SWITCH)
				{
					element_version_lut[0] = ElmContext::VERSION_BACKGROUND;
					element_version_lut[1] = ElmContext::VERSION_ICON;
					element_version_lut[2] = ElmContext::VERSION_OUTLINE;
					element_count = 3;
					uv_adjust = -0.001f;
				}
				else
				{
					element_version_lut[0] = ElmContext::VERSION_ICON;
					element_count = 1;
				}
			}

			// FOR each element in a button
			for (UINT e = 0; e < element_count; e++)
			{
				// Add to display list
				INT button_handle = pi_model->gfx->AddElement(
						display_list_handle,
						b[c][r].name,
						b[c][r].group_id,
						b[c][r].id,
						&pi_model->obj_config->Callback_Button_Configure
					);

				// IF "Record" icon THEN store handle
				BOOL switched_on = FALSE;
				if ( b[c][r].id == CAM_REPORT_RECORD )
				{
					switched_on = TRUE;
				}

				// Store column 1 text handles
				if (c == 1) pi_model->handle_cam_report_text[r] = button_handle;

				// Initialiase
				vs_system::ElmContext *elm = NULL;
				INT get_element_result = pi_model->gfx->GetElementContext(button_handle, &elm);
				if (get_element_result != NULL)
				{
					result = elm->SetFontName("CamModes");
					result = elm->SetFontAlignment(VS_ALIGN_LEFT);
					result = elm->SetText(b[c][r].text);

					if (c == 1)
					{
						switch (r)
						{
							case 1:
							case 2:
							case 3:
								{
								std::string tmp;
								tmp = group_text[r - 1];
								result = elm->SetText(tmp);
								}
								break;
							default:
								break;
						}
					}
					else


					result = elm->SetBehaviour(b[c][r].behaviour);
					result = elm->SetVersion(element_version_lut[e]);
					result = elm->SetSwitchedOn(switched_on);
					result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

					// Parameters
					FLOAT param[VS_MAX_ELM_PARAMETER_DATA] =
					{
						// SRT
						(FLOAT)ElmContext::VIEWPORT_HEIGHT,

						// XY
						b[c][r].x, b[c][r].y,
						b[c][r].width, b[c][r].height,

						// UV
						b[c][r].bg_col, b[c][r].bg_row,
						b[c][r].icon_col, b[c][r].icon_row,
						b[c][r].dim_col, b[c][r].dim_row,

						// IF background element THEN eat edge ELSE use specified UV adjust
						element_version_lut[e] == ElmContext::VERSION_BACKGROUND ? uv_adjust : uv_adjust
					};
					result = elm->SetParameter(sizeof(param) / sizeof(FLOAT), param);
				}

			}   // --- FOR each element in a button

		}   // --- FOR each column

	}   // --- FOR each row

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupCamModes ----------
/*!
\brief Setup cam modes display list
\author Gareth Edwards
\return INT - Display list handle
*/
INT Model::SetupCamModes()
{
	// Local
	INT result;

	// Add, Setup and then Update
	pi_model->handle_cam_modes = pi_model->gfx->AddDisplayList("CamModes");
	result = SetupCamModesDisplayList(pi_model->handle_cam_modes);
	result = UpdateCamModesDisplayList(TRUE);
	return pi_model->handle_cam_modes;
}


// ---------- SetupCamModesDisplayList ----------
/*!
\brief Setup camera config display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCamModesDisplayList(INT display_list_handle)
{
	// Local
	INT result = 0;

	// Initialise location/layout parameters
	FLOAT x_top         = 0.10f;
	FLOAT y_top         = 0.10f;
	FLOAT space_width   = 0.04f;
	FLOAT space_height  = 0.04f;
	FLOAT text_width    = 0.07f;
	FLOAT text_height   = 0.07f;
	FLOAT icon_width    = 0.10f;
	FLOAT icon_height   = 0.10f;

	// Initialise row location
	FLOAT r[8][2];
	FLOAT y = y_top;
	for ( INT i=0; i<8; i++ )
	{
		INT j = i-1; // No increment for first row
		y += j >= 0 ? (j % 2 == 0 ? text_height : icon_height + space_height) : 0;
		r[i][0] = y;
		r[i][1] = i % 2 == 0 ? text_height : icon_height;
	}

	// Initialise # of buttons
	const UINT number_of_buttons = 15;

	// Initialise column location
	FLOAT c[number_of_buttons] =
	{
		//	========== LIGHTING ==========
		x_top,                                     // 0
		x_top,                                     // 1
		x_top  + (space_width*1) + (icon_width*1), // 2 
		x_top  + (space_width*2) + (icon_width*2), // 3
		x_top  + (space_width*3) + (icon_width*3), // 4

		//	========== BRIGHTNESS ==========
		x_top,                                     // 5
		x_top,                                     // 6
		x_top  + (space_width*1) + (icon_width*1), // 7
		x_top  + (space_width*2) + (icon_width*2), // 8

		//	========== FRAME RATE ==========
		x_top,                                     // 9
		x_top,                                     // 10
		x_top  + (space_width*1) + (icon_width*1), // 11
		x_top  + (space_width*2) + (icon_width*2), // 12

		//	========== SAVE ==========
		x_top,                                     // 13
		x_top                                      // 14
	};


	//	========== BUILD BUTTON LUT ==========

	// System
	using namespace vs_system;

	// Initialise element count and lut
	UINT element_count = 1;
	UINT element_version_lut[3] =
	{
		ElmContext::VERSION_BACKGROUND,
		ElmContext::VERSION_ICON,
		ElmContext::VERSION_OUTLINE
	};

	// Initialise button list
	vs_system::ElmContext::ElmButton b[number_of_buttons] =
	{
		//	========== LIGHTING ==========
		{
			"", "Lighting Condition", 0, 0,                            // 0
			ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
			c[0], r[0][0], r[0][1], r[0][1], 0, 0, 0, 4, 1, 1
		},
		{
			"", "", CamModes::LIGHT_GROUP, CamModes::LIGHT_SUNSHINE, // 1
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[1], r[1][0], r[1][1], r[1][1], 0, 0, 0, 4, 1, 1
		},
		{
			"", "", CamModes::LIGHT_GROUP, CamModes::LIGHT_CLOUDY,   // 2
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[2], r[1][0], r[1][1], r[1][1], 0, 0, 1, 4, 1, 1
		},
		{
			"", "", CamModes::LIGHT_GROUP, CamModes::LIGHT_OVERCAST, // 3
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[3], r[1][0], r[1][1], r[1][1], 0, 0, 2, 4, 1, 1
		},
		{
			"", "", CamModes::LIGHT_GROUP, CamModes::LIGHT_BULB_1,   // 4
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[4], r[1][0], r[1][1], r[1][1], 0, 0, 3, 4, 1, 1
		},

		//	========== BRIGHTNESS ==========
		{
			"", "Brightness", 0, 0,                                    // 5
			ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,
			c[5], r[2][0], r[2][1], r[2][1], 0, 0, 0, 4, 1, 1
		},
		{
			"", "", CamModes::BRIGHT_GROUP, CamModes::BRIGHT_HIGH,   // 6
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[6], r[3][0], r[3][1], r[3][1], 0, 0, 5, 4, 1, 1
		},
		{
			"", "", CamModes::BRIGHT_GROUP, CamModes::BRIGHT_MEDIUM, // 7
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[7], r[3][0], r[3][1], r[3][1], 0, 0, 6, 4, 1, 1
		},
		{
			"", "", CamModes::BRIGHT_GROUP, CamModes::BRIGHT_LOW,    // 8
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[8], r[3][0], r[3][1], r[3][1], 0, 0, 7, 4, 1, 1
		},

		//	========== FRAME RATE ==========
		{
			"", "Frame Rate", 0, 0,
			ElmContext::TYPE_TEXT, ElmContext::BEHAVIOUR_NONE, 0,              // 9
			c[9], r[4][0], r[4][1], r[4][1], 0, 0, 0, 3, 1, 1
		},
		{
			"", "", CamModes::FRAME_RATE_GROUP, CamModes::FRAME_RATE_LOW,    // 10
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[10], r[5][0], r[5][1], r[5][1], 0, 0, 5, 3, 1, 1
		},
		{
			"", "", CamModes::FRAME_RATE_GROUP, CamModes::FRAME_RATE_MEDIUM, // 11
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[11], r[5][0], r[5][1], r[5][1], 0, 0, 6, 3, 1, 1
		},
		{
			"", "", CamModes::FRAME_RATE_GROUP, CamModes::FRAME_RATE_HIGH,   // 12
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_RADIO, 0,
			c[12], r[5][0], r[5][1], r[5][1], 0, 0, 7, 3, 1, 1
		},

		//	========== SAVE ==========
		{
			"", "Save", 0, 0, ElmContext::TYPE_TEXT,                           // 13
			ElmContext::BEHAVIOUR_NONE, 0,
			c[13], r[6][0], r[6][1], r[6][1], 0, 0, 0, 3, 1, 1
		},
		{
			"", "", CamModes::UPDATE_GROUP, CamModes::UPDATE_SAVE,           // 14
			ElmContext::TYPE_ICON, ElmContext::BEHAVIOUR_CLICK, 0,
			c[14], r[7][0], r[7][1], r[7][1], 0, 0, 3, 3, 1, 1
		},
	};

	//	========== USE BUTTON LUT TO CREATE DISPLAY LIST ==========

	// Application
	using namespace record_03;

	// Add buttons to display list
	for ( UINT i=0; i<number_of_buttons; i++ )
	{
		// IF type == icon THEN amplify # of elements
		element_count = b[i].type == vs_system::ElmContext::TYPE_ICON ? 3 : 1;

		// IF type == text THEN version == text ELSE version == background
		element_version_lut[0] =
			b[i].type == vs_system::ElmContext::TYPE_TEXT ?
				vs_system::ElmContext::VERSION_TEXT :
					vs_system::ElmContext::VERSION_BACKGROUND;

		// FOR each element in a button
		for ( UINT e=0; e<element_count; e++ )
		{
			// Add to display list
			INT button_handle = pi_model->gfx->AddElement(
					display_list_handle,
					b[i].name,
					b[i].group_id,
					b[i].id,
					&pi_model->obj_config->Callback_Button_Configure
				);

			// IF "Frame Rate" THEN store handle
			if ( !strcmp("Frame Rate", b[i].text) )
				pi_model->handle_cam_modes_frame_rate_text = button_handle;

			// Initialiase
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = pi_model->gfx->GetElementContext(button_handle, &elm);
			if ( get_element_result != NULL )
			{
				result = elm->SetFontName("CamModes");
				result = elm->SetFontAlignment(VS_ALIGN_LEFT);
				result = elm->SetText(b[i].text);
				result = elm->SetBehaviour(b[i].behaviour);
				result = elm->SetVersion(element_version_lut[e]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

				// Parameters
				FLOAT param[VS_MAX_ELM_PARAMETER_DATA] =
				{
					// SRT
					(FLOAT)vs_system::ElmContext::VIEWPORT_HEIGHT,

					// XY
					b[i].x, b[i].y,
					b[i].width, b[i].height,

					// UV
					b[i].bg_col, b[i].bg_row,
					b[i].icon_col, b[i].icon_row,
					b[i].dim_col, b[i].dim_row,

					// UV adjust
					0.01f
				};
				result = elm->SetParameter(sizeof(param) / sizeof(FLOAT), param);
			}
		}
	}

	// Done
	return VS_OK;
}


// ---------- UpdateCamModesDisplayList ----------
/*!
\brief Setup camera config display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateCamModesDisplayList(BOOL run_time)
{
	// Local
	INT result = 0;
	vs_system::ElmContext *element_context = NULL;

	// Number of elements in display list
	INT number_of_config_elements = pi_model->gfx->GetChildCount(pi_model->handle_cam_modes);

	// FOR each cam mode element
	INT elm = pi_model->gfx->GetFirst(pi_model->handle_cam_modes);
	while ( elm != 0 )
	{
		// IF context THEN
		result = pi_model->gfx->GetElementContext(elm, &element_context);
		if ( result != NULL )
		{
			// Update ICONS - MUST have a background!
			UINT element_version = element_context->GetVersion();
			if ( element_version == vs_system::ElmContext::VERSION_BACKGROUND )
			{
				UINT group_id = element_context->GetGroupId();
				switch ( group_id )
				{
					case vs_system::CamModes::LIGHT_GROUP:
					case vs_system::CamModes::BRIGHT_GROUP:
					case vs_system::CamModes::FRAME_RATE_GROUP:
						{
							// Get group mode index
							UINT mode = pi_model->cam_modes->GetCamModesIndex(group_id);

							// Set ON/OFF
							if ( element_context->GetId() == mode )
							{
								result = element_context->SetSwitchedOn(TRUE);
							}
							else
							{
								result = element_context->SetSwitchedOn(FALSE);
							}
						}
						break;

					default:
						break;
				}
			}
			// Update TEXT
			else if( element_version == vs_system::ElmContext::VERSION_TEXT)
			{
				UINT group_id = element_context->GetGroupId();
				switch ( group_id )
				{
					case vs_system::CamModes::LIGHT_GROUP:
					case vs_system::CamModes::BRIGHT_GROUP:
					case vs_system::CamModes::FRAME_RATE_GROUP:
						{
							// Get group mode index
							UINT mode = pi_model->cam_modes->GetCamModesIndex(group_id);
						}
						break;

					default:
						break;
				}
			}
		}

		// Next
		elm = pi_model->gfx->GetNext(elm);
	}

	// Done
	return VS_OK;
}



// ---------- GetCamModes ----------
/*!
\brief Get camera modes
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetCamModes(vs_system::CamModes **cam_modes)
{
	*cam_modes = this->pi_model->cam_modes;
	return VS_OK;
}


// ---------- GetCamModesPaf ----------
/*!
\brief Get camera modes config path filename
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetCamModesPaf(CHAR *file_name)
{
	INT result = pi_model->cam_modes->GetCamModesPaf(file_name);
	return result;
}


// ---------- ReadCamModes ----------
/*!
\brief Read camera modes
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReadCamModes(CHAR *file_name)
{
	INT result = pi_model->cam_modes->Read(file_name);
	return result;
}


// ---------- UpdateCamModes ----------
/*!
\brief Update camera modes
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateCamModes(UINT group_id, UINT element_id)
{
	INT result = pi_model->cam_modes->Update(group_id, element_id);
	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetCamProps ----------
/*!
\brief Get camera props
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetCamProps(vs_system::CamProps **cam_props)
{
	*cam_props = pi_model->cam_props;
	return VS_OK;
}


// ---------- GetPropsConfigPaf ----------
/*!
\brief Get camera properties config path filename
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetPropsConfigPaf(CHAR *file_name)
{
	// strcpy(file_name, pi_model->cam_props_config_paf);
	return VS_OK;
}


// ---------- ReadCamProps ----------
/*!
\brief Read camera properties
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReadCamProps(CHAR *file_name)
{
	// strcpy(pi_model->cam_props_config_paf, file_name);
	INT result = pi_model->cam_props->LoadConfigHdlFile(file_name);
	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupAbout ----------
/*!
\brief Setup about display list
\author Gareth Edwards
\return INT - Display list handle
*/
INT Model::SetupAbout()
{
	// Local
	INT result;

	// Add, then Setup
	pi_model->handle_about = pi_model->gfx->AddDisplayList("About");
	result = SetupAboutDisplayList(pi_model->handle_about);
	return pi_model->handle_about;
}


// ---------- SetupAboutDisplayList ----------
/*!
\brief Setup about display list
\author Gareth Edwards
\return INT - Display list handle
*/
INT Model::SetupAboutDisplayList(INT display_list_handle)
{
	// Local
	INT result;

	// Initialise layout parameters
	FLOAT x_top = 0.05f;
	FLOAT y_top = 0.05f;
	FLOAT text_width = 0.08f;
	FLOAT text_height = 0.08f;
	FLOAT text_top = y_top + 0.025f;
	FLOAT text_space = -0.015f;

	// Initialise const's
	const INT number_of_rows = 11;

	// Button list
	vs_system::ElmContext::ElmButton b[number_of_rows];

	// Local
	CHAR about[VS_MAXCHARLEN];
	CHAR version[VS_MAXCHARLEN];
	CHAR licensed_to[VS_MAXCHARLEN];
	CHAR license_expiry[VS_MAXCHARLEN];
	vs_system::GfxInterface *gfx_interface = pi_model->app_dependency->GetGfxInterface();
	vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
	CHAR *app_title = sys_context->GetChar(vs_system::SysContext::APP_TITLE);
	CHAR *app_name = sys_context->GetChar(vs_system::SysContext::APP_NAME);

	// VS Version
	DWORD sys_version_major = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_MAJOR);
	DWORD sys_version_minor = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_MINOR);
	DWORD sys_version_patch = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_PATCH);
	DWORD sys_version_build = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_BUILD);

	// APP Version
	DWORD app_version_major = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_MAJOR);
	DWORD app_version_minor = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_MINOR);
	DWORD app_version_patch = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_PATCH);
	DWORD app_version_build = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_BUILD);

	// About + VS info ....
	sprintf_s(
			about,
			VS_MAXCHARLEN,
			"ABOUT (vs version %d.%d.%d.%d, built %s on %s)",
			sys_version_major,
			sys_version_minor,
			sys_version_patch,
			sys_version_build,
			__TIME__,
			__DATE__
		);

	// Application info....
	sprintf_s(
			version,
			VS_MAXCHARLEN,
			"%s (%s version %d.%d.%d.%d)",
			app_title,
			app_name,
			app_version_major,
			app_version_minor,
			app_version_patch,
			app_version_build
		);

	// Licensed to....
	sprintf_s(
			licensed_to,
			VS_MAXCHARLEN,
			"Licensed to: %s",
			VS_LICENSEE
		);


	// Licensed unto....
	if ( VS_LICENSE_EXPIRE_YEAR == 0 )
	{
		sprintf_s(
			license_expiry,
			VS_MAXCHARLEN,
			"License: No Expiry"
			);
	}
	else
	{
		// Licesned to....
		CHAR *month_name[12] =
		{
			"Jan", "Feb", "Mar",
			"Apr", "May", "Jun",
			"Jul", "Aug", "Sep",
			"Oct", "Nov", "Dec"
		};
		INT month = VS_LICENSE_EXPIRE_MONTH < 1 ? 1 :
			( VS_LICENSE_EXPIRE_MONTH > 12 ? 12 : VS_LICENSE_EXPIRE_MONTH )
			;
		sprintf_s(
				license_expiry,
				VS_MAXCHARLEN,
				"License expires: %s %d %d",
				month_name[month - 1], // BUG - span needs to be checked
				VS_LICENSE_EXPIRE_DAY,
				VS_LICENSE_EXPIRE_YEAR
			);
	}

	// Initialise button group names
	CHAR *n[number_of_rows] =
	{
		about,
		version,
		licensed_to,
		license_expiry,
		"Observant Innovations Limited",
		"        The Museum Room, Avington Park",
		"        Winchester, Hampshire SO21 1DB, UK",
		"        Office Tel #: +44 (0) 2381 680050",
		"        Support Tel #: +44 (0) 1962 779107",
		"        Web: www.observant-innovations.com",
		"Copyright: © 2013 to 2016 Observant Innovations Limited"
	};

	// Initialise row location
	FLOAT r[number_of_rows];
	FLOAT y = y_top;
	for (INT i = 0; i<number_of_rows; i++)
	{
		switch (i)
		{
			case 0:
				break;
			case 1:
				y += y_top / 2;
			case 2:
			case 4:
				y += text_height / 4.0f;
			case number_of_rows - 1:
				y += text_height / 4.0f;
			default:
				y += text_height + text_space;
				break;
		}
		r[i] = y;
	}

	// Initialise button list by data amplification 
	UINT index = 0;
	for (INT i = 0; i<number_of_rows; i++)
	{
		b[index] = {
			"T", "T",
			0, 0,
			vs_system::ElmContext::TYPE_TEXT,
			vs_system::ElmContext::BEHAVIOUR_NONE,
			0,
			x_top + (i == 0 ? 0 : x_top * 3), r[i], text_width, text_height,
			0, 0, 0, 0, 1, 1
		};
		strcpy(b[index++].name, n[i]);
	}

	// Item index
	INT item_counter = 0;

	// Add buttons
	for (UINT i = 0; i<number_of_rows; i++)
	{
		// FOR each element in a button
		{
			// Add to display list
			INT button_handle = gfx_interface->AddElement(
					display_list_handle,
					b[i].name,
					b[i].group_id,
					b[i].id,
					&pi_model->obj_config->Callback_Button_Configure
				);

			// Initialise
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx_interface->GetElementContext(button_handle, &elm);
			if (get_element_result != NULL)
			{
				result = elm->SetFontName("About");
				result = elm->SetFontAlignment(VS_ALIGN_LEFT);
				result = elm->SetText(b[i].name);
				result = elm->SetBehaviour(b[i].behaviour);
				result = elm->SetVersion(vs_system::ElmContext::VERSION_TEXT);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

				// Parameters
				FLOAT param[VS_MAX_ELM_PARAMETER_DATA] =
				{
					// SRT
					(FLOAT)vs_system::ElmContext::VIEWPORT_HEIGHT,

					// XY
					b[i].x, b[i].y,
					b[i].width, b[i].height,

					// UV
					0, 0, 0, 0, 0, 0,

					// UV adjust
					0.01f

				};
				result = elm->SetParameter(sizeof(param) / sizeof(FLOAT), param);
			}
		}
	}

	// Add Close
	UINT element_version_lut[3] =
	{
		vs_system::ElmContext::VERSION_BACKGROUND,
		vs_system::ElmContext::VERSION_ICON,
		vs_system::ElmContext::VERSION_OUTLINE
	};
	for (INT i = 0; i < 3; i++)
	{
		// Add to display list
		INT button_handle = gfx_interface->AddElement(
				display_list_handle,
				"About Close",
				ABOUT_BUTTON_GROUP_ID,
				ABOUT_CLOSE,
				&pi_model->obj_config->Callback_Button_Configure
			);

			// Initialise
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx_interface->GetElementContext(button_handle, &elm);
			if ( get_element_result != NULL )
			{
				result = elm->SetFontName("About");
				result = elm->SetFontAlignment(VS_ALIGN_LEFT);
				result = elm->SetText("EXIT");
				result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLICK);
				result = elm->SetVersion(element_version_lut[i]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

				// Parameters
				FLOAT param[VS_MAX_ELM_PARAMETER_DATA] =
				{
					// SRT
					(FLOAT)vs_system::ElmContext::VIEWPORT_HEIGHT,

					// XY
					-text_width-y_top/2, y_top/2,
					text_width, text_width,

					// UV
					0, 0, 7, 1, 1, 1,

					// UV adjust
					0.0f

				};
				result = elm->SetParameter(sizeof(param) / sizeof(FLOAT), param);
			}
	}
	// Done
	return VS_OK;
}


// ---------- SetAboutDisplayStatus ----------
/*!
\brief Setup about panel display status
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetAboutDisplayStatus()
{
	// Local
	INT result = VS_ERROR;

	if ( pi_model->about_panel_gfx_context != NULL )
	{
		BOOL panel_status = TRUE;
		pi_model->about_panel_gfx_context->GetDisplayStatus(&panel_status);
		panel_status = panel_status == TRUE ? FALSE : TRUE;
		pi_model->about_panel_gfx_context->SetDisplayStatus(panel_status);
		result = panel_status == TRUE ? VS_OK : VS_ERROR;
	}

	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetupFrameset ----------
/*!
\brief Uppdate frameset
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::SetupFrameset(
	vs_system::AppDependency *app_dependency,
	vs_system::AppView *app_view
	)
{
	// Local
	INT result = VS_OK;

	// Accelerate
	vs_system::SysContext *sys_context = app_dependency->GetSysContext();

	// ==================== SETUP ====================

	// Get client rectangle
	RECT client_rect;
	sys_context->GetClientRect(&client_rect);

	// Set width and height
	FLOAT width = (FLOAT)(client_rect.right - client_rect.left);
	FLOAT height = (FLOAT)(client_rect.bottom - client_rect.top);
	FLOAT aspect = width / height;

	// Get border width
	FLOAT border_width = sys_context->GetFloat(sys_context->BORDER_WIDTH);

	// ==================== Y ====================

	// Set y proportional spacing
	FLOAT y_bar_prop = 0.050f + 0.02f;
	FLOAT y_space_prop = 0.004f + 0.001f;
	FLOAT y_border = y_space_prop * border_width;

	// Y
	FLOAT y_top = y_space_prop;
	FLOAT y_bottom = 1.0f - (y_space_prop * border_width);
	FLOAT y_height = y_bottom - y_top;

	// Y Title
	FLOAT y_title_top = y_border;
	FLOAT y_title_bottom = y_title_top + y_bar_prop * 0.8f;

	// Y Content
	FLOAT y_content_top = y_title_bottom + y_space_prop;
	FLOAT y_content_bottom = y_bottom;

	// ==================== X ====================

	// Set x proportional spacing
	FLOAT x_space_prop = y_space_prop / aspect;
	FLOAT x_border = x_space_prop * border_width;

	// Set x borders
	FLOAT x_left = x_border;
	FLOAT x_right = 1 - x_left;

	// ==================== SET UP PANEL LUT ====================

	// Set # of panels
	const int lut_number = 4;

	// Panel name and rectangle
	struct panel_nar {
		CHAR *name;
		VS_FloatRectangle fr;
	};

	// Initialise
	panel_nar lut[lut_number] =
	{
		{ "TitleBar",  x_left, y_title_top, x_right, y_title_bottom },
		{ "CamReport", x_left, y_content_top, x_right, y_content_bottom },
		{ "CamModes", x_left, y_content_top, x_right, y_content_bottom },
		{ "About", x_left+0.1f, y_content_top+0.1f, x_right-0.1f, y_content_bottom-0.1f },
	};

	// Note: BUG - what if no name match!!!

	// Setup framesets
	CHAR name[VS_MAXCHARNAME];
	vs_system::AppFrame *frame = app_view->GetFirstFrame();
	while (frame)
	{
		// Setup panels
		vs_system::AppPanel *panel = frame->GetFirstPanel();
		while (panel)
		{
			// Set rect
			vs_system::GfxContext *gfx_context = panel->GetGfxContext();
			VS_FloatRectangle *vr = gfx_context->GetViewrect();
			INT result = gfx_context->GetName(name);
			for (UINT i = 0; i<lut_number; i++)
			{
				if (lut[i].name != 0 && !strcmp(lut[i].name, name))
				{
					*vr = lut[i].fr;
					break;
				}
			}

			// Next panel
			panel = panel->GetNext();
		}

		// Next frame
		frame = frame->GetNext();
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetConfigFiles ----------
/*!
\brief Set sensor config path filename
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetConfigFiles(ConfigFiles **cf)
{
	*cf = pi_model->config_files;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetDeviceStatus ----------
/*!
\brief Get device status
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::GetDeviceStatus()
{
	return pi_model->gfx->GetDeviceStatus();
}


// ---------- SetConfigureAllBitmask ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::SetConfigureAllBitmask(BYTE setup_bitmask)
{
	pi_model->gfx->SetConfigureBitmask(setup_bitmask, "");
	return VS_OK;
}


// ---------- UpdateRecordText ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::UpdateRecordText(
	CHAR * filename
	)
{
	// IF display list THEN
	if ( pi_model->handle_cam_report != 0 )
	{
		// IF record text element THEN
		if ( pi_model->handle_cam_report_text[Model::PI_Model::CAM_REPORT_TEXT_RECORD] != 0 )
		{
			// IF element context THEN update frame rate text
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = pi_model->gfx->GetElementContext(
				pi_model->handle_cam_report_text[Model::PI_Model::CAM_REPORT_TEXT_RECORD],
				&elm
				);
			if (get_element_result != NULL)
			{
				INT result = elm->SetText(filename);
			}
		}
	}

	// Done
	return VS_OK;
}


// ---------- UpdateFrameRateText ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::UpdateFrameRateText(
		UINT ips
	)
{
	// List of display list handles with frame rate text
	INT display_list_handles[2] =
	{
		pi_model->handle_cam_report,
		pi_model->handle_cam_modes
	};

	// List of frame rate text element handles
	INT frame_rate_handles[2] =
	{
		pi_model->handle_cam_report_text[Model::PI_Model::CAM_REPORT_TEXT_FRAMERATE],
		pi_model->handle_cam_modes_frame_rate_text
	};

	// FOR each display list in list
	for ( int i = 0; i < 2; i++ )
	{
		// IF display list THEN
		if ( display_list_handles[i] != 0 )
		{
			// IF frame rate element THEN
			if ( frame_rate_handles[i] != 0 )
			{
				// IF element context THEN update frame rate text
				vs_system::ElmContext *elm = NULL;
				INT get_element_result = pi_model->gfx->GetElementContext(
						frame_rate_handles[i],
						&elm
					);
				if (get_element_result != NULL)
				{
					CHAR text[VS_MAXCHARNAME];
					sprintf_s(text, "Frame Rate ( %d FPS )", ips);
					INT result = elm->SetText(text);
				}
			}
		}
	}

	// Done
	return VS_OK;

}


////////////////////////////////////////////////////////////////////////////////
