////////////////////////////////////////////////////////////////////////////////

// ---------- vs_fractal.cpp ----------
/*!
\file c1_fractal.cpp
\brief Implementation of the Fractal class
\author Gareth Edwards
\note
*/

#include "../header/vs_fractal.h"


using namespace vs_system;


// ---------- Private Implementation of PI_Fractal class ----------
class Fractal::PI_Fractal
{

public:

	// ---- cdtor
		PI_Fractal();
		~PI_Fractal();

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Fractal::PI_Fractal::PI_Fractal()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Fractal::PI_Fractal::~PI_Fractal()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
Fractal::Fractal()
{
	pi_fractal = new PI_Fractal();
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
Fractal::~Fractal()
{
	if ( pi_fractal != 0 )
	{
		delete pi_fractal;
	}
}


////////////////////////////////////////////////////////////////////////////////


// ---------- FRACTAL 2D ----------


// ---------- constructor ----------
/*!
\brief create a 2d fractal surface array
\author Gareth Edwards
\param float* (destination array)
\param int (size of the destination array)
\param int (random seed value)
\param float (height scalar)
\param float (height)
\return bool (TRIE if ok)
*/
int Fractal::Fractal2D(
		float*	fa,
		int		size,
		int		seedValue,
		float	heightScale,
		float	height_roughness_power
	)
{

	// ---- local
		int	i, j;
		int	stride;
		int	oddline;
		int subSize;
		float ratio, scale;

	// ---- can't tesselate the array if it is not a power of 2
		if ( !PowerOf2( size ) || ( size == 1 ) ) {
			return 0;
		}

	// ---- subSize is the dimension of the array in terms of
		// connected line segments, while size is the dimension
		// in terms of number of vertices
		subSize = size;


		size++; // the BIG one - must be +1 for subdivsion


	// ---- initialize random number generator
		srand(seedValue);

	// ---- set up our roughness constants.
		ratio = (float)pow(2, -height_roughness_power);
		scale = -heightScale * ratio;
		float lumpy     = 1.0f;
		float scaleMin = -0.5f;
		float scaleMax =  0.5f;

	// ---- seed the first four values
		stride = subSize / 2;
		fa[(0       * size) + 0      ] = 0.0f;
		fa[(subSize * size) + 0      ] = 0.0f;
		fa[(subSize * size) + subSize] = 0.0f;
		fa[(0       * size) + subSize] = 0.0f;

	// Now we add ever-increasing detail based on the "diamond" seeded
	// values. We loop over stride, which gets cut in half at the
	// bottom of the loop. Since it's an int, eventually division by 2
	// will produce a zero result, terminating the loop

		while ( stride )
		{
			// ---- take the existing "square" data and produce "diamond" data
				for (i = stride; i<subSize; i += stride)
				{
					for (j = stride; j<subSize; j += stride)
					{
						fa[(i * size) + j] =
							scale * RandNumber(scaleMin, scaleMax) +
							AvgSquareVals(i, j, stride, size, lumpy, fa);
						j += stride;
					}
					i += stride;
				}


			// ---- take the existing "diamond" data and make it into "squares"
				oddline = 0;
				for (i = 0; i<subSize; i += stride)
				{
						oddline = (oddline == 0);
						for (j = 0; j<subSize; j += stride)
						{
								if ((oddline) && !j)
								{
									j += stride;
								}

								// ---- i and j are setup
									// Call avgDiamondVals with the
									// current position. It will return the average of the
									// surrounding diamond data points

									fa[(i * size) + j] =
										scale * RandNumber(scaleMin, scaleMax) +
										AvgDiamondVals(i, j, stride, size, subSize, lumpy, fa);

									// ---- to wrap edges seamlessly, copy edge values around to other side of array
										if (i == 0)
										{
											fa[(subSize*size) + j] =
												fa[(i * size) + j];
										}
										if (j == 0)
										{
											fa[(i*size) + subSize] =
												fa[(i * size) + j];
										}

								j += stride;
						}
				}

			// ---- reduce random number range
				scale *= ratio;
				stride >>= 1;
		}

	return 1;
}


// ---------- RandNumber ----------
/*!
\brief Generate a flotining point random value between a minum and maximum value
\author Gareth Edwards
\param float (minimum value)
\param float (maximum value)
\return float (random value)
\note Invoked by fractal2D constructor.
Return a random floating point number such that
(min <= return-value <= max) 32,767 values
are possible for any given range.
*/
float Fractal::RandNumber(float min, float max)
{
	int   r;
	float x;
	r = rand();
	x = (float)(r & 0x7fff) /
		(float)0x7fff;
	return (x * (max - min) + min);
}


// ---------- AvgDiamondVals ----------
/*!
\brief compute the fractal average of four diamond values
\author Gareth Edwards
\param int (row)
\param int (column)
\param int (represents the distance from the diamond center to a diamond corner)
\param int (size of a row)
\param int (sub size - only used when computing tiled arrays)
\param float (lumpiness or roughness)
\param float (array)
\note Invoked by fractal2D constructor.
Given the i,j location as the center of a diamond,
average the data values at the four corners of the
diamond and return it.
*/
float Fractal::AvgDiamondVals(
		int    i,
		int    j,
		int    stride,
		int    size,
		int    subSize,
		float  lumpy,
		float *fa
	)
{

	//bool edge_blending = true;

	// ---- diamond without edge blending
	/*
		if (i == 0)
		{
			return (
				(float)(
					fa[(i*size) + j - stride] +
					fa[(i*size) + j + stride] +
					fa[((i + stride)*size) + j]
					) * .33f * lumpy);
		}
		else if (i == size - 1)
		{
			return (
				(float)(
					fa[(i*size) + j - stride] +
					fa[((i - stride)*size) + j] +
					fa[((0 + stride)*size) + j]
					) * .33f * lumpy);
		}
		else if (j == 0)
		{
			return (
				(float)(
					fa[((i - stride)*size) + j] +
					fa[((i + stride)*size) + j] +
					fa[(i*size) + j + stride]
					) * .33f * lumpy);
		}
		else if (j == size - 1)
		{
			return (
				(float)(
					fa[((i - stride)*size) + j] +
					fa[((i + stride)*size) + j] +
					fa[(i*size) + j - stride]
					) * .33f * lumpy);
		}
		else
		{
			return (
				(float)(
					fa[((i - stride)*size) + j] +
					fa[((i + stride)*size) + j] +
					fa[(i*size) + j - stride] +
					fa[(i*size) + j + stride]
					) * .25f * lumpy);
		}
	*/
	/*
	// In order to support tiled surfaces which meet seamless at the
	// edges (that is, they "wrap"), We need to be careful how we
	// calculate averages when the i,j diamond center lies on an edge
	// of the array. The first four 'if' clauses handle these
	// cases. The final 'else' clause handles the general case (in
	// which i,j is not on an edge).
	*/
		if (i == 0)
		{
			return ((float) (fa[(i*size) + j-stride] +
			fa[(i*size) + j+stride] +
			fa[((subSize-stride)*size) + j] +
			fa[((i+stride)*size) + j]) * .25f);
		}
		else if (i == size-1)
		{
			return ((float) (fa[(i*size) + j-stride] +
			fa[(i*size) + j+stride] +
			fa[((i-stride)*size) + j] +
			fa[((0+stride)*size) + j]) * .25f);
		}
		else if (j == 0)
		{
			return ((float) (fa[((i-stride)*size) + j] +
			fa[((i+stride)*size) + j] +
			fa[(i*size) + j+stride] +
			fa[(i*size) + subSize-stride]) * .25f);
		}
		else if (j == size-1)
		{
			return ((float) (fa[((i-stride)*size) + j] +
			fa[((i+stride)*size) + j] +
			fa[(i*size) + j-stride] +
			fa[(i*size) + 0+stride]) * .25f);
		}
		else
		{
			return ((float) (fa[((i-stride)*size) + j] +
			fa[((i+stride)*size) + j] +
			fa[(i*size) + j-stride] +
			fa[(i*size) + j+stride]) * .25f);
		}

}


// ---------- AvgSquareVals ----------
/*!
\brief compute the fractal average of four square values
\author Gareth Edwards
\param int (row)
\param int (column)
\param int (represents the distance from the diamond center to a diamond corner)
\param int (size of a row)
\param float (lumpiness or roughness)
\param float (array)
\note Invoked by fractal2D constructor.
Given the i,j location as the center of a square,
average the data values at the four corners of the
diamond and return it.
*/
float Fractal::AvgSquareVals(
		int i,
		int j,
		int stride,
		int size,
		float lumpy,
		float *fa
	)
{
	return ((float)(fa[((i - stride)*size) + j - stride] +
		fa[((i - stride)*size) + j + stride] +
		fa[((i + stride)*size) + j - stride] +
		fa[((i + stride)*size) + j + stride]) * .25f * lumpy);
}


// ---------- PowerOf2 ----------
/*!
\brief tests if a values is a power of two
\author Gareth Edwards
\param int (power of two)
\returns int (1 - if size is a power of 2 OR 0 if size is not a power of 2, or is zero)
\note Invoked by fractal2D constructor.
*/
int Fractal::PowerOf2(int size)
{

	// ---- local
		int i, bitcount = 0;

	// ---- portable byte count
		// Note this code assumes that (sizeof(int)*8) will yield the
		// number of bits in an int. Should be portable to most
		// platforms.
		//
		for (i = 0; i<sizeof(int) * 8; i++)
		{
			if (size & (1 << i))
			{
				bitcount++;
			}
		}

	// --- one bit - ust be a power of 2
		if (bitcount == 1)
		{
			return (1);
		}

	// either size==0, or size not a power of 2
		else
		{
			return (0);
		}
}


////////////////////////////////////////////////////////////////////////////////