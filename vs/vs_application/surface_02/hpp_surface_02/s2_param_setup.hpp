////////////////////////////////////////////////////////////////////////////////

// ---------- s2_param_setup.hpp ----------
/*!
\file s2_param_setup.hpp
\brief surface_02 setup parameter methods
\author Gareth Edwards
*/


// ---------- SetupParamSurface ----------
/*!
\brief initialise Surface param group
\author Gareth Edwards
*/
INT Surface_02::SetupParamSurface(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_02::Model::PARAM_GROUP_CAMERA);
		SetParamGroupOffset(*offset); 
		SetParamGroupSize(1);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- fill group with "null" values
		SetupParamNull();


	// ---- set mode
		INT param_index_value = GetParamIndexValue();
		FLOAT param_array[10];
		GetParamArray(0, param_array_size, param_index_value, param_array);
		param_array[0] = (FLOAT)vs_system::ObjConfig::MESH_VERSION_CAMERA;
		SetParamArray(0, param_array_size, param_index_value, param_array);

	return VS_OK;
}


// ---------- SetupParamWeld ----------
/*!
\brief initialise Weld param group
\author Gareth Edwards
*/
INT Surface_02::SetupParamWeld(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_02::Model::PARAM_GROUP_BUMP_MAP);
		SetParamGroupOffset(*offset); 
		SetParamGroupSize(1);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- fill group with "null" values
		SetupParamNull();


	// ---- set mode
		INT param_index_value = GetParamIndexValue();
		FLOAT param_array[10];
		GetParamArray(0, param_array_size, param_index_value, param_array);
		param_array[0] = (FLOAT)vs_system::ObjConfig::MESH_VERSION_RR;
		SetParamArray(0, param_array_size, param_index_value, param_array);

	return VS_OK;
}


// ---------- SetupParamFunction ----------
/*!
\brief initialise Surface param group
\author Gareth Edwards
*/
INT Surface_02::SetupParamFunction(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(6);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- note float_array[0] is the mesh type id


	// ---- init per mesh fractal
		if ( vs_system::ObjConfig::MESH_FRACTAL == vs_system::ObjConfig::MESH_FRACTAL_DS1 )
		{

			// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
				std::string str_array[2][10] =
				{
					{     "F-Surf",   "Seed",      "Scalar",   "Rough",  "Smooth",     "Min",       "Depth",   "NA",       "NA",       "NA"         },
					{     "i:u",      "f:l",       "f:u",      "f:u",    "f:p",        "f:l",       "f:u",     "f:u",      "u:u",      "u:u",       }
				};
				FLOAT float_array[4][10] =
				{
					{      3,          2000,        1,          1,          0,          0,          0,          0,          0,           0          },
					{      1,          1,           0.1f,       0.1f,       0.1f,       0.01f,      0.01f,      0,          0,           0          },
					{      0,          0,           0,          0,          0,         -90,         0,          0,          0,           0          },
					{      0,          1,           0,          0,          0,          90,         0,          0,          0,           0          },
				};

			// ---- set
				INT param_group_size    = GetParamGroupSize();
				INT param_index_value   = GetParamIndexValue();
				INT param_index_default = GetParamIndexDefault();
				INT param_index_incr    = GetParamIndexIncrement();
				INT seed = (INT)float_array[0][1];
				for (INT i = 0; i < param_group_size; i++)
				{
					float_array[0][1] = (FLOAT)(seed + i);
					SetParamArray(i, param_array_size, param_index_value,   float_array[0]);
					SetParamArray(i, param_array_size, param_index_default, float_array[0]);
					SetParamArray(i, param_array_size, param_index_incr,    float_array[1]);
					SetParamArrayLabels(i, param_array_size, str_array[0]);
				}
		}
		else // vs_system::ObjConfig::MESH_FRACTAL_DS2
		{

			// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
				std::string str_array[2][10] =
				{
					{     "F-Surf",   "Seed",      "Range",    "Rough",    "Smooth",   "Nada",     "Offset",   "Peak",     "NA",       "NA"         },
					{     "i:u",      "f:l",       "f:u",      "f:u",      "f:p",      "f:l",      "f:u",      "f:u",      "u:u",      "u:u",       }
				};
				FLOAT float_array[4][10] =
				{
					{      3,          2000,        0.2f,       1,          0,          0,          0,          0,          0,           0          },
					{      1,          1,           0.01f,      0.1f,       0.1f,       0.01f,      0.01f,      0.01f,      0,           0          },
					{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0          },
					{      0,          1,           0,          0,          0,          0,          0,          0,          0,           0          },
				};


			// ---- set
				INT param_group_size    = GetParamGroupSize();
				INT param_index_value   = GetParamIndexValue();
				INT param_index_default = GetParamIndexDefault();
				INT param_index_incr    = GetParamIndexIncrement();
				INT seed = (INT)float_array[0][1];
				for (INT i = 0; i < param_group_size; i++)
				{
					float_array[0][1] = (FLOAT)(seed + i);
					SetParamArray(i, param_array_size, param_index_value,   float_array[0]);
					SetParamArray(i, param_array_size, param_index_default, float_array[0]);
					SetParamArray(i, param_array_size, param_index_incr,    float_array[1]);
					SetParamArrayLabels(i, param_array_size, str_array[0]);
				}

		}

	return VS_OK;
}


// ---------- SetupParamLights ----------
/*!
\brief initialise Lights param group
\author Gareth Edwards
*/
INT Surface_02::SetupParamLights(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_02::Model::PARAM_GROUP_LIGHTS);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(PI_Surface_02::MAX_EXP_LGT);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- setup proportional to size of surface
		FLOAT dist = pi_surface_02->_configuration.acquire_dimensions.size * 2.0f;
		FLOAT range = dist * 2.0f;


	// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
		std::string str_array[2][10] =
			{
			{      "Light",    "Intensity", "Distance", "Range",  "Attenuation","Elevation","Azimuth",  "Height",   "NA",       "NA"        },
			{      "i:u",      "f:l",       "f:u",      "f:u",    "f:p",        "f:l",       "f:u",     "f:u",      "u:u",      "u:u",      }
			};
		std::string str_ambient_array[10] =
			{ 
				   "Light",    "Intensity", "NA",       "NA",     "NA",         "NA",       "NA",       "NA",       "NA",       ""
			};
		FLOAT float_array[4][10] =
			{ 
			{      0,          0.2f,        dist,       range,      0.1f,       15,         0,          0,          0,           0          },
			{      0,          0.01f,       1,          1,          0.005f,     1,          1,          1,          0,           0          },
			{      0,          0,           0,          0,          0,         -90,         0,          0,          0,           0          },
			{      0,          1,           0,          0,          0,          90,         0,          0,          0,           0          },
			};
		FLOAT float_ambient_array[4][10] =
			{ 
			{      0,          0.05f,       0,          0,          0,          0,          0,          0,          0,           0          },
			{      0,          0.01f,       0,          0,          0,          0,          0,          0,          0,           0          },
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0          },
			{      0,          1,           0,          0,          0,          0,          0,          0,          0,           0          },
			};


	// ---- set
		INT param_group_size    = GetParamGroupSize();
		INT param_index_value   = GetParamIndexValue();
		INT param_index_default = GetParamIndexDefault();
		INT param_index_incr    = GetParamIndexIncrement();
		for (INT i = 0; i < param_group_size; i++)
		{
			if ( i < param_group_size - 1 )
			{
				SetParamArray(i, param_array_size, param_index_value,   float_array[0]);
				SetParamArray(i, param_array_size, param_index_default, float_array[0]);
				SetParamArray(i, param_array_size, param_index_incr,    float_array[1]);
			}
			else
			{
				SetParamArray(i, param_array_size, param_index_value,   float_ambient_array[0]);
				SetParamArray(i, param_array_size, param_index_default, float_ambient_array[0]);
				SetParamArray(i, param_array_size, param_index_incr,    float_ambient_array[1]);
			}
		}


	// ---- init lights
		std::string str_light_names[PI_Surface_02::MAX_EXP_LGT] =
		{
			"Green", "Orange", "Magenta", "Blue", "Ambient",
		};
		for (INT i = 0; i < param_group_size; i++)
		{
			if ( i < PI_Surface_02::MAX_EXP_LGT - 1 )
			{
				str_array[0][0] = str_light_names[i];
				SetParamArrayLabels(i, param_array_size, str_array[0]);
			}
			else
			{
				str_ambient_array[0] = str_light_names[i];
				SetParamArrayLabels(i, param_array_size, str_ambient_array);
			}
		}


	// ---- turn lights # 1 (Green), #2 (Orange) and #5 (Ambient) ON
		float_array[0][0] = 1;
		SetParamArray(0, param_array_size, param_index_value, float_array[0]);
		SetParamArray(1, param_array_size, param_index_value, float_array[0]);
		float_ambient_array[0][0] = 1;
		SetParamArray(param_group_size-1, param_array_size, param_index_value, float_ambient_array[0]);

	return VS_OK;
}


// ---------- SetupParamAcquire ----------
/*!
\brief initialise Acquire param group
\author Gareth Edwards
*/
INT Surface_02::SetupParamAcquire(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- set param stuff
		SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(PI_Surface_02::MAX_ACQ_PARAM);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- labels
		std::string str_array[PI_Surface_02::MAX_ACQ_PARAM][6] =
		{
			{ "Acquire Lighting",  "Intensity",  "Distance", "Elevation",  "Azimuth",      "Scalar" },
			{ "Acquire Ambient",   "Intensity",  "NA",       "NA",         "NA",           "NA"     },
			{ "Resolvability",     "Min",        "Max",      "NA",         "NA",           "NA"     },
			{ "----------------",  "-----",      "---",      "----",      "----",          "-----"  },
			{ "----------------",  "-----",      "---",      "----",      "----",          "-----"  },
			{ "----------------",  "-----",      "---",      "----",      "----",          "-----"  },
		};


	// ---- init value table

		// Objective is to initialise light such that it at a potentially manufacurable
		// distance (e.g., 50mm) from a target surface area ( e.g., 10mm), and that
		// the intensity of this light be provide for a "best fit" illumination based
		// on a micro surface located at the origin and inclined perpendicular to the
		// the light (e.g., with dot product == 1).

		// shortcut
		FLOAT size = pi_surface_02->_configuration.acquire_dimensions.size;

		// set distance so that a 10mm surface is illuminated from 50mm 
		FLOAT dist = size * 5.0f;

		// IF light type point THEN intensity set as per standard shader
		//  - given att = 1 / (attenuation * distance * distance).
		//  - if quadratic attenuation == 1; 
		//  - diffuse surface value == 1, and 
		//  - point/surface dot product == 1, then 
		//  - intensity = dist*dist.
		FLOAT inty = LIGHT_TYPE == vs_system::VS_LIGHT_DIRECTIONAL ?
						1.0f :
						dist * dist;

		// set range just beyond furthest corner viewed at 45 degrees (distance + 1/2 * diagonal of surface)
		FLOAT range  = dist + (FLOAT)sqrt(size*size);

		FLOAT float_array[PI_Surface_02::MAX_ACQ_PARAM][6] =
		{
			{  0,    inty,    dist,   45,     0,      range },
			{  0,    0.0f,    0,      0,      0,      0     },
			{  0,    0.01f,   0.99f,  0,      0,      0     },
			{  0,    0,       0,      0,      0,      0     },
			{  0,    0,       0,      0,      0,      0     },
			{  0,    0,       0,      0,      0,      0     },
		};


	// ---- init increment table

		// IF light type point THEN increment proportional to intensity
		FLOAT incr = LIGHT_TYPE == vs_system::VS_LIGHT_DIRECTIONAL ? 0.01f : 0.01f * inty;

		FLOAT vali = 0.01f;
		FLOAT fincr_array[PI_Surface_02::MAX_ACQ_PARAM][6] =
		{
			{  0,    incr,    1,      1,      1,      1,     },
			{  0,    0.01f,   0,      0,      0,      0,     },
			{  0,    vali,    vali,   vali,   vali,   0,     },
			{  0,    vali,    vali,   vali,   vali,   0,     },
			{  0,    vali,    vali,   vali,   vali,   0,     },
			{  0,    vali,    vali,   vali,   vali,   0,     },
		};


	// ---- set
		INT param_group_size    = GetParamGroupSize();
		INT param_index_value   = GetParamIndexValue();
		INT param_index_default = GetParamIndexDefault();
		INT param_index_incr    = GetParamIndexIncrement();
		for (INT i = 0; i < param_group_size; i++)
		{
			SetParamArray(i, 6,    param_index_value,     float_array[i]);
			SetParamArray(i, 6,    param_index_default,   float_array[i]);
			SetParamArray(i, 6,    param_index_incr,      fincr_array[i]);
			SetParamArrayLabels(i, 6, str_array[i]);
		}

	return VS_OK;
}


// ---------- SetupParamNull ----------
/*!
\brief initialise current param group with null default values
\author Gareth Edwards
*/
INT Surface_02::SetupParamNull()
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
		std::string str_param[2][10] =
			{
			{      "NA",      "NA",        "NA",       "NA",       "NA",        "NA",        "NA",      "NA",       "NA",       "NA"       },
			{      "u:u",     "u:u",       "u:u",      "u:u",      "u:u",       "u:u",       "u:u",     "u:u",      "u:u",      "u:u",     }
			};
		FLOAT float_param[4][10] =
			{ 
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,          0         },
			{      1,          1,           1,          1,          1,          1,          1,          1,          1,          1         },
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,          0         },
			{      1,          1,           1,          1,          1,          1,          1,          1,          1,          1         },
			};


	// ---- init values
		INT param_group_size    = GetParamGroupSize();
		INT param_index_value   = GetParamIndexValue();
		INT param_index_default = GetParamIndexDefault();
		INT param_index_incr    = GetParamIndexIncrement();
		for (INT i = 0; i < param_group_size; i++)
		{
			SetParamArray(i, param_array_size, param_index_value,   float_param[0]);
			SetParamArray(i, param_array_size, param_index_default, float_param[0]);
			SetParamArray(i, param_array_size, param_index_default, float_param[1]);
			SetParamArrayLabels(i, param_array_size, str_param[0]);
		}

	return VS_OK;
}


// ---------- SetupParamStore ----------
/*!
\brief copy current param
\author Gareth Edwards
*/
INT Surface_02::SetupParamStore()
{

	// ---- local
		INT result = VS_OK;

	// ---- copy from parameter to parameter store
		for (UINT i = 0; i < TOTAL_PARAM_SETS; i++)
		{

			// --- set dependant param
				SetAcquireSurfaceSize(GetAcquireSetupSourceSurfaceSize(i));
				SetAcquireSurfaceYScalar(GetAcquireSetupSourceSurfaceYScalar(i));

			// ---- set parameters derived from these parameters
				INT parameter_top = 0;
				result = SetupParamAcquire(&parameter_top);
				result = SetupParamSurface(&parameter_top);
				result = SetupParamWeld(&parameter_top);
				result = SetupParamLights(&parameter_top);
				result = SetupParamFunction(&parameter_top);

			// ---- copy parameters into a parmater set
				parameter_store[i].CopyParamSet(&parameter);

		}

	return VS_OK;
}

