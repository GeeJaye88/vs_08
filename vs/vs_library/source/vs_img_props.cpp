////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_props.cpp ----------
/*!
\file vs_img_props.cpp
\brief Implementation of the ImgProps class.
\author Gareth Edwards 
\note 2017
*/

// ---- include ----
#include "../header/vs_img_props.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ImgProps class ----------

class ImgProps::PI_ImgProps
{

public:

	// ---- structs
		struct BCS
		{
			FLOAT brightness;
			FLOAT contrast;
			FLOAT saturation;
		};


	// ---- ctor
		PI_ImgProps()
		{
			Initialise();
		}


	// ---- dtor
		~PI_ImgProps()
		{
			;
		}


	// ---- methods
		INT Initialise();


	// VS object pointers
		vs_system::AppDependency *_inj_app_dependency; 


	// ---- properties
		VNode* _config_hdl = NULL;
		BOOL   _gfx_shader_increment = 1;


	// ---- properties set from "img_props.hdl"
		UINT _gfx_shader_mode = 1;
		BCS  _gfx_shader[4];

};


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ImgProps::PI_ImgProps::Initialise()
{

	// ---- gfx shader
		_gfx_shader_mode = 1;
		for (INT i = 0; i<4; i++)
		{
			_gfx_shader[i].brightness = 1;
			_gfx_shader[i].contrast   = 0;
			_gfx_shader[i].saturation = 0;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgProps::ImgProps()
{
	_pi_img_props = new PI_ImgProps();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgProps::~ImgProps()
{
	if ( _pi_img_props != NULL ) { delete _pi_img_props; _pi_img_props = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ImgProps::Setup(vs_system::AppDependency *app_dependency)
{
	_pi_img_props->_inj_app_dependency = app_dependency;
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ImgProps::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Read ----------
/*!
\brief Read camera properties configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgProps::Read(CHAR *hdl_paf)
{

	// ---- file exists ?
	if ( GetFileAttributes(hdl_paf) != INVALID_FILE_ATTRIBUTES )
	{

		// ---- READ config file
			vs_system::VNodeIO vio;
			VNode *config_hdl = _pi_img_props->_config_hdl = vio.Read(hdl_paf);


		// ---- sensor config file NOT READ ? -- report && return
			if ( config_hdl == NULL )
			{
				vs_system::SysOutput *sys_output = _pi_img_props->_inj_app_dependency->GetSysOutput();
				sys_output->MessageError(
						"FAILED to READ image config file.",
						"File Error",
						hdl_paf
					);
				return VS_ERROR;
			}

		// ---- ELSE READ THEN --
			else
			{

				// ---- shader
					VNode *shader = config_hdl->SearchThis("Shader");
					if ( shader != NULL )
					{

						VNode *b = shader->SearchName("Mode");
						if ( b != NULL ) _pi_img_props->_gfx_shader_mode = (UINT)b->GetFloatFromValue();

						CHAR *name[4] =
						{
							"1",
							"2",
							"3",
							"4",
						};
						for (UINT index = 0; index < 4; index++)
						{
							VNode *c = shader->SearchThis(name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Brightness") ) _pi_img_props->_gfx_shader[index].brightness = d->GetFloatFromValue();
								if (d = c->SearchName("Contrast")   ) _pi_img_props->_gfx_shader[index].contrast   = d->GetFloatFromValue();
								if (d = c->SearchName("Saturation") ) _pi_img_props->_gfx_shader[index].saturation = d->GetFloatFromValue();
							}
						}
					}


				// ---- REPORT
					{
						CHAR msg[VS_MAXCHARLEN];
						sprintf(msg, "+++ Read image properties from config file:\n");
						OutputDebugString(msg);
						sprintf(msg, "   %s\n", hdl_paf);
						OutputDebugString(msg);
					}

				return VS_OK;
			}
		}


	// ---- REPORT
		{
			CHAR msg[VS_MAXCHARLEN];
			sprintf(msg, "+++ FAILED to read image properties from config file:\n");
			OutputDebugString(msg);
			sprintf(msg, "   %s\n", hdl_paf);
			OutputDebugString(msg);
		}


	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write camera properties configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgProps::Write(CHAR * hdl_paf)
{

	// ---- IF file exists THEN --
	if ( _pi_img_props->_config_hdl != NULL )
	{

		VNode *config_hdl = _pi_img_props->_config_hdl;


		// ---- shader
			VNode *shader = config_hdl->SearchThis("Shader");
			if (shader != NULL)
			{
				VNode *b = shader->SearchName("Mode");
				if ( b != NULL ) b->SetValue((FLOAT)_pi_img_props->_gfx_shader_mode);
				CHAR *name[4] =
				{
					"1",
					"2",
					"3",
					"4",
				};
				for (UINT index = 0; index < 4; index++)
				{
					VNode *c = shader->SearchThis(name[index]);
					if (c != NULL)
					{
						VNode *d = NULL;
						if ( d = c->SearchName("Brightness") ) d->SetValue(_pi_img_props->_gfx_shader[index].brightness);
						if ( d = c->SearchName("Contrast")   ) d->SetValue(_pi_img_props->_gfx_shader[index].contrast  );
						if ( d = c->SearchName("Saturation") ) d->SetValue(_pi_img_props->_gfx_shader[index].saturation);
					}
				}
			}


		// ---- write config file
			vs_system::VNodeIO vio;
			INT result = vio.Write(hdl_paf, _pi_img_props->_config_hdl);


		// ---- REPORT
			CHAR msg[VS_MAXCHARLEN];
			if ( result == VS_OK )
			{
				sprintf(msg, "+++ SAVED image properties to config file:\n");
				OutputDebugString(msg);
				sprintf(msg, "   %s\n", hdl_paf);
				OutputDebugString(msg);
			}
			else
			{
				sprintf(msg, "+++ FAILED to save image properties to config file:\n");
				OutputDebugString(msg);
				sprintf(msg, "   %s\n", hdl_paf);
				OutputDebugString(msg);
			}


		// ----  NOT successful ? -- return error
			if (result == VS_ERROR) return VS_ERROR;

	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT ImgProps::SetGfxShaderMode(UINT mode)
{
	// TBD error checking
	_pi_img_props->_gfx_shader_mode = mode;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetProperty ----------
INT ImgProps::GetProperty(
		UINT group,
		UINT index,
		UINT id,
		INT *value
	)
{
	FLOAT f = 0;
	INT result = GetProperty
		(
			group,
			index,
			id,
			&f
		);
	*value = (INT)f;
	return VS_OK;
}


// ---------- GetProperty ----------
/*!
\brief Get property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ImgProps::GetProperty(
		UINT group,
		UINT mode,    // 1 to 4
		UINT id,
		FLOAT *value
	)
{
	
	using namespace vs_system;

	switch ( group )
	{
		case GFX_SHADER:
		{
			UINT index = mode - 1;
			switch ( id )
			{
				case MODE      : *value = (FLOAT) _pi_img_props->_gfx_shader_mode;              return VS_OK;
				case INCREMENT : *value = (FLOAT) _pi_img_props->_gfx_shader_increment;         return VS_OK;
				case BRIGHTNESS: *value = (FLOAT) _pi_img_props->_gfx_shader[index].brightness; return VS_OK;
				case CONTRAST  : *value = (FLOAT) _pi_img_props->_gfx_shader[index].contrast;   return VS_OK;
				case SATURATION: *value = (FLOAT) _pi_img_props->_gfx_shader[index].saturation; return VS_OK;
				default:
					;
			}
		}
		break;

		default:
			break;
	}

	return VS_ERROR;
}


// ---------- SetProperty ----------
/*!
\brief Set property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT ImgProps::SetProperty(
		UINT  group,   // [in] 
		UINT  mode,    // [in] 1 to 4
		UINT  id,      // [in] e.g. MODE
		FLOAT value    // [in]
	)
{

	using namespace vs_system;

	switch ( group )
	{

		case GFX_SHADER:
		{
			UINT index = mode - 1;
			switch (id)
			{
				case MODE      : _pi_img_props->_gfx_shader_mode = (UINT)value; return VS_OK;
				case INCREMENT : _pi_img_props->_gfx_shader_increment = (BOOL)( (INT)value ); return VS_OK;
				case BRIGHTNESS: _pi_img_props->_gfx_shader[index].brightness = value; return VS_OK;
				case CONTRAST  : _pi_img_props->_gfx_shader[index].contrast   = value; return VS_OK;
				case SATURATION: _pi_img_props->_gfx_shader[index].saturation = value; return VS_OK;
				default:
					;
			}
		}
		break;

		default:
			break;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
