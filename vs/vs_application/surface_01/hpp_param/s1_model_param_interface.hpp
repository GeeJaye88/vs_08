////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_param_interface.hpp ----------
/*!
\file s1_model_param_interface.hpp
\brief
\author Gareth Edwards
*/


// ---------- predefined indices ----------
	INT Model::GetParamIndexLid()
	{
		return s_pi_model->parameter.GetParamIndexLid();
	}
	INT Model::GetParamIndexValue()
	{
		return s_pi_model->parameter.GetParamIndexValue();
	}
	INT Model::GetParamIndexDefault()
	{
		return s_pi_model->parameter.GetParamIndexDefault();
	}
	INT Model::GetParamIndexIncrement()
	{
		return s_pi_model->parameter.GetParamIndexIncrement();
	}


// ---------- group ----------
	INT Model::GetParamGroup()
	{
		return s_pi_model->parameter.GetParamGroup();
	}
	INT Model::SetParamGroup(INT param_group)
	{
		return s_pi_model->parameter.SetParamGroup(param_group);
	}


// ---------- group properties ----------
	INT Model::GetParamGroupSize()
	{		
		return s_pi_model->parameter.GetParamGroupSize();
	}
	INT Model::SetParamGroupSize(INT param_group_size)
	{
		s_pi_model->parameter.SetParamGroupSize(param_group_size);
		return VS_OK;
	}
	INT Model::GetParamGroupIndex()
	{		
		return s_pi_model->parameter.GetParamGroupIndex();
	}
	INT Model::SetParamGroupIndex(INT group_index)
	{
		INT result = s_pi_model->parameter.SetParamGroupIndex(group_index);
		return result;
	}
	INT Model::GetParamGroupOffset()
	{		
		return s_pi_model->parameter.GetParamGroupOffset();
	}
	INT Model::SetParamGroupOffset(INT group_offset)
	{
		INT result = s_pi_model->parameter.SetParamGroupOffset(group_offset);
		return result;
	}


// ---------- array properties ----------
	INT Model::GetParamArrayIndex()
	{		
		return s_pi_model->parameter.GetParamArrayIndex();
	}
	INT Model::SetParamArrayIndex(INT array_index)
	{		
		INT result = s_pi_model->parameter.SetParamArrayIndex(array_index);
		return result;
	}


// ---------- labels and values ----------
	INT Model::GetParamArraySize()
	{
		return s_pi_model->parameter.GetParamArraySize();
	}
	INT Model::GetParamGroupName(std::string & group_name)
	{		
		INT result = s_pi_model->parameter.GetParamGroupName(group_name);
		return VS_OK;
	}
	INT Model::GetParamArrayLabels(INT group_index, std::string param_labels[])
	{
		INT result = (INT)s_pi_model->parameter.GetParamArrayLabels(group_index, param_labels);
		return result;
	}
	INT Model::GetParamArrayLabels(INT group_index, CHAR param_labels[])
	{
		INT param_array_size = GetParamArraySize();
		std::string *str_param_labels = new std::string[param_array_size];

		INT result = (INT)s_pi_model->parameter.GetParamArrayLabels(group_index, str_param_labels);

		for (INT i = 0; i < param_array_size; i++)
		{
			strcpy((param_labels + i), str_param_labels[i].c_str());
		}
		delete[] str_param_labels;

		return result;
	}
	INT Model::GetParamArrayValues(INT group_index, INT *group_id, INT *array_count, FLOAT *array_values)
	{
		INT result = (INT)s_pi_model->parameter.GetParamArrayValues(group_index, group_id, array_count, array_values);
		return result;
	}
	INT Model::GetParamArrayValues(INT group_index, INT *group_id, INT *array_count, INT type_values, FLOAT *array_values)
	{		
		INT result = (INT)s_pi_model->parameter.GetParamArrayValues(group_index, group_id, array_count, type_values, array_values);
		return result;
	}
	INT Model::SetParamArray(INT group_index, INT max_values, FLOAT *array_values)
	{
		INT result = (INT)s_pi_model->parameter.SetParamArray(group_index,max_values,array_values);
		return VS_OK;
	}
	INT Model::SetParamArray(INT group_index, INT max_values,INT type_values,FLOAT *array_values)
	{
		INT result = (INT)s_pi_model->parameter.SetParamArray(group_index,max_values,type_values,array_values);
		return VS_OK;
	}
	INT Model::SetParamArrayLabels(INT group_index, INT max_labels, std::string array_labels[])
	{
		INT result = (INT)s_pi_model->parameter.SetParamArrayLabels(group_index, max_labels, array_labels);
		return result;
	}
	INT Model::SetParamArrayValues(INT group_index, INT group_id, INT array_count, FLOAT *array_values)
	{
		INT result = (INT)s_pi_model->parameter.SetParamArrayValues(group_index, group_id, array_count, array_values);
		return result;
	}
	INT Model::SetParamArrayDefaults(INT group_id)
	{
		INT result = (INT)s_pi_model->parameter.SetParamArrayDefaults(group_id);
		return result;
	}
