////////////////////////////////////////////////////////////////////////////////

// ---------- vs_fractal_ds.cpp ----------
/*!
\file vs_fractal_ds.cpp
\brief Implementation of the FractalDS2 class
\author Gareth Edwards
\note
*/ 

#include "../header/vs_fractal_ds.h"


using namespace vs_system;


// ---------- Private Implementation of PI_Fractal class ----------
class FractalDS2::PI_FractalDS2
{

public:

	// ---- cdtor
		PI_FractalDS2() {};
		~PI_FractalDS2() {};

	// ---- properties
		int    iteration = 0;
		int    size  =  513;
		float  range = -0.5f;
		float  rough =  1.0f;
		float *map   =  0;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
FractalDS2::FractalDS2()
{
	pi_plasma_cloud = new PI_FractalDS2();
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
FractalDS2::~FractalDS2()
{
	if ( pi_plasma_cloud != 0 )
	{
		delete pi_plasma_cloud;
		pi_plasma_cloud = 0;
	}
}


#define PCR(size, row, col) ( size * row + col)


void FractalDS2::Average(int x, int y, int side_length)
{
	float *map    = pi_plasma_cloud->map;
	float range   = pi_plasma_cloud->range;
	float rough   = pi_plasma_cloud->rough;
	int   size    = pi_plasma_cloud->size;
	int half_side = side_length / 2;
	float counter = 0;
	float accumulator = 0;

	if (x != 0)
	{
		counter     += 1.0f;
		accumulator += map[ PCR(size,  y,              (x - half_side) ) ];
	}
	if (y != 0)
	{
		counter     += 1.0f;
		accumulator += map[ PCR(size, (y - half_side), x               ) ];
	}
	if (x != size - 1)
	{
		counter     += 1.0f;
		accumulator += map[ PCR(size,  y,             (x + half_side)  ) ];
	}
	if (y != size - 1)
	{
		counter     += 1.0f;
		accumulator += map[ PCR(size, (y + half_side), x               ) ];
	}

	map[ PCR(size, y, x) ] = (accumulator / counter) + (RandomNumber(-range, range) * rough);
}


void FractalDS2::Create(
		float *map,
		int   size,
		int   seed,
		float range,
		float rough
	)
{

	pi_plasma_cloud->map   =  map;
	pi_plasma_cloud->range =  range;
	pi_plasma_cloud->rough = -rough;
	pi_plasma_cloud->size  =  size;
	pi_plasma_cloud->iteration = 0;

	srand(seed);

	Initialise();

	//int iteration = 0;

	Diamond(size);
	Square(size);
	int side_length = size / 2;
	pi_plasma_cloud->range /= 2;

	while (side_length >= 2)
	{

		//if ( iteration == 5 ) Filter(side_length + 1);
		//iteration++;

		Diamond(side_length + 1);
		Square(side_length + 1);
		side_length /= 2;
		pi_plasma_cloud->range /= 2;
	}

}


void FractalDS2::Clamp(int* val, int min, int max)
{
	;
}


void FractalDS2::Clamp_map()
{
	;
}


void FractalDS2::Diamond(int side_length)
{

	float *map      = pi_plasma_cloud->map;
	float range     = pi_plasma_cloud->range;
	float rough     = pi_plasma_cloud->rough;
	int   size      = pi_plasma_cloud->size;
	int sub_length  = side_length - 1;
	int half_length = side_length / 2;
	int side_incr   = size / sub_length;

	for (int y = 0; y < side_incr; y++)
	{
		for (int x = 0; x < side_incr; x++)
		{
			int center_x = x * sub_length + half_length;
			int center_y = y * sub_length + half_length;

			float avg = (
							map[ PCR(size, (  x      * (side_length - 1) ), (  y      * (side_length - 1) ) ) ] +
							map[ PCR(size, (  x      * (side_length - 1) ), ( (y+1)   * (side_length - 1) ) ) ] +
							map[ PCR(size, ( (x + 1) * (side_length - 1) ), (  y      * (side_length - 1) ) ) ] +
							map[ PCR(size, ( (x + 1) * (side_length - 1) ), ( (y + 1) * (side_length - 1) ) ) ]
						 ) / 4.0f;

			map[ PCR(size, center_x, center_y) ] = avg + (RandomNumber(-range, range) * rough);
		}
	}

}

#include "windows.h"
#include "stdio.h"
#include <iostream>

void FractalDS2::Filter(int side_length)
{

	// ---- local
		float *map       = pi_plasma_cloud->map;
		int   size       = pi_plasma_cloud->size;
		int   sub_length = side_length - 1;
		int   side_incr  = size / sub_length;
		int   extent     = (side_incr + 1) * (side_incr + 1);

	// ---- allocate memory for filtering
		float *source = new float[extent];

	// ---- copy initialised map into source
		float *sptr = source;
		for (int y = 0; y < size; y += sub_length)
		{
			for (int x = 0; x < size; x += sub_length)
			{
				*(sptr++) = *(map + PCR(size, y, x));
			}
		}

		float filter_array[9] =
		{
			0.0625, 0.1250, 0.0625,
			0.1250, 0.2500, 0.1250,
			0.0625, 0.1250, 0.0625
		};
		Box(
				source,
				side_incr + 1,
				filter_array,
				9,
				0.5f
			);

	// ---- copy filtered source into map
		sptr = source;
		for (int y = 0; y < size; y += sub_length)
		{
			for (int x = 0; x < size; x += sub_length)
			{
				*(map + PCR(size, y, x)) = *(sptr++);
			}
		}

	// ---- cleanup source
		delete[] source;


	char ods[128];
	sprintf_s(ods, 128, " +-> %d - %d - %d - %d\n", size, side_incr, side_length, extent);
	OutputDebugString(ods);
}


void FractalDS2::Box(
		float         *y_array,
		unsigned int   y_stride,
		float         *f_array,
		unsigned int   f_extent,
		float          f_rmu
	)
{

	// ---- ok extent
		switch (f_extent)
		{
			case 9:   // 3^2
			case 25:  // 5^2
			case 49:  // 7^2
				break;
			default:
				return;
		}
	

	// ---- row/column size
		INT f_size = (int)sqrt((float)f_extent);


	// ---- y buffer
		INT y_extent = y_stride * y_stride;
		INT y_offset = 0;
		INT y_index  = 0;
		FLOAT y = 0;


	// ---- filter
		INT f_dby2  = (f_size - 1) / 2;
		INT f_index = 0;
		INT f_rows, f_row_index, f_row_offset;
		INT f_cols, f_col_offset;


	// ---- create filter array index offset table
		INT *f_offset = new INT[f_extent];
		for (f_rows = 0; f_rows < f_size; f_rows++)
		{
			f_row_index = f_rows * f_size;
			f_row_offset = (f_rows - f_dby2) * y_stride;
			for (f_cols = 0; f_cols < f_size; f_cols++)
			{
				f_index = f_row_index + f_cols;
				f_col_offset = f_row_offset + (f_cols - f_dby2);
				*(f_offset + f_index) = f_col_offset;
			}
		}


	// ---- for each y cel
		FLOAT *y_filtered = new FLOAT[y_extent];
		for ( y_index = 0; y_index<y_extent; y_index++ )
		{
			// ---- apply filter
				y = 0;
				float count = 0;
				for ( f_index = 0; f_index < (int)f_extent; f_index++ )
				{
					// ---- within ?
						y_offset = *(f_offset + f_index);
						if ( f_index != 4 && y_offset >= 0 && y_offset < y_extent )
						{
							// y += *(y_array + y_offset) * *(f_array + f_index);
							y += *(y_array + y_offset);
							count++;
						}
				}
				*(y_filtered + y_index) = y / count;
		}


	// ---- interpolate with original
		for (y_index =0; y_index<y_extent; y_index++)
		{
			*(y_array + y_index) = (1-f_rmu) * *(y_array + y_index) +
										f_rmu * *(y_filtered + y_index);
		}


	// ---- delete
		delete [] y_filtered;
		delete [] f_offset;

}


void FractalDS2::Initialise()
{
	float *map  = pi_plasma_cloud->map;
	int    size = pi_plasma_cloud->size;
	float range = pi_plasma_cloud->range;

	map[ PCR(size,       0,        0  ) ] = RandomNumber(0, range);
	map[ PCR(size,       0,  (size-1) ) ] = RandomNumber(0, range);
	map[ PCR(size, (size-1),       0  ) ] = RandomNumber(0, range);
	map[ PCR(size, (size-1), (size-1) ) ] = RandomNumber(0, range);
}


float FractalDS2::RandomNumber(float min, float max)
{
	int   r;
	float x;
	r = rand();
	x = (float)(r & 0x7fff) /
		(float)0x7fff;
	return (x * (max - min) + min);
	//return min + (rand() % static_cast<int>(max - min + 1));
}


void FractalDS2::Square(int side_length)
{
	float *map      = pi_plasma_cloud->map;
	float range     = pi_plasma_cloud->range;
	int   size      = pi_plasma_cloud->size;
	int sub_length  = side_length - 1;
	int half_length = side_length / 2;
	int side_incr   = size / sub_length;

	for (int y = 0; y < side_incr; y++)
	{
		for (int x = 0; x < side_incr; x++)
		{
			Average(  x     *(side_length - 1) + half_length,  y     *(side_length - 1),               side_length); // top
			Average( (x + 1)*(side_length - 1)              ,  y     *(side_length - 1) + half_length, side_length); // right
			Average(  x     *(side_length - 1) + half_length, (y + 1)*(side_length - 1),               side_length); // bottom
			Average(  x     *(side_length - 1)              ,  y     *(side_length - 1) + half_length, side_length); // left 
		}
	}
}


////////////////////////////////////////////////////////////////////////////////