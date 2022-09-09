////////////////////////////////////////////////////////////////////////////////

// ---------- vs_lum_buffer.h.cpp ----------
/*!
\file vs_lum_buffer.h.cpp
\brief Implementation of the LuminafieldBuffer class.
\author Gareth Edwards 
\note 2020
*/

// ---- include ----
#include "../header/vs_lum_buffer.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Private Implementation of PI_FloatBuffer class ----------


////////////////////////////////////////////////////////////////////////////////
//
//
//template <class t>
//bb<t>::bb()
//{
//}
//template <class t>
//bb<t>::~bb()
//{
//}
//

////////////////////////////////////////////////////////////////////////////////


//constructor with the default size 10
//template <class T>
//Stack<T>::Stack(int s)
//{
//	size = s > 0 && s < 1000 ? s : 10 ;  
//	top = -1 ;  // initialize stack
//	stackPtr = new T[size] ; 
//}
//
//// push an element onto the Stack 
//template <class T>
//int Stack<T>::push(const T& item)
//{
//	if (!isFull())
//	{
//		stackPtr[++top] = item ;
//		return 1 ;  // push successful
//	}
//	return 0 ;  // push unsuccessful
//}
//
//// pop an element off the Stack
//template <class T> 
//int Stack<T>::pop(T& popValue) 
//{
//	if (!isEmpty())
//	{
//		popValue = stackPtr[top--] ;
//		return 1 ;  // pop successful
//	}
//	return 0 ;  // pop unsuccessful
//}


////////////////////////////////////////////////////////////////////////////////


//template <class T>
//class PI_LuminafieldBuffer
//{
//
//public:
//
//	// ---- constructor ----
//		PI_LuminafieldBuffer()
//		{
//		}
//
//	// ---- destructor ----
//		~PI_LuminafieldBuffer()
//		{
//			if (_buffer != NULL)
//			{
//				delete[] _buffer;
//				_buffer = NULL;
//			}
//		}
//
//	// ---- properties ----
//		T     *_buffer = NULL;
//		UINT  _columns = 0;
//		UINT  _extent = 0;
//		UINT  _block = 0;
//		UINT  _rows = 0;
//		UINT  _stride = 0;
//
//};

/*

// ---------- Constructor ----------
template <class T>
LuminafieldBuffer<T>::LuminafieldBuffer()
{
	;
}


// ---------- Destructor ----------
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
	if ( _buffer != NULL ) return VS_ERROR;

	if (
			_columns == 0 ||
			_rows    == 0 ||
			_block   == 0
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
	if ( _buffer == NULL ) return VS_ERROR;
	delete [] _buffer;
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
	_rows    = rows;
	_block   = block;
	_stride  = _columns * _block;
	_extent  = _rows * _stride;
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
T      *LuminafieldBuffer<T>::GetBuffer()  { return _buffer;  }

template <class T>
UINT    LuminafieldBuffer<T>::GetBlock()   { return _block;   }

template <class T>
UINT    LuminafieldBuffer<T>::GetColumns() { return _columns; }

template <class T>
UINT    LuminafieldBuffer<T>::GetExtent()  { return _extent;  }

template <class T>
UINT    LuminafieldBuffer<T>::GetRows()    { return _rows;    }

template <class T>
UINT    LuminafieldBuffer<T>::GetStride()  { return _stride ; }

*/