////////////////////////////////////////////////////////////////////////////////

// ---------- vs_flt_buffer.h ----------
/*!
\file vs_flt_buffer.h
\brief Interface for the FloatBuffer class.
\author Gareth Edwards
\note 2017
*/

#pragma once

#include "../../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


// ---------- FloatBuffer class ----------
class FloatBuffer
{

public:

	// ---- c/dtor ----
	FloatBuffer();
	virtual ~FloatBuffer();

	INT CreateBuffer();
	INT DestroyBuffer();
	INT Initialise(UINT, UINT, UINT);

	INT Set(INT);

	FLOAT* FloatBuffer::GetBuffer();
	FLOAT* FloatBuffer::GetBuffer(UINT);
	UINT   FloatBuffer::GetStride();
	INT    FloatBuffer::GetExtent();
	UINT   FloatBuffer::GetRows();
	UINT   FloatBuffer::GetPitch();
	UINT   FloatBuffer::GetColumns();

private:

	// private implementation
	class PI_FloatBuffer; PI_FloatBuffer *_pi_float_buffer;

};


} // namespace
