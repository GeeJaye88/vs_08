////////////////////////////////////////////////////////////////////////////////

// ---------- r2_img_process.h ----------
/*!
\file r2_img_process.h
\brief
\author Gareth Edwards
*/


#pragma once

// ---------- include ----------
#include <windows.h>


// ---------- application include ----------
#include "../header/vs_img_lut.h"


// ---- namespace ----
using namespace vs_system;


//#define LUT_THREADED
#define LUT_MULTITHREAD


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of PI_ImgLutProcess_Runtime_Data struct ----------

struct PI_ImgLutProcess_Runtime_Data
{
	INT  handle_this = NULL;
	UINT lut_mode = ImgLutProcess::COLOUR;
} runtime_data;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ArchTime class ----------

class ImgLutProcess::PI_ImgLutProcess
{

public:

	static const UINT _num_boost = 3;

	// ---- c/d tor ----
	PI_ImgLutProcess()
	{

		FLOAT gamma[]  = { 1.20f, 1.00f, 0.70f };
		FLOAT scalar[] = { 1.00f, 1.50f, 2.25f };
		FLOAT offset[] = { 0.00f, 0.10f, 0.25f };

		for (UINT i = 0; i < _num_boost; i++)
		{
			_gamma[i]  = gamma[i];
			_scalar[i] = scalar[i];
			_offset[i] = offset[i];
		}

		for (UINT i = 0; i < _num_boost; i++)
		{
			_init_colour[i] = FALSE;
			_init_mono[i]   = FALSE;
			_init_green[i]  = FALSE;
			_init_sepia[i]  = FALSE;
		}

	};
	~PI_ImgLutProcess()
	{
		;
	};

	
	// ---- runtime ----
	INT Runtime();
	static DWORD WINAPI RuntimeThread(LPVOID);

	// ---- initialise ----
	INT InitColour();
	INT InitMono();
	INT InitTint(UINT);

	// ---- process ----

	INT Colour();
	INT Mono();
	INT Green();
	INT Sepia();


	// ---- state ----
	BOOL _runtime = TRUE;
	BOOL _locked  = FALSE;

	// ---- "current" buffer ----
	ImgLutProcessBuffer *_ippb = NULL;


	// ---- pre-initialised boost process values----
	FLOAT _gamma[_num_boost];
	FLOAT _scalar[_num_boost];
	FLOAT _offset[_num_boost];


	// ---- pre-initialised process flags----
	BOOL  _spp_ready[4];


	// ---- NULL pre-initialised boost initialisation flags ----
	BOOL _init_colour[_num_boost];
	BOOL _init_mono[_num_boost];
	BOOL _init_green[_num_boost];
	BOOL _init_sepia[_num_boost];


	// ---- un-initialised boost luts ----
	BYTE _colour[_num_boost][256];
	BYTE _mono[_num_boost][256][256][256];
	BYTE _green[_num_boost][256][256][256][3];
	BYTE _sepia[_num_boost][256][256][256][3];

};


// ---------- Runtime ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/

INT ImgLutProcess::PI_ImgLutProcess::Runtime()
{

	if ( _locked == TRUE ) return VS_ERROR;


	_locked = TRUE;


	// ---- COLOUR ----
	static PI_ImgLutProcess_Runtime_Data runtime_data_colour;
	runtime_data_colour.handle_this = (INT)this;
	runtime_data_colour.lut_mode = ImgLutProcess::COLOUR;
	HANDLE thread_handle_colour =
			CreateThread(NULL,
			0,
			RuntimeThread,
			(LPVOID)&runtime_data_colour,
			0,
			0
		);


	#ifdef LUT_MULTITHREAD

	// ---- MONO ----
	static PI_ImgLutProcess_Runtime_Data runtime_data_mono;
	runtime_data_mono.handle_this = (INT)this;
	runtime_data_mono.lut_mode = ImgLutProcess::MONO;
	HANDLE thread_handle_mono =
			CreateThread(NULL,
			0,
			RuntimeThread,
			(LPVOID)&runtime_data_mono,
			0,
			0
		);

	// ---- SEPIA ----
	static PI_ImgLutProcess_Runtime_Data runtime_data_sepia;
	runtime_data_sepia.handle_this = (INT)this;
	runtime_data_sepia.lut_mode = ImgLutProcess::SEPIA;
	HANDLE thread_handle_sepia =
			CreateThread(NULL,
			0,
			RuntimeThread,
			(LPVOID)&runtime_data_sepia,
			0,
			0
		);


	#endif


	return VS_OK;
}


// ---------- RuntimeThread ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
DWORD WINAPI ImgLutProcess::PI_ImgLutProcess::RuntimeThread(
	LPVOID lpParam
	)
{

	// ---- cast arg to app type ----
	PI_ImgLutProcess_Runtime_Data *runtime_data =
		(PI_ImgLutProcess_Runtime_Data *)lpParam;


	// ---- extract runtime data ----
	ImgLutProcess::PI_ImgLutProcess *pi =
		(ImgLutProcess::PI_ImgLutProcess *)runtime_data->handle_this;
	UINT lut_mode = runtime_data->lut_mode;



	#ifndef LUT_MULTITHREAD
	// ---- for each lut ----
	for (lut_mode = 0; lut_mode < 3; lut_mode++)
	#endif

	{

		// ---- init ----
		INT result = VS_OK;
		switch ( lut_mode )
		{
			case vs_system::ImgLutProcess::COLOUR:
				{
					for (INT i = 0; i < 3; i++)
					{
						pi->_ippb->boost_mode = i;
						result = pi->InitColour();
					}
				}
				break;

			case vs_system::ImgLutProcess::MONO:
				{
					for (INT i = 0; i < 3; i++)
					{
						pi->_ippb->boost_mode = i;
						result = pi->InitMono();
					}
				}
				break;

			case vs_system::ImgLutProcess::SEPIA:
				{
					for (INT i = 0; i < 3; i++)
					{
						pi->_ippb->boost_mode = i;
						result = pi->InitTint(ImgLutProcess::SEPIA);
					}
				}
				break;

			case vs_system::ImgLutProcess::GREEN:
				break;

			default:
				break;
		}

		printf("      COMPLETED LUT # %d of %d\n", lut_mode, pi->_ippb->boost_mode);

	}

	// ---- job done ----
	#ifdef LUT_MULTITHREAD
	
	if ( lut_mode == vs_system::ImgLutProcess::SEPIA)
	{
		pi->_ippb->boost_mode = 0;
		pi->_locked = FALSE;
		pi->_runtime = FALSE;
	}

	#else
	
	pi->_ippb->boost_mode = 0;
	pi->_locked = FALSE;
	pi->_runtime = FALSE;
	
	#endif


	return VS_OK;
}

// ---------- InitColour ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::InitColour()
{

	// ---- return or set init flag ----
	if ( _init_colour[_ippb->boost_mode] ) return VS_OK;
	_init_colour[_ippb->boost_mode] = TRUE;


	// ---- local ----
	INT v = 0;


	// ---- init lut ----
	UINT  i  = _ippb->boost_mode;
	FLOAT sc = _scalar[i];
	FLOAT mu = _offset[i] * 255 + 0.5f;
	{
		for (UINT j = 0; j < 256; j++)
		{
			v = (INT)( (FLOAT)j * sc + mu );
			_colour[i][j] = v > 255 ? 255 : v;
		}
	}

	return VS_OK;
}


// ---------- InitMono ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::InitMono()
{

	// ---- return or set init flag ----
	if (_init_mono[_ippb->boost_mode]) return VS_OK;
	_init_mono[_ippb->boost_mode] = TRUE;


	// ---- init lut ----
	INT bm = _ippb->boost_mode;
	FLOAT mu = _offset[bm] * 255;

	if ( bm == 0 )
	{

		FLOAT po[256];
		for (UINT p = 0; p < 256; p++)
		{
			po[p] = pow((FLOAT)p * _scalar[0] / 255, _gamma[bm]);
		}

		for (UINT r = 0; r < 256; r++)
		{
			for (UINT g = 0; g < 256; g++)
			{
				for (UINT b = 0; b < 256; b++)
				{
					// ---- luminance approximation <= 255 !!! ----
					FLOAT l = (FLOAT)(r + r + g + g + g + b) / 6;

					INT v = (INT)(po[(INT)(l)] * 255 + mu + 0.5f);
					v = v > 255 ? 255 : v;
					_mono[bm][r][g][b] = v;
				}
			}
		}
	}
	else
	{
		for (UINT r = 0; r < 256; r++)
		{
			for (UINT g = 0; g < 256; g++)
			{
				for (UINT b = 0; b < 256; b++)
				{
					// ---- luminance approximation <= 255 !!! ----
					FLOAT l = (FLOAT)(r + r + g + g + g + b) / 6;

					INT v = (INT)(l * _scalar[bm] + mu + 0.5f);
					v = v > 255 ? 255 : v;
					_mono[bm][r][g][b] = v;
				}
			}
		}
	}

	return VS_OK;
}


// ---------- InitTint ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

\note

Old power code was:

INT r/g/b = (INT)(
		pow( ( fg0 + (fb * m[0][2]) ) * sc / 255, ga ) * 255 + mu
	);
INT vg = (INT)(
		pow( ( fg1 + (fb * m[1][2]) ) * sc / 255, ga ) * 255 + mu
	);
INT vb = (INT)(
		pow( ( fg2 + (fb * m[2][2]) ) * sc / 255, ga ) * 255 + mu
	);

BYTE *sepia = &_sepia[0][r][g][b][0];
*(sepia    ) = (BYTE)(vr > 255 ? 255 : vr);
*(sepia + 1) = (BYTE)(vg > 255 ? 255 : vg);
*(sepia + 2) = (BYTE)(vb > 255 ? 255 : vb);

*/
INT ImgLutProcess::PI_ImgLutProcess::InitTint(
		UINT tint
	)
{

	// ---- return or set init flag ----
	if ( _init_sepia[_ippb->boost_mode] ) return VS_OK;
	_init_sepia[_ippb->boost_mode] = TRUE;


	// ---- standard sepia matrix ----
	float m[3][3] =
	{
		0.393f, 0.769f, 0.189f,
		0.349f, 0.686f, 0.168f,
		0.272f, 0.534f, 0.131f
	};


	// ---- crush all - it's too bright! ----
	for (UINT i = 0; i < 9; i++)
	{
		*(m[0] + i) *= 0.85f;
	}


	// ---- init stuff ----
	FLOAT fr = 0, fg = 0, fb = 0;
	UINT  bm = _ippb->boost_mode;
	FLOAT mu = (FLOAT)_offset[bm] * 255.0f + 0.5f;


	// ---- init lut ----

	FLOAT sc = 1;
	FLOAT po[256];
	for (UINT p = 0; p < 256; p++)
	{
		po[p] = pow((FLOAT)p * _scalar[0] / 255, _gamma[bm]);
	}

	for (UINT r = 0; r < 256; r++)
	{
		FLOAT fr0 = (FLOAT)r * m[0][0];
		FLOAT fr1 = (FLOAT)r * m[1][0];
		FLOAT fr2 = (FLOAT)r * m[2][0];

		for (UINT g = 0; g < 256; g++)
		{
			FLOAT fg0 = (FLOAT)g * m[0][1] + fr0;
			FLOAT fg1 = (FLOAT)g * m[1][1] + fr1;
			FLOAT fg2 = (FLOAT)g * m[2][1] + fr2;

			for (UINT b = 0; b < 256; b++)
			{
				fb = (FLOAT)b;

				FLOAT ar = fg0 + (fb * m[0][2]);
				FLOAT ag = fg1 + (fb * m[1][2]);
				FLOAT ab = fg2 + (fb * m[2][2]);

				if (bm == 0)
				{
					ar = po[(INT)(ar > 255 ? 255 : ar)];
					ag = po[(INT)(ag > 255 ? 255 : ag)];
					ab = po[(INT)(ab > 255 ? 255 : ab)];
					sc = 255;
				}
				else
				{
					sc = _scalar[bm];
				}

				INT vr = (INT)(ar * sc + mu);
				INT vg = (INT)(ag * sc + mu);
				INT vb = (INT)(ab * sc + mu);

				BYTE *sepia = &_sepia[bm][r][g][b][0];
				*(sepia    ) = (BYTE)(vr > 255 ? 255 : vr);
				*(sepia + 1) = (BYTE)(vg > 255 ? 255 : vg);
				*(sepia + 2) = (BYTE)(vb > 255 ? 255 : vb);

				vr = (INT)(ag * sc * 0.6f + mu);
				vg = (INT)(ar * sc        + mu);
				vb = (INT)(ab * sc * 0.6f + mu);

				BYTE *green = &_green[bm][r][g][b][0];
				*(green    ) = (BYTE)(vr > 255 ? 255 : vr);
				*(green + 1) = (BYTE)(vg > 255 ? 255 : vg);
				*(green + 2) = (BYTE)(vb > 255 ? 255 : vb);
			}
		}
	}

	return VS_OK;
}



// ---------- Colour ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::Colour()
{

	// ---- init ----
	INT result = InitColour();


	// ---- shortcut ----
	DWORD  boost_mode = _ippb->boost_mode;
	DWORD  w          = _ippb->width;
	DWORD  h          = _ippb->height;
	DWORD  src_bpp    = _ippb->src_bpp;
	DWORD  dest_bpp   = _ippb->dest_bpp;
	BYTE *src_data    = _ippb->src_data;
	BYTE *dest_data   = _ippb->dest_data;


	// ---- process source into destination ----
	UINT total = h * w;
	if ( boost_mode == 0 )
	{
		for (UINT t = 0; t<total; t++)
		{
			*(dest_data)     = *(src_data + 2);
			*(dest_data + 1) = *(src_data + 1);
			*(dest_data + 2) = *(src_data + 0);
			*(dest_data + 3) = src_bpp == 4 ? *(src_data + 3) : 0;
			dest_data += dest_bpp;
			src_data  += src_bpp;
		}
	}
	else
	{
		BYTE *boost_lut = &_colour[boost_mode][0];
		for (UINT t = 0; t<total; t++)
		{
			*(dest_data    ) = *( &boost_lut[*(src_data + 2)] );
			*(dest_data + 1) = *( &boost_lut[*(src_data + 1)] );
			*(dest_data + 2) = *( &boost_lut[*(src_data    )] );
			*(dest_data + 3) = src_bpp == 4 ? *(src_data + 3) : 0;
			dest_data += dest_bpp;
			src_data  += src_bpp;
		}
	}

	return VS_OK;
}


// ---------- Mono ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::Mono()
{

	// ---- init ----
	INT result = InitMono();


	// ---- shortcut ----
	DWORD  boost_mode = _ippb->boost_mode;
	DWORD  w          = _ippb->width;
	DWORD  h          = _ippb->height;
	DWORD  src_bpp    = _ippb->src_bpp;
	DWORD  dest_bpp   = _ippb->dest_bpp;
	BYTE *src_data    = _ippb->src_data;
	BYTE *dest_data   = _ippb->dest_data;


	// ---- local ----
	BYTE *mono = &_mono[boost_mode][0][0][0];
	UINT  lmult = 256 * 256;


	// ---- process source into destination ----
	UINT  total = w * h;
	for (UINT t = 0; t<total; t++)
	{
		BYTE *q = &( mono[ *(src_data + 0)*lmult + *(src_data + 1)*256 + *(src_data + 2) ]  );
		*(dest_data + 0) = *(dest_data + 1) = *(dest_data + 2) = *q;
		*(dest_data + 3) = src_bpp == 4 ? *(src_data + 3) : 0;
		dest_data += dest_bpp;
		src_data += src_bpp;
	}

	return VS_OK;
}


// ---------- Green ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::Green()
{

	// ---- init ----
	INT result = InitTint(ImgLutProcess::SEPIA);


	// ---- shortcut ----
	DWORD  boost_mode = _ippb->boost_mode;
	DWORD  w          = _ippb->width;
	DWORD  h          = _ippb->height;
	DWORD  src_bpp    = _ippb->src_bpp;
	DWORD  dest_bpp   = _ippb->dest_bpp;
	BYTE *src_data    = _ippb->src_data;
	BYTE *dest_data   = _ippb->dest_data;


	// ---- local ----
	BYTE *green = &_green[boost_mode][0][0][0][0];
	UINT  lmult = 256 * 256 * 3;
	UINT  rmult = 256 * 3;


	// ---- process source into destination ----
	UINT  total = w * h;
	for (UINT p = 0; p<total; p++)
	{
		BYTE *q = &( green[ *(src_data + 0)*lmult + *(src_data + 1)*rmult + *(src_data + 2)*3 ] );
		*(dest_data + 0) = *(q + 2);
		*(dest_data + 1) = *(q + 1);
		*(dest_data + 2) = *(q + 0);
		*(dest_data + 3) = src_bpp == 4 ? *(src_data + 3) : 0;
		dest_data += dest_bpp;
		src_data += src_bpp;
	}

	return VS_OK;
}


// ---------- Sepia ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PI_ImgLutProcess::Sepia()
{

	// ---- init ----
	INT result = InitTint(ImgLutProcess::SEPIA);


	// ---- shortcut ----
	DWORD  boost_mode = _ippb->boost_mode;
	DWORD  w          = _ippb->width;
	DWORD  h          = _ippb->height;
	DWORD  src_bpp    = _ippb->src_bpp;
	DWORD  dest_bpp   = _ippb->dest_bpp;
	BYTE *src_data    = _ippb->src_data;
	BYTE *dest_data   = _ippb->dest_data;


	// ---- local ----
	BYTE *sepia = &_sepia[boost_mode][0][0][0][0];
	UINT  lmult = 256 * 256 * 3;
	UINT  rmult = 256 * 3;


	// ---- process source into destination ----
	UINT  total = w * h;
	for (UINT p = 0; p<total; p++)
	{
		BYTE *q = &( sepia[ *(src_data + 0)*lmult + *(src_data + 1)*rmult + *(src_data + 2)*3 ] );
		*(dest_data + 0) = *(q + 2);
		*(dest_data + 1) = *(q + 1);
		*(dest_data + 2) = *(q + 0);
		*(dest_data + 3) = src_bpp == 4 ? *(src_data + 3) : 0;
		dest_data += dest_bpp;
		src_data += src_bpp;
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ImgLutProcess::ImgLutProcess()
{
	pi_img_pre_process = new PI_ImgLutProcess();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgLutProcess::~ImgLutProcess()
{
	if ( pi_img_pre_process != NULL ) { delete pi_img_pre_process; pi_img_pre_process = NULL; }
}


// ---------- PreProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ImgLutProcess::PreProcess(ImgLutProcessBuffer *ippb)
{

	INT result = VS_OK;


	pi_img_pre_process->_ippb = ippb;

	
	#ifdef LUT_THREADED

	if ( pi_img_pre_process->_runtime )
		pi_img_pre_process->Runtime();


	if ( pi_img_pre_process->_locked )
	{
		DWORD mode = ippb->lut_mode;
		DWORD boost = ippb->boost_mode;
		result = pi_img_pre_process->Colour();
		ippb->lut_mode = mode;
		ippb->boost_mode = boost;
		return VS_OK;
	}

	#endif


	switch ( ippb->lut_mode )
	{
		case vs_system::ImgLutProcess::COLOUR:
			result = pi_img_pre_process->Colour();
			break;
		case vs_system::ImgLutProcess::MONO:
			result = pi_img_pre_process->Mono();
			break;
		case vs_system::ImgLutProcess::GREEN:
			result = pi_img_pre_process->Green();
			break;
		case vs_system::ImgLutProcess::SEPIA:
			result = pi_img_pre_process->Sepia();
			break;
		default:
			break;
	}

	return VS_OK;
}
