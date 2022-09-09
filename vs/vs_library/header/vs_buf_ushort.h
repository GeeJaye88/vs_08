////////////////////////////////////////////////////////////////////////////////

// ---------- vs_buf_ushort.h ----------
/*!
\file vs_buf_ushort.h.h
\brief Interface for the UnsignedShortBuffer class.
\author Gareth Edwards
\note 2017
*/

#pragma once

#include "../../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{

	class UnsignedShortBuffer
	{

	public:

		UnsignedShortBuffer();
		~UnsignedShortBuffer();

		INT CreateBuffer();
		INT DestroyBuffer();
		INT Initialise(
			UINT columns,
			UINT rows,
			UINT block
		);

		INT Set(INT);

		USHORT* GetBuffer();
		UINT    GetBlock();
		UINT    GetColumns();
		UINT    GetExtent();
		UINT    GetRows();
		UINT    GetStride();

	private:

		// ---- private implementation
		class  PI_UnsignedShortBuffer; PI_UnsignedShortBuffer *_pi_unsigned_short_buffer;

	};


} // namespace