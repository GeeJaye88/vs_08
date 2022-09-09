////////////////////////////////////////////////////////////////////////////////

// ---------- vs_inc_maths.h ----------
/*!
\file vs_inc_maths.h
\brief Maths definitions & structs
\author Gareth Edwards
\note 2017
*/

#pragma once


#include "vs_inc_defs.h"



// ---- VsColour ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsColour
{
	FLOAT red;
	FLOAT green;
	FLOAT blue;
	FLOAT alpha;
};


// ---- VsColourValue ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsColourValue
{
	FLOAT red;
	FLOAT green;
	FLOAT blue;
	FLOAT alpha;
};


// ---- VsFloatBox ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsFloatBox
{
	FLOAT left;
	FLOAT top;
	FLOAT back;
	FLOAT right;
	FLOAT bottom;
	FLOAT front;
};


// ---- VsFloatRectangle----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsFloatRectangle
{
	FLOAT left;
	FLOAT top;
	FLOAT right;
	FLOAT bottom;
};


// ---- VsMatrix
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsMatrix
{
	FLOAT m[4][4];
};


// ---- VsFloatSphere ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsFloatSphere
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	FLOAT radius;
};


// ---- VsVector3 ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsVector3
{

public:
		
	FLOAT x;
	FLOAT y;
	FLOAT z;

	VsVector3::VsVector3()
	{
		x = y = z = 0;
	}

	VsVector3::VsVector3(FLOAT xn, FLOAT yn, FLOAT zn)
	{
		x = xn,
		y = yn,
		z = zn;
	}

	/*
	VOID operator = (const VsVector3 &v) {
		this->x = v.x;
		this->y = v.y;
		this->z = v.z;
	}
	*/

	FLOAT Normalise()
	{
		FLOAT len = (FLOAT)sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		if ( len < 0.00001f )
		{
			return 0;
		}
		{
			this->x /= len;
			this->y /= len;
			this->z /= len;
		}
		return len;
	}

	VOID CrossProduct(VsVector3 *a, VsVector3 *b)
	{
		this->x = a->y * b->z - a->z * b->y;
		this->y = a->z * b->x - a->x * b->z;
		this->z = a->x * b->y - a->y * b->x;
	};

	FLOAT ScalarProduct(VsVector3 &v)
	{
		return (x*v.x) + (y*v.y) + (z*v.z);
	}

};


