////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_buffer_template.hpp ----------
/*!
\file s2_model_buffer_template.hpp
\brief
\author Gareth Edwards
*/


namespace vs_system
{

	// ---------- Constructor ----------
	/*!
	\brief Destructor
	\author Gareth Edwards
	*/
	template <class T>
	LuminafieldBuffer<T>::LuminafieldBuffer()
	{
		;
	}


	// ---------- Destructor ----------
	/*!
	\brief Destructor
	\author Gareth Edwards
	*/
	template <class T>
	LuminafieldBuffer<T>::~LuminafieldBuffer()
	{
		if (_buffer != NULL)
		{
			delete[] _buffer;
			_buffer = NULL;
		}
	}

	// ---------- CreateBuffer ----------
	template <class T>
	INT LuminafieldBuffer<T>::CreateBuffer()
	{
		if (_buffer != NULL) return VS_ERROR;

		if (
			_columns == 0 ||
			_rows == 0 ||
			_block == 0
			) return VS_ERROR;

		_stride = _columns * _block;
		_extent = _rows * _stride;
		_buffer = new T[_extent];

		return VS_OK;
	};


	// ---------- DestroyBuffer ----------
	template <class T>
	INT LuminafieldBuffer<T>::DestroyBuffer()
	{
		if (_buffer == NULL) return VS_ERROR;
		delete[] _buffer;
		_buffer = NULL;
		return VS_OK;
	};


	// ---------- Initialise ----------
	template <class T>
	INT LuminafieldBuffer<T>::Initialise(
		UINT columns,
		UINT rows,
		UINT block
	)
	{
		_columns = columns;
		_rows = rows;
		_block = block;
		_stride = _columns * _block;
		_extent = _rows * _stride;
		return VS_OK;
	};


	// ---------- Set ----------
	template <class T>
	INT LuminafieldBuffer<T>::Set(INT value)
	{
		memset(GetBuffer(), value, GetExtent() * sizeof(T));
		return VS_OK;
	}


	// ---------- Get ----------
	template <class T>
	T      *LuminafieldBuffer<T>::GetBuffer() { return _buffer; }

	template <class T>
	UINT    LuminafieldBuffer<T>::GetBlock() { return _block; }

	template <class T>
	UINT    LuminafieldBuffer<T>::GetColumns() { return _columns; }

	template <class T>
	UINT    LuminafieldBuffer<T>::GetExtent() { return _extent; }

	template <class T>
	UINT    LuminafieldBuffer<T>::GetRows() { return _rows; }

	template <class T>
	UINT    LuminafieldBuffer<T>::GetStride() { return _stride; }


}