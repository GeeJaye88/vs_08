////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_img_analysis.hpp ----------
/*!
\file s2_img_atlas.hpp
\brief
\author Gareth Edwards
*/


using namespace surface_02;


////////////////////////////////////////////////////////////////////////////////


class LuminafieldBuffer::PI_LuminafieldBuffer
{

public:


	// ---- constructor ----
		PI_LuminafieldBuffer()
		{
		}

	// ---- destructor ----
		~PI_LuminafieldBuffer()
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
LuminafieldBuffer::LuminafieldBuffer()
{
	_pi_luminafield_buffer = new PI_LuminafieldBuffer();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
LuminafieldBuffer::~LuminafieldBuffer()
{
	if ( _pi_luminafield_buffer != NULL )
	{
		delete _pi_luminafield_buffer;
		_pi_luminafield_buffer = NULL;
	}
}


// ---------- CreateBuffer ----------
INT LuminafieldBuffer::CreateBuffer()
{
	if ( _pi_luminafield_buffer->_buffer != NULL ) return VS_ERROR;

	if (
			_pi_luminafield_buffer->_columns == 0 ||
			_pi_luminafield_buffer->_rows    == 0 ||
			_pi_luminafield_buffer->_block   == 0
		) return VS_ERROR;

	_pi_luminafield_buffer->_stride = _pi_luminafield_buffer->_columns * _pi_luminafield_buffer->_block;
	_pi_luminafield_buffer->_extent = _pi_luminafield_buffer->_rows * _pi_luminafield_buffer->_stride;
	_pi_luminafield_buffer->_buffer = new USHORT[_pi_luminafield_buffer->_extent];

	return VS_OK;
};


// ---------- DestroyBuffer ----------
INT LuminafieldBuffer::DestroyBuffer()
{
	if ( _pi_luminafield_buffer->_buffer == NULL ) return VS_ERROR;
	delete [] _pi_luminafield_buffer->_buffer;
	_pi_luminafield_buffer->_buffer = NULL;
	return VS_OK;
};


// ---------- Initialise ----------
INT LuminafieldBuffer::Initialise(
		UINT columns,
		UINT rows,
		UINT block
	)
{
	_pi_luminafield_buffer->_columns = columns;
	_pi_luminafield_buffer->_rows    = rows;
	_pi_luminafield_buffer->_block   = block;
	_pi_luminafield_buffer->_stride  = _pi_luminafield_buffer->_columns * _pi_luminafield_buffer->_block;
	_pi_luminafield_buffer->_extent  = _pi_luminafield_buffer->_rows * _pi_luminafield_buffer->_stride;
	return VS_OK;
};


// ---------- Set ----------
INT LuminafieldBuffer::Set(INT value)
{
	memset(GetBuffer(), value, GetExtent() * sizeof(FLOAT));
	return VS_OK;
}


// ---------- Get ----------
USHORT *LuminafieldBuffer::GetBuffer()  { return _pi_luminafield_buffer->_buffer;  }
UINT    LuminafieldBuffer::GetColumns() { return _pi_luminafield_buffer->_columns; }
UINT    LuminafieldBuffer::GetExtent()  { return _pi_luminafield_buffer->_extent;  }
UINT    LuminafieldBuffer::GetBlock()   { return _pi_luminafield_buffer->_block;   }
UINT    LuminafieldBuffer::GetRows()    { return _pi_luminafield_buffer->_rows;    }
UINT    LuminafieldBuffer::GetStride()  { return _pi_luminafield_buffer->_stride ; }
