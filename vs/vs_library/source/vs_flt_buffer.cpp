////////////////////////////////////////////////////////////////////////////////

// ---------- vs_flt_buffer.cpp ----------
/*!
\file vs_flt_buffer.cpp
\brief Implementation of the FloatBuffer class.
\author Gareth Edwards 
\note 2017
*/

// ---- include ----
#include "../header/vs_flt_buffer.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Private Implementation of PI_FloatBuffer class ----------


class FloatBuffer::PI_FloatBuffer
{

public:


	// ---- constructor ----
	PI_FloatBuffer()
	{
	}

	// ---- destructor ----
	~PI_FloatBuffer()
	{
		if (_buffer != NULL)
		{
			delete[] _buffer;
			_buffer = NULL;
		}
	}


	// ---- properties ----
	FLOAT *_buffer  = NULL;
	UINT   _stride  = 0;
	INT    _extent  = 0;
	UINT   _rows    = 0;
	UINT   _pitch   = 0;
	UINT   _columns = 0;

};


// ---------- Implementation of FloatBuffer class ----------


// ---------- Constructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
FloatBuffer::FloatBuffer()
{
	_pi_float_buffer = new PI_FloatBuffer();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
FloatBuffer::~FloatBuffer()
{
	if ( _pi_float_buffer != NULL )
	{
		delete _pi_float_buffer;
		_pi_float_buffer = NULL;
	}
}


// ---------- CreateBuffer ----------
INT FloatBuffer::CreateBuffer()
{
	if (_pi_float_buffer->_buffer != NULL) return VS_ERROR;
	if (
			_pi_float_buffer->_columns == 0 ||
			_pi_float_buffer->_rows    == 0 ||
			_pi_float_buffer->_stride  == 0
		) return VS_ERROR;
	_pi_float_buffer->_pitch  = _pi_float_buffer->_columns * _pi_float_buffer->_stride;
	_pi_float_buffer->_extent = _pi_float_buffer->_rows * _pi_float_buffer->_pitch;
	_pi_float_buffer->_buffer = new FLOAT[_pi_float_buffer->_extent];
	return VS_OK;
};


// ---------- DestroyBuffer ----------
INT FloatBuffer::DestroyBuffer()
{
	if (_pi_float_buffer->_buffer == NULL) return VS_ERROR;
	delete _pi_float_buffer->_buffer;
	_pi_float_buffer->_buffer = NULL;
	return VS_OK;
};


// ---------- Initialise ----------
INT FloatBuffer::Initialise(
		UINT columns,
		UINT rows,
		UINT stride
	)
{
	_pi_float_buffer->_columns = columns;
	_pi_float_buffer->_rows    = rows;
	_pi_float_buffer->_stride  = stride;
	_pi_float_buffer->_pitch   = _pi_float_buffer->_columns * _pi_float_buffer->_stride;
	_pi_float_buffer->_extent  = _pi_float_buffer->_rows * _pi_float_buffer->_pitch;
	return VS_OK;
};


// ---------- Set ----------
INT FloatBuffer::Set(INT value)
{

	memset(GetBuffer(), value, GetExtent() * sizeof(FLOAT));
	return VS_OK;
}


// ---------- Get ----------
FLOAT *FloatBuffer::GetBuffer()  { return _pi_float_buffer->_buffer;  }
UINT   FloatBuffer::GetStride()  { return _pi_float_buffer->_stride;  }
INT    FloatBuffer::GetExtent()  { return _pi_float_buffer->_extent;  }
UINT   FloatBuffer::GetRows()    { return _pi_float_buffer->_rows;    }
UINT   FloatBuffer::GetPitch()   { return _pi_float_buffer->_pitch;   }
UINT   FloatBuffer::GetColumns() { return _pi_float_buffer->_columns; }

FLOAT* FloatBuffer::GetBuffer(UINT index)
{
	INT offset = index * GetPitch();
	return _pi_float_buffer->_buffer+offset;
}