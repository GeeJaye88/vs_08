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

public:

	FLOAT red;
	FLOAT green;
	FLOAT blue;
	FLOAT alpha;

	VsColour::VsColour()
	{
		red = green = blue = alpha = 0;
	}

	VsColour::VsColour(FLOAT r, FLOAT g, FLOAT b, FLOAT a)
	{
		red   = r,
		green = g,
		blue  = b;
		alpha = a;
	}

	VsColour operator*(float f)
	{
		return VsColour(red * f, green * f, blue * f, alpha * f);
	}

	void VsColour::Clamp()
	{
		red   = red   < 0 ? 0 : ( red   > 1 ? 1 : red   );
		green = green < 0 ? 0 : ( green > 1 ? 1 : green );
		blue  = blue  < 0 ? 0 : ( blue  > 1 ? 1 : blue  );
		alpha = alpha < 0 ? 0 : ( alpha > 1 ? 1 : alpha );
	}

	VsColour operator+=(const VsColour &vsc)
	{
		return VsColour(red += vsc.red, green += vsc.green, blue += vsc.blue, alpha += vsc.alpha);
	}

	VsColour operator-=(const VsColour &vsc)
	{
		return VsColour(red -= vsc.red, green -= vsc.green, blue -= vsc.blue, alpha -= vsc.alpha);
	}

	VsColour operator*=(const VsColour &vsc)
	{
		return VsColour(red *= vsc.red, green *= vsc.green, blue *= vsc.blue, alpha *= vsc.alpha);
	}

	VsColour operator=(const VsColour &vsc)
	{
		return VsColour(red = vsc.red, green = vsc.green, blue = vsc.blue, alpha = vsc.alpha);
	}

	VsColour operator+(const VsColour &vsc)
	{
		return VsColour(red + vsc.red, green + vsc.green, blue + vsc.blue, alpha + vsc.alpha);
	}

	VsColour operator-(const VsColour &vsc)
	{
		return VsColour(red - vsc.red, green - vsc.green, blue - vsc.blue, alpha - vsc.alpha);
	}

	VsColour operator*(const VsColour &vsc)
	{
		return VsColour(red * vsc.red, green * vsc.green, blue * vsc.blue, alpha * vsc.alpha);
	}
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


// ---- VsRGBA ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsByteRGBA
{

public:
	BYTE red;
	BYTE green;
	BYTE blue;
	BYTE alpha;
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


// ---- VsIntRectangle----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsIntRectangle
{
	UINT left;
	UINT top;
	UINT right;
	UINT bottom;
};


// ---- VsMatrix
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsMatrix
{
	FLOAT m[4][4] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
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


// ---- VsVector2 ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsVector2
{

public:
		
	FLOAT x;
	FLOAT y;

	VsVector2::VsVector2()
	{
		x = y = 0;
	}

	VsVector2::VsVector2(FLOAT vx, FLOAT vy)
	{
		x = vx;
		y = vy;
	}
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

	FLOAT Length()
	{
		FLOAT len = (FLOAT)sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		if (len < 0.00001f)
		{
			return 0;
		}
		return len;
	}

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

	FLOAT DotProduct(VsVector3 &v)
	{
		return (x*v.x) + (y*v.y) + (z*v.z);
	}

	FLOAT ScalarProduct(VsVector3 &v)
	{
		return (x*v.x) + (y*v.y) + (z*v.z);
	}

	VsVector3 operator=(const VsVector3 &vec)
	{
		return VsVector3(x = vec.x, y = vec.y, z = vec.z);
	}

	VsVector3 operator+(const VsVector3 &vec)
	{
		return VsVector3(x + vec.x, y + vec.y, z + vec.z);
	}

	VsVector3 operator+=(const VsVector3 &vec)
	{
		return VsVector3(x += vec.x, y += vec.y, z += vec.z);
	}

	VsVector3 operator-(const VsVector3 &vec)
	{
		return VsVector3(x - vec.x, y - vec.y, z - vec.z);
	}

	void operator*=(const FLOAT &value)
	{
		x *= value; y *= value; z *= value;
	}

	void operator/=(const FLOAT &value)
	{
		x /= value; y /= value; z /= value;
	}
};




// ---- VsVector3D ----
/*!
\brief primitive type
\author Gareth Edwards
*/
struct VsVector3D
{

public:
		
	double x;
	double y;
	double z;

	VsVector3D::VsVector3D()
	{
		x = y = z = 0;
	}

	VsVector3D::VsVector3D(double xn, double yn, double zn)
	{
		x = xn,
		y = yn,
		z = zn;
	}

};

