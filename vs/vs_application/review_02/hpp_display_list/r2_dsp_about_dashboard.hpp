////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_about_dashboard.hpp ----------
/*!
\file r2_dl_about_dashboard.hpp
\brief 
\author Gareth Edwards
*/

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupAboutDashboard ----------
/*!
\brief Setup about dashboard text
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupAboutDashboard()
{
 
	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- format scaling
		db_param.global_scale = format == vs_system::SysContext::PORTRAIT ? 0.5f : 1.0f;


	// ---- text height
		FLOAT y_text = format == vs_system::SysContext::PORTRAIT ?
				db_param.portrait_y_text :
					db_param.landscape_y_text;


	// --- offsets
		FLOAT global_scale, x_off_1, x_off_2, y_off_1, y_off_2;
		FLOAT y_loc[5];
		if (format == vs_system::SysContext::PORTRAIT)
		{
			global_scale = 0.7f * db_param.global_scale;
			x_off_1 = -1.0f * global_scale;
			x_off_2 = -1.0f * global_scale;
			y_off_1 = -1.0f * global_scale;
			y_off_2 = -1.0f * global_scale;
			FLOAT y[5] = { -1.45f, -0.85f, -0.25f, 0.80f, 1.40f };
			for (INT i = 0; i < 5; i++) y_loc[i] = y[i] * global_scale;
		}
		else
		{
			global_scale = 0.7f * db_param.global_scale;
			x_off_1 = -1.4f;
			x_off_2 =  0.1f;
			y_off_1 = -0.1f ;
			y_off_2 =  y_off_1;
			FLOAT y[5] = { -1.00f, -0.33f, 0.33f, 1.00f, -1.00f };
			for (INT i = 0; i < 5; i++) y_loc[i] = y[i] * global_scale;
		}
		

	// ---- locate
		struct label_location
			{
				FLOAT x, y;
			};
		label_location loc[] =
			{
				{ x_off_1, y_loc[0] + y_off_1 },
				{ x_off_1, y_loc[1] + y_off_1 },
				{ x_off_1, y_loc[2] + y_off_1 },
				{ x_off_1, y_loc[3] + y_off_1 },
				{ x_off_2, y_loc[4] + y_off_2 },
			};


	// ---- font
		FLOAT lal = (FLOAT)pi_model->obj_config->DBZ_TEXT_LABEL_ALIGNED_LEFT;
		FLOAT nal = (FLOAT)pi_model->obj_config->DBZ_TEXT_NORMAL_ALIGNED_LEFT;


	// ---- text
		std::string label[5];
		CHAR content[4][2][VS_MAXCHARLEN];
		CHAR contact[16][VS_MAXCHARLEN];


	// ---- lambda ------------------------------------------------------------
		auto initialise_label = [&]()
		{
			label[0] = "Application";
			label[1] = "Build";
			label[2] = "License";
			label[3] = "Copright";
			label[4] = "Contact";
		};


	// ---- lambda ------------------------------------------------------------
		auto initialise_content = [&]()
		{

		// ---- context
			vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();


		// ---- 0 : application
			DWORD app_version_major = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_MAJOR);
			DWORD app_version_minor = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_MINOR);
			DWORD app_version_patch = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_PATCH);
			DWORD app_version_build = sys_context->GetDWord(vs_system::SysContext::APP_VERSION_BUILD);
			CHAR *app_title = sys_context->GetChar(vs_system::SysContext::APP_TITLE);
			CHAR *app_name = sys_context->GetChar(vs_system::SysContext::CONFIG_NAME);
			sprintf_s(
					content[0][0],
					VS_MAXCHARLEN,
					"%s ( \"%s\" )",
					app_title,
					app_name
				);
			sprintf_s(
					content[0][1],
					VS_MAXCHARLEN,
					"Version %d.%d.%d.%d",
					app_version_major,
					app_version_minor,
					app_version_patch,
					app_version_build
				);


		// ---- 1 : build ------------------------------------------------------------
			DWORD sys_version_major = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_MAJOR);
			DWORD sys_version_minor = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_MINOR);
			DWORD sys_version_patch = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_PATCH);
			DWORD sys_version_build = sys_context->GetDWord(vs_system::SysContext::SYS_VERSION_BUILD);
			sprintf_s(
					content[1][0],
					VS_MAXCHARLEN,
					"Built %s on %s",
					__TIME__,
					__DATE__
				);
			sprintf_s(
					content[1][1],
					VS_MAXCHARLEN,
					"Version %d.%d.%d.%d",
					sys_version_major,
					sys_version_minor,
					sys_version_patch,
					sys_version_build
				);


		// ---- 2 : license ------------------------------------------------------------
			if ( sys_context->GetBool(sys_context->APP_LICENSED) )
			{
				sprintf_s(
						content[2][0],
						VS_MAXCHARLEN,
						"%s",
						VS_LICENSEE
					);
				CHAR *month_name[12] =
				{
					"Jan", "Feb", "Mar",
					"Apr", "May", "Jun",
					"Jul", "Aug", "Sep",
					"Oct", "Nov", "Dec"
				};
				if (VS_LICENSE_EXPIRE_MONTH <= 0)
				{
					sprintf_s(
							content[2][1],
							VS_MAXCHARLEN,
							"Locked to System not Date"
						);
				}
				else
				{
					INT month = VS_LICENSE_EXPIRE_MONTH < 1 ? 1 :
						(VS_LICENSE_EXPIRE_MONTH > 12 ? 12 : VS_LICENSE_EXPIRE_MONTH)
						;
					sprintf_s(
							content[2][1],
							VS_MAXCHARLEN,
							"Expires on %s %d %d",
							month_name[month - 1],
							VS_LICENSE_EXPIRE_DAY,
							VS_LICENSE_EXPIRE_YEAR
						);
				}
			}
			else
			{
				sprintf_s(
						content[2][0],
						VS_MAXCHARLEN,
						"Not Required"
					);
				sprintf_s(
						content[2][1],
						VS_MAXCHARLEN,
						""
					);
			}


		// ---- 3 : copyright ------------------------------------------------------------
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
			sprintf_s(
					content[3][0],
					VS_MAXCHARLEN,
					"Copyright © 2013 to %d",
					(INT)SystemTime.wYear
				);
			sprintf_s(
					content[3][1],
					VS_MAXCHARLEN,
					"Observant Innovations Limited"
				);

		};


	// ---- contact ------------------------------------------------------------
		auto initialise_contact = [&]()
		{
			CHAR *c[] =
			{
				"1Observant Innovations Limited",
				"3",
				"2   The Museum Room",
				"2   Avington Park",
				"2   Winchester",
				"2   Hampshire",
				"2   SO21 1DB, UK",
				"3",
				"2   www.observant-innovations.com",
				"1",
				"0Office",
				"2   T: +44 (0) 2381 680050",
				"1",
				"0Support",
				"2   T: +44 (0) 1962 779107",
				"1   E: support@observant-innovations.com",
			};
			for (UINT i = 0; i < 16; i++)
			{
				strcpy(contact[i], c[i]);
			}
		};


	// ---- label ------------------------------------------------------------
		auto add_label = [&](INT i)
		{
			// ---- add
				INT handle = gfx->AddElement(
						pi_model->handle_dspl_about,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Text
					);

				vs_system::ElmContext *elm = NULL;
			result = gfx->GetElementContext(handle, &elm);
	
			// ---- set elm
				elm->SetText(label[i]);
				elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);

			// ---- set db
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetConfig(p, lal);
				result = pi_model->obj_config->Dbz_SetVisible(p, 1);
				result = pi_model->obj_config->Dbz_SetScale(p, 0, y_text);
				result = pi_model->obj_config->Dbz_SetTranslate(p, loc[i].x, loc[i].y);
		};


	// ---- add ------------------------------------------------------------
		auto add_enclose = [&](INT i)
		{

			// ---- local
				FLOAT x_width = i < 4 ? 1.300f * db_param.global_scale : 1.35f * db_param.global_scale;
				FLOAT y_width = i < 4 ? 0.235f * db_param.global_scale : 1.40f * db_param.global_scale;
				FLOAT x_off =  0.00f * db_param.global_scale;
				FLOAT y_off =  0.06f * db_param.global_scale;
				FLOAT x_loc = loc[i].x + x_off + (x_width / 2);
				FLOAT y_loc = loc[i].y + y_off + (y_width / 2);


			// ---- add
				INT handle_enclose = gfx->AddElement(
						pi_model->handle_dspl_about,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Rect
					);

				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(handle_enclose, &elm);


			// ---- set elm
				elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 0.25f);


			// ---- set db
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetConfig(p, 0);
				result = pi_model->obj_config->Dbz_SetVisible(p, 1);
				result = pi_model->obj_config->Dbz_SetScale(p, 1.0f, 1.0f);
				result = pi_model->obj_config->Dbz_SetTranslate(p, x_loc, y_loc);

				result = pi_model->obj_config->Dbz_SetRectDimension(p, x_width, y_width);
				result = pi_model->obj_config->Dbz_SetRectQuad(p,
						60,
						db_param.nclose_radius * db_param.global_scale + db_param.nclose_width,
						db_param.nclose_radius * db_param.global_scale
					);
				result = pi_model->obj_config->Dbz_SetRectRGBMax(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);
				result = pi_model->obj_config->Dbz_SetRectRGBMin(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);
		
		};


	// ---- add ------------------------------------------------------------
		auto add_content = [&](INT i)
		{
			FLOAT x_off, y_off, y_line, y_loc, y_inc_2, y_inc_3;
			if (format == vs_system::SysContext::PORTRAIT)
			{
				x_off  = 0.050f;
				y_off  = i < 4 ? 0.0125f : 0;
				y_line = i < 4 ? 0.0525f : 0.085f;
				y_loc  = loc[i].y + y_off + y_line;
				y_inc_2 = 0.4f;
				y_inc_3 = 0.08f;
			}
			else
			{
				x_off  = 0.100f;
				y_off  = 0.025f;
				y_line = 0.105f;
				y_loc  = loc[i].y + y_off + y_line;
				y_inc_2 = 0.7f;
				y_inc_3 = 0.25f;
			}

			UINT  total  = i < 4 ? 2 : 16;

			for (UINT j = 0; j < total; j++)
			{

				// ---- add
					INT handle = gfx->AddElement(
							pi_model->handle_dspl_about,
							"",
							0,
							0,
							&pi_model->obj_config->Dbz_Callback_Configure_Text
						);

					vs_system::ElmContext *elm = NULL;
					result = gfx->GetElementContext(handle, &elm);


				// ---- set elm
					CHAR *text = i < 4 ? content[i][j] : contact[j]+1;
					elm->SetText(text);
					elm->SetRGBA(db_param.m_blip, db_param.m_blip, db_param.m_blip, 1.0f);


				// ---- set db
					FLOAT *p = elm->GetFloatParameter();
					result = pi_model->obj_config->Dbz_SetDefault(p);
					result = pi_model->obj_config->Dbz_SetConfig(p, nal);
					result = pi_model->obj_config->Dbz_SetVisible(p, 1);
					result = pi_model-> obj_config->Dbz_SetScale(p, 0, 0.07f * db_param.global_scale);

					result = pi_model->obj_config->Dbz_SetTranslate(p, loc[i].x + x_off, y_loc);

					FLOAT y_incr = y_line;

					if ( i == 4 )
					{
						CHAR c = *contact[j];
						switch (c)
						{
							case '0' :
								break;
							case '1':
							case '2':
								y_incr *= y_inc_2;
								break;
							default:
								y_incr *= y_inc_3;
								break;
						}
					}

					y_loc += y_incr;

			
				}
		};


	// ---- initialise
		initialise_label();
		initialise_content();
		initialise_contact();

		 
	// ---- add
		for (UINT i = 0; i < 5; i++)
		{
			add_label(i);
			add_enclose(i);
			add_content(i);
		}

	return VS_OK;
}
