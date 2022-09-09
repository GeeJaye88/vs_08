////////////////////////////////////////////////////////////////////////////////

// ---------- vs_elm_matrix.h ----------
/*!
\file vs_elm_matrix.h
\brief Interface for the VsMatrix struct, VsMatrixOp and VsMatrixStack classes
\author Gareth Edwards
*/

#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "vs_inc_dems.h"
#include "vs_inc_maths.h"


namespace vs_system
{


// ---- definitions
#define VS_MATRIX_TOP 32
#define PI 3.14159265358979


// ---- matrix operation
class VsMatrixOp
{

public:

	// ---- cdtor
		VsMatrixOp::VsMatrixOp()  {};
		VsMatrixOp::~VsMatrixOp() {};


	// ---- intialise function
		VOID Initialise(VsMatrix*, FLOAT);


	// ---- housekeeping
		VOID Copy(VsMatrix*, VsMatrix*);
		VOID Identity(VsMatrix*);
		BOOL IsIdentity(VsMatrix*);
		VOID Load(VsMatrix*);


	// ---- transform
		VOID Rotate(VsMatrix*, FLOAT, CHAR);
		VOID Translate(VsMatrix*, FLOAT, FLOAT, FLOAT);
		VOID Scale(VsMatrix*, FLOAT);
		VOID Scale(VsMatrix*, FLOAT, FLOAT, FLOAT);
		VOID Multiply(VsMatrix*, VsMatrix*, VsMatrix*);

};


// ---- matrix stack
class VsMatrixStack
{

public:

	// ---- cdtor
		VsMatrixStack::VsMatrixStack()  {};
		VsMatrixStack::~VsMatrixStack() {};


	// ---- properties
		UINT       index = 0;
		VsMatrix   stack[VS_MATRIX_TOP];
		VsMatrixOp op;


	// ---- methods
		VOID Initialise();
		VOID Rotate(FLOAT, CHAR);
		VOID Translate(FLOAT, FLOAT, FLOAT);
		VOID Scale(FLOAT);
		VOID Scale(FLOAT, FLOAT, FLOAT);
		BOOL Push();
		BOOL Pop();

};


}