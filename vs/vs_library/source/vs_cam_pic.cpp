////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_pic.cpp ----------
/*!
\file vs_jply.cpp
\brief Implementation of the CamPic class.
\author Gareth Edwards
\note 2017
*/

// ---- Use preprocessor directive to turn OFF MS warnings ----
#define _CRT_SECURE_NO_WARNINGS


// ---- library include ----
#include "../header/vs_cam_pic.h"


// ---- application include ----
#include "../../vs_application/header/vs_app_setup.h"


// ---- system include ----
#include "../../vs_system/header/vs_sys_stream.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


//---------- private implmentation ----------
class CamPic::PI_CamPic
{

public:

	// ---- cdtor
		PI_CamPic();
		virtual ~PI_CamPic();


	// ---- framework
		INT Initialise();
		INT Setup();
		INT Cleanup();


	// ---- callback
		INT(*user_callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT []);


	// ---- application dependency pointers
		vs_system::SysContext *sys_context = NULL;
		vs_system::SysOutput  *sys_output = NULL;


	// ---- application name
		char app_name[VS_MAXCHARNAME];


	// ---- cameras config object pointer
		vs_system::CamProps *cam_props = NULL;


	// ---- state properties
		INT   run_mode = 0;
		ULONG pic_tick = 0;
		UINT  pic_fps  = 0;


	// ---- image buffer
		UINT image_width     = 100;
		UINT image_height    = 100;
		UINT bytes_per_pixel = 3;
		vs_system::ImgBuffer *pic_img_buffer = NULL;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CamPic::PI_CamPic::PI_CamPic()
{
	INT result = Initialise();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamPic::PI_CamPic::~PI_CamPic()
{
	INT result = Cleanup();
}


// ---------- Initialise ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
INT CamPic::PI_CamPic::Initialise()
{

	// ---- application dependency pointers
		sys_context = NULL;
		sys_output = NULL;
	

	// ---- state properties
		run_mode = 0;
		pic_tick = 0;
		pic_fps  = 10;


	// ---- image buffer
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		image_width  = 4096;
		image_height = 2304;
		//image_width  = 4104;   // WILMA6
		//image_height = 3006;
		#else
		image_width  = 3684;
		image_height = 3684;
		#endif
		bytes_per_pixel = 3;
		pic_img_buffer  = NULL;

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief 
\author Gareth Edwards
*/
INT CamPic::PI_CamPic::Setup()
{

	// ---- local
		INT result = 0;
		CHAR paf[VS_MAXCHARPATHLEN];


	// ---- initialise runtime config image path/filename
		sprintf(
				paf,
				"%sdata/%s/pic/image.jpg",
				sys_context->GetChar(
				sys_context->APP_FOLDER_PATH),
				app_name
			);


	// ---- initialise pic buffer
		pic_img_buffer = new vs_system::ImgBuffer();
		result = pic_img_buffer->Initialise(image_width, image_height, bytes_per_pixel);
		result = pic_img_buffer->CreateBuffer();


	// ---- read test pic into buffer
		vs_system::ImgJpegLib img_jpeglib;
		result = img_jpeglib.Read(pic_img_buffer, paf);


	// ---- SWAP RGB to BGR
		UCHAR *src = pic_img_buffer->GetBuffer();
		UINT pmax = image_width * image_height;
		for ( UINT p=0; p<pmax; p++ )
		{
			UCHAR c  = *src;
			*src     = *(src+2);
			*(src+2) =   c;
			src += bytes_per_pixel;
		}

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief 
\author Gareth Edwards
*/
INT CamPic::PI_CamPic::Cleanup()
{
	if ( pic_img_buffer ) { delete pic_img_buffer; pic_img_buffer = NULL; }
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- static assignments
CamPic::PI_CamPic  * CamPic::pi_cam_pic = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CamPic::CamPic()
{
	pi_cam_pic = new PI_CamPic();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamPic::~CamPic()
{
	if ( pi_cam_pic != NULL ) { delete pi_cam_pic; pi_cam_pic = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT CamPic::Setup(vs_system::AppDependency *app_dep)
{

	// ---- store application dependency pointer
		app_dependency = app_dep;


	// ---- local private dependency properties
		pi_cam_pic->sys_context = app_dependency->GetSysContext();
		pi_cam_pic->sys_output = app_dependency->GetSysOutput();


	// ---- store app name
		app_dependency->GetAppName(pi_cam_pic->app_name);


	// ---- setup private properties
		pi_cam_pic->Setup();

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT CamPic::Cleanup()
{
	return VS_OK;
}


// ---------- Process ----------
/*!
\brief Process camera events
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamPic::Process()
{

	// ---- second changed ?
	LONG now = (LONG)pi_cam_pic->sys_context->GetTimeElapsed() / (1000 / pi_cam_pic->pic_fps);
	if ( now != pi_cam_pic->pic_tick )
	{

		// ---- local
			vs_system::ImgBuffer *pic_img_buffer = pi_cam_pic->pic_img_buffer;


		// ---- (re-)create accumulate image buffer
			if ( pic_img_buffer == NULL )
			{
				pic_img_buffer = new vs_system::ImgBuffer();
				pic_img_buffer->Initialise(
						pi_cam_pic->image_width,
						pi_cam_pic->image_height,
						pi_cam_pic->bytes_per_pixel
					);
				pic_img_buffer->CreateBuffer();
			}
			else if ( pic_img_buffer->GetWidth() != pi_cam_pic->image_width )
			{
				if ( pic_img_buffer->GetHeight() != pi_cam_pic->image_height )
				{
					pic_img_buffer->DestroyBuffer();
					pic_img_buffer->Initialise(
							pi_cam_pic->image_width,
							pi_cam_pic->image_height,
							pi_cam_pic->bytes_per_pixel
						);
					pic_img_buffer->CreateBuffer();
				}
			}


		// ---- IF NEW image THEN callback
			if ( pic_img_buffer->GetBuffer() != NULL )
			{
			
				// ---- timestamp
					SYSTEMTIME SystemTime;
					GetLocalTime(&SystemTime);
					INT timestamp[7];
					timestamp[0] = (INT)SystemTime.wYear;
					timestamp[1] = (INT)SystemTime.wMonth;
					timestamp[2] = (INT)SystemTime.wDay;
					timestamp[3] = (INT)SystemTime.wHour;
					timestamp[4] = (INT)SystemTime.wMinute;
					timestamp[5] = (INT)SystemTime.wSecond;
					timestamp[6] = (INT)SystemTime.wMilliseconds;

				// ---- invoke callback ----
					INT result = pi_cam_pic->user_callback_on_capture_image(
							pi_cam_pic->image_width,
							pi_cam_pic->image_height,
							pi_cam_pic->bytes_per_pixel,
							pic_img_buffer->GetBuffer(),
							timestamp
						);
			}

		// ---- update tick
			pi_cam_pic->pic_tick = now;

	}

	return VS_OK;

}

// ---------- Start ----------
/*!
\brief Start camera
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT CamPic::Start()
{
	pi_cam_pic->run_mode = 1;
	return VS_OK;
}


// ---------- Stop ----------
/*!
\brief Stop camera
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamPic::Stop()
{
	pi_cam_pic->run_mode = 0;
	return VS_OK;
}


// ---------- SetCamProps ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamPic::SetCamProps(vs_system::CamProps *cam_props)
{
	pi_cam_pic->cam_props = cam_props;
	return VS_OK;
}


// ---------- SetCallbackOnCaptureImage ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamPic::SetCallbackOnCaptureImage(
			INT(*user_callback_on_capture_image)(
			WORD,WORD, WORD, BYTE *, INT[]
		)
	)
{
	pi_cam_pic->user_callback_on_capture_image =
			user_callback_on_capture_image;
	return VS_OK;
}


// ---- set callback ----
INT CamPic::CallbackUpdateBlackLevel() { return VS_OK; }
INT CamPic::CallbackUpdateColour()     { return VS_OK; }
INT CamPic::CallbackUpdateExposure()   { return VS_OK; }
INT CamPic::CallbackUpdateFramerate()  { return VS_OK; }
INT CamPic::CallbackUpdateGainColour() { return VS_OK; }
INT CamPic::CallbackUpdateGainMaster() { return VS_OK; }
INT CamPic::CallbackUpdateGamma()      { return VS_OK; }
INT CamPic::CallbackUpdateRop()        { return VS_OK; }
INT CamPic::CallbackUpdateSetup()      { return VS_OK; }
INT CamPic::CallbackWriteIniFile()     { return VS_OK; }


////////////////////////////////////////////////////////////////////////////////
