////////////////////////////////////////////////////////////////////////////////

// ---------- s3_param_setup.hpp ----------
/*!
\file s3_param_setup.hpp
\brief surface_03 setup parameter methods
\author Gareth Edwards
*/


// ---------- SetupParamCamera ----------
/*!
\brief initialise Surface param group
\author Gareth Edwards
*/
INT Surface_03::SetupParamCamera(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_CAMERA);
		SetParamGroupOffset(*offset); 
		SetParamGroupSize(1);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += 1;


	// ---- default values
		SetupParamNull();


	// ---- set mode
		INT param_index_value = GetParamIndexValue();
		FLOAT param_array[10];
		GetParamArray(0, param_array_size, param_index_value, param_array);
		param_array[0] = (FLOAT)vs_system::ObjConfig::MESH_VERSION_CAMERA;
		SetParamArray(0, param_array_size, param_index_value, param_array);

	return VS_OK;
}


// ---------- SetupParamHeightMap ----------
/*!
\brief initialise height map param group
\author Gareth Edwards
*/
INT Surface_03::SetupParamHeightMap(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_BUMP_MAP);
		SetParamGroupOffset(*offset); 
		SetParamGroupSize(1);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += 1;


	// ---- default values
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
INT Surface_03::SetupParamFunction(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_FUNCTION);
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
					{     "F-Surf",   "Seed",      "Scalar",   "Rough",  "Smooth",     "Clip",     "N A",      "NA",       "NA",       "NA"         },
					{     "i:u",      "f:l",       "f:u",      "f:u",    "f:p",        "f:l",      "f:u",      "f:u",      "u:u",      "u:u",       }
				};
				FLOAT float_array[4][10] =
				{
					{      3,          2000,        1,          1,          0,          0.1f,       0,          0,          0,           0          },
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
					{      3,          2000,        0.2f,       1,          0,          0,         -0.1f,       0,          0,           0          },
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
INT Surface_03::SetupParamLights(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_LIGHTS);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(PI_Surface_03::MAX_EXP_LGT);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += PI_Surface_03::MAX_EXP_LGT;


	// ---- setup proportional to size of surface
		FLOAT inten = 0.002f * GetAcquireSurfaceSize();
		FLOAT dist = GetAcquireSurfaceSize() * 2.0f;
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
			{      0,          inten,       dist,       range,      0.1f,       15,         0,          0,          0,           0          },
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
		std::string str_light_names[PI_Surface_03::MAX_EXP_LGT] =
		{
			"Green", "Orange", "Magenta", "Blue", "Ambient",
		};
		for (INT i = 0; i < param_group_size; i++)
		{
			if ( i < PI_Surface_03::MAX_EXP_LGT - 1 )
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


// ---------- SetupParamMeasure ----------
/*!
\brief initialise measurement param group
\author Gareth Edwards
*/
INT Surface_03::SetupParamMeasure(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_MEASURE);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(7);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += GetParamGroupSize();


	// ---- note float_array[0] is the mesh type id


	// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
		std::string str_array[2][10] =
		{
			{     "M",        "Rotation",  "X",        "Y",        "Z",        "NA",       "NA" ,      "NA",       "NA",       "NA"         },
			{     "i:u",      "f:u",       "f:u",      "f:u",      "f:u",      "f:u",      "f:u",      "f:u",      "u:u",      "u:u",       }
		};
		FLOAT float_array[4][10] =
		{
			{      3,          0,           0,          0,          0,          0,          0,          0,          0,           0          },
			{      1,          1,           0.1f,       0.1f,       0.1f,       0,          0,          0,          0,           0          },
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0          },
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0          },
		};


	// ---- update group name
		str_array[0][0] = "Locations of Interest";


	// ---- init XYZ location
		FLOAT width=0, height=0, depth=0;
		GetAcquireSurfaceDimensions(&width, &height, &depth);
		FLOAT radius = depth / 3;
		VsVector3 v[7];
		v[0].x = 0;
		v[0].y = 0;
		v[0].z = 0;
		for (UINT i = 1; i < 7; i++)
		{
			FLOAT radian = (FLOAT)((i - 1) * 60) * (FLOAT)VS_RADIANS_PER_DEGREE;
			v[i].x =  radius * (FLOAT)cos(radian);
			v[i].y =  0;
			v[i].z = -radius * (FLOAT)sin(radian);
		}


	// ---- set
		INT param_group_size    = GetParamGroupSize();
		INT param_index_value   = GetParamIndexValue();
		INT param_index_default = GetParamIndexDefault();
		INT param_index_incr    = GetParamIndexIncrement();
		for (INT i = 0; i < param_group_size; i++)
		{
			float_array[0][1] = 0;
			float_array[0][2] = v[i].x;
			float_array[0][3] = v[i].y;
			float_array[0][4] = v[i].z;
			SetParamArray(i, param_array_size, param_index_value,   float_array[0]);
			SetParamArray(i, param_array_size, param_index_default, float_array[0]);
			SetParamArray(i, param_array_size, param_index_incr,    float_array[1]);
			SetParamArrayLabels(i, param_array_size, str_array[0]);
		}


	return VS_OK;
}


// ---------- SetupParamAcquire ----------
/*!
\brief initialise Acquire param group
\author Gareth Edwards
*/
INT Surface_03::SetupParamAcquire(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- set param stuff
		SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		SetParamGroupOffset(*offset);
		SetParamGroupSize(PI_Surface_03::MAX_ACQ_PARAM);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += 6;


	// ---- labels
		std::string str_array[PI_Surface_03::MAX_ACQ_PARAM][6] =
		{
			{ "Acquire Lighting",  "Intensity",  "Distance", "Elevation",  "Azimuth", "NA"    },
			{ "Acquire Ambient",   "Intensity",  "NA",       "NA",         "NA",      "NA"    },
			{ "Min False Colour",  "Value",      "Red",      "Green",      "Blue",    "Alpha" },
			{ "Max False Colour",  "Value",      "Red",      "Green",      "Blue",    "Alpha" },
			{ "Reproducibility",   "Strength",   "Minimum",  "Maximum",    "Sigma",   "NA"    },
			{ "Consistency",       "Alpha Mu",   "Beta Mu",  "Coherence",  "Slope Delta",  "Slope Climb" },
		};


	// ---- value
		FLOAT dist = GetAcquireSurfaceSize() * 1.5f;
		FLOAT intsty = dist;
		FLOAT float_array[PI_Surface_03::MAX_ACQ_PARAM][6] =
		{
			{  0,    intsty,  dist,   45,     0,      0,     },
			{  0,    0,       0,      0,      0,      0,     },
			{  0,    0.01f,   0,      0,      1,      0,     },
			{  0,    0.99f,   1,      0,      0,      0,     },
			{  0,    0.1f,    0.1f,   0.43f,  3,      0,     },
			{  0,    0.1f,    0.24f,  3.28f,  1,      0,     },
		};


	// ---- increment
		FLOAT vali = 0.01f;
		FLOAT rgbi = 0.01f;
		FLOAT fincr_array[PI_Surface_03::MAX_ACQ_PARAM][6] =
		{
			{  0,    1,       1,      1,      1,      0,     },
			{  0,    0.1f,    0,      0,      0,      0,     },
			{  0,    vali,    rgbi,   rgbi,   rgbi,   0,     },
			{  0,    vali,    rgbi,   rgbi,   rgbi,   0,     },
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
INT Surface_03::SetupParamNull()
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
