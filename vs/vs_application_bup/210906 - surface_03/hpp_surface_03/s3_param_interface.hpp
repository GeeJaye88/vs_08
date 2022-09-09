////////////////////////////////////////////////////////////////////////////////

// ---------- s3_param_interface.hpp ----------
/*!
\file s3_param_interface.hpp
\brief surface_03 param methods
\author Gareth Edwards
*/


// ---------- predefined indices ----------
	INT Surface_03::GetParamIndexLid()
	{
		return parameter.GetParamIndexLid();
	}
	INT Surface_03::GetParamIndexValue()
	{
		return parameter.GetParamIndexValue();
	}
	INT Surface_03::GetParamIndexDefault()
	{
		return parameter.GetParamIndexDefault();
	}
	INT Surface_03::GetParamIndexIncrement()
	{
		return parameter.GetParamIndexIncrement();
	}


// -------- group --------
	INT Surface_03::GetParamGroup()
	{
		return parameter.GetParamGroup();
	}
	INT Surface_03::SetParamGroup(INT param_group)
	{
		parameter.SetParamGroup(param_group);
		return VS_OK;
	}


// ---------- group properties ----------
	INT Surface_03::GetParamGroupSize()
	{
		return parameter.GetParamGroupSize();
	}
	INT Surface_03::SetParamGroupSize(INT param_group_size)
	{
		parameter.SetParamGroupSize(param_group_size);
		return VS_OK;
	}
	INT Surface_03::GetParamGroupIndex()
	{
		return parameter.GetParamGroupIndex();
	}
	INT Surface_03::SetParamGroupIndex(INT group_index)
	{
		parameter.SetParamGroupIndex(group_index);
		return VS_OK;
	}
	INT Surface_03::GetParamGroupOffset()
	{
		return parameter.GetParamGroupOffset();
	}
	INT Surface_03::SetParamGroupOffset(INT param_group_offset)
	{
		parameter.SetParamGroupOffset(param_group_offset);
		return VS_OK;
	}


// ---------- array properties ----------
	INT Surface_03::GetParamArrayIndex()
	{
		return parameter.GetParamArrayIndex();
	}
	INT Surface_03::SetParamArrayIndex(INT array_index)
	{
		parameter.SetParamArrayIndex(array_index);
		return VS_OK;
	}


// ---------- labels and values ----------
	INT Surface_03::GetParamArraySize()
	{
		return parameter.GetParamArraySize();
	}
	INT Surface_03::GetParamGroupName(std::string & group_name)
	{
		parameter.GetParamGroupName(group_name);
		return VS_OK;
	}
	INT Surface_03::SetParamArrayLabels(INT group_index, INT max_labels, std::string array_labels[])
	{
		parameter.SetParamArrayLabels(group_index, max_labels, array_labels);
		return VS_OK;
	}
	INT Surface_03::GetParamArrayLabels(INT group_index, std::string array_labels[])
	{
		parameter.GetParamArrayLabels(group_index,  array_labels);
		return VS_OK;
	}
	INT Surface_03::GetParamArray(
			INT    group_index,
			INT    max_values,
			FLOAT *array_values
		)
	{
		INT result = parameter.GetParamArray(group_index, max_values, parameter.GetParamIndexValue(), array_values);
		return VS_OK;
	}
	INT Surface_03::GetParamArray(
			INT    group_index,
			INT    max_values,
			INT    type_values,
			FLOAT *array_values
		)
	{
		parameter.GetParamArray(group_index,max_values,type_values,array_values);
		return VS_OK;
	}
	INT Surface_03::GetParamArrayValues(
			INT    group_index,
			INT   *group_id,
			INT   *array_count,
			FLOAT *array_values
		)
	{
		parameter.GetParamArrayValues(group_index,group_id,array_count,array_values);
		return VS_OK;
	}
	INT Surface_03::GetParamArrayValues(
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
	INT Surface_03::SetParamArray(
		INT    group_index,
		INT    max_values,
		FLOAT *array_values
	)
	{
		parameter.SetParamArray(group_index,max_values,array_values);
		return VS_OK;
	}
	INT Surface_03::SetParamArray(
		INT    group_index,
		INT    max_values,
		INT    type_values,
		FLOAT *array_values
	)
	{
		parameter.SetParamArray(group_index,max_values,type_values,array_values);
		return VS_OK;
	}
	INT Surface_03::SetParamArrayValues(
			INT    group_index,
			INT    group_id,
			INT    array_count,
			FLOAT *array_values
		)
	{
		parameter.SetParamArrayValues(group_index,group_id,array_count,array_values);
		return VS_OK;
	}
	INT Surface_03::SetParamArrayDefaults(
			INT param_group
		)
	{
		parameter.SetParamArrayDefaults(param_group);
		return VS_OK;
	}
