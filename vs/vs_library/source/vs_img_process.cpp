////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_process.cpp ----------
/*!
\file vs_img_process.cpp
\brief Implementation of the ImgProcess class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_img_process.h"
#include <math.h>


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ImgProcess class ----------
class ImgProcess::PI_ImgProcess
{

public:

	// Constructor/destructor
	PI_ImgProcess() {};
	~PI_ImgProcess() {};

	INT Validate(ImgBuffer *image)
	{
		// If image properties NOT ok THEN return error
		if (
			image->GetWidth() <= 0 ||
			image->GetHeight() <= 0 ||
			image->GetBytes()  <= 1 ||
			image->GetStride() != image->GetBytes() ||
			image->GetPitch()  != image->GetWidth() * image->GetBytes() ||
			image->GetExtent() != image->GetHeight() * image->GetWidth() * image->GetBytes() ||
			image->GetBuffer() == NULL )
				return VS_ERROR;

		// OK
		return VS_OK;
	};

};



////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
ImgProcess::ImgProcess()
{
	pi_img_process = new PI_ImgProcess();
};


// ---------- Destructor ----------
ImgProcess::~ImgProcess()
{
	if ( pi_img_process != NULL ) { delete pi_img_process; pi_img_process = NULL; }
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Bars ----------
int ImgProcess::Bars(
		ImgBuffer *bars_image
	)
{
	
	// Local
	UINT column_width = bars_image->GetWidth() / 8;

	// RGB colour struct
	typedef struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} RGB;

	// RGB colour bars LUT
	RGB BAR_COLOUR[8] =
	{
		{ 255, 255, 255 },  // 100% White
		{ 255, 255,   0 },  // Yellow
		{   0, 255, 255 },  // Cyan
		{   0, 255,   0 },  // Green
		{ 255,   0, 255 },  // Magenta
		{ 255,   0,   0 },  // Red
		{   0,   0, 255 },  // Blue
		{   0,   0,   0 },  // Black
	};

	// FOR each row
	unsigned char *ptr = bars_image->GetBuffer();
	UINT bytes = bars_image->GetBytes();
	UINT width = bars_image->GetWidth();
	UINT height = bars_image->GetHeight();
	for ( unsigned int y=0; y<height; y++ )
	{
		// FOR each col 
		for (unsigned int x=0; x<width; x++)
		{
			unsigned idx = x / column_width;
			ptr[0] = BAR_COLOUR[idx].r;
			ptr[1] = BAR_COLOUR[idx].g;
			ptr[2] = BAR_COLOUR[idx].b;
			if (bytes == 4)
			{
				ptr[3] = 255;
				ptr+=4;
			}
			else
				ptr+=3;
		}
	}

	// done
    return VS_OK;
};


// ---------- ResizeLite ----------
int ImgProcess::ResizeLite(
		ImgBuffer *too,
		ImgBuffer *from
	)
{
	// from parameters
	int fromHeightM1 = from->GetHeight() - 1;
	int fromWidthM1  = from->GetWidth() - 1;
	int fromStride   = from->GetStride();
	int fromPitch    = from->GetPitch();
	unsigned char* fromBuffer	= from->GetBuffer();

	// too parameters
	int tooHeightM1 = too->GetHeight()- 1;
	int tooWidthM1  = too->GetWidth() - 1;
	int tooStride   = too->GetStride();
	int tooPitch    = too->GetPitch();
	unsigned char* tooBuffer	= too->GetBuffer();

	// Allocate and create pixel buffer
	struct pixel {
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	};
	int bufExt = from->GetWidth() > from->GetHeight() ? from->GetWidth() : from->GetHeight();
	pixel* buffer = new pixel[bufExt];


	// ========== WIDTH ==========

	// sample parameter
	float sample = (float)from->GetWidth() / (float)too->GetWidth();

	// for each row of from
	for (int fromRow=0; fromRow<=fromHeightM1; fromRow++)
	{

		// copy from row into buffer
		for (int fromCol=0; fromCol<=fromWidthM1; fromCol++)
		{
			int offset = fromCol*fromStride + fromRow*fromPitch;
			buffer[fromCol].r = fromBuffer[offset];
			buffer[fromCol].g = fromBuffer[offset+1];
			buffer[fromCol].b = fromBuffer[offset+2];
			if (fromStride == 4)
				buffer[fromCol].a = fromBuffer[offset+3];
		}

		// for each col of too
		for (int tooCol=0; tooCol<=tooWidthM1; tooCol++)
		{

			// from sample bounds
			float fStart = sample * (float)tooCol;
			float fEnd   = fStart + sample;
			int   iStart = (int)fStart;
			int   iEnd   = (int)fEnd;

			// for each sample span in buffer
			float accum[4] = { 0, 0, 0, 0 };
			for (int col=iStart; col<=iEnd; col++)
			{
				float mu = col == iStart ? 1-(fStart-(float)iStart) :
					col == iEnd ? fEnd-(float)iEnd : 1.0f;
				accum[0] += mu * (float)buffer[col].r;
				accum[1] += mu * (float)buffer[col].g;
				accum[2] += mu * (float)buffer[col].b;
				if (fromStride == 4)
					accum[3] += mu * (float)buffer[col].a;
			}

			// average
			accum[0] /= sample;
			accum[1] /= sample;
			accum[2] /= sample;
			if (fromStride == 4)
				accum[3] /= sample;

			// update
			int offset = tooCol*fromStride + fromRow*fromPitch;
			fromBuffer[offset]   = (int)(accum[0]+0.5);
			fromBuffer[offset+1] = (int)(accum[1]+0.5);
			fromBuffer[offset+2] = (int)(accum[2]+0.5);
			if (tooStride == 4)
				fromBuffer[offset+3] = (int)(accum[3]+0.5);
		}
	}


	// ========== HEIGHT ==========

	// sample parameter
	sample = (float)from->GetHeight() / (float)too->GetHeight();

	// for each col of too
	for (int tooCol=0; tooCol<=tooWidthM1; tooCol++)
	{

		// copy from column into buffer
		for (int fromRow=0; fromRow<=fromHeightM1; fromRow++)
		{
			int offset = tooCol*fromStride + fromRow*fromPitch;
			buffer[fromRow].r = fromBuffer[offset];
			buffer[fromRow].g = fromBuffer[offset+1];
			buffer[fromRow].b = fromBuffer[offset+2];
			if (fromStride == 4)
				buffer[fromRow].a = fromBuffer[offset+3];
		}

		// for each row of too
		for (int tooRow=0; tooRow<=tooHeightM1; tooRow++)
		{

			// from sample bounds
			float fStart = sample * (float)tooRow;
			float fEnd   = fStart + sample;
			int   iStart = (int)fStart;
			int   iEnd   = (int)fEnd;

			// for each sample span in buffer
			float accum[4] = { 0, 0, 0, 0 };
			for (int col=iStart; col<=iEnd; col++)
			{
				float mu = col == iStart ? 1-(fStart-(float)iStart) :
					col == iEnd ? fEnd-(float)iEnd : 1.0f;
				accum[0] += mu * (float)buffer[col].r;
				accum[1] += mu * (float)buffer[col].g;
				accum[2] += mu * (float)buffer[col].b;
				if (fromStride == 4)
					accum[3] += mu * (float)buffer[col].a;
			}

			// average
			accum[0] /= sample;
			accum[1] /= sample;
			accum[2] /= sample;
			if (fromStride == 4)
				accum[3] /= sample;

			// update
			int offset = tooCol*tooStride + tooRow*tooPitch;
			tooBuffer[offset]   = (int)(accum[0]+0.5);
			tooBuffer[offset+1] = (int)(accum[1]+0.5);
			tooBuffer[offset+2] = (int)(accum[2]+0.5);
			if (tooStride == 4)
				tooBuffer[offset+3] = (int)(accum[3]+0.5);
		}
	}
	

	// ---- cleanup ----
	delete [] buffer;

    return VS_OK;
}


// ---------- ResizeDown  ----------
int ImgProcess::ResizeDown(
		ImgBuffer* too,
		ImgBuffer *from
	)
{
	// test
	if ( too->GetHeight() > from->GetHeight() ||
			too->GetWidth() > from->GetWidth() )
				return VS_ERROR;
	if (from == 0) return VS_ERROR;
	if (too == 0) return VS_ERROR;

	// from parameters
	int fromHeightM1 = from->GetHeight() - 1;
	int fromWidthM1  = from->GetWidth() - 1;
	int fromStride   = from->GetStride();
	int fromPitch    = from->GetPitch();
	unsigned char* fromBuffer = from->GetBuffer();

	// too parameters
	int tooHeightM1 = too->GetHeight() - 1;
	int tooWidthM1  = too->GetWidth() - 1;
	int tooStride   = too->GetStride();
	int tooPitch    = too->GetPitch();
	int tooBytes    = too->GetBytes();
	unsigned char* tooBuffer = too->GetBuffer();

	// sample parameters
	float sampleWidth = (float)from->GetWidth() / (float)too->GetWidth();
	float sampleHeight = (float)from->GetHeight() / (float)too->GetHeight();

	// temp buffer parameters
	int tempHeight = (int)(sampleHeight+0.5)+2;
	int tempWidth  = too->GetWidth();

	// Allocate and create temp image
	ImgBuffer* temp = new ImgBuffer();
	int ok = temp->Initialise(tempWidth, tempHeight, too->GetBytes());

	// temp parameters
	int tempHeightM1 = temp->GetHeight() - 1;
	int tempWidthM1  = temp->GetWidth() - 1;
	int tempStride   = temp->GetStride();
	int tempBytes    = temp->GetBytes();
	int tempPitch    = temp->GetPitch();
	unsigned char* tempBuffer = temp->GetBuffer();

	// ---- START RESIZE ----

	// for each row of "too"
	for (int tooRow=0; tooRow<=tooHeightM1; tooRow++)
	{

		// ---- WIDTH : RESIZE BLOCK "from" INTO BLOCK "temp" ----

		// set "from" row sample bounds
		float fromRowStart     = sampleHeight * (float)tooRow;
		float fromRowEnd       = fromRowStart + sampleHeight;
		float fromSampleHeight = sampleHeight;

		// catch illegal from height 
		if ((int)fromRowEnd > fromHeightM1)
		{
			float fromRowDif  = fromRowEnd - (float)fromHeightM1;
			fromRowEnd       -= fromRowDif;
			fromSampleHeight -= fromRowDif;
		}

		// set int "from" row sample bounds
		int iFromRowStart = (int)fromRowStart;
		int iFromRowEnd   = (int)fromRowEnd;

		// for each row within "from" sample bounds
		for (int fromRow=iFromRowStart; fromRow<=iFromRowEnd; fromRow++)
		{

			// accumulate into "temp" row
			int tempRow = fromRow - iFromRowStart;

			// for each column of "temp"
			for (int tempCol=0; tempCol<=tempWidthM1; tempCol++)
			{

				// set "from" column sample bounds
				float fStart  = sampleWidth * (float)tempCol;
				float fEnd    = fStart + sampleWidth;
				float fSample = sampleWidth;

				// catch fEnd overflow
				if ((int)fEnd > fromWidthM1)
				{
					float fDif = fEnd - (float)fromWidthM1;
					fEnd    -= fDif;
					fSample -= fDif;
				}

				// set int "from" row sample bounds
				int iStart = (int)fStart;
				int iEnd   = (int)fEnd;

				// accumulate column sample span
				float accum[4] = { 0, 0, 0, 0 };
				for (int col=iStart; col<=iEnd; col++)
				{
					// offset
					int offset = col*fromStride + fromRow*fromPitch;

					// coverage
					float coverage = col == iStart ? 1-(fStart-(float)iStart) :
						col == iEnd ? fEnd-(float)iEnd : 1.0f;

					// accumulate
					accum[0] += coverage * (float)fromBuffer[offset];
					accum[1] += coverage * (float)fromBuffer[offset+1];
					accum[2] += coverage * (float)fromBuffer[offset+2];
					if (tempBytes == 4)
						accum[3] += coverage * (float)fromBuffer[offset+3];
				}

				// store in "temp"
				int offset = tempCol*tempStride + tempRow*tempPitch;
				tempBuffer[offset]   = (int)(accum[0]/fSample+0.5);
				tempBuffer[offset+1] = (int)(accum[1]/fSample+0.5);
				tempBuffer[offset+2] = (int)(accum[2]/fSample+0.5);
				if (tempBytes == 4)
					tempBuffer[offset+3] = (int)(accum[3]/fSample+0.5);

			} // tempCol

		} // fromRow


		// ---- HEIGHT : RESIZE BLOCK "temp" INTO ROW OF "too" ----

		// for each column of "temp"
		for (int tempCol=0; tempCol<=tempWidthM1; tempCol++)
		{

			// for each row of "temp"
			float accum[4] = { 0, 0, 0, 0 };
			for (int fromRow=iFromRowStart; fromRow<=iFromRowEnd; fromRow++)
			{
				// index and offset
				int tempRow = fromRow - iFromRowStart;
				int offset = tempCol*tempStride + tempRow*tempPitch;

				// coverage
				float coverage = fromRow == iFromRowStart ? 1-(fromRowStart-(float)iFromRowStart) :
					fromRow == iFromRowEnd ? fromRowEnd-(float)iFromRowEnd : 1.0f;

				// accumulate
				accum[0] += coverage * (float)tempBuffer[offset];
				accum[1] += coverage * (float)tempBuffer[offset+1];
				accum[2] += coverage * (float)tempBuffer[offset+2];
				if (tempBytes == 4)
					accum[3] += coverage * (float)tempBuffer[offset+3];

				// store in "too"
				offset = tempCol*tooStride + tooRow*tooPitch;
				tooBuffer[offset]   = (int)(accum[0]/fromSampleHeight+0.5);
				tooBuffer[offset+1] = (int)(accum[1]/fromSampleHeight+0.5);
				tooBuffer[offset+2] = (int)(accum[2]/fromSampleHeight+0.5);
				if (tooBytes == 4)
					tooBuffer[offset+3] = (int)(accum[3]/fromSampleHeight+0.5);

			} // tooCol

		} // fromRow

	} // tooRow


	// cleanup
	delete [] temp->GetBuffer();
	delete temp;

    return VS_OK;
}


// ---------- Offset ----------
int ImgProcess::Filter(
		ImgBuffer *too,
		DOUBLE     mix,
		DOUBLE    *filter,
		UINT       filter_size,
		ImgBuffer *from
	)
{
	// test
	if ( from == 0 ) return VS_ERROR;
	if ( too == 0 ) return VS_ERROR;
	if ( too->GetHeight() != from->GetHeight() ||
		 too->GetWidth() != too->GetWidth() )
				return VS_ERROR;

	// From
	UINT sby2 = (UINT)(filter_size / 2);
	UINT height_msby2 = from->GetHeight() - sby2 - 1;
	UINT width_msby2 = from->GetWidth() - sby2 - 1;
	UINT stride = from->GetStride();
	UINT pitch = from->GetPitch();
	BYTE* from_buffer = from->GetBuffer();

	// Too
	BYTE* too_buffer = too->GetBuffer();

	
	// ---- Process ----
	UINT offset=0, f_offset = 0;
	BYTE *p, r, g, b;

	// FOR each pixel except margin
	for (UINT row = sby2; row <= height_msby2; row++)
	{
		for (UINT col = sby2; col <= width_msby2; col++)
		{
			offset = col*stride + row*pitch;
			DOUBLE linc_r = 0, linc_g = 0, linc_b = 0;

			for (UINT f_row = 0; f_row <= 2; f_row++)
			{
				for (UINT f_col = 0; f_col <= 2; f_col++)
				{
					f_offset = (col + f_col - 1)*stride + (row + f_row - 1)*pitch;
					p = from_buffer + f_offset;
					linc_r += *(p    ) * filter[f_row * 3 + f_col];
					linc_g += *(p + 1) * filter[f_row * 3 + f_col];
					linc_b += *(p + 2) * filter[f_row * 3 + f_col];
				}
			}
			r = (BYTE)round(linc_r > 255 ? 255 : (linc_r < 0 ? 0 : linc_r) );
			g = (BYTE)round(linc_g > 255 ? 255 : (linc_g < 0 ? 0 : linc_g) );
			b = (BYTE)round(linc_b > 255 ? 255 : (linc_b < 0 ? 0 : linc_b) );
			*(too_buffer + offset    ) = r;
			*(too_buffer + offset + 1) = g;
			*(too_buffer + offset + 2) = b;
		}
	}

	// done
	return VS_OK;
}


// ---------- Offset ----------
int ImgProcess::Offset(
		ImgBuffer* too,
		CHAR axis,
		UINT offset,
		ImgBuffer *from
	)
{
	// test
	if ( too->GetHeight() > from->GetHeight() ||
		too->GetWidth() > too->GetWidth() )
				return VS_ERROR;
	if (from == 0) return VS_ERROR;
	if (too == 0) return VS_ERROR;

	// from parameters
	int fromHeightM1 = from->GetHeight() - 1;
	int fromWidthM1  = from->GetWidth() - 1;
	int fromStride   = from->GetStride();
	int fromPitch    = from->GetPitch();
	unsigned char* fromBuffer = from->GetBuffer();

	// too GetHeight()
	int tooHeightM1 = too->GetHeight() - 1;
	int tooWidthM1  = too->GetWidth() - 1;
	int tooStride   = too->GetStride();
	int tooPitch    = too->GetPitch();
	int tooBytes    = too->GetBytes();
	unsigned char* tooBuffer = too->GetBuffer();

	// ---- START ROTATE ----

	// for each row of "too"
	for (int tooRow=0; tooRow<=tooHeightM1; tooRow++)
	{
		// for each col of "too"
		for (int tooCol=0; tooCol<=tooWidthM1; tooCol++)
		{
			int tooColOff = tooCol + offset;
			tooColOff = tooColOff < 0 ? tooWidthM1+tooColOff : ( tooColOff > tooWidthM1 ? tooColOff-tooWidthM1 : tooColOff);
			int tooOffset  = tooColOff*fromStride + tooRow*fromPitch;
			int fromOffset = tooCol*fromStride + tooRow*fromPitch;
			*(tooBuffer + tooOffset)   = *(fromBuffer + fromOffset);
			*(tooBuffer + tooOffset+1) = *(fromBuffer + fromOffset+1);
			*(tooBuffer + tooOffset+2) = *(fromBuffer + fromOffset+2);
			if (fromStride == 4)
				*(tooBuffer + tooOffset+3) = *(fromBuffer + fromOffset+3);
		}
	}

	// done
    return VS_OK;
}


// ---------- SwapRB ----------
int ImgProcess::SwapRB(ImgBuffer* image)
{
	unsigned char *ptr = image->GetBuffer();
	UINT bytes = image->GetBytes();
	UINT width = image->GetWidth();
	UINT height = image->GetHeight();
	UINT tmp = 0;
	for ( unsigned int y = 0; y<height; y++ )
	{
		for ( unsigned int x = 0; x<width; x++ )
		{
			tmp    = ptr[2];
			ptr[2] = ptr[0];
			ptr[0] = tmp;
			if (bytes == 4) ptr += 4; else ptr += 3;
		}
	}
	return VS_OK;
};


// ---------- Luminance ----------
/*!
\brief Luminance
\author Gareth Edwards
\param vs_system::ImgBuffer* - [in] image
\param FLOAT - [in] Gamma factor
\param FLOAT - [in] Luminance factor
\return INT - ERROR <=0 < VS_OK
*/
#include "stdio.h"

int ImgProcess::Luminance(
		ImgBuffer *image,
		FLOAT gamma,
		FLOAT luminance
	)
{
	// Validate.....

	// Unpack image parameters
	UINT width  = image->GetWidth();
	UINT height = image->GetHeight();
	UINT bytes  = image->GetBytes();
	unsigned char *ptr = image->GetBuffer();

	// Minimax
	FLOAT lumin_min = 255;
	FLOAT lumin_max = 0;
	for (UINT y = 0; y<height; y++)
	{
		for (UINT x = 0; x<width; x++)
		{
			BYTE r = ptr[0];
			BYTE g = ptr[1];
			BYTE b = ptr[2];
			FLOAT lumin = (FLOAT)(r + r + g + g + g + b)/6;
			lumin_min = lumin < lumin_min ? lumin : lumin_min;
			lumin_max = lumin > lumin_max ? lumin : lumin_max;
			if (bytes == 4) ptr += 4; else ptr += 3;
		}
	}

	// Scalar
	FLOAT lumin_scalar = 255.0f / (FLOAT)(lumin_max - lumin_min);

	// Process
	ptr = image->GetBuffer();
	for (UINT y = 0; y<height; y++)
	{
		for (UINT x = 0; x<width; x++)
		{
			FLOAT r = (FLOAT)pow(((FLOAT)(*(ptr    ) - lumin_min) * lumin_scalar) / 255, gamma) * 255 + 0.5f;
			FLOAT g = (FLOAT)pow(((FLOAT)(*(ptr + 1) - lumin_min) * lumin_scalar) / 255, gamma) * 255 + 0.5f;
			FLOAT b = (FLOAT)pow(((FLOAT)(*(ptr + 2) - lumin_min) * lumin_scalar) / 255, gamma) * 255 + 0.5f;
			*(ptr    ) = (INT)(r > 255 ? 255 : r < 0 ? 0 : r);
			*(ptr + 1) = (INT)(g > 255 ? 255 : g < 0 ? 0 : g);
			*(ptr + 2) = (INT)(b > 255 ? 255 : b < 0 ? 0 : b);
			if (bytes == 4) ptr += 4; else ptr += 3;
		}
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- PanoBars ----------
int ImgProcess::PanoBars(ImgBuffer *image, float min_radius, float max_radius)
{
	// RGB colour struct
	typedef struct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
	} RGB;

	// RGB colour bars LUT
	RGB BAR_COLOUR[8] =
	{
		{ 255, 255, 255 },  // 100% White
		{ 255, 255, 0 },    // Yellow
		{ 0, 255, 255 },    // Cyan
		{ 0, 255, 0 },      // Green
		{ 255, 0, 255 },    // Magenta
		{ 255, 0, 0 },      // Red
		{ 0, 0, 255 },      // Blue
		{ 64, 64, 64 },     // Black
	};

	// centre
	UINT  width  = image->GetHeight();
	UINT  height = image->GetHeight();
	UINT  bytes  = image->GetBytes();
	float centre_x = (float)width/2;
	float centre_y = (float)height/2;
	float rad2deg  = 180.0f / 3.14159265f;
	float pixel_min = min_radius * centre_y;
	float pixel_max = max_radius * centre_y;

	// FOR each row
	unsigned char *ptr = image->GetBuffer();
	for ( unsigned int y=0; y<height; y++ )
	{
		float diff_y = (float)y - centre_y;
		// FOR each col
		for ( unsigned int x=0; x<width; x++ )
		{
			float diff_x = (float)x - centre_x;
			float angle = (FLOAT)atan2(diff_y, diff_x) * rad2deg + 180.0f;
			unsigned int idx = (unsigned int)(fabs(angle)/360.0f * 8.0f);
			float pixel_dist = (FLOAT)sqrt(diff_x*diff_x + diff_y*diff_y);
			if ( pixel_dist > pixel_max || pixel_dist < pixel_min )
			{
				ptr[0] = 0;
				ptr[1] = 0;
				ptr[2] = 0;
			}
			else
			{
				ptr[0] = BAR_COLOUR[idx].r;
				ptr[1] = BAR_COLOUR[idx].g;
				ptr[2] = BAR_COLOUR[idx].b;
			}
			if ( bytes == 4 ) { ptr[3] = 0; ptr+=4; }
			else ptr+=3;
		}
	}

	// Ok
	return VS_OK;
}


// ---------- GetPanoDimensions ----------
/*!
\brief return width and height proportional to source donut image
\author Gareth Edwards
\param INT   - [in] height of annular image
\param FLOAT - [in] radial interpolant (0.0 <= value <= 1.0)
\param INT*  - [out] width of TARGET panorama image
\param INT*  - [out] height of TARGET panorama image
\return INT - INT - ERROR <=0 < VS_OK

Radial interpolant, where when:
0.0, height == min radius of annular image
1.0, height == max radius of annular image
0.5, height == mid radius "", and the best compromise

*/
int ImgProcess::GetPanoDimensions(
		INT   annular_height,
		FLOAT mu,
		FLOAT min_angle, 
		FLOAT max_angle,
		FLOAT min_radius,
		FLOAT max_radius,
		INT*  width,
		INT*  height
	)
{
	// Calculate aspect
	FLOAT aspect = (max_angle - min_angle) / 360.0f;

	// Calculate radii
	FLOAT radius_max = (FLOAT)annular_height / 2 * max_radius;
	FLOAT radius_min = (FLOAT)annular_height / 2 * min_radius;

	// Calculate circumfernce minimax
	FLOAT cMax = 2 * 3.14159265f * (FLOAT)radius_max;
	FLOAT cMin = 2 * 3.14159265f * (FLOAT)radius_min;

	// Calculate dimensions
	FLOAT mOk = mu < 0 ? 0 : mu > 1 ? 1 : mu;
	FLOAT w = (1 - mOk) * cMin + mOk * cMax;
	*width = (INT)(w + 0.5f);
	*height = (INT)(aspect * w + 0.5f);

	// Ok
	return VS_OK;
}


// ---------- GetPanoHeight ----------
/*!
\brief return panorama height proportional to width
\author Gareth Edwards
\param INT - [in] width of panorama image
\return INT - height
*/
int ImgProcess::GetPanoHeight(
		INT   width,
		FLOAT min_angle,
		FLOAT max_angle
	)
{
	FLOAT height = (max_angle - min_angle) / 360 * (FLOAT)width;
	return (INT)(height + 0.5f);
}


// ---------- PanoFromAnnular ----------
/*!
\brief transform annular to panorama
\author Gareth Edwards
\param vs_system::ImgBuffer* - [in] target panorama image
\param UINT - [in] super-sample (1 <= value <= more than 4 seems pointless!)
\param vs_system::ImgBuffer* - [in] source annular image
\return INT - ERROR <=0 < VS_OK
*/
int ImgProcess::PanoFromAnnular(
		ImgBuffer *panorama_image,
		FLOAT u_offset,
		FLOAT v_offset,
		FLOAT min_radius, 
		FLOAT max_radius,
		UINT  sample,
		ImgBuffer *annular_image
	)
{
	// Validate annular image
	if ( pi_img_process->Validate(annular_image) != VS_OK)
		return VS_ERROR;
	// Validate annular image
	if ( pi_img_process->Validate(panorama_image) != VS_OK )
		return VS_ERROR;

	// Annular
	int   annular_HeightM1 = annular_image->GetHeight() - 1;
	int   annular_WidthM1  = annular_image->GetWidth() - 1;
	float annular_Height   = (float)annular_image->GetHeight();
	float annular_Width    = (float)annular_image->GetWidth();
	float annular_Radius   = (float)annular_image->GetHeight() / 2;
	int   annular_Stride   = annular_image->GetStride();
	int   annular_Pitch    = annular_image->GetPitch();
	unsigned char* annular_Buffer = annular_image->GetBuffer();

	// Panorama
	int   panorama_HeightM1 = panorama_image->GetHeight() - 1;
	int   panorama_WidthM1  = panorama_image->GetWidth() - 1;
	int   panorama_Stride   = panorama_image->GetStride();
	int   panorama_Pitch    = panorama_image->GetPitch();
	unsigned char* panorama_Buffer = panorama_image->GetBuffer();

	// Image mapping parameters
	float uCen = (0.5f - u_offset) * annular_Width;
	float vCen = (0.5f + v_offset) * annular_Height;
	float rMin = min_radius * annular_Radius;
	float rMax = max_radius  * annular_Radius;

	// IF single sample THEN
	if ( sample == 1 )
	{

		// For each row of rect
		for ( int rectRow = 0; rectRow <= panorama_HeightM1; rectRow++ )
		{

			// Radius
			float rmu = (float)rectRow / (float)panorama_HeightM1;
			float radius = (1 - rmu)*rMin + rmu*rMax;

			// for each column of rect
			for ( int rectCol = 0; rectCol <= panorama_WidthM1; rectCol++ )
			{

				// Angle around
				float angle = ( ( (float)rectCol / (float)panorama_WidthM1 * 360 ) + 180 ) * (float)VS_RADIANS_PER_DEGREE;

				// Get from ordinates
				int annular_Col = (int)(sin(angle) * radius + uCen + (float)0.5);
				int annular_Row = (int)(cos(angle) * radius + vCen + (float)0.5);

				// Minimax
				annular_Col = annular_Col < 0 ? 0 : annular_Col >= annular_WidthM1 ? annular_WidthM1 : annular_Col;
				annular_Row = annular_Row < 0 ? 0 : annular_Row >= annular_HeightM1 ? annular_HeightM1 : annular_Row;

				// Store
				int d = (annular_WidthM1 - annular_Col) * annular_Stride + annular_Row * annular_Pitch;
				int r = (panorama_WidthM1 - rectCol) * panorama_Stride + (panorama_HeightM1 - rectRow) * panorama_Pitch;
				panorama_Buffer[r + 0] = annular_Buffer[d + 0];
				panorama_Buffer[r + 1] = annular_Buffer[d + 1];
				panorama_Buffer[r + 2] = annular_Buffer[d + 2];
				if ( panorama_Stride == 4 )
				if ( annular_Stride == 4 )
					panorama_Buffer[r + 3] = annular_Buffer[d + 3];
				else
					panorama_Buffer[r + 3] = (char)255;
			}
		}
	}

	// ELSE multi sampling
	else
	{

		// Local sample buffer
		int bufferWidth  = panorama_image->GetWidth() * sample;
		int bufferHeight = sample;
		int bufferExtent = bufferHeight * bufferWidth * panorama_image->GetBytes();
		unsigned char* buffer = new unsigned char[bufferExtent];

		// Actual sample buffer bounds
		int bufferWidthM1 = bufferWidth - 1;
		int bufferHeightM1 = (panorama_image->GetHeight() * sample) - 1;

		// Weighted denominator
		float weight = (float)(sample*sample);

		// For each row of sample buffer
		for ( int panorama_Row = 0; panorama_Row <= bufferHeightM1; panorama_Row++ )
		{

			// Radius
			float rmu = (float)panorama_Row / (float)bufferHeightM1;
			float radius = (1 - rmu) * rMin + rmu * rMax;

			// For each column of sample buffer
			for ( int panorama_Col = 0; panorama_Col <= bufferWidthM1; panorama_Col++ )
			{

				// Angle around
				float angle = (((float)panorama_Col / (float)bufferWidthM1 * 360) + 180) * (float)VS_RADIANS_PER_DEGREE;
				//float angle = ( ( (float)panorama_Col / (float)bufferWidthM1 * 360 ) * (float)VS_RADIANS_PER_DEGREE;

				// Donut source coordinates
				int annular_Col = (int)(sin(angle) * radius + uCen + (float)0.5);
				int annular_Row = (int)(cos(angle) * radius + vCen + (float)0.5);

				// Minimax
				annular_Col = annular_Col < 0 ? 0 : annular_Col >= annular_WidthM1 ? annular_WidthM1 : annular_Col;
				annular_Row = annular_Row < 0 ? 0 : annular_Row >= annular_HeightM1 ? annular_HeightM1 : annular_Row;

				// Store in sample buffer
				int dIndex = (annular_WidthM1 - annular_Col) * annular_Stride + annular_Row * annular_Pitch;
				int rIndex = (panorama_Row % bufferHeight) * bufferWidth;
				int bIndex = (panorama_Col + rIndex) * panorama_Stride;
				buffer[bIndex + 0] = annular_Buffer[dIndex + 0];
				buffer[bIndex + 1] = annular_Buffer[dIndex + 1];
				buffer[bIndex + 2] = annular_Buffer[dIndex + 2];
				if ( panorama_Stride == 4 )
				if ( annular_Stride == 4 )
					buffer[bIndex + 3] = annular_Buffer[dIndex + 3];
			}

			// IF sample buffer filled THEN average
			if ( panorama_Row % sample == sample - 1 )
			{
				if (sample == 3)
				{
					FLOAT filter[3][3] =
					{
						{ 0.0625, 0.1250, 0.0625 },
						{ 0.1250, 0.2500, 0.1250 },
						{ 0.0625, 0.1250, 0.0625 },
					};

					// For each column of rect
					for (int rectCol = 0; rectCol <= panorama_WidthM1; rectCol++)
					{
						// Accumulate
						FLOAT r = 0, g = 0, b = 0, a = 0;
						for (UINT y = 0; y < sample; y++)
						{
							int yOffset = y*bufferWidth;
							for (UINT x = 0; x < sample; x++)
							{
								int xOffset = rectCol*sample + x;
								int bOffset = (yOffset + xOffset) * panorama_Stride;
								r += (FLOAT)buffer[bOffset + 0] * filter[y][x];
								g += (FLOAT)buffer[bOffset + 1] * filter[y][x];
								b += (FLOAT)buffer[bOffset + 2] * filter[y][x];
								if (panorama_Stride == 4)
									a += (FLOAT)buffer[bOffset + 3] * filter[y][x];
							}
						}


						// Store
						int ri = (panorama_WidthM1 - rectCol) * panorama_Stride + (panorama_HeightM1 - panorama_Row / sample) * panorama_Pitch;
						panorama_Buffer[ri + 0] = (BYTE)(r > 255 ? 255 : r);
						panorama_Buffer[ri + 1] = (BYTE)(g > 255 ? 255 : g);
						panorama_Buffer[ri + 2] = (BYTE)(b > 255 ? 255 : b);
						if (panorama_Stride == 4)
							panorama_Buffer[ri + 3] = (BYTE)(a > 255 ? 255 : a);
					}
				}
				else
				{
					// For each column of rect
					for (int rectCol = 0; rectCol <= panorama_WidthM1; rectCol++)
					{
						// Accumulate
						int r = 0, g = 0, b = 0, a = 0;
						for (UINT y = 0; y < sample; y++)
						{
							int yOffset = y*bufferWidth;
							for (UINT x = 0; x < sample; x++)
							{
								int xOffset = rectCol*sample + x;
								int bOffset = (yOffset + xOffset) * panorama_Stride;
								r += buffer[bOffset + 0];
								g += buffer[bOffset + 1];
								b += buffer[bOffset + 2];
								if (panorama_Stride == 4)
									a += buffer[bOffset + 3];
							}
						}

						// Store
						int ri = (panorama_WidthM1 - rectCol) * panorama_Stride + (panorama_HeightM1 - panorama_Row / sample) * panorama_Pitch;
						panorama_Buffer[ri + 0] = (char)((float)r / weight + 0.5);
						panorama_Buffer[ri + 1] = (char)((float)g / weight + 0.5);
						panorama_Buffer[ri + 2] = (char)((float)b / weight + 0.5);
						if (panorama_Stride == 4)
							panorama_Buffer[ri + 3] = (char)((float)a / weight + 0.5);
					}
				}
			}
		}

		// free
		delete[] buffer;

	}

	// Done
	return VS_OK;
}


// ---------- PanoLuminance ----------
/*!
\brief Luminance
\author Gareth Edwards
\param vs_system::ImgBuffer* - [in] target annular image
\param INT - [in] minimum radius
\param FLOAT - [in] Accumalation factor
\param INT - [in] maximum radius
\return INT - ERROR <=0 < VS_OK
*/
int ImgProcess::PanoLuminance(
	ImgBuffer *panorama_dest_image,
	FLOAT u_offset,
	FLOAT v_offset,
	FLOAT gamma,
	FLOAT accumulation_factor,
	FLOAT radius_min,
	FLOAT radius_max,
	ImgBuffer *panorama_src_image
	)
{
	// Validate.....

	// Unpack image parameters
	int width   = panorama_src_image->GetWidth();
	int height  = panorama_src_image->GetHeight();

	// Unpack radius parameters
	int rmax = (INT)(radius_max * (FLOAT)height/2 + 0.5);
	int rmin = (INT)(radius_min * (FLOAT)height/2 + 0.5);

	// Destination image parameters
	BYTE *dest    = panorama_dest_image->GetBuffer();
	int   dest_stride = panorama_dest_image->GetStride();
	int   line_dest_stride = width * dest_stride;

	// Source image parameters
	BYTE *src     = panorama_src_image->GetBuffer();
	int   src_stride = panorama_src_image->GetStride();
	int   line_src_stride = width * src_stride;

	// Pano parameters
	int pano_x_offset = width/2;
	int pano_y_offset = height/2  + (INT)( v_offset*(FLOAT)(height) + 0.5);

	// Create and initialise scanline LUT's
	BYTE *scanline_flag = new BYTE[height];
	int  *scanline_lut  = new int[height*4];
	BYTE *gs_lut        = new BYTE[256];
	memset(scanline_flag, 0, height);
	memset(scanline_lut,  0, sizeof(int)*height);
	memset(gs_lut,        0, 256);

	// Initialise grey scale look up table
	for ( UINT i=0; i<256; i++ )
		gs_lut[i] = (BYTE)round( 255.0 * pow( (FLOAT)i/255.0, 1.0/gamma ));


	// ***************************** MIDDLE *****************************

	int scanline_index = pano_y_offset;
	scanline_flag[scanline_index] = 1;

	int x0 = -rmax + pano_x_offset;
	int x1 = -rmin + pano_x_offset;
	int x2 =  rmin + pano_x_offset;
	int x3 =  rmax + pano_x_offset;

	int scanline_offset = scanline_index * 4;
	*(scanline_lut + scanline_offset + 0) = x0;
	*(scanline_lut + scanline_offset + 1) = x1;
	*(scanline_lut + scanline_offset + 2) = x2;
	*(scanline_lut + scanline_offset + 3) = x3;


	// ***************************** MAX *****************************
	
	// Radius maximum parameters
	int x_rmax = 0;
	int y_rmax = rmax;
	int d_rmax = 1-rmax;
	int deltaE_rmax  = 3;
	int deltaSE_rmax = -2*rmax + 5;

	// For each line in first radius maximum quadrant
	while ( y_rmax > x_rmax )
	{
		if ( d_rmax < 0 ) /* select E */
		{
			d_rmax += deltaE_rmax;
			deltaE_rmax += 2;
			deltaSE_rmax +=2;
		}
		else /* select SE */
		{
			d_rmax += deltaSE_rmax;
			deltaE_rmax += 2;
			deltaSE_rmax += 4;
			y_rmax--;
		}
		x_rmax++;

		// 4 lines
		int xy[4][4] = 
		{
			{ -x_rmax,  y_rmax,  x_rmax,  y_rmax }, // -x,  y to x,  y
			{ -x_rmax, -y_rmax,  x_rmax, -y_rmax }, // -x, -y to x, -y
			{ -y_rmax,  x_rmax,  y_rmax,  x_rmax }, // -y,  x to y,  x
			{ -y_rmax, -x_rmax,  y_rmax, -x_rmax }  // -y, -x to y, -x
		};

		// For each line
		for ( int line=0; line<4; line++ )
		{
			int scanline_index = xy[line][1] + pano_y_offset;
			if ( scanline_index >= 0 && scanline_index < height )
			{
				scanline_flag[scanline_index] = 1;
			
				int xs = xy[line][0] + pano_x_offset;
				int xe = xy[line][2] + pano_x_offset;

				int scanline_offset = scanline_index * 4;
				*(scanline_lut + scanline_offset + 0) = xs;
				*(scanline_lut + scanline_offset + 1) = pano_x_offset;
				*(scanline_lut + scanline_offset + 2) = pano_x_offset;
				*(scanline_lut + scanline_offset + 3) = xe;
			}
		}
	}


	// ***************************** MIN *****************************

	// Radius minumum parameters
	int x_rmin = 0;
	int y_rmin = rmin;
	int d_rmin = 1-rmin;
	int deltaE_rmin  = 3;
	int deltaSE_rmin = -2*rmin + 5;

	// For each line in first radius maximum quadrant
	while ( y_rmin > x_rmin )
	{
		if ( d_rmin < 0 ) /* select E */
		{
			d_rmin += deltaE_rmin;
			deltaE_rmin += 2;
			deltaSE_rmin +=2;
		}
		else /* select SE */
		{
			d_rmin += deltaSE_rmin;
			deltaE_rmin += 2;
			deltaSE_rmin += 4;
			y_rmin--;
		}
		x_rmin++;

		// 4 lines
		int xy[4][4] =
		{
			{ -x_rmin,  y_rmin,  x_rmin,  y_rmin }, // -x,  y to x,  y
			{ -x_rmin, -y_rmin,  x_rmin, -y_rmin }, // -x, -y to x, -y
			{ -y_rmin,  x_rmin,  y_rmin,  x_rmin }, // -y,  x to y,  x
			{ -y_rmin, -x_rmin,  y_rmin, -x_rmin }  // -y, -x to y, -x
		};

		// For each line
		for ( int line=0; line<4; line++ )
		{
			int scanline_index = xy[line][1] + pano_y_offset;
			if ( scanline_index >= 0 && scanline_index < height)
			{
				scanline_flag[scanline_index] = 1;

				int xs = xy[line][0] + pano_x_offset;
				int xe = xy[line][2] + pano_x_offset;

				int scanline_offset = scanline_index * 4;
				*(scanline_lut + scanline_offset + 1) = xs;
				*(scanline_lut + scanline_offset + 2) = xe;
			}
		}
	}


	// ***************************** SCAN *****************************

	// For each scanline
	for ( int scanline=0; scanline<height; scanline++ )
	{
		if ( scanline_flag[scanline] )
		{
			int scanline_offset = scanline * 4;

			int xlut[2][2] = 
			{
				{
					*(scanline_lut + scanline_offset + 0) + pano_x_offset,
					*(scanline_lut + scanline_offset + 1) + pano_x_offset
				},
				{
					*(scanline_lut + scanline_offset + 2) + pano_x_offset,
					*(scanline_lut + scanline_offset + 3) + pano_x_offset
				}
			};

			int line_src_offset = line_src_stride * scanline;
			int line_dest_offset = line_dest_stride * scanline;

			for ( int line=0; line<2; line++)
			{
				int xs = xlut[line][0];
				int xe = xlut[line][1];
				for ( int x=xs; x<=xe; x++ )
				{
					int byte_src_offset = line_src_offset + (x - pano_x_offset) * src_stride;
					BYTE *line_src_offset = src + byte_src_offset;

					int byte_dest_offset = line_dest_offset + (x - pano_x_offset) * dest_stride;
					BYTE *line_dest_offset = dest + byte_dest_offset;

					int red   = (INT)((FLOAT)*(line_dest_offset+0) * accumulation_factor + (FLOAT)*(line_src_offset+0) + 0.5);
					int green = (INT)((FLOAT)*(line_dest_offset+1) * accumulation_factor + (FLOAT)*(line_src_offset+1) + 0.5);
					int blue  = (INT)((FLOAT)*(line_dest_offset+2) * accumulation_factor + (FLOAT)*(line_src_offset+2) + 0.5);

					*(line_dest_offset+0) = gs_lut[(BYTE)(red   > 255 ? 255 : red)];
					*(line_dest_offset+1) = gs_lut[(BYTE)(green > 255 ? 255 : green)];
					*(line_dest_offset+2) = gs_lut[(BYTE)(blue  > 255 ? 255 : blue)];

					line_dest_offset += dest_stride;
					line_src_offset += src_stride;
				}
			}
		}
	}


	// ***************************** CLEANUP *****************************

	if ( scanline_flag != NULL ) delete[] scanline_flag;
	if ( scanline_lut  != NULL ) delete[] scanline_lut;
	if ( gs_lut        != NULL ) delete[] gs_lut;

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

