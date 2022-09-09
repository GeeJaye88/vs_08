////////////////////////////////////////////////////////////////////////////////

// ---------- vs_lum_buffer.h.h ----------
/*!
\file vs_lum_buffer.h.h
\brief Interface for the LuminafieldBuffer class.
\author Gareth Edwards
\note 2017
*/

#pragma once

#include "../../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


	template <class T>
	class LuminafieldBuffer
	{

	public:

		LuminafieldBuffer();
		 ~LuminafieldBuffer();

		INT CreateBuffer();
		INT DestroyBuffer();
		INT Initialise(
				UINT columns,
				UINT rows,
				UINT block
			);

		INT Set(INT);

		T    *GetBuffer();
		UINT  GetBlock();
		UINT  GetColumns();
		UINT  GetExtent();
		UINT  GetRows();
		UINT  GetStride();

	private:

		T     *_buffer = NULL;
		UINT  _columns = 0;
		UINT  _extent  = 0;
		UINT  _block   = 0;
		UINT  _rows    = 0;
		UINT  _stride  = 0;

	};

} // namespace

