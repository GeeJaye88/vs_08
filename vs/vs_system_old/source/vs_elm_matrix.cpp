////////////////////////////////////////////////////////////////////////////////

// ---------- vs_elm_matrix.h ----------
/*!
\file vs_elm_matrix.cpp
\brief implementation of matrix functions based on the vs_matrix struct
\author Gareth Edwards
*/

#include "../header/vs_elm_matrix.h"

using namespace vs_system;


// ---------- creates a matrix with all elements as the input v ----------

VOID VsMatrixOp::Initialise(VsMatrix *vsm, FLOAT v)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			vsm->m[r][c] = v;
		}
	}
}


//  ----------loads an identity matrix ----------

VOID VsMatrixOp::Identity(VsMatrix *vsm)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			vsm->m[r][c] = 0;
		}
		vsm->m[r][r] = 1;
	}
}


//  ----------copy one matrix into another ----------

VOID VsMatrixOp::Copy(VsMatrix *too, VsMatrix *from)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			too->m[r][c] = from->m[r][c];
		}
	}
}


// ---------- creates a rotation matrix that rotates a point by angle r around the desired axis ----------

VOID VsMatrixOp::Rotate(VsMatrix *vsm, FLOAT r, CHAR axis)
{

	Identity(vsm);

	float rdn  = r * float(PI/180);
	float sinr = (FLOAT)sin (rdn);
	float cosr = (FLOAT)cos (rdn);
	switch (axis)
	{
			case 'x' : // x axis
			case 'X' : 
				vsm->m[1][1] =  cosr;
				vsm->m[2][1] = -sinr;
				vsm->m[1][2] =  sinr;
				vsm->m[2][2] =  cosr;
				break;
			case 'y' : // y axis
			case 'Y' :
				vsm->m[0][0] =  cosr;
				vsm->m[2][0] =  sinr;
				vsm->m[0][2] = -sinr;
				vsm->m[2][2] =  cosr;
				break;
			case 'z' : // z axis
			case 'Z' : 
				vsm->m[0][0] =  cosr;
				vsm->m[1][0] = -sinr;
				vsm->m[0][1] =  sinr;
				vsm->m[1][1] =  cosr;
				break;
			default  :
				break;
	}
}


// ---------- creates a matrix that translates a point by variables: x, y and z ----------

VOID VsMatrixOp::Translate(VsMatrix *vsm, float x, float y, float z)
{

	Identity(vsm);

	vsm->m[3][0] = x;
	vsm->m[3][1] = y;
	vsm->m[3][2] = z;

}


//  ----------creates a matrix that transforms an object with an inputted scale from the origin  ----------

VOID VsMatrixOp::Scale(VsMatrix *vsm, float scale)
{

	Identity(vsm);

	for (int s = 0; s < 3; s++)
		vsm->m[s][s] = scale;

}


//  ----------creates a matrix that transforms an object with an inputted scale from the origin  ----------

VOID VsMatrixOp::Scale(VsMatrix *vsm, float x, float y, float z)
{

	Identity(vsm);
	vsm->m[0][0] = x;
	vsm->m[1][1] = y;
	vsm->m[2][2] = z;

}


// ---------- multiply two matrices together ----------

VOID VsMatrixOp::Multiply(VsMatrix *result, VsMatrix *a, VsMatrix *b)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float r = 0;
			for (int k = 0; k < 4; k++)
			{
				r += a->m[i][k] * b->m[k][j];
			}
			result->m[i][j] = r;
		}
	}
}


// ---------- initialise index and load an identity matrix ----------

VOID VsMatrixStack::Initialise()
{
	index = 0;
	op.Identity(&stack[index]);
}


// ---------- applies a translation matrix ----------

VOID VsMatrixStack::Translate(FLOAT x, FLOAT y, FLOAT z)
{
	VsMatrix t;
	op.Translate(&t, x, y, z);

	VsMatrix tmp;
	op.Multiply(&tmp, &t, &stack[index]);
	op.Copy(&stack[index], &tmp);
}


// ---------- applies a rotation matrix ----------

VOID VsMatrixStack::Rotate(FLOAT rot, CHAR axis)
{
	VsMatrix r;
	op.Rotate(&r, rot, axis);

	VsMatrix tmp;
	op.Multiply(&tmp, &r, &stack[index]);
	op.Copy(&stack[index], &tmp);
}


// ---------- applies a scaling matrix  ----------

VOID VsMatrixStack::Scale(FLOAT scale)
{
	VsMatrix s;
	op.Scale(&s, scale);

	VsMatrix tmp;
	op.Multiply(&tmp, &s, &stack[index]);
	op.Copy(&stack[index], &tmp);
}


// ---------- applies a scaling matrix  ----------

VOID VsMatrixStack::Scale(FLOAT x, FLOAT y, FLOAT z)
{
	VsMatrix s;
	op.Scale(&s, x, y, z);

	VsMatrix tmp;
	op.Multiply(&tmp, &s, &stack[index]);
	op.Copy(&stack[index], &tmp);
}


// ---------- push matrix stack ----------

BOOL VsMatrixStack::Push()
{
	if ( index < VS_MATRIX_TOP - 1 )
	{
		VsMatrix* from = &stack[index];
		index++;
		VsMatrix* too = &stack[index];
		op.Copy(too, from);
		return TRUE;
	}
	else
	{
		;
	}
	return FALSE;
}


// ---------- pop matrix stack ----------

BOOL VsMatrixStack::Pop()
{
	if ( index >= 1 )
	{
		index--;
		return TRUE;
	}
	return FALSE;
}
