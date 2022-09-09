////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_heightfield.cpp ----------
/*!
\file vs_img_heightfield.cpp
\brief implementation of the Heightfield class
\author Gareth Edwards
*/

#include <vs/vs_library/header/vs_img_heightfield.h>


using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Fractal class ----------
class Heightfield::PI_Heightfield
{

public:

	// ---- cdtor
		PI_Heightfield();
		~PI_Heightfield();

	// ---- methods
		INT PowerOf2(INT);

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Heightfield::PI_Heightfield::PI_Heightfield()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Heightfield::PI_Heightfield::~PI_Heightfield()
{
	;
}


// ---------- PowerOf2 ----------
/*!
\brief tests if a values is a power of two
\author Gareth Edwards
\param int (power of two)
\returns int (1 - if size is a power of 2 OR 0 if size is not a power of 2, or is zero)
\note Invoked by fractal2D constructor.
*/
INT Heightfield::PI_Heightfield::PowerOf2(INT size)
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


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
Heightfield::Heightfield()
{
	pi_heightfield = new PI_Heightfield();
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
Heightfield::~Heightfield()
{
	if ( pi_heightfield != 0 )
	{
		delete pi_heightfield;
		pi_heightfield = NULL;
	}
}


BOOL Heightfield::Get(
		CHAR   *raw_file_name,
		UCHAR **heightfield,
		INT    *num_vertices
	)
{

	// ---- height field: load data
		char path[MAX_PATH] = { 0 };
		Media::GetMediaFile(raw_file_name, path);
		std::ifstream heightStream;
		heightStream.open(path, std::ios::binary);
		if ( heightStream.fail() )
		{
			CHAR msg[128];
			sprintf(msg, "Could not open height file. [ %s, %d ]\n", __FILE__, __LINE__);
			OutputDebugString(msg);
			return FALSE;
		}


    // ---- set number of vertices
		heightStream.seekg( 0, std::ios::end );
		*num_vertices = (UINT)heightStream.tellg();
		heightStream.seekg( 0, std::ios::beg );


    // ---- height field: allocate memory and read the data
		*heightfield = new UCHAR[*num_vertices];
		heightStream.read( (char *)*heightfield, *num_vertices);
		heightStream.close();

	return TRUE;
}


INT Heightfield::Flatfield2D(
		FLOAT*	dest,
		INT		dest_stride
	)
{

	// ---- local
		INT dx, dz, dz_offset;

	// ---- fill
		for (dz = 0; dz < dest_stride; dz++)
		{
			dz_offset = dz * dest_stride;
			for (dx = 0; dx < dest_stride; dx++)
			{
				dest[dz_offset + dx] = 0;
			}
		}

	return TRUE;
}


INT Heightfield::Heightfield2D(
		FLOAT*	dest,
		INT		dest_stride,
		FLOAT	dest_height_scale,
		UCHAR  *heightfield,
		INT     num_vertices
	)
{
	 
	// ---- local
		UCHAR u;
		INT hx, hz_offset;
		INT dx, dz, dz_offset;
		INT dest_width = (int)sqrt((float)dest_stride-1);

	// ---- calc sample increment
		INT heightfield_stride = (int)sqrt((float)num_vertices);
		INT heightfield_width  = heightfield_stride - 1;
		INT heightfield_incr   = heightfield_width / (dest_stride-1);

	// ---- bad ju-ju if increment not a whole number
		FLOAT f_heightfield_incr = (FLOAT)heightfield_width / (FLOAT)(dest_stride - 1);
		if ( (FLOAT)((INT)f_heightfield_incr) != f_heightfield_incr )
			return VS_ERROR;

	// ---- fill
		INT i = 0;
		for (dz = 0; dz < dest_stride; dz++)
		{
			hx = 0;
			hz_offset = dz * heightfield_stride * heightfield_incr;
			dz_offset = dz * dest_stride;
			for (dx = 0; dx < dest_stride; dx++)
			{
				u = heightfield[hz_offset + hx];
				dest[dz_offset + dx] = (FLOAT)u / 256.0f * dest_height_scale;
				hx += heightfield_incr;
			}
		}

	return TRUE;
}
