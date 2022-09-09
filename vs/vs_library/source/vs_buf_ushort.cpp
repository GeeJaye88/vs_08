////////////////////////////////////////////////////////////////////////////////

// ---------- vs_buf_ushort.h.cpp ----------
/*!
\file vs_buf_ushort.h.cpp
\brief Implementation of the UnsignedShortBuffer class.
\author Gareth Edwards 
\note 2020
*/

// ---- include ----
#include "../header/vs_buf_ushort.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Private Implementation of PI_FloatBuffer class ----------


////////////////////////////////////////////////////////////////////////////////


class UnsignedShortBuffer::PI_UnsignedShortBuffer
{

public:

	// ---- constructor ----
		PI_UnsignedShortBuffer()
		{
		}

	// ---- destructor ----
		~PI_UnsignedShortBuffer()
		{
			if (_buffer != NULL)
			{
				delete[] _buffer;
				_buffer = NULL;
			}
		}

	// ---- properties ----
		USHORT *_buffer   = NULL;
		UINT    _columns  = 0;
		UINT    _extent   = 0;
		UINT    _block    = 0;
		UINT    _rows     = 0;
		UINT    _stride   = 0;

};


// ---------- Constructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
UnsignedShortBuffer::UnsignedShortBuffer()
{
	_pi_unsigned_short_buffer = new PI_UnsignedShortBuffer();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
UnsignedShortBuffer::~UnsignedShortBuffer()
{
	if ( _pi_unsigned_short_buffer != NULL )
	{
		delete _pi_unsigned_short_buffer;
		_pi_unsigned_short_buffer = NULL;
	}
}


// ---------- CreateBuffer ----------
INT UnsignedShortBuffer::CreateBuffer()
{
	if ( _pi_unsigned_short_buffer->_buffer != NULL ) return VS_ERROR;

	if (
			_pi_unsigned_short_buffer->_columns == 0 ||
			_pi_unsigned_short_buffer->_rows    == 0 ||
			_pi_unsigned_short_buffer->_block   == 0
		) return VS_ERROR;

	_pi_unsigned_short_buffer->_stride = _pi_unsigned_short_buffer->_columns * _pi_unsigned_short_buffer->_block;
	_pi_unsigned_short_buffer->_extent = _pi_unsigned_short_buffer->_rows * _pi_unsigned_short_buffer->_stride;
	_pi_unsigned_short_buffer->_buffer = new USHORT[_pi_unsigned_short_buffer->_extent];

	return VS_OK;
};


// ---------- DestroyBuffer ----------
INT UnsignedShortBuffer::DestroyBuffer()
{
	if ( _pi_unsigned_short_buffer->_buffer == NULL ) return VS_ERROR;
	delete [] _pi_unsigned_short_buffer->_buffer;
	_pi_unsigned_short_buffer->_buffer = NULL;
	return VS_OK;
};


// ---------- Initialise ----------
INT UnsignedShortBuffer::Initialise(
		UINT columns,
		UINT rows,
		UINT block
	)
{
	_pi_unsigned_short_buffer->_columns = columns;
	_pi_unsigned_short_buffer->_rows    = rows;
	_pi_unsigned_short_buffer->_block   = block;
	_pi_unsigned_short_buffer->_stride  = _pi_unsigned_short_buffer->_columns * _pi_unsigned_short_buffer->_block;
	_pi_unsigned_short_buffer->_extent  = _pi_unsigned_short_buffer->_rows * _pi_unsigned_short_buffer->_stride;
	return VS_OK;
};


// ---------- Set ----------
INT UnsignedShortBuffer::Set(INT value)
{
	memset(GetBuffer(), value, GetExtent() * sizeof(FLOAT));
	return VS_OK;
}


// ---------- Get ----------
USHORT *UnsignedShortBuffer::GetBuffer()  { return _pi_unsigned_short_buffer->_buffer;  }
UINT    UnsignedShortBuffer::GetBlock()   { return _pi_unsigned_short_buffer->_block;   }
UINT    UnsignedShortBuffer::GetColumns() { return _pi_unsigned_short_buffer->_columns; }
UINT    UnsignedShortBuffer::GetExtent()  { return _pi_unsigned_short_buffer->_extent;  }
UINT    UnsignedShortBuffer::GetRows()    { return _pi_unsigned_short_buffer->_rows;    }
UINT    UnsignedShortBuffer::GetStride()  { return _pi_unsigned_short_buffer->_stride ; }