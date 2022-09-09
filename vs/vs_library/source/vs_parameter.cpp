////////////////////////////////////////////////////////////////////////////////

// ---------- vs_parameter.cpp ----------
/*!
\file vs_parameter.cpp
\brief Implementation of the Parameter class.
\author Gareth Edwards 
*/


// ---- include ----
#include "../header/vs_parameter.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ArchTime class ----------

class Parameter::PI_Parameter
{

public:

	UINT  param_group = 0;
	UINT  param_group_offset[MAX_PARAM_GROUP];
	UINT  param_group_index[MAX_PARAM_GROUP];
	UINT  param_array_index[MAX_PARAM_GROUP];
	UINT  param_group_size[MAX_PARAM_GROUP];
	FLOAT param_array_values[MAX_PARAM_ARRAY][PARAM_ARRAY_SIZE];
	FLOAT param_array_increments[MAX_PARAM_ARRAY][PARAM_ARRAY_SIZE];
	FLOAT param_array_defaults[MAX_PARAM_ARRAY][PARAM_ARRAY_SIZE];
	CHAR  param_labels[MAX_PARAM_ARRAY][PARAM_ARRAY_SIZE][MAX_PARAM_CHAR];

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of Parameter class ----------


// ---------- Constructor ----------
	/*!
	\brief Constructor.
	\author Gareth Edwards
	*/
	Parameter::Parameter()
	{
		_pi_parameter = new PI_Parameter();

		for ( UINT pg = 0; pg < MAX_PARAM_ARRAY; pg++ )
		{
			for ( UINT pa = 0; pa < PARAM_ARRAY_SIZE; pa++ )
			{
				_pi_parameter->param_array_values[pg][pa]     = 0;
				_pi_parameter->param_array_increments[pg][pa] = 0;
				_pi_parameter->param_array_defaults[pg][pa]   = 0;
				strcpy(_pi_parameter->param_labels[pg][pa], "NA");
			}
		}

	}

// ---------- Destructor ----------
	/*!
	\brief Destructor.
	\author Gareth Edwards
	*/
	Parameter::~Parameter()
	{
		if ( _pi_parameter != NULL ) { delete _pi_parameter; _pi_parameter = NULL; }
	}


// ---------- predefined indices----------
	INT Parameter::GetParamIndexLid()       { return PARAM_LID;       }
	INT Parameter::GetParamIndexValue()     { return PARAM_VALUE;     }
	INT Parameter::GetParamIndexDefault()   { return PARAM_DEFAULT;   }
	INT Parameter::GetParamIndexIncrement() { return PARAM_INCREMENT; }


// ---------- group ----------
	INT Parameter::GetParamGroup()
	{
		return _pi_parameter->param_group;
	}

	INT Parameter::SetParamGroup(INT param_group)
	{
		_pi_parameter->param_group = param_group;
		return VS_OK;
	}


// ---------- group properties ----------
	INT Parameter::GetParamGroupOffset()
	{
		INT param_group = _pi_parameter->param_group;
		return _pi_parameter->param_group_offset[param_group];
	}

	INT Parameter::SetParamGroupOffset(INT param_group_offset)
	{
		INT param_group = _pi_parameter->param_group;
		_pi_parameter->param_group_offset[param_group] = param_group_offset;
		return VS_OK;
	}

	INT Parameter::GetParamGroupSize()
	{
		INT param_group = _pi_parameter->param_group;
		return _pi_parameter->param_group_size[param_group];
	}

	INT Parameter::SetParamGroupSize(INT param_group_size)
	{
		INT param_group = _pi_parameter->param_group;
		_pi_parameter->param_group_size[param_group] = param_group_size;
		return VS_OK;
	}

	INT Parameter::GetParamGroupIndex()
	{
		INT param_group = _pi_parameter->param_group;
		return _pi_parameter->param_group_index[param_group];
	}

	INT Parameter::SetParamGroupIndex(INT group_index)
	{
		INT param_group = _pi_parameter->param_group;
		_pi_parameter->param_group_index[param_group] = group_index;
		return VS_OK;
	}


// ---------- array properties ----------
	INT Parameter::GetParamArrayIndex()
	{
		INT param_group = _pi_parameter->param_group;
		return _pi_parameter->param_array_index[param_group];
	}

	INT Parameter::SetParamArrayIndex(INT array_index)
	{
		INT param_group = _pi_parameter->param_group;
		_pi_parameter->param_array_index[param_group] = array_index;
		return VS_OK;
	}



// ---------- labels and values ----------
	INT Parameter::GetParamArraySize()
	{
		return Parameter::PARAM_ARRAY_SIZE;
	}

	INT Parameter::GetParamGroupName(std::string & group_name)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = _pi_parameter->param_group_index[param_group] + group_offset;
		group_name = _pi_parameter->param_labels[gi][0];
		return VS_OK;
	}

	INT Parameter::SetParamArrayLabels(INT group_index, INT max_labels, std::string array_labels[])
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;
		INT mv = Parameter::PARAM_ARRAY_SIZE;
		INT max = max_labels < 0 ? 0 : (max_labels >= mv ? mv : max_labels);
		for (INT i = 0; i < max; i++)
		{
			strcpy(_pi_parameter->param_labels[gi][i], array_labels[i].c_str());
		}
		return VS_OK;
	}

	INT Parameter::GetParamArrayLabels(INT, std::string array_labels[])
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = _pi_parameter->param_group_index[param_group] + group_offset;
		for (INT i = 1; i < Parameter::PARAM_ARRAY_SIZE; i++)
		{
			array_labels[i-1] = _pi_parameter->param_labels[gi][i];
		}
		return VS_OK;
	}

	INT Parameter::GetParamArray(
			INT    group_index,
			INT    max_values,
			FLOAT *array_values
		)
	{
		INT result = GetParamArray(group_index, max_values, Parameter::PARAM_VALUE,array_values);
		return VS_OK;
	}

	INT Parameter::GetParamArray(
			INT    group_index,
			INT    max_values,
			INT    type_values,
			FLOAT *array_values
		)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;

		INT mv  = Parameter::PARAM_ARRAY_SIZE;
		INT max = max_values < 0 ? 0 : (max_values >= mv ? mv : max_values);
		for (INT i = 0; i < max; i++)
		{
			FLOAT *v = NULL;
			switch ( type_values )
			{
				default :
					return VS_ERROR;
				case Parameter::PARAM_VALUE:
					v = &_pi_parameter->param_array_values[gi][i];
					break;
				case Parameter::PARAM_DEFAULT:
					v = &_pi_parameter->param_array_defaults[gi][i];
					break;
				case Parameter::PARAM_INCREMENT:
					v = &_pi_parameter->param_array_increments[gi][i];
					break;
			}
			*(array_values + i) = *v;
		}

		return VS_OK;
	}

	INT Parameter::GetParamArrayValues(
			INT    group_index,
			INT   *group_id,
			INT   *array_count,
			FLOAT *array_values
		)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;

		*group_id = (INT)_pi_parameter->param_array_values[gi][0];
		*array_count = Parameter::PARAM_ARRAY_SIZE - 1;

		// TBD : array_count = something

		for (INT i = 1; i < Parameter::PARAM_ARRAY_SIZE; i++)
		{
			*(array_values + i - 1) = _pi_parameter->param_array_values[gi][i];
		}

		return VS_OK;
	}

	INT Parameter::GetParamArrayValues(
			INT    group_index,
			INT   *group_id,
			INT   *array_count,
			INT    type_values,
			FLOAT *array_values
		)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;

		*group_id = (INT)_pi_parameter->param_array_values[gi][0];
		*array_count = Parameter::PARAM_ARRAY_SIZE - 1;

		for (INT i = 1; i < Parameter::PARAM_ARRAY_SIZE; i++)
		{
			FLOAT *v = NULL;
			switch ( type_values )
			{
				default:
					return VS_ERROR;
				case Parameter::PARAM_VALUE:
					v = &_pi_parameter->param_array_values[gi][i];
					break;
				case Parameter::PARAM_DEFAULT:
					v = &_pi_parameter->param_array_defaults[gi][i];
					break;
				case Parameter::PARAM_INCREMENT:
					v = &_pi_parameter->param_array_increments[gi][i];
					break;
			}
			*(array_values + i - 1) = *v;
		}

		return VS_OK;
	}

	INT Parameter::SetParamArray(
		INT    group_index,
		INT    max_values,
		FLOAT *array_values
	)
	{
		INT result = SetParamArray(group_index, max_values, Parameter::PARAM_VALUE, array_values);
		return VS_OK;
	}

	INT Parameter::SetParamArray(
		INT    group_index,
		INT    max_values,
		INT    type_values,
		FLOAT *array_values
	)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;

		INT mv = Parameter::PARAM_ARRAY_SIZE;
		INT max = max_values < 0 ? 0 : (max_values >= mv ? mv : max_values);
		for (INT i = 0; i < max; i++)
		{
			FLOAT *v = NULL;
			switch ( type_values )
			{
				default :
					return VS_ERROR;
				case Parameter::PARAM_VALUE:
					v = &_pi_parameter->param_array_values[gi][i];
					break;
				case Parameter::PARAM_DEFAULT:
					v = &_pi_parameter->param_array_defaults[gi][i];
					break;
				case Parameter::PARAM_INCREMENT:
					v = &_pi_parameter->param_array_increments[gi][i];
					break;
			}
			*v = *(array_values + i);
		}

		return VS_OK;
	}

	INT Parameter::SetParamArrayValues(
			INT    group_index,
			INT    group_id,
			INT    array_count,
			FLOAT *array_values
		)
	{
		INT param_group = _pi_parameter->param_group;
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT gi = group_index + group_offset;

		_pi_parameter->param_array_values[gi][0] = (FLOAT)group_id;

		// TBD: something == array_count

		for (INT i = 1; i < Parameter::PARAM_ARRAY_SIZE; i++)
		{
			_pi_parameter->param_array_values[gi][i] = *(array_values + i - 1);
		}
		return VS_OK;
	}

	INT Parameter::SetParamArrayDefaults(
			INT param_group
		)
	{

		UINT group_size   = _pi_parameter->param_group_size[param_group];
		UINT group_offset = _pi_parameter->param_group_offset[param_group];

		for (UINT j = 0; j < group_size; j++)
		{
			INT gi = group_offset + j;
			for (INT i = 0; i < Parameter::PARAM_ARRAY_SIZE; i++)
			{
				_pi_parameter->param_array_values[gi][i] =
					_pi_parameter->param_array_defaults[gi][i];
			}
		}

		return VS_OK;
	}


// --- fast stuff

	INT Parameter::Get(
			INT    param_group,
			INT    group_index,
			INT    get_size,
			INT    get_id,
			FLOAT *array_values
		)
	{
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT get_index = group_index + group_offset;
		for (INT i = 0; i < get_size; i++)
		{
			*(array_values + i) = _pi_parameter->param_array_values[get_index][i];
		}
		return VS_OK;
	}

	INT Parameter::Put(
			INT    param_group,
			INT    group_index,
			INT    get_size,
			INT    get_id,
			FLOAT *array_values
		)
	{
		INT group_offset = _pi_parameter->param_group_offset[param_group];
		INT get_index = group_index + group_offset;
		for (INT i = 0; i < get_size; i++)
		{
			_pi_parameter->param_array_values[get_index][i] = *(array_values + i);
		}
		return VS_OK;
	}


// ---- store

	INT Parameter::CopyParamSet(
			Parameter *source
		)
	{

		_pi_parameter->param_group = source->_pi_parameter->param_group;

		for (INT i = 0; i < Parameter::MAX_PARAM_GROUP; i++)
		{
			_pi_parameter->param_group_offset[i] = source->_pi_parameter->param_group_offset[i];
			_pi_parameter->param_group_index[i]  = source->_pi_parameter->param_group_index[i];
			_pi_parameter->param_array_index[i]  = source->_pi_parameter->param_array_index[i];
			_pi_parameter->param_group_size[i]   = source->_pi_parameter->param_group_size[i];
		}

		for (INT j = 0; j < Parameter::MAX_PARAM_ARRAY; j++)
		{

			FLOAT *target_values     = &_pi_parameter->param_array_values[j][0];
			FLOAT *target_increments = &_pi_parameter->param_array_increments[j][0];
			FLOAT *target_defaults   = &_pi_parameter->param_array_defaults[j][0];
			CHAR  *target_labels     =  _pi_parameter->param_labels[j][0];

			FLOAT *source_values     = &source->_pi_parameter->param_array_values[j][0];
			FLOAT *source_increments = &source->_pi_parameter->param_array_increments[j][0];
			FLOAT *source_defaults   = &source->_pi_parameter->param_array_defaults[j][0];
			CHAR  *source_labels     =  source->_pi_parameter->param_labels[j][0];

			for (INT i = 0; i < Parameter::PARAM_ARRAY_SIZE; i++)
			{
				*(target_values++)     = *(source_values++);
				*(target_increments++) = *(source_increments++);
				*(target_defaults++)   = *(source_defaults++);
				strcpy(target_labels, source_labels);
			}
		}

		return VS_OK;
	}