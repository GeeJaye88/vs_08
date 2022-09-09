////////////////////////////////////////////////////////////////////////////////

// ---------- vs_fractal.h ----------
/*!
\file vs_fractal.h
\brief Interface for the Fractal class.
\author Gareth Edwards
*/

#pragma once


#include <stdlib.h>     /* srand, rand */
#include <math.h>       /* pow         */


// ---- namespace ----
namespace vs_system
{


	// ---------- Fractal class ----------
	class Fractal
	{

	public:

		// ---- cdtor
			Fractal();
			virtual ~Fractal();


		// ---- methods

			float AvgDiamondVals(
					int i,
					int j,
					int stride,
					int size,
					int subSize,
					float lumpy,
					float *fa
				);

			float AvgSquareVals(
					int i,
					int j,
					int stride,
					int size,
					float lumpy,
					float *fa
				);

			float RandNumber(float min, float max);

			int Fractal2D(
					float*	fa,
					int		size,
					int		seedValue,
					float	heightScale,
					float	height
				);

			int PowerOf2(int size);

	private:

		// ---- private implementation
			class  PI_Fractal; PI_Fractal *pi_fractal;


	};


}