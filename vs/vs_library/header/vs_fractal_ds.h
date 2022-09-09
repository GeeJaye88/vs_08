////////////////////////////////////////////////////////////////////////////////

// ---------- vs_fractal_ds.h ----------
/*!
\file vs_fractal_ds.h
\brief Interface for the FractalDS2 class.
\author Gareth Edwards
*/

#pragma once


#include <stdlib.h>     // srand, rand
#include <math.h>       // pow


// ---- namespace ----
namespace vs_system
{


	// ---------- FractalDS2 class ----------
	class FractalDS2
	{

	public:

		// ---- cdtor
			FractalDS2();
			virtual ~FractalDS2();


		// ---- methods
			void  Average(int x, int y, int sideLength);
			void  Create(
					float *map,
					int   size,
					int   seed,
					float range,
					float rough
				);
			void  Diamond(int side_length);
			void  Filter(int side_length);
			void  Initialise();
			float RandomNumber(float min, float max);
			void  Square(int sideLength);

			void  Clamp(int* val, int min, int max);
			void  Clamp_map();

			void  Box(
					float *y_array,
					unsigned int   y_stride,
					float *f_array,
					unsigned int   f_extent,
					float  f_rmu
				);

	private:

		// ---- private implementation
			class  PI_FractalDS2; PI_FractalDS2 *pi_plasma_cloud;


	};


}