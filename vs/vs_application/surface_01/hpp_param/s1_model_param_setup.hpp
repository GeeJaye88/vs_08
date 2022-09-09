////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_param_setup.hpp ----------
/*!
\file s1_model_param_setup.hpp
\brief
\author Gareth Edwards
*/


// ---------- SetupParamCamera ----------
/*!
\brief initialise camera param group
\author Gareth Edwards
*/
INT Model::SetupParamCamera(INT *offset)
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param stuff
		SetParamGroup(Model::PARAM_GROUP_CAMERA);
		SetParamGroupOffset(*offset); 
		SetParamGroupSize(12);
		SetParamGroupIndex(0);
		SetParamArrayIndex(0);


	// ---- increment offset
		*offset += 12;


	// ---- init
		SetupParamNull();

	return VS_OK;
}


// ---------- SetupParamNull ----------
/*!
\brief initialise current param group with null default values
\author Gareth Edwards
*/
INT Model::SetupParamNull()
{

	// ---- local
		INT result = VS_OK;
		INT param_array_size = GetParamArraySize();


	// ---- param  0 --------- 1 ---------- 2 --------- 3 --------- 4 --------- 5 --------- 6 --------- 7 --------- 8 --------- 9 ---------
		std::string str_array[2][10] =
			{
			{      "NA",      "NA",        "NA",       "NA",       "NA",        "NA",        "NA",      "NA",       "NA",       "NA"       },
			{      "u:u",     "u:u",       "u:u",      "u:u",      "u:u",       "u:u",       "u:u",     "u:u",      "u:u",      "u:u",     }
			};
		FLOAT float_array[4][10] =
			{ 
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0         },
			{      0,          1,           1,          1,          1,          1,          1,          1,          0,           0         },
			{      0,          0,           0,          0,          0,          0,          0,          0,          0,           0         },
			{      0,          1,           1,          1,          1,          1,          1,          1,          0,           0         },
			};


	// ---- init values
		INT param_group_size = GetParamGroupSize();
		for (INT i = 0; i < param_group_size; i++)
		{
			SetParamArray(i, param_array_size, GetParamIndexValue(),     float_array[0]);
			SetParamArray(i, param_array_size, GetParamIndexDefault(),   float_array[0]);
			SetParamArray(i, param_array_size, GetParamIndexIncrement(), float_array[1]);
			SetParamArrayLabels(i, param_array_size, str_array[0]);
		}

	return VS_OK;
}

