////////////////////////////////////////////////////////////////////////////////

// ---------- s2_param_interface.hpp ----------
/*!
\file s2_param_interface.hpp
\brief Surface_02 param methods
\author Gareth Edwards
*/


// ---------- predefined indices ----------
	INT Surface_02::GetParamIndexLid()
	{
		return parameter.GetParamIndexLid();
	}
	INT Surface_02::GetParamIndexValue()
	{
		return parameter.GetParamIndexValue();
	}
	INT Surface_02::GetParamIndexDefault()
	{
		return parameter.GetParamIndexDefault();
	}
	INT Surface_02::GetParamIndexIncrement()
	{
		return parameter.GetParamIndexIncrement();
	}


// -------- group --------
	INT Surface_02::GetParamGroup()
	{
		return parameter.GetParamGroup();
	}
	INT Surface_02::SetParamGroup(INT param_group)
	{
		parameter.SetParamGroup(param_group);
		return VS_OK;
	}


// ---------- group properties ----------
	INT Surface_02::GetParamGroupSize()
	{
		return parameter.GetParamGroupSize();
	}
	INT Surface_02::SetParamGroupSize(INT param_group_size)
	{
		parameter.SetParamGroupSize(param_group_size);
		return VS_OK;
	}
	INT Surface_02::GetParamGroupIndex()
	{
		return parameter.GetParamGroupIndex();
	}
	INT Surface_02::SetParamGroupIndex(INT group_index)
	{
		parameter.SetParamGroupIndex(group_index);
		return VS_OK;
	}
	INT Surface_02::GetParamGroupOffset()
	{
		return parameter.GetParamGroupOffset();
	}
	INT Surface_02::SetParamGroupOffset(INT param_group_offset)
	{
		parameter.SetParamGroupOffset(param_group_offset);
		return VS_OK;
	}


// ---------- array properties ----------
	INT Surface_02::GetParamArrayIndex()
	{
		return parameter.GetParamArrayIndex();
	}
	INT Surface_02::SetParamArrayIndex(INT array_index)
	{
		parameter.SetParamArrayIndex(array_index);
		return VS_OK;
	}


// ---------- labels and values ----------
	INT Surface_02::GetParamArraySize()
	{
		return parameter.GetParamArraySize();
	}
	INT Surface_02::GetParamGroupName(std::string & group_name)
	{
		parameter.GetParamGroupName(group_name);
		return VS_OK;
	}
	INT Surface_02::SetParamArrayLabels(INT group_index, INT max_labels, std::string array_labels[])
	{
		parameter.SetParamArrayLabels(group_index, max_labels, array_labels);
		return VS_OK;
	}
	INT Surface_02::GetParamArrayLabels(INT group_index, std::string array_labels[])
	{
		parameter.GetParamArrayLabels(group_index,  array_labels);
		return VS_OK;
	}
	INT Surface_02::GetParamArray(
			INT    group_index,
			INT    max_values,
			FLOAT *array_values
		)
	{
		INT result = parameter.GetParamArray(group_index, max_values, parameter.GetParamIndexValue(), array_values);
		return VS_OK;
	}
	INT Surface_02::GetParamArray(
			INT    group_index,
			INT    max_values,
			INT    type_values,
			FLOAT *array_values
		)
	{
		parameter.GetParamArray(group_index,max_values,type_values,array_values);
		return VS_OK;
	}
	INT Surface_02::GetParamArrayValues(
			INT    group_index,
			INT   *group_id,
			INT   *array_count,
			FLOAT *array_values
		)
	{
		parameter.GetParamArrayValues(group_index,group_id,array_count,array_values);
		return VS_OK;
	}
	INT Surface_02::GetParamArrayValues(
			INT    group_index,
			INT   *group_id,
			INT   *array_count,
			INT    type_values,
			FLOAT *array_values
		)
	{
		parameter.GetParamArrayValues(group_index,group_id,array_count,type_values,array_values);
		return VS_OK;
	}	
	INT Surface_02::SetParamArray(
		INT    group_index,
		INT    max_values,
		FLOAT *array_values
	)
	{
		parameter.SetParamArray(group_index,max_values,array_values);
		return VS_OK;
	}
	INT Surface_02::SetParamArray(
		INT    group_index,
		INT    max_values,
		INT    type_values,
		FLOAT *array_values
	)
	{
		parameter.SetParamArray(group_index,max_values,type_values,array_values);
		return VS_OK;
	}
	INT Surface_02::SetParamArrayValues(
			INT    group_index,
			INT    group_id,
			INT    array_count,
			FLOAT *array_values
		)
	{
		parameter.SetParamArrayValues(group_index,group_id,array_count,array_values);
		return VS_OK;
	}
	INT Surface_02::SetParamArrayDefaults(
			INT param_group
		)
	{
		parameter.SetParamArrayDefaults(param_group);
		return VS_OK;
	}
