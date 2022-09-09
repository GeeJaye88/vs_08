////////////////////////////////////////////////////////////////////////////////

// ---------- param_setup.hpp ----------
/*!
\file param_setup.hpp
\brief Setup getting mesh param
\author Gareth Edwards
*/

	
// ---- indices
	INT    param_group_index  = GetParamGroupIndex();
	INT    param_array_index  = GetParamArrayIndex();


// ---- size
	INT    param_array_size   = GetParamArraySize();


// ---- init
	INT    param_group_id     = 0;
	INT    param_array_count  = 0;
	FLOAT *param_array_values     = new FLOAT[param_array_size];
	FLOAT *param_array_increments = new FLOAT[param_array_size];

	std::string  param_group_name;
	std::string *param_array_labels = new std::string[param_array_size];


// ---- get
	result = GetParamGroupName(param_group_name);
	result = GetParamArrayValues(
				param_group_index,
				&param_group_id,
				&param_array_count,
				1,
				param_array_values
			);
	result = GetParamArrayValues(
				param_group_index,
				&param_group_id,
				&param_array_count,
				3,
				param_array_increments
			);
	result = GetParamArrayLabels(
				param_group_index,
				param_array_labels
			);


// ---- to char
	CHAR char_param_group_name[VS_MAXCHARNAME];
	strcpy(char_param_group_name, param_group_name.c_str());
	CHAR char_param_array_labels[8][VS_MAXCHARNAME];
	for (INT i = 0; i < 8; i++)
	{
		strcpy(char_param_array_labels[i], param_array_labels[i].c_str());
	}