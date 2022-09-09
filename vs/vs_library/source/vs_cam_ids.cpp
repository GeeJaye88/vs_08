////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_ids.cpp ----------
/*!
\file vs_cam_ids.cpp
\brief Implementation of the CamIds class.
\author Gareth Edwards 
\note 2017
*/

// ---- Use preprocessor directive to turn OFF MS warnings
#define _CRT_SECURE_NO_WARNINGS


// ---- IDS include
#include "../header/vs_cam_ids.h"


// ---- IDS API - link against the specified .lib file
#ifdef VS_CAM_IDS_VIDEO_SDK 
#pragma comment(lib, "uEye_api.lib")
#endif


// ---- pixel op
//#define VS_CAM_IDS_TINT_AOI


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


//---------- private implmentation ----------
class CamIds::PI_CamIds
{

public:

	// ---- cdtor
		PI_CamIds();
		virtual ~PI_CamIds();


	// ---- initialise
		INT InitCamera(HIDS *, HWND);
		INT InitImage();


	// ---- single image buffer
		INT ImageBufferSetup();
		INT ImageBufferCleanup();


	// ---- multi image sequence buffer
		INT SeqBuffersSetup();
		INT SeqBuffersStart();
		INT SeqBuffersStop();
		INT SeqBuffersCleanup();


	// ---- LED
		INT SetLedOff();


	// ---- get
		INT GetId(HIDS *, CHAR *);
		INT GetMaxImageSize(INT *, INT *);


	// ---- exposure

		INT SetExposureAoi();

		INT SetupExposureAoiRect(
				#ifdef VS_CAM_IDS_VIDEO_SDK
				IS_RECT *
				#endif
			);

		INT OffsetExposureAoiRect(
				#ifdef VS_CAM_IDS_VIDEO_SDK
				IS_RECT *
				#endif
			);

		INT StoreExposureAoiRect(
				#ifdef VS_CAM_IDS_VIDEO_SDK
				IS_RECT
				#endif
			);

		INT TintExposureAoi(WORD, WORD, WORD, BYTE *);


	// ---- callback
		INT(*callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT []);


	// ---- identity
		INT   camera_id;
		CHAR  serial_number[3][VS_MAXCHARNAME];


	// ---- cameras config object pointer
		vs_system::CamProps *cam_props;


	// ---- setup flags
		BOOL  initialise_at_runtime;
		BOOL  no_connection_error_message;
		UINT  size_mode;
		BOOL  use_cam_props_hdl_file;
		BOOL  use_ids_ini_file;  // e.g. "data/camera_03/ids/uEye359xLE.ini"


	// ---- flip
		BOOL  h_flip_from_obj_config;
		BOOL  v_flip_from_obj_config;


	// ---- buffer
		BOOL  seq_buffer;


	// ---- exposure
		DOUBLE exposure_range[3];


		#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- ids init
			HIDS       m_hCam;           // Handle to camera
			INT        m_Ret;            // Return value of uEye SDK functions - NOT USED
			INT        m_nColorMode;     // Y8/RGB16/RGB24/REG32
			INT        m_nBitsPerPixel;  // Number of bits needed store one pixel
			INT        m_nSizeX;         // Width of video 
			INT        m_nSizeY;         // Height of video
			SENSORINFO m_sInfo;          // Sensor information struct


		// ---- AOI
			IS_RECT    m_rectAOI;
			IS_RECT    m_copyAOI;


		// ---- image buffer
			INT        m_lMemoryId;      // Grabber memory - buffer ID
			CHAR      *m_pcImageMemory;  // Grabber memory - pointer to buffer


		// ---- sequence image buffers
			#define IDS_NUM_BUFFERS 6
			INT       m_nBuffersToUse = IDS_NUM_BUFFERS;
			INT       m_nBuffersInUse = IDS_NUM_BUFFERS;
			CHAR     *m_pcSeqImgMem[IDS_NUM_BUFFERS];
			INT       m_lSeqMemId[IDS_NUM_BUFFERS];
			INT       m_nSeqNumId[IDS_NUM_BUFFERS];

			BOOL      is_exit_image_queue;

		#endif


	// ---- dependency injection
		vs_system::AppDependency *app_dependency;
		vs_system::SysOutput *sys_output;
		CHAR msg[256];

};  


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CamIds::PI_CamIds::PI_CamIds()
{

	// ---- identity - TBD: Could be enumerated!
		camera_id = 0;


	// ----NULL serial number
		for ( INT i=0; i<3; i++) strcpy(serial_number[0], "");


	// ---- cameras config object pointers
		cam_props = NULL;


	// ---- setup flags
		no_connection_error_message = FALSE;
		initialise_at_runtime  = TRUE;
		use_cam_props_hdl_file = FALSE;
		use_ids_ini_file       = FALSE;


	// ---- flip
		h_flip_from_obj_config = FALSE;
		v_flip_from_obj_config = FALSE;


	// ---- buffer
		seq_buffer = FALSE;  // WILMA6


		#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- ids init
			m_hCam          = 0;
			m_Ret           = 0;
			m_nColorMode    = 0;
			m_nBitsPerPixel = 32;
			m_nSizeX        = 0;
			m_nSizeY        = 0;
			m_lMemoryId     = 0;
			m_pcImageMemory = 0;


		// ---- sequence of buffers
			m_nBuffersInUse = 0;
			for (INT i = 0; i < m_nBuffersToUse; i++)
			{
				m_pcSeqImgMem[i] = NULL;
				m_lSeqMemId[i] = 0;
				m_nSeqNumId[i] = 0;
			}

			is_exit_image_queue = FALSE;

		#endif


	// ---- default dependency injection
		app_dependency = NULL;
		sys_output = NULL;
		strcpy(msg, "");
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamIds::PI_CamIds::~PI_CamIds()
{
	;
}


// ---------- InitCamera ----------
/*!
\brief Hardware setup 
\author Gareth Edwards
\return INT - ERROR, VS_ERROR or IS_NO_SUCCESS <=0 < VS_OK or IS_SUCCESS
*/
INT CamIds::PI_CamIds::InitCamera(HIDS *hCam, HWND hWnd)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_InitCamera(hCam, hWnd);
		if ( nRet == IS_SUCCESS )
		{
			return IS_SUCCESS;
		}
		else if ( nRet == IS_STARTER_FW_UPLOAD_NEEDED )
		{

			// ---- time for the firmware upload = 25 seconds by default
				INT nUploadTime = 25000;
				is_GetDuration(*hCam, IS_STARTER_FW_UPLOAD, &nUploadTime);

			// ---- try again to open the camera
				//
				// This time we allow the automatic upload of the firmware
				// by specifying "IS_ALLOW_STARTER_FIRMWARE_UPLOAD"
				//
				*hCam = (HIDS)(((INT)*hCam) | IS_ALLOW_STARTER_FW_UPLOAD);
				nRet = is_InitCamera(hCam, hWnd);
		}
		else if ( nRet == IS_NO_SUCCESS )
		{
			return IS_NO_SUCCESS;
		}


		pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_InitCamera: failed!");


		return IS_NO_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- InitImage ----------
/*!
\brief Initialise camera display mode
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::InitImage()
{

	// ---- 1. Get CamProps' AOI properties
		INT result = 0;


	// ---- camera
		FLOAT sb = 0;
		result = cam_props->GetProperty(
				CamProps::SENSOR,
				0,
				CamProps::SEQ_BUFFER,
				&sb
			);
		seq_buffer = FALSE; //(BOOL)((INT)sb); // WILMA6


	// ---- roi
		FLOAT ho = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::H_OFFSET,
				&ho
			);
		FLOAT vo = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::V_OFFSET,
				&vo
			);
		FLOAT w = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::WIDTH,
				&w
			);
		FLOAT h = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::HEIGHT,
				&h
			);
		FLOAT sampling = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::SAMPLING,
				&sampling
			);

		float fred = 2;


	#ifdef VS_CAM_IDS_VIDEO_SDK


		// ---- local
			INT nRet = IS_NO_SUCCESS;


		// ---- IF no camera THEN fail
			if ( m_hCam == NULL ) return IS_NO_SUCCESS;


		// ---- 2 IF memory allocated THEN free
			if ( m_pcImageMemory != NULL )
			{
				is_FreeImageMem(
						m_hCam,
						m_pcImageMemory,
						m_lMemoryId
					);
			}
			m_pcImageMemory = NULL;


		// ---- 3 set display mode to bitmap (DIB) display mode
			nRet = is_SetDisplayMode(m_hCam, IS_SET_DM_DIB);
			if ( nRet != IS_SUCCESS )
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetDisplayMode: IS_SET_DM_DIB failed!");
				return nRet;
			}


		// ---- 4. set colour mode
			if ( m_sInfo.nColorMode == IS_COLORMODE_BAYER )
			{
				// Set colour mode to BGR???
				//
				// Notes: TBD
				//
				// For GPU:
				//    IS_CM_BGR8_PACKED requires alpha to be interleaved
				//    IS_CM_BGRA8_PACKED is perfect
				//
				// For saving to JPEG:
				//    IS_CM_RGB8_PACKED is perfect
				//
				// For interleaving BGR buffer into BGRA buffer, and then in
				// Archive Save method buffer requires B and R to be swapped. 
				//
				m_nColorMode = IS_CM_BGR8_PACKED;
				nRet = is_SetColorMode(m_hCam, m_nColorMode);

				// Set number of bits per pixel
				m_nBitsPerPixel = 24;

				// Get the color depth to the current windows setting
				//
				// Don't do this!!! Sets color mode to BGRA and bitsper pixel to 32 
				//
				// INT i = is_GetColorDepth(m_hCam, &m_nBitsPerPixel, &m_nColorMode);
			}
			// ELSE video
			else if ( m_sInfo.nColorMode == IS_COLORMODE_CBYCRY )
			{
				// for color camera models use RGB32 mode
				m_nColorMode = IS_CM_BGRA8_PACKED;
				m_nBitsPerPixel = 32;
			}
			// ELSE mono video
			else
			{
				// for monochrome camera models use Y8 mode
				m_nColorMode = IS_CM_MONO8;
				m_nBitsPerPixel = 8;
			}
	

		//  ---- 5 get AOI - (in correct init order) 
			nRet = is_AOI(
					m_hCam,
					IS_AOI_IMAGE_GET_AOI,
					(void*)&m_rectAOI,
					sizeof(m_rectAOI)
				);
			if ( nRet != IS_SUCCESS )
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AOI: IS_AOI_IMAGE_GET_AOI failed!");
				return nRet;
			}


		// ---- 6 subsampling - (in correct init order)

			/*if ( pi_cam_ids->use_cam_props_hdl_file )
			{
				m_rectAOI.s32X = (INT)ho;
				m_rectAOI.s32Y = (INT)vo;
				m_rectAOI.s32Width = (INT)w;
				m_rectAOI.s32Height = (INT)h;

				switch ((INT)sampling)
				{
					case 2:
						nRet = is_SetSubSampling(
								m_hCam,
								IS_SUBSAMPLING_2X_VERTICAL + IS_SUBSAMPLING_2X_HORIZONTAL
							);
						if ( nRet != IS_SUCCESS )
						{
							pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetSubSampling: (2) failed!");
							return nRet;
						}
						break;

					case 4:
						nRet = is_SetSubSampling(
								m_hCam,
								IS_SUBSAMPLING_4X_VERTICAL + IS_SUBSAMPLING_4X_HORIZONTAL
							);
						if ( nRet != IS_SUCCESS )
						{
							pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetSubSampling: (4) failed!");
							return nRet;
						}
						break;

					default:
						INT vo = is_SetSubSampling(
								m_hCam,
								IS_GET_SUBSAMPLING_FACTOR_VERTICAL
							);
						INT ho = is_SetSubSampling(
								m_hCam,
								IS_GET_SUBSAMPLING_FACTOR_HORIZONTAL
							);
						INT ss = is_SetSubSampling(
								m_hCam,
								IS_GET_SUBSAMPLING
							);


						//nRet = is_SetSubSampling(
						//		m_hCam,
						////		0
						//	);

						if (nRet != IS_SUCCESS)
						{
							pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetSubSampling: (default) failed!");
							return nRet;
						}

						break;
					}
			} */


		// ---- 7 set AOI - (in correct init order)
			nRet = is_AOI(
					m_hCam,
					IS_AOI_IMAGE_SET_AOI,
					(void*)&m_rectAOI,
					sizeof(m_rectAOI)
				);
			if (nRet == IS_SUCCESS)
			{
				m_nSizeX = m_rectAOI.s32Width;
				m_nSizeY = m_rectAOI.s32Height;
			}
			else
			{
				INT msgBoxID = MessageBox(
						NULL,
						"Initialisation of the camera AOI failed.\n"
						"Click \"OK\" to exit.\n",
						"Could not Initialise the camera!",
						MB_OK
					);
			}


		// ---- 8 store AOI
			memcpy(
					(void *)&m_copyAOI,
					(void *)&m_rectAOI,
					sizeof(IS_RECT)
				);


		// ---- 9 buffer(s)
			if ( seq_buffer )
			{

				// ---- creates an image memory sequence
					nRet = SeqBuffersSetup();

				// ---- enables queue mode for the image memory sequence
					nRet = is_InitImageQueue(m_hCam, 0);
					if ( nRet != IS_SUCCESS )
					{
						pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_InitImageQueue: failed!");
						return nRet;
					}

			}
			else
			{
				// ---- creates an image memory to receive image data
					nRet = ImageBufferSetup();
			}


			return nRet; // TBD - return what???

	#else

		return VS_OK;

	#endif

}


// ---------- ImageBufferSetup ----------
/*!
\brief Allocate and add image memory
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::ImageBufferSetup()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_AllocImageMem(
				m_hCam,
				m_nSizeX,
				m_nSizeY,
				m_nBitsPerPixel,
				&m_pcImageMemory,
				&m_lMemoryId
			);

		if ( nRet == IS_SUCCESS )
		{
			nRet = is_SetImageMem(
					m_hCam,
					m_pcImageMemory,
					m_lMemoryId
				);
			if ( nRet != IS_SUCCESS )
			{
				::MessageBox(0, "Error : Set image memory failed!", 0, 0);
				return nRet;
			}
		}
		else
		{
			::MessageBox(0, "Error : Allocate image memory in sequence failed!", 0, 0);
			return nRet;
		}

		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- ImageBufferCleanup ----------
/*!
\brief Cleanup image buffer
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::ImageBufferCleanup()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		if ( pi_cam_ids->m_pcImageMemory != NULL )
		{
			INT nRet = is_FreeImageMem(
					pi_cam_ids->m_hCam,
					pi_cam_ids->m_pcImageMemory,
					pi_cam_ids->m_lMemoryId
				);
			pi_cam_ids->m_pcImageMemory = NULL;

			if ( nRet != IS_SUCCESS )
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_FreeImageMem: failed!");
				return nRet;
			}
			
		}
		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- SeqBuffersSetup ----------
/*!
\brief Allocate sequence of buffers
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::SeqBuffersSetup()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK  

		INT nBuffersInUse = 0;
		for (INT i = 0; i < m_nBuffersToUse; i++)
		{

			INT nRet = is_AllocImageMem(
					m_hCam,
					m_nSizeX,
					m_nSizeY,
					m_nBitsPerPixel,
					&m_pcSeqImgMem[i],
					&m_lSeqMemId[i]
				);
			OutputDebugString("is_AllocImageMem\n");

			if ( nRet == IS_SUCCESS )
			{
				nRet = is_AddToSequence(
						m_hCam,
						m_pcSeqImgMem[i],
						m_lSeqMemId[i]
					);
				OutputDebugString("is_AddToSequence\n");

				m_nSeqNumId[i] = i + 1;
				if ( nRet != IS_SUCCESS )
				{
					is_FreeImageMem(
							m_hCam,
							m_pcSeqImgMem[i],
							m_lSeqMemId[i]
						);
					OutputDebugString("is_FreeImageMem\n");

					m_pcSeqImgMem[i] = 0;
					m_lSeqMemId[i] = 0;
					::MessageBox(0, "Error : Add image memory to sequence failed!", 0, 0);
					return nRet;
				}
			}
			else
			{
				::MessageBox(0, "Error : Allocate image memory in sequence failed!", 0, 0);
				return nRet;
			}
			nBuffersInUse = i;
		}
		m_nBuffersInUse = nBuffersInUse + 1;

		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- SeqBuffersStart ----------
/*!
\brief Start sequence
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::SeqBuffersStart()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- enables the queue mode for existing image memory sequences
			if ( pi_cam_ids->seq_buffer )
			{

				INT nRet = is_InitImageQueue(pi_cam_ids->m_hCam, 0);
				OutputDebugString("is_InitImageQueue\n");

				if ( nRet != IS_SUCCESS )
				{
					pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_InitImageQueue: failed!");
				}

				pi_cam_ids->is_exit_image_queue = FALSE;
			}

		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- SeqBuffersStop ----------
/*!
\brief Stop sequence
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::SeqBuffersStop()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- deletes a queue which has been initialized
			if ( pi_cam_ids->seq_buffer )
			{
				if ( pi_cam_ids->is_exit_image_queue )
				{
					;
				}
				else
				{

					INT nRet = is_ExitImageQueue(pi_cam_ids->m_hCam);
					OutputDebugString("is_ExitImageQueue\n");

					if ( nRet != IS_SUCCESS )
					{
						pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_ExitImageQueue: failed!");
					}

					pi_cam_ids->is_exit_image_queue = TRUE;
				}
			}

		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- SeqBuffersCleanup ----------
/*!
\brief Cleanup sequence of buffers
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::SeqBuffersCleanup()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_ClearSequence(m_hCam);
		if  ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_ClearSequence: failed!");
			return nRet;
		}


		INT i;
		for (i = (m_nBuffersInUse - 1); i >= 0; i--)
		{

			 nRet = is_FreeImageMem(
							m_hCam,
							m_pcSeqImgMem[i],
							m_lSeqMemId[i]
						);
			 OutputDebugString("is_FreeImageMem\n");

			if ( nRet != IS_SUCCESS )
			{
				::MessageBox(0, "   is_FreeImageMem : failed!", 0, 0);
				return nRet;
			}
			m_pcSeqImgMem[i] = 0;
			m_lSeqMemId[i] = 0;

		}
		m_nBuffersInUse = i;

		return IS_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- SetLedOff ----------
/*!
\brief Setup board LED OFF
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::SetLedOff()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK


		// ---- IF no camera THEN fail
			INT nRet = IS_NO_SUCCESS;
			if (m_hCam == NULL) return IS_NO_SUCCESS;


		// ---- Turn LED OFF - discovered on 28/10/16 (phew!!!)

			// UINT nCurrentState = IO_LED_DISABLE;
			// JME the above is not found in the IDS 4.80 uEye SDK // IO_LED_STATE_1

			UINT nCurrentState = IO_LED_DISABLE;
			nRet = is_IO(m_hCam, IS_IO_CMD_LED_SET_STATE, (void*)&nCurrentState, sizeof(nCurrentState));
			if ( nRet != IS_SUCCESS )
			{
				// NOTE: this might not be a terminal fail, older boards, etc...
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_IO: IS_IO_CMD_LED_SET_STATE failed!");
				return nRet;
			}


		return nRet;


	#else

		return VS_OK;

	#endif

}


// ---------- GetId ----------
/*!
\brief Get camera id
\author Gareth Edwards
\param HIDS * - is sdk camera handle
\param char * - is camera name (e.g. "UI158xLE-C")
\return INT (>0) camera id
*/
INT CamIds::PI_CamIds::GetId(HIDS *hCam, CHAR *name)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- IF one or more cameras THEN
			INT nNumCam;
			INT nRet = is_GetNumberOfCameras(&nNumCam);
			if ( nNumCam >= 1 )
			{

				// ---- create list with suitable size
					UEYE_CAMERA_LIST* pucl;
					pucl = (UEYE_CAMERA_LIST*) new BYTE[sizeof (DWORD)+nNumCam * sizeof (UEYE_CAMERA_INFO)];
					pucl->dwCount = nNumCam;

				// ---- retrieve camera list info
					if ( is_GetCameraList(pucl) == IS_SUCCESS )
					{

						int iCamera;
						for ( iCamera = 0; iCamera < (int)pucl->dwCount; iCamera++ )
						{
							// ---- IF name matched THEN --
							if ( !strcmp(name, pucl->uci[iCamera].Model) )
							{
								*hCam = (HIDS)pucl->uci[iCamera].dwCameraID;
								INT camId = pucl->uci[iCamera].dwCameraID;
								delete[] pucl;

								// enable the dialog based error report
								nRet = is_SetErrorReport(m_hCam, IS_ENABLE_ERR_REP); //IS_DISABLE_ERR_REP);
								if ( nRet != IS_SUCCESS )
								{
									pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetErrorReport: failed!");
									return false;
								}

								// return matched camera ID
								return camId;
							}
						}

					}

					pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_GetCameraList: failed!");

					delete[] pucl;

			}


		// ---- report
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_GetNumberOfCameras: failed!");


		return IS_NO_SUCCESS;

	#else

		return VS_OK;

	#endif

}


// ---------- GetMaxImageSize ----------
/*!
\brief Get maximum buffer size of camera
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::GetMaxImageSize(INT *pnSizeX, INT *pnSizeY)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		// ---- check if the camera supports an arbitrary AOI
			INT nAOISupported = 0;
			BOOL bAOISupported = TRUE;
			if ( is_ImageFormat(
					m_hCam,
					IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
					(void*)&nAOISupported,
					sizeof(nAOISupported)) == IS_SUCCESS
				)
			{
				bAOISupported = (nAOISupported != 0);
			}
			else
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_ImageFormat: IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED failed!");
			}


		// ---- get maximum image size
			if ( bAOISupported )
			{

				SENSORINFO sInfo;
				INT nRet = is_GetSensorInfo(m_hCam, &sInfo);
				if ( nRet == IS_SUCCESS )
				{
					*pnSizeX = sInfo.nMaxWidth;
					*pnSizeY = sInfo.nMaxHeight;
				}
				else
				{
					pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_GetSensorInfo: failed!");
				}

			}
			else
			{
				// Note: Only ueye xs

				// get image size of the current format
				IS_SIZE_2D imageSize;
				INT nRet = is_AOI(
						m_hCam,
						IS_AOI_IMAGE_GET_SIZE,
						(void*)&imageSize,
						sizeof(imageSize)
					);
				if ( nRet == IS_SUCCESS )
				{
					*pnSizeX = imageSize.s32Width;
					*pnSizeY = imageSize.s32Height;
				}
				else
				{
					pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AOI: IS_AOI_IMAGE_GET_SIZE failed!");
				}

			}

	#endif

	return VS_OK;
}


// ---------- SetExposureAoi ----------
/*!
\brief
\author Gareth Edwards
\note

Invoked from:

	CamIds::Setup
		-> CamIds::PI_CamIds::SetupImage()

	CamIds::SetExposure

*/
INT CamIds::PI_CamIds::SetExposureAoi()
{

	// ---- local
		INT nRet = 0;


	#ifdef VS_CAM_IDS_VIDEO_SDK


	// ----	get configuration
		CHAR *pBuffer = new char[sizeof(AES_CONFIGURATION) - sizeof(CHAR) + sizeof(AES_PEAK_WHITE_CONFIGURATION)];
		AES_CONFIGURATION *pAesConfiguration = (AES_CONFIGURATION*)pBuffer;

	
	// ---- get aes supported bitmask
		INT nSupported = 0;
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_GET_SUPPORTED_TYPES,
				&nSupported,
				sizeof(nSupported)
			);
		if (nRet != IS_SUCCESS)
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_GET_SUPPORTED_TYPES failed!");
		}


	// ---- supported ?
		if ( nSupported & IS_AES_MODE_PEAK )
		{
			pAesConfiguration->nMode = IS_AES_MODE_PEAK;
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_MODE_PEAK not suported!");
			return VS_ERROR;
		}


	// ----	get peak white configuration
		AES_PEAK_WHITE_CONFIGURATION *pPeakWhiteConfiguration =
			(AES_PEAK_WHITE_CONFIGURATION*)pAesConfiguration->pConfiguration;


	// ---- AES_PEAK_WHITE_CONFIGURATION default values
		//
		//    rectUserAOI   { 0, 0, 0, 0 } : If all 0 then no AOI.
		//		                Note: order is s32X, s32Y, s32Width, s32Height
		//
		//    nFrameSkip    4 : # frames to be skipped during automatic image control.
		//
		//    nHysteresis   1 : Must be at least one less than the target value of the auto control.
		//
		//    nReference   20 : Brightness reference value for auto control.
		//
		//    nChannel      7 : Sets the channels to be used for analysis.
		//                      Note: 7 == IS_AES_CHANNEL_RED | IS_AES_CHANNEL_GREEN | IS_AES_CHANNEL_BLUE
		//
		//    f64Maximum    0 : (0 = no limit)
		//    f64Minimum    0 : (0 = no limit)


	// ---- get default configuration
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_GET_CONFIGURATION_DEFAULT,
				pAesConfiguration,
				sizeof(AES_CONFIGURATION) -
				sizeof(CHAR) +
				sizeof(AES_PEAK_WHITE_CONFIGURATION)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_GET_CONFIGURATION_DEFAULT failed!");
		}


	// ---- get configuration
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_GET_CONFIGURATION,
				pAesConfiguration,
				sizeof(AES_CONFIGURATION) -
				sizeof(CHAR) +
				sizeof(AES_PEAK_WHITE_CONFIGURATION)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_GET_CONFIGURATION failed!");
		}


	// ---- get configuration range
		AES_PEAK_WHITE_CONFIGURATION_RANGE *pPeakWhiteConfigRange =
			(AES_PEAK_WHITE_CONFIGURATION_RANGE*)pAesConfiguration->pConfiguration;
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_GET_CONFIGURATION_RANGE,
				pAesConfiguration,
				sizeof(AES_CONFIGURATION) -
				sizeof(CHAR) +
				sizeof(AES_PEAK_WHITE_CONFIGURATION_RANGE)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_GET_CONFIGURATION_RANGE failed!");
		}


	// ---- get exposure range
		nRet = is_Exposure(m_hCam,
				IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
				(void*)exposure_range,
				sizeof(exposure_range)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Exposure: IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE failed!");
		}


	// ---- initialise aoi rect by zone
		IS_RECT exposure_aoi_rect;
		nRet = SetupExposureAoiRect(&exposure_aoi_rect);


	// ---- set configuration ----

		// ---- rect
			pPeakWhiteConfiguration->rectUserAOI.s32X      = exposure_aoi_rect.s32X;
			pPeakWhiteConfiguration->rectUserAOI.s32Y      = exposure_aoi_rect.s32Y;
			pPeakWhiteConfiguration->rectUserAOI.s32Width  = exposure_aoi_rect.s32Width;
			pPeakWhiteConfiguration->rectUserAOI.s32Height = exposure_aoi_rect.s32Height;


		// ---- get index
			INT index = 0;
			INT result = cam_props->GetProperty(
					CamProps::SETUP,
					0,
					CamProps::EXPOSURE_INDEX,
					&index
				);


		// ---- get bright reference
			FLOAT f_bright_reference = 0;
			result = cam_props->GetProperty(
					CamProps::EXPOSURE,
					index,
					CamProps::BRIGHT_REF,
					&f_bright_reference
				);


		// ... and the rest!
		/*
		pPeakWhiteConfiguration->nFrameSkip  = 4;
		pPeakWhiteConfiguration->nHysteresis = 1;
		pPeakWhiteConfiguration->nReference  = 10;
		pPeakWhiteConfiguration->nChannel    = 7;
		pPeakWhiteConfiguration->f64Minimum  = exposure_range[0];
		pPeakWhiteConfiguration->f64Maximum  = exposure_range[1];
		*/


	// ---- catch NO AOI
		/*
		IS_RECT rectAOI = pPeakWhiteConfiguration->rectUserAOI;
		if ( rectAOI.s32X == 0 && rectAOI.s32Y == 0 && rectAOI.s32Width == 0 && rectAOI.s32Height == 0 )
		{
			float fred = 2;
		}
		*/

	// ---- store rect for tint ----
		nRet = StoreExposureAoiRect(pPeakWhiteConfiguration->rectUserAOI);


	// ---- offset ----
		nRet = OffsetExposureAoiRect(&pPeakWhiteConfiguration->rectUserAOI);


	/*
		invoked from ----> INT PI_Camera::SetExposure()

			and then ----> INT CamIds::SetExposure()
	*/


	// ---- set AUTO AES ?


	// ---- auto shutter enabled ?
		DOUBLE p1 = 0;
		nRet = is_SetAutoParameter(
				pi_cam_ids->m_hCam,
				IS_GET_ENABLE_AUTO_SHUTTER,
				&p1,
				0
			);
		if (nRet != IS_SUCCESS)
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_GET_ENABLE_AUTO_SHUTTER failed!");
		}
		//OutputDebugString("+++ OK is_AutoParameter\n");

	// ---- yes, then enable AES... 

		/*

		if ( p1 == 1 )
		{

			// ---- enable AES 
				//
				// set the automatic controls for auto exposure shutter,
				// auto gain and black level
				//
				INT nEnable = IS_AUTOPARAMETER_ENABLE;
				nRet = is_AutoParameter(
						m_hCam,
						IS_AES_CMD_SET_ENABLE,
						&nEnable,
						sizeof(nEnable)
					);
				if (nRet != IS_SUCCESS)
				{
					pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_SET_ENABLE failed!");
				}
		}


	// ---- set AES configuration (plus AUTO EXPOSURE AOI)
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_SET_CONFIGURATION,
				pAesConfiguration,
				sizeof(AES_CONFIGURATION) -
				sizeof(CHAR) +
				sizeof(AES_PEAK_WHITE_CONFIGURATION)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AutoParameter: IS_AES_CMD_SET_CONFIGURATION failed!");
		}

		*/


	// ---- set AUTO BRIGHTNESS AOI
		nRet = is_AOI(
				m_hCam,
				IS_AOI_AUTO_BRIGHTNESS_SET_AOI,
				(VOID *)&pPeakWhiteConfiguration->rectUserAOI,
				sizeof(IS_RECT)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AOI: IS_AOI_AUTO_BRIGHTNESS_SET_AOI failed!");
		}
		//OutputDebugString("+++ OK IS_AOI_AUTO_BRIGHTNESS_SET_AOI\n");


	// ---- set AUTO WHITEBALANCE AOI
		nRet = is_AOI(
				m_hCam,
				IS_AOI_AUTO_WHITEBALANCE_SET_AOI,
				(VOID *)&pPeakWhiteConfiguration->rectUserAOI,
				sizeof(IS_RECT)
			);
		if ( nRet != IS_SUCCESS )
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_AOI: IS_AOI_AUTO_WHITEBALANCE_SET_AOI failed!");
		}
		//OutputDebugString("+++ OK IS_AOI_AUTO_WHITEBALANCE_SET_AOI\n");


	// ---- note: AES mode
		nSupported = 0;
		nRet = is_AutoParameter(
				m_hCam,
				IS_AES_CMD_GET_SUPPORTED_TYPES,
				&nSupported,
				sizeof(nSupported)
			);
		if (nSupported & IS_AES_MODE_PEAK)
		{
			// In peak white mode the image is adjusted to a reference
			// quantity of white pixels(value range 1…20 %).
			;
		}
		if (nSupported & IS_AES_MODE_MEAN)
		{
			// In mean mode, the average brightness and color rendering
			// of the image is adjusted to the preset desired value
			;
		}

	#endif

	return VS_OK;

}


// ---------- SetupExposureAoiRect ----------
/*!
\brief Set exposure AOI
\author Gareth Edwards
\notes

Included via "vs_obj_config.h":

	// ---- aoi zone ----
	enum {
		AOI_ALL     = 0,
		AOI_ANNULAR = 1,  OR rect
		AOI_TOP     = 2,
		AOI_LEFT    = 3,
		AOI_BOTTOM  = 4,
		AOI_RIGHT   = 5
	};

*/
INT CamIds::PI_CamIds::SetupExposureAoiRect(
		#ifdef VS_CAM_IDS_VIDEO_SDK
		IS_RECT * rectAOI
		#endif
	)
{

	// ---- local ----
		INT nRet = 0;


	#ifdef VS_CAM_IDS_VIDEO_SDK


		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


			// ---- get exposure zone ----
				FLOAT exp_zone_f = 0;
				nRet = cam_props->GetProperty(
						CamProps::AOI,
						0,
						CamProps::ZONE,
						&exp_zone_f
					);
				UINT exp_zone = (UINT)exp_zone_f;


			// ---- shortcut for readibiltity
				const UINT all  = vs_system::ObjConfig::AOI_ALL;
				const UINT rect = vs_system::ObjConfig::AOI_ANNULAR;


			// ---- select area
				switch ( exp_zone )
				{

					default:

					case all:
						{
							FLOAT roi_ho = 0;
							INT result = cam_props->GetProperty(
									CamProps::ROI,
									0,
									CamProps::H_OFFSET,
									&roi_ho
								);
							FLOAT roi_vo = 0;
							result = cam_props->GetProperty(
									CamProps::ROI,
									0,
									CamProps::V_OFFSET,
									&roi_vo
								);
							FLOAT roi_w = 0;
							result = cam_props->GetProperty(
									CamProps::ROI,
									0,
									CamProps::WIDTH,
									&roi_w
								);
							FLOAT roi_h = 0;
							result = cam_props->GetProperty(
									CamProps::ROI,
									0,
									CamProps::HEIGHT,
									&roi_h
								);

							rectAOI->s32X      = (INT)roi_ho;
							rectAOI->s32Y      = (INT)roi_vo;
							rectAOI->s32Width  = (INT)roi_w;
							rectAOI->s32Height = (INT)roi_h;

						}
						return VS_OK;


					case rect:
						{

							// ---- get
								FLOAT left=0, right=1, bottom=0, top=1;
								nRet = cam_props->GetProperty(CamProps::AOI, 0, CamProps::LEFT,   &left );
								nRet = cam_props->GetProperty(CamProps::AOI, 0, CamProps::TOP,    &top  );
								nRet = cam_props->GetProperty(CamProps::AOI, 0, CamProps::BOTTOM, &bottom);
								nRet = cam_props->GetProperty(CamProps::AOI, 0, CamProps::RIGHT,  &right);


							// ---- calc dimensions and offset 
								FLOAT w = (FLOAT)m_rectAOI.s32Width  * (right-left);
								FLOAT h = (FLOAT)m_rectAOI.s32Height * (bottom-top); // BUG FIX 2/9/18 - was "top - bottom"
								FLOAT x = (FLOAT)m_rectAOI.s32Width  * left;
								FLOAT y = (FLOAT)m_rectAOI.s32Height * top;

							// ---- store
								rectAOI->s32X = (INT)(x + 0.5f);
								rectAOI->s32Y = (INT)(y + 0.5f);
								rectAOI->s32Width  = (INT)(w + 0.5f);
								rectAOI->s32Height = (INT)(h + 0.5f);

						}
						break;
				}

		#else


			// ---- get exposure zone
				FLOAT ann_zone_f = 0;
				nRet = cam_props->GetProperty(
						CamProps::ANNULAR_AOI,
						0,
						CamProps::ZONE,
						&ann_zone_f
					);
				UINT ann_zone = (UINT)ann_zone_f;


			// ---- get exposure param
				FLOAT ann_min_aoi = 0;
				nRet = cam_props->GetProperty(
						CamProps::ANNULAR_AOI,
						0,
						CamProps::MIN,
						&ann_min_aoi
					);
				FLOAT ann_max_aoi = 0;
				nRet = cam_props->GetProperty(
						CamProps::ANNULAR_AOI,
						0,
						CamProps::MAX,
						&ann_max_aoi
					);


			// ---- shortcut for readibiltity
				const UINT all     = vs_system::ObjConfig::AOI_ALL;
				const UINT annular = vs_system::ObjConfig::AOI_ANNULAR;
				const UINT top     = vs_system::ObjConfig::AOI_TOP;
				const UINT left    = vs_system::ObjConfig::AOI_LEFT;
				const UINT bottom  = vs_system::ObjConfig::AOI_BOTTOM;
				const UINT right   = vs_system::ObjConfig::AOI_RIGHT;


			// ---- select area
				switch ( ann_zone )
				{

					default:

					case all:
						{
							rectAOI->s32X = 0;
							rectAOI->s32Y = 0;
							rectAOI->s32Width  = 0;
							rectAOI->s32Height = 0;
						}
						return VS_OK;


					case annular:
						{

							// ---- square aoi proportional to average of radii 
								FLOAT x = (ann_max_aoi + ann_min_aoi) / 2.0f;
								FLOAT h = (FLOAT)m_rectAOI.s32Height / 2;


							// ---- remeber to ROI offset
								INT px = (INT)((1 - x) * h + 0.5f) + m_rectAOI.s32X;
								INT py = px + m_rectAOI.s32Y;
								INT pw = (INT)(x * h + 0.5f) * 2;
								INT ph = pw;
								rectAOI->s32X = px;
								rectAOI->s32Y = py;
								rectAOI->s32Width  = pw;
								rectAOI->s32Height = ph;
						}
						break;

					case top:
					case left:
					case bottom:
					case right:
						{

							// ---- flip
								if ( ! h_flip_from_obj_config )
								{
									if ( ann_zone == left || ann_zone == right )
									{
										ann_zone = ann_zone == left ? right : left;
									}
								}
								if ( v_flip_from_obj_config )
								{
									if ( ann_zone == top || ann_zone == bottom )
									{
										ann_zone = ann_zone == top ? bottom : top;
									}
								}

							// ---- y axis up parametric ordinates and dimensions
								FLOAT fr = ann_max_aoi;
								FLOAT mu = 0.75f;
								FLOAT fy = (1-mu)*ann_min_aoi + mu*ann_max_aoi;
								FLOAT fh = fy - ann_min_aoi;
								FLOAT fx = //exp_shape == square ?
											// fh / 2 :
											sqrt(fr*fr - fy*fy);

							// & "magic" limit shape
								FLOAT fw = 1;
								BOOL small_size = FALSE;
								if (small_size)
								{
									fw = fx * 1.5f;
									fx *= 0.75f;
								}
								else
								{
									fw = fx * 2.0f;
									fx *= 1.0f;
								}

							// ---- orientate
								FLOAT x, y;
								switch ( ann_zone )
								{
									default:
									case top:    x = -fx; y =  fy - fh; break;
									case left:   x =  fy; y = -fx;      break;
									case bottom: x = -fx; y = -fy;      break;
									case right:  x = -fy; y = -fx;      break;
								}


							// ---- map proportional to half image height
								FLOAT hby2 = (FLOAT)m_rectAOI.s32Height / 2;


							// ---- map into "pixel" space
								INT px, py, pw, ph;
								if ( ann_zone == top || ann_zone == bottom )
								{
									px = (INT)(hby2 + x * hby2 + 0.5f);
									pw = (INT)(hby2 * fw + 0.5f);
									ph = (INT)(hby2 * fh + 0.5f);
									if ( ann_zone == top )
									{
										py = (INT)((hby2 * y) + hby2 + 0.5f);
									}
									else
									{
										py = (INT)(hby2 + (hby2 * y) + 0.5f);
									}
								}
								else if ( ann_zone ==  left || ann_zone == right )
								{
									px = (INT)(hby2 + x * hby2 + 0.5f);
									pw = (INT)(hby2 * fh + 0.5f);
									ph = (INT)(hby2 * fw + 0.5f);
									if ( ann_zone == left )
									{
										px -= pw;
										py = (INT)((hby2 * y) + hby2 + 0.5f);
									}
									else
									{
										py = (INT)(hby2 + (hby2 * y) + 0.5f);
									}
								}


							// ---- set AOI without ROI offset !!!
								rectAOI->s32X      = px;
								rectAOI->s32Y      = py;
								rectAOI->s32Width  = pw;
								rectAOI->s32Height = ph;

						}
						break;
				}


				// ---- set parametric location and dimensions
					FLOAT p_left   = (FLOAT)rectAOI->s32X      / (FLOAT)m_rectAOI.s32Width;
					FLOAT p_top    = (FLOAT)rectAOI->s32Y      / (FLOAT)m_rectAOI.s32Height;
					FLOAT p_width  = (FLOAT)rectAOI->s32Width  / (FLOAT)m_rectAOI.s32Width;
					FLOAT p_height = (FLOAT)rectAOI->s32Height / (FLOAT)m_rectAOI.s32Height;


				// ---- NOTE hijack AOI to store location and dimension
					nRet = cam_props->SetProperty(CamProps::AOI, 0, CamProps::LEFT,   p_left);
					nRet = cam_props->SetProperty(CamProps::AOI, 0, CamProps::TOP,    p_top );
					nRet = cam_props->SetProperty(CamProps::AOI, 0, CamProps::WIDTH,  p_width  );
					nRet = cam_props->SetProperty(CamProps::AOI, 0, CamProps::HEIGHT, p_height );

		#endif


		// ---- limit within ROI bounds
			INT x_max = m_rectAOI.s32Width  - 1;
			INT y_max = m_rectAOI.s32Height - 1;
			INT w_max = m_rectAOI.s32Width;
			INT h_max = m_rectAOI.s32Height;

			rectAOI->s32X =
				rectAOI->s32X < 0 ? 0 :
					rectAOI->s32X > x_max ? x_max : rectAOI->s32X;

			rectAOI->s32Y =
				rectAOI->s32Y < 0 ? 0 :
					rectAOI->s32Y > y_max ? y_max : rectAOI->s32Y;

			rectAOI->s32Width =
				rectAOI->s32Width < 0 ? 0 :
					rectAOI->s32Width > w_max ? w_max : rectAOI->s32Width;

			INT x_lim = rectAOI->s32X + rectAOI->s32Width;
			rectAOI->s32Width =
				x_lim > m_rectAOI.s32Width ? m_rectAOI.s32Width - rectAOI->s32X :
					rectAOI->s32Width;

			rectAOI->s32Height =
				rectAOI->s32Height < 0 ? 0 :
					rectAOI->s32Height > h_max ? h_max : rectAOI->s32Height;

			INT y_lim = rectAOI->s32Y + rectAOI->s32Height;
			rectAOI->s32Height =
				y_lim > m_rectAOI.s32Height ? m_rectAOI.s32Height - rectAOI->s32Y :
					rectAOI->s32Height;


		// ---- offset to ROI XY
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			;
		#else
			rectAOI->s32X += m_rectAOI.s32X;
			rectAOI->s32Y += m_rectAOI.s32Y;
		#endif


		// ---- store for tint
			//nRet = StoreExposureAoiRect(m_rectAOI);


	#endif

	return VS_OK;
}


// ---------- OffsetExposureAoiRect ----------
/*!
\brief 
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::OffsetExposureAoiRect(
	#ifdef VS_CAM_IDS_VIDEO_SDK
	IS_RECT * rectAOI
	#endif
	)
{

	// ---- local
		INT nRet = 0;


	// ---- get aoi zone
		FLOAT exp_zone_f = 0;
		nRet = cam_props->GetProperty(
				CamProps::AOI,
				0,
				CamProps::ZONE,
				&exp_zone_f
			);
		UINT exp_zone = (UINT)exp_zone_f;


	// ---- get horizontal offset
		FLOAT ho = 0;
		nRet = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::H_OFFSET,
				&ho
			);


	// ---- get vertical offset
		FLOAT vo = 0;
		nRet = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::V_OFFSET,
				&vo
			);


	#ifdef VS_CAM_IDS_VIDEO_SDK

		const UINT all     = vs_system::ObjConfig::AOI_ALL;
		const UINT annular = vs_system::ObjConfig::AOI_ANNULAR;
		const UINT top     = vs_system::ObjConfig::AOI_TOP;
		const UINT left    = vs_system::ObjConfig::AOI_LEFT;
		const UINT bottom  = vs_system::ObjConfig::AOI_BOTTOM;
		const UINT right   = vs_system::ObjConfig::AOI_RIGHT;

		switch ( exp_zone )
		{
			case annular:
			case top:
			case left:
			case bottom:
			case right:
				{
					rectAOI->s32X += (INT)(ho + 0.5f);
					rectAOI->s32Y += (INT)(vo + 0.5f);
				}
				break;
			default:
				break;
		}

	#endif

	return VS_OK;
}


// ---------- StoreExposureAoiRect ----------
/*!
\brief Set exposure AOI rect
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::StoreExposureAoiRect(
		#ifdef VS_CAM_IDS_VIDEO_SDK
		IS_RECT rectAOI
		#endif
	)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		memcpy(
				(void *)&m_copyAOI,
				(void *)&rectAOI,
				sizeof(IS_RECT)
			);

		//CHAR msg[256];
		//sprintf(
		//		msg,
		//		"Exposure AOI rect is %d,  %d,  %d,  %d\n",
		//		m_copyAOI.s32X,
		//		m_copyAOI.s32Y,
		//		m_copyAOI.s32Width,
		//		m_copyAOI.s32Height
		//	);
		//OutputDebugString(msg);

	#endif

	return VS_OK;
}


// ---------- TintExposureAoi ----------
/*!
\brief Set exposure AOI
\author Gareth Edwards
*/
INT CamIds::PI_CamIds::TintExposureAoi(
		WORD  size_x,
		WORD  size_y,
		WORD  bpp,
		BYTE *buffer
	)
{

	//#if defined(VS_CAMERA_03) || defined(VS_REVIEW_02)
	//if ( TRUE ) return VS_OK;
	//#endif


	// ---- local
		INT nRet = 0;


		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		// ---- get aoi zone
			FLOAT exp_zone_f = 0;
			nRet = cam_props->GetProperty(
					CamProps::AOI,
					0,
					CamProps::ZONE,
					&exp_zone_f
				);
			UINT exp_zone = (UINT)exp_zone_f;

		// ---- shortcut for readibiltity ----
			const UINT all  = vs_system::ObjConfig::AOI_ALL;
			const UINT rect = vs_system::ObjConfig::AOI_ANNULAR;


		#else


		// ---- get annular aoi zone
			FLOAT ann_zone_f = 0;
			nRet = cam_props->GetProperty(
					CamProps::AOI,
					0,
					CamProps::ZONE,
					&ann_zone_f
				);
			UINT ann_zone = (UINT)ann_zone_f;

		// ---- shortcut for readibiltity ----
			const UINT all     = vs_system::ObjConfig::AOI_ALL;
			const UINT annular = vs_system::ObjConfig::AOI_ANNULAR;
			const UINT top     = vs_system::ObjConfig::AOI_TOP;
			const UINT left    = vs_system::ObjConfig::AOI_LEFT;
			const UINT bottom  = vs_system::ObjConfig::AOI_BOTTOM;
			const UINT right   = vs_system::ObjConfig::AOI_RIGHT;
		

		#endif


	// ---- select exposure zone ----
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		switch ( exp_zone )
		#else
		switch ( ann_zone )
		#endif
		{

			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			case all:
			case rect:
			#else
			case all:
			case annular:
			case top:
			case left:
			case bottom:
			case right:
			#endif

				{

					#ifdef VS_CAM_IDS_VIDEO_SDK

					// --- get location and dimension
						FLOAT h = 0, w = 0, l = 0, t = 0;
						cam_props->GetProperty(CamProps::AOI, 0, CamProps::WIDTH,  &w);
						cam_props->GetProperty(CamProps::AOI, 0, CamProps::HEIGHT, &h);
						cam_props->GetProperty(CamProps::AOI, 0, CamProps::LEFT,   &l);
						cam_props->GetProperty(CamProps::AOI, 0, CamProps::TOP,    &t);

						INT d = 0;
						#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
						cam_props->GetProperty(CamProps::AOI, 0, CamProps::DISPLAY,&d);
						#else
						cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::DISPLAY, &d);
						#endif

						BOOL display = d == 0 ? FALSE : TRUE;
						if ( ! display ) return VS_OK;


					// ---- calc rect
						UINT  bpp = pi_cam_ids->m_nBitsPerPixel / 8;
						BYTE *src = buffer;

						FLOAT f_x_min = (FLOAT)pi_cam_ids->m_rectAOI.s32Width  * l;
						FLOAT f_y_min = (FLOAT)pi_cam_ids->m_rectAOI.s32Height * t;
						FLOAT f_x_max = (FLOAT)pi_cam_ids->m_rectAOI.s32Width  * w + f_x_min;
						FLOAT f_y_max = (FLOAT)pi_cam_ids->m_rectAOI.s32Height * h + f_y_min;

						INT x_min = (INT)f_x_min;
						INT y_min = (INT)f_y_min;
						INT x_max = (INT)f_x_max - 1;
						INT y_max = (INT)f_y_max - 1;


					// ---- tint
						INT bo = 32;
						INT xo[4][2] = {
							{ x_min,    x_max      },
							{ x_min,    x_min + bo },
							{ x_max-bo, x_max      },
							{ x_min,    x_max      },
							};
						INT yo[4][2] = {
							{ y_min,    y_min + bo },
							{ y_min+bo, y_max - bo },
							{ y_min+bo, y_max - bo },
							{ y_max-bo, y_max      },
							};

						for (INT i = 0; i < 4; i++)
						{
							INT xl = xo[i][0];
							INT xh = xo[i][1];
							INT yl = yo[i][0];
							INT yh = yo[i][1];
							for (INT y = yl; y < yh; y++)
							{
								INT y_off = pi_cam_ids->m_nSizeX * y * bpp;
								for (INT x = xl; x < xh; x++)
								{
									INT x_off = x * bpp;
									BYTE *s = src + y_off + x_off;
									*s = 255;
								}
							}
						}

					#endif

				}
				break;
			default:
				break;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// IDS static assignments
CamIds::PI_CamIds  * CamIds::pi_cam_ids = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CamIds::CamIds()
{
	pi_cam_ids = new PI_CamIds();
}
 

// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamIds::~CamIds()
{
	if ( pi_cam_ids != NULL ) { delete pi_cam_ids; pi_cam_ids = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK


// ---- example returned values... etc.
	nRet = IS_CANT_COMMUNICATE_WITH_DRIVER;
	nRet = IS_CANT_OPEN_DEVICE;
	nRet = IS_INVALID_CAMERA_TYPE;
	nRet = IS_INVALID_CAMERA_HANDLE;

// ---- note: invoked only at runtime

*/
INT CamIds::Setup(vs_system::AppDependency *app_dep)
{

	// ---- store application dependency pointer
		pi_cam_ids->app_dependency = app_dep;
		pi_cam_ids->sys_output = app_dep->GetSysOutput();
		vs_system::SysOutput *sys_output = pi_cam_ids->sys_output;


	#ifdef VS_CAM_IDS_VIDEO_SDK


	// ---- get window handle
		vs_system::SysContext *sys_context = app_dep->GetSysContext();
		HWND hWindow = (HWND)sys_context->GetDWord(sys_context->WINDOW_HANDLE);


	// ---- local
		INT result = VS_OK;
		INT nRet = IS_NO_SUCCESS;


	// ---- 1 cleanup camera
		result = Cleanup();


	// ---- 2 hardware initialisation succeeds ? 
		pi_cam_ids->m_hCam = (HIDS)0;
		nRet = pi_cam_ids->InitCamera(
				&pi_cam_ids->m_hCam,
				hWindow
			);
		if ( nRet == IS_SUCCESS )
		{

			// ---- 3 use name of camera type to set id and initialise

				// Note: can assign a unique camera ID to a camera
				// using INT is_SetCameraID (HIDS hCam, INT nID)

				CHAR paf[VS_MAXCHARPATHLEN];
				CHAR *file = "";

				pi_cam_ids->camera_id = 0;
				if ( pi_cam_ids->GetId( &pi_cam_ids->m_hCam, "UI359xLE-C" ) == 1 )
				{
					pi_cam_ids->camera_id = 1;
					file = "uEye359xLE.ini";
				}

				else if ( pi_cam_ids->GetId( &pi_cam_ids->m_hCam, "UI359xCP-C" ) == 1 )
				{
					pi_cam_ids->camera_id = 2;
					file = "uEye359xCP.ini";
				}
				else if ( pi_cam_ids->GetId(&pi_cam_ids->m_hCam, "UI300xSE-M") == 1 )
				{
					pi_cam_ids->camera_id = 3;
					file = "uEye300xSE.ini";
				}
				else
				{
					sys_output->Message("Camera Identity:-----------------------------------------------------------");
					sys_output->Message("+++ Camera identity not recognised.");
					sys_output->Message("+++ Please restart this application.");
					sys_output->Message("+++ If this does not work then please contact Observant Innovations.");
					sys_output->Message("End:-----------------------------------------------------------------------");
					INT msgBoxID = MessageBox(
							NULL,
							"Camera: Identity (type) not recognised.\n"
							"Please restart this application.\n"
							"If this does not solve problem then please contact Observant Innovations.\n"
							"Click \"OK\" to exit.\n",
							"Camera: Identity (type)",
							MB_OK
						);
					exit(0);
					return VS_ERROR;
				}


			// ---- 4 load ".ini" FILE 
				if ( pi_cam_ids->use_ids_ini_file )
				{
					sprintf(
							paf,
							"%sdata/%s/ids_%d/%s",
							sys_context->GetChar(sys_context->APP_FOLDER_PATH),
							sys_context->GetChar(sys_context->CONFIG_NAME),
							pi_cam_ids->size_mode,
							file
						);

					size_t size = strlen(paf) + 1;
					wchar_t* w_paf = new wchar_t[size];
					mbstowcs(w_paf, paf, size);

					switch ( pi_cam_ids->camera_id )
					{
						case 1: // UI359xLE-C
						case 2: // UI359xCP-C
						case 3: // UI300xSE-M
							nRet = is_ParameterSet(
										pi_cam_ids->m_hCam,
										IS_PARAMETERSET_CMD_LOAD_FILE,
										w_paf,
										NULL
									);
							if ( nRet == IS_NO_SUCCESS ) return VS_ERROR;
							break;
						default:; // ---- note: can't get here!
					}

				}


			// ---- 5. get camera info 
				CAMINFO cInfo;
				nRet = is_GetCameraInfo(pi_cam_ids->m_hCam, &cInfo);


			// ---- 6 serial #'d
				if ( strcmp(pi_cam_ids->serial_number[0], "") != 0 )
				{
					if (
							( strcmp(pi_cam_ids->serial_number[0], cInfo.SerNo) == 0 ) ||
							( strcmp(pi_cam_ids->serial_number[1], cInfo.SerNo) == 0 ) ||
							( strcmp(pi_cam_ids->serial_number[2], cInfo.SerNo) == 0 )
						)
					{
						CHAR msg[VS_MAXCHARNAME];
						sprintf(msg, "Camera: Serial #: %s", cInfo.SerNo);
						sys_output->Message(msg);
					}
					else
					{
						sys_output->Message("Licensing:-----------------------------------------------------------------");
						sys_output->Message("+++ This application is not licensed for use with this camera.");
						sys_output->Message("+++ Please contact Observant Innovations..");
						sys_output->Message("End:-----------------------------------------------------------------------");
						INT msgBoxID = MessageBox(
								NULL,
								"This application is not licensed for use with this camera.\n"
								"Please contact Observant Innovations.\n"
								"Click \"OK\" to exit.\n",
								"Licensing",
								MB_OK
							);
						exit(0);
					}
				}


			// ---- 7 get sensor info
				nRet = is_GetSensorInfo(
						pi_cam_ids->m_hCam,
						&pi_cam_ids->m_sInfo
					);
				if ( nRet == IS_NO_SUCCESS ) return VS_ERROR;


			// ---- 8 get maximum image size for this sensor
				nRet = pi_cam_ids->GetMaxImageSize(
						&pi_cam_ids->m_nSizeX,
						&pi_cam_ids->m_nSizeY
					);
				if ( nRet == IS_NO_SUCCESS ) return VS_ERROR;


			// ---- 9 pixel clock

				// get
				CamProps *cam_props = pi_cam_ids->cam_props;
				FLOAT pixel_clock_supplied = 0;
				result = cam_props->GetProperty(
						CamProps::SENSOR,
						0,
						CamProps::PIXEL_CLOCK,
						&pixel_clock_supplied
					);
				if ( result == VS_ERROR ) return VS_ERROR;


				// over write CamProps
				UINT nPixelClockValue = (UINT)pixel_clock_supplied;
				CHAR msg[VS_MAXCHARLEN];
				if ( nPixelClockValue == 0 )
				{
					switch ( pi_cam_ids->camera_id )
					{
						case 1: nPixelClockValue = 240; break; // opt 260 : UI359xLE-C (4912 x 3684, USB 3.0)
						case 2: nPixelClockValue = 400; break; // opt 420 : UI359xLE-C (4912 x 3684, USB 3.0)
						case 3: nPixelClockValue = 474; break; // opt 474 : uEye300xSE-M (4104 x 3006, USB 3.0)
						default:; // ---- note: can't get here!
					}
					sprintf(
							msg,
							"   Using application preset pixel clock value of %d for %s.",
							(INT)nPixelClockValue,
							pi_cam_ids->m_sInfo.strSensorName
						);
				}
				else
				{
					sprintf(
							msg,
							"   Using supplied pixel clock value of %d for %s.",
							(INT)nPixelClockValue,
							pi_cam_ids->m_sInfo.strSensorName
						);
				}
				sys_output->Message(msg);


				// tell
				sys_output->Message("+++ Pixel clock value set in the data/[app]/ids/ids_config.hdl file.");
				sys_output->Message("+++    e.g:");
				sys_output->Message("+++    Camera{");
				sys_output->Message("+++       Pixel-Clock:[value];");
				sys_output->Message("+++    } ");
				sys_output->Message("+++    Note: If value == 0 then set to application preset value");


				// set
				nRet = is_PixelClock(
						pi_cam_ids->m_hCam,
						IS_PIXELCLOCK_CMD_SET,
						(void*)&nPixelClockValue,
						sizeof(nPixelClockValue)
					);
				if ( nRet == IS_NO_SUCCESS ) return VS_ERROR;


				// confirm
				UINT nPixelClock = 0;
				nRet = is_PixelClock(
						pi_cam_ids->m_hCam,
						IS_PIXELCLOCK_CMD_GET,
						(void*)&nPixelClock,
						sizeof(nPixelClock)
					);
				if ( nRet == IS_NO_SUCCESS ) return VS_ERROR;


			// ---- 10 initialise image
				nRet = pi_cam_ids->InitImage();
				if ( nRet == IS_SUCCESS )
				{
					nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_DEVICE_REMOVED, hWindow);
					nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_DEVICE_RECONNECTED, hWindow);
					nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_FRAME, hWindow);
				}
				else
				{
					sys_output->Message("Initialisation:------------------------------------------------------------");
					sys_output->Message("+++ Initialisation of the camera failed.");
					sys_output->Message("+++ Please restart this application.");
					sys_output->Message("+++ If this does not work then please contact Observant Innovations.");
					sys_output->Message("End:-----------------------------------------------------------------------");
					INT msgBoxID = MessageBox(
							NULL,
							"Initialisation of the camera failed.\n"
							"Please restart this application.\n"
							"If this does not work then please contact Observant Innovations.\n"
							"Click \"OK\" to exit.\n",
							"Initialisation",
							MB_OK
						);
					return VS_EXIT;
				}


			// ---- 11 turn board LED OFF
				nRet = pi_cam_ids->SetLedOff();
				if (nRet == IS_NO_SUCCESS) return VS_ERROR;
				sys_output->Message("+++ LED is OFF");
	

			// ---- 12 complete setup by invoking ALL the update callbacks

				// -- setup
				result = CallbackUpdateSetup();

				// -- then everything else
				result = CallbackUpdateBlackLevel();
				result = CallbackUpdateColour();
				result = CallbackUpdateExposure();
				result = CallbackUpdateFramerate();
				result = CallbackUpdateGainColour();
				result = CallbackUpdateGainMaster();
				result = CallbackUpdateGamma();
				result = CallbackUpdateRop();


			// ---- all done here
				sys_output->Message("Camera:--------------------------------------------------------------------");


			// ---- initialisation succeeded
				return VS_OK;

		}


	// ---- ELSE, oooops, hardware initialisation did not succeed
		else
		{
			if ( pi_cam_ids->no_connection_error_message )
			{
				;
			}
			else
			{
				pi_cam_ids->no_connection_error_message = TRUE;
				sys_output->Message("Camera Connection:---------------------------------------------------------");
				sys_output->Message("+++ Please connect a camera and restart this application.");
				sys_output->Message("End:-----------------------------------------------------------------------");
				INT msgBoxID = MessageBox(
						NULL,
						"Please connect a camera and restart this application.\n"
						"Click \"OK\" to exit.\n",
						"Runtime Camera Connection",
						MB_OK
					);
			}
			return VS_EXIT;
		}


	#endif

	return VS_ERROR;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT CamIds::Cleanup()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		if ( pi_cam_ids->m_hCam != 0 )
		{

			// ---- local
				INT nRet = 0;

			// ---- disable messages
				nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_FRAME, NULL);
				nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_DEVICE_REMOVED, NULL);
				nRet = is_EnableMessage(pi_cam_ids->m_hCam, IS_DEVICE_RECONNECTED, NULL);


			// ---- stop live video
				nRet = is_StopLiveVideo(pi_cam_ids->m_hCam, IS_WAIT);


			// ---- free buffer(s)
				if ( pi_cam_ids->seq_buffer )
				{
					nRet = pi_cam_ids->SeqBuffersCleanup();
				}
				else
				{
					nRet = pi_cam_ids->ImageBufferCleanup();
				}


			// ---- exit camera
				nRet = is_ExitCamera(pi_cam_ids->m_hCam);
				pi_cam_ids->m_hCam = NULL;

		}

	#endif

	
	// ---- re-intialise from scratch
		pi_cam_ids->initialise_at_runtime = TRUE;

	return VS_OK;
}


// ---------- Start ----------
/*!
\brief Start camera
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT CamIds::Start()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		// IF -- no camera -- THEN setup
			if ( pi_cam_ids->m_hCam == 0 )
			{
				Setup(pi_cam_ids->app_dependency);
			}


		// IF -- camera -- THEN
			if ( pi_cam_ids->m_hCam != 0 )
			{

				// ---- start camera
					INT nRet = is_CaptureVideo(pi_cam_ids->m_hCam, IS_WAIT);
					switch ( nRet )
					{
						case IS_NO_SUCCESS:
							{
								vs_system::SysOutput *sys_output = pi_cam_ids->app_dependency->GetSysOutput();
								sys_output->MessageOk(
										"During restart, could not detect a camera!\n\n"
										"Please connect a camera and\nrestart this application.\n\n"
										"Click OK to exit."
									);
								exit(0);
							}
							break;
						default:
							break;
					}
	

				// ---- start sequence buffering 
					pi_cam_ids->SeqBuffersStart();

			}

	#endif

	return VS_OK;
}


// ---------- Process ----------
/*!
\brief Process camera events
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT CamIds::Process()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

	#define IS_UEYE_MESSAGE        (WM_USER + 0x0100) 
	#define IS_FRAME               0x0000
	#define IS_DEVICE_REMOVED      0x1000
	#define IS_DEVICE_RECONNECTED  0x0004
		

	// ---- local
		INT result = VS_OK;


	// ---- local context
		vs_system::SysContext *sys_context = pi_cam_ids->app_dependency->GetSysContext();
		UINT   msg     = (UINT)sys_context->GetDWord(sys_context->MSG);
		WPARAM w_param = (WPARAM)sys_context->GetDWord(sys_context->MSG_WPARAM);
		LPARAM l_param = (LPARAM)sys_context->GetDWord(sys_context->MSG_LPARAM);


	// ---- IF uEye message THEN
		if ( msg == IS_UEYE_MESSAGE )
		{
				switch (w_param)
				{

					case IS_DEVICE_REMOVED:
						Beep(400, 50);
						{
							CHAR * my_msg = "+++ Camera: Has been diconnected(EVENT: IS_DEVICE_REMOVED).";
							vs_system::SysOutput *sys_output = pi_cam_ids->app_dependency->GetSysOutput();
							sys_output->Message(my_msg);
						}
						break;

					case IS_DEVICE_RECONNECTED:
						Beep(400, 50);
						{
							CHAR * my_msg = "+++ Camera: Has been reconnected ( EVENT: IS_DEVICE_RECONNECTED ).";
							vs_system::SysOutput *sys_output = pi_cam_ids->app_dependency->GetSysOutput();
							sys_output->Message(my_msg);
							result = Setup(pi_cam_ids->app_dependency);
							result = Start();
						}
						break;

					case IS_FRAME:
						{

							// ---- sequence buffer ?
								if ( pi_cam_ids->seq_buffer )
								{

									// ---- get
										CHAR* pBuffer = 0;
										INT   pnNum = 0;
										INT nRet = is_GetActSeqBuf(pi_cam_ids->m_hCam, &pnNum, 0, &pBuffer);
										

									// ---- LMK
										/*
										INT nRet = is_GetActSeqBuf(pi_cam_ids->m_hCam, 0, 0, &pBuffer);
										*/
										/*
										CHAR my_msg[128];
										sprintf(my_msg, "is_GetActSeqBuf pnNum = %d (%d)\n", pnNum, nRet);
										OutputDebugString(my_msg);
										*/

									// ---- buffer ?
										BOOL doit = true;
										if ( pBuffer && doit )
										{
											for (INT s = 0; s < pi_cam_ids->m_nBuffersToUse; s++)
											{
												if ( pBuffer == pi_cam_ids->m_pcSeqImgMem[s] )
												{

													// ---- lock buffer
														INT nRet = is_LockSeqBuf(
																pi_cam_ids->m_hCam,
																pi_cam_ids->m_lSeqMemId[s],
																pi_cam_ids->m_pcSeqImgMem[s]
															);
														//sprintf(my_msg, "is_LockSeqBuf (%d)\n", nRet);
														//OutputDebugString(my_msg);


													// ---- timestamp
														INT timestamp[7];
														result = GetTimeStamp(
																	timestamp,
																	pi_cam_ids->m_lSeqMemId[s]
																);

													// ---- process buffer
														UINT bytes_per_pixel = pi_cam_ids->m_nBitsPerPixel / 8;
														result = pi_cam_ids->callback_on_capture_image(
																pi_cam_ids->m_nSizeX,
																pi_cam_ids->m_nSizeY,
																bytes_per_pixel,
																(BYTE *)pi_cam_ids->m_pcSeqImgMem[s],
																timestamp
															);

													// ---- unlock buffer
														nRet = is_UnlockSeqBuf(
																pi_cam_ids->m_hCam,
																pi_cam_ids->m_lSeqMemId[s],
																pi_cam_ids->m_pcSeqImgMem[s]
															);

														//sprintf(my_msg, "is_UnlockSeqBuf (%d)\n", nRet);
														//OutputDebugString(my_msg);

													break;
												}
											}
										}
								}

							// ---- single buffer
								else
								{

										if (pi_cam_ids->m_pcImageMemory != NULL)
										{

											INT timestamp[7];
											result = GetTimeStamp(timestamp, 1);

											// ---- invokes application::CallbackOnCaptureImage
												UINT bytes_per_pixel = pi_cam_ids->m_nBitsPerPixel / 8;
												result = pi_cam_ids->callback_on_capture_image(
														pi_cam_ids->m_nSizeX,
														pi_cam_ids->m_nSizeY,
														bytes_per_pixel,
														(BYTE *)pi_cam_ids->m_pcImageMemory,
														timestamp
													);

										}
								}
						}
						break;
				
						default:
							break;
				}


			// ---- consume message
				result = sys_context->SetDWord(sys_context->MSG, 0);
				result = sys_context->SetDWord(sys_context->MSG_WPARAM, 0);
				result = sys_context->SetDWord(sys_context->MSG_LPARAM, 0);

		}

	#endif

	return VS_OK;
}


#ifdef VS_CAM_IDS_TINT_AOI

	result = pi_cam_ids->TintExposureAoi(
		pi_cam_ids->m_nSizeX,
		pi_cam_ids->m_nSizeY,
		pi_cam_ids->m_nBitsPerPixel / 8,
		(BYTE *)pi_cam_ids->m_pcSeqImgMem[s]
	);

#endif


#ifdef IDS_CAM_SAVE

	// ---- save path/filename
		size_t size = strlen("test.jpg") + 1;
		wchar_t* wa = new wchar_t[size];
		mbstowcs(wa, "test.jpg", size);


	// ---- save immediately
		IMAGE_FILE_PARAMS ImageFileParams;
		ImageFileParams.pwchFileName = NULL;
		ImageFileParams.pnImageID    = NULL;
		ImageFileParams.ppcImageMem  = NULL;
		ImageFileParams.pwchFileName = wa;
		ImageFileParams.nFileType    = IS_IMG_JPG;
		ImageFileParams.nQuality     = 80;
		INT nRet = is_ImageFile(
					pi_cam_ids->m_hCam,
					IS_IMAGE_FILE_CMD_SAVE,
					(void*)&ImageFileParams,
					sizeof(ImageFileParams)
				);

#endif


// ---------- Stop ----------
/*!
\brief Stop camera
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::Stop()
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		if (pi_cam_ids->m_hCam != 0)
		{
			is_StopLiveVideo(
					pi_cam_ids->m_hCam,
					IS_FORCE_VIDEO_STOP
				);
			pi_cam_ids->SeqBuffersStop();
		}
	
	#endif

	return VS_OK;
}

////////////////////////////////////////////////////////////////////////////////


// ---------- GetDimensions ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetDimensions(INT *width, INT *height)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		*width  = pi_cam_ids->m_nSizeX;
		*height = pi_cam_ids->m_nSizeY;

	#endif

	return VS_OK;
}


// ---------- GetExposure ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetExposure(FLOAT *value)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		DOUBLE exposure = 0;
		INT nRet = is_Exposure(
				pi_cam_ids->m_hCam,
				IS_EXPOSURE_CMD_GET_EXPOSURE,
				&exposure,
				8
			);
		*value = (FLOAT)exposure;

	#else
	
		*value = 0;

	#endif

	return VS_OK;
}


// ---------- GetExposureMin ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetExposureMin(FLOAT *min)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_Exposure(
				pi_cam_ids->m_hCam,
				IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
				(void*)pi_cam_ids->exposure_range,
				sizeof(pi_cam_ids->exposure_range)
			);

		*min = (FLOAT)pi_cam_ids->exposure_range[0];

	#else
	
		*min = 0;

	#endif

	return VS_OK;
}


// ---------- GetExposureMax ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetExposureMax(FLOAT *max)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_Exposure(
				pi_cam_ids->m_hCam,
				IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
				(void*)pi_cam_ids->exposure_range,
				sizeof(pi_cam_ids->exposure_range)
			);

		*max = (FLOAT)pi_cam_ids->exposure_range[1];

	#else
	
		*max = 0;

	#endif

	return VS_OK;
}
// ---------- GetExposureRange ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetExposureRange(FLOAT range[])
{

	range[0] = (FLOAT)pi_cam_ids->exposure_range[0];
	range[1] = (FLOAT)pi_cam_ids->exposure_range[1];
	range[2] = (FLOAT)pi_cam_ids->exposure_range[2];

	return VS_OK;
}


// ---------- GetFPS ----------
/*!
\brief Get current fps
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT CamIds::GetFPS(DOUBLE *fps)
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nRet = is_GetFramesPerSecond(
				pi_cam_ids->m_hCam,
				fps
			);

	#endif

	return VS_OK;
}


// ---------- GetGain ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetGain(FLOAT gain[])
{

	#ifdef VS_CAM_IDS_VIDEO_SDK

		gain[0] = (FLOAT)is_SetHardwareGain(
					pi_cam_ids->m_hCam,
					IS_GET_MASTER_GAIN,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER
				);
		gain[1] = (FLOAT)is_SetHardwareGain(
					pi_cam_ids->m_hCam,
					IS_GET_RED_GAIN,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER
				);
		gain[2] = (FLOAT)is_SetHardwareGain(
					pi_cam_ids->m_hCam,
					IS_GET_GREEN_GAIN,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER
				);
		gain[3] = (FLOAT)is_SetHardwareGain(
					pi_cam_ids->m_hCam,
					IS_GET_BLUE_GAIN,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER,
					IS_IGNORE_PARAMETER
				);

	#endif

	return VS_OK;
}


// ---------- GetHOffset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::GetHOffset(INT *offset)
{

	FLOAT ho = 0;
	INT result = pi_cam_ids->cam_props->GetProperty(
			CamProps::ROI,
			0,
			CamProps::H_OFFSET,
			&ho
		);
	*offset = (INT)ho;

	return VS_OK;
}


// ---------- GetTimeStamp ----------
/*!
\brief Get current images time stamp
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT CamIds::GetTimeStamp(
		INT timestamp[],
		const INT cam_id
	)
{

	// ---- local
		INT result = VS_OK;

		#ifdef VS_CAM_IDS_VIDEO_SDK

		UEYEIMAGEINFO ImageInfo;
		INT nRet = is_GetImageInfo(
				pi_cam_ids->m_hCam,
				cam_id,
				&ImageInfo,
				sizeof(ImageInfo)
			);

 		if ( nRet == IS_SUCCESS )
		{
			unsigned long long u64TimestampDevice;
			u64TimestampDevice = ImageInfo.u64TimestampDevice;
			timestamp[0] = (INT)ImageInfo.TimestampSystem.wYear;
			timestamp[1] = (INT)ImageInfo.TimestampSystem.wMonth;
			timestamp[2] = (INT)ImageInfo.TimestampSystem.wDay;
			timestamp[3] = (INT)ImageInfo.TimestampSystem.wHour;
			timestamp[4] = (INT)ImageInfo.TimestampSystem.wMinute;
			timestamp[5] = (INT)ImageInfo.TimestampSystem.wSecond;
			timestamp[6] = (INT)ImageInfo.TimestampSystem.wMilliseconds;
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_GetImageInfo: timestamp failed!");
			for (UINT i = 0; i < 7; i++) timestamp[0] = 0;
			result = VS_ERROR;
		}

		#endif

	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetCamProps ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetCamProps(vs_system::CamProps *cam_props)
{
	pi_cam_ids->cam_props = cam_props;
	return VS_OK;
}


// ---------- SetExposure ----------
/*!
\brief Set exposure with aec zone
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

Invoked by:

	INT application::SwitchCameraLoopHardStart()
	INT application::SwitchCameraMode()
	INT application::SetupGraphics()

		----> INT application::StartLiveCamera()

			----> INT PI_Camera::SetExposure()

				----> INT CamIds::SetExposure()

	INT application::Display()

		----> INT application::UpdateCameraExposure()

			----> INT PI_Camera::SetExposure()

				----> INT CamIds::SetExposure()

*/
INT CamIds::SetExposure()
{

	/*
	// ---- get aoi zone
		FLOAT exp_zone_f = 0;
		INT result = pi_cam_ids->cam_props->GetProperty(
				CamProps::AOI,
				0,
				CamProps::ZONE,
				&exp_zone_f
			);
		UINT exp_zone = (UINT)exp_zone_f;

	*/

	INT result = pi_cam_ids->SetExposureAoi();

	return VS_OK;
}


// ---------- SetHOffset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetHOffset()
{

	// ----  local
		vs_system::SysOutput *sys_output = pi_cam_ids->app_dependency->GetSysOutput();


	// ---- IF NOT using cam_props file THEN report and return
		if ( !pi_cam_ids->use_cam_props_hdl_file )
		{
			sys_output->Message("+++ SetHOffset: NOT using config file - could NOT apply!");
			return VS_ERROR;
		}


	// ---- local
		INT result = 0;
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- get horizontal offset
		FLOAT ho = 0;
		result = cam_props->GetProperty(
				CamProps::ROI,
				0,
				CamProps::H_OFFSET,
				&ho
			);


	#ifdef VS_CAM_IDS_VIDEO_SDK

	//  ---- get the AOI position and size
		INT nRet = is_AOI(
				pi_cam_ids->m_hCam,
				IS_AOI_IMAGE_GET_AOI,
				(void*)&pi_cam_ids->m_rectAOI,
				sizeof(pi_cam_ids->m_rectAOI)
			);

	
	// ---- update AOI horizontal offset
		pi_cam_ids->m_rectAOI.s32X = (INT)ho;


	// ---- set new AOI horizontal position
		nRet = is_AOI(
				pi_cam_ids->m_hCam,
				IS_AOI_IMAGE_SET_AOI,
				(void*)&pi_cam_ids->m_rectAOI,
				sizeof(pi_cam_ids->m_rectAOI)
			);


	// ---- IF error THEN report
		if ( nRet != IS_SUCCESS )
		{
			::MessageBox(0, "+++ SetHOffset->is_AOI: failed!", 0, 0);
		}

	#endif

	return VS_OK;
}


// ---------- SetSerialNumbers ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note Must be invoked BEFORE setup
*/
INT CamIds::SetSerialNumbers(
		CHAR *serial_number_client,
		CHAR *serial_number_dev_01,
		CHAR *serial_number_dev_02
	)
{

	strcpy_s(
			pi_cam_ids->serial_number[0],
			VS_MAXCHARNAME,
			serial_number_client
		);
	strcpy_s(
			pi_cam_ids->serial_number[1],
			VS_MAXCHARNAME,
			serial_number_dev_01
		);
	strcpy_s(
			pi_cam_ids->serial_number[2],
			VS_MAXCHARNAME,
			serial_number_dev_02
		);

	return VS_OK;
}


// ---------- SetSizeMode ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetMode(UINT mode)
{

	pi_cam_ids->size_mode = mode;

	return VS_OK;
}


// ---------- SetUseIdsInitialisation ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetUseIdsInitialisation(BOOL flag)
{

	pi_cam_ids->use_ids_ini_file = flag;

	return VS_OK;
}


// ---------- SetUseCamProps ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetUseCamProps(BOOL flag)
{

	pi_cam_ids->use_cam_props_hdl_file = flag;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackUpdateBlackLevel ----------
/*!
\brief CallbackUpdateBlackLevel
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateBlackLevel()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- note: auto mode as per exposure


	// ---- get index
		INT index = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_MASTER_INDEX,
				&index
			);


	// ---- get
		FLOAT f_black_level = 0;
		result = cam_props->GetProperty(
				CamProps::GAIN_MASTER,
				index,
				CamProps::BLACK_LEVEL,
				&f_black_level
			);


	// ---- set
		#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nBlacklevelCaps = 0;
		INT nRet = is_Blacklevel(
				pi_cam_ids->m_hCam,
				IS_BLACKLEVEL_CMD_GET_CAPS,
				(void*)&nBlacklevelCaps,
				sizeof(nBlacklevelCaps)
			);

		if ( nRet == IS_SUCCESS )
		{

			// can change the state of the auto blacklevel
			BOOL bSetAutoBlacklevel = (nBlacklevelCaps & IS_BLACKLEVEL_CAP_SET_AUTO_BLACKLEVEL) != 0;

			// can change the offset
			BOOL bSetBlacklevelOffset = (nBlacklevelCaps & IS_BLACKLEVEL_CAP_SET_OFFSET) != 0;


			// ---- set black level mode ?
				INT nMode = IS_AUTO_BLACKLEVEL_OFF;

				if (bSetAutoBlacklevel)
				{

					// get default blacklevel mode
					nRet = is_Blacklevel(
							pi_cam_ids->m_hCam,
							IS_BLACKLEVEL_CMD_GET_MODE_DEFAULT,
							(void*)&nMode, sizeof(nMode)
						);

					// get current blacklevel mode
					nRet = is_Blacklevel(
							pi_cam_ids->m_hCam,
							IS_BLACKLEVEL_CMD_GET_MODE,
							(void*)&nMode, sizeof(nMode)
						);

					// set new mode (enable auto blacklevel)
					nMode = IS_AUTO_BLACKLEVEL_ON;
					nRet = is_Blacklevel(
							pi_cam_ids->m_hCam,
							IS_BLACKLEVEL_CMD_SET_MODE,
							(void*)&nMode, sizeof(nMode)
						);
					if ( nRet == IS_SUCCESS )
					{
						sprintf(pi_cam_ids->msg, "+++ is_Blacklevel: ON");
						pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
					}
					else
					{
						pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Blacklevel: enable failed!");
					}

				}


			// ---- set black level ?
				if ( nMode == IS_AUTO_BLACKLEVEL_OFF )
				{
					if ( bSetBlacklevelOffset )
					{

						INT nOffset = 0;

						// get default blacklevel offset
						nRet = is_Blacklevel(
								pi_cam_ids->m_hCam,
								IS_BLACKLEVEL_CMD_GET_OFFSET_DEFAULT,
								(void*)&nOffset, sizeof(nOffset)
							);

						// get offset range
						IS_RANGE_S32 nRange;
						nRet = is_Blacklevel(
								pi_cam_ids->m_hCam,
								IS_BLACKLEVEL_CMD_GET_OFFSET_RANGE,
								(void*)&nRange, sizeof(nRange)
							);
						INT nOffsetMin = nRange.s32Min;
						INT nOffsetMax = nRange.s32Max;
						INT nOffsetInc = nRange.s32Inc;

						// get current blacklevel offset
						nRet = is_Blacklevel(
								pi_cam_ids->m_hCam,
								IS_BLACKLEVEL_CMD_GET_OFFSET,
								(void*)&nOffset, sizeof(nOffset)
							);

						// set new offset
						nOffset = (INT)(f_black_level * 255 + 0.5f);
						nRet = is_Blacklevel(
								pi_cam_ids->m_hCam,
								IS_BLACKLEVEL_CMD_SET_OFFSET,
								(void*)&nOffset, sizeof(nOffset)
							);
						if (nRet == IS_SUCCESS)
						{
							sprintf(pi_cam_ids->msg, "+++ is_Blacklevel: %d", nOffset);
							pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
						}
						else
						{
							pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Blacklevel: failed!");
						}

					}
				}

		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateColour ----------
/*!
\brief CallbackUpdateColour
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateColour()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- note: no auto mode


	// ---- get index
		INT index = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::COLOUR_INDEX,
				&index
			);


	// ---- get
		FLOAT saturation = 0; 
		result = pi_cam_ids->cam_props->GetProperty(
				CamProps::COLOUR,
				index,
				CamProps::SATURATION,
				&saturation
			);
	

	// ---- set

		#ifdef VS_CAM_IDS_VIDEO_SDK

		INT value  = (INT)(saturation * 100 + 0.5f);
			value = value < 0 ? 0 : value > 200 ? 200 : value;
		INT ChromU = value;
		INT ChromV = value;
		INT nRet = is_SetSaturation(
				pi_cam_ids->m_hCam,
				ChromU,
				ChromV
			);
		if ( nRet == IS_SUCCESS )
		{
			sprintf(pi_cam_ids->msg, "+++ is_SetSaturation: %d %d", ChromU, ChromV);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetSaturation: failed!");
		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateExposure ----------
/*!
\brief CallbackUpdateExposure
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateExposure()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- auto exposure ?
		INT auto_exposure = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_EXPOSURE,
				&auto_exposure
			);
		if ( auto_exposure )
		{
			// TBD - set black level
			return VS_OK;
		}


	// ---- get index
		INT index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::EXPOSURE_INDEX,
				&index
			);
		if ( index == 0 )
		{
			// ignore auto mode
			return VS_OK;
		}


	// ---- get
		FLOAT f_time_ms = 0; 
		result = cam_props->GetProperty(
				CamProps::EXPOSURE,
				index,
				CamProps::TIME_EXP_MS,
				&f_time_ms
			);


		#ifdef VS_CAM_IDS_VIDEO_SDK

		UINT nCaps = 0;
		INT nRet = is_Exposure(pi_cam_ids->m_hCam, IS_EXPOSURE_CMD_GET_CAPS, (void*)&nCaps, sizeof(nCaps));
		if ( nRet == IS_SUCCESS )
		{

			//if (nCaps & IS_EXPOSURE_CAP_FINE_INCREMENT)
			{

				// ---- update
					DOUBLE newExposure = f_time_ms;
					nRet = is_Exposure(
							pi_cam_ids->m_hCam,
							IS_EXPOSURE_CMD_SET_EXPOSURE,
							&newExposure,
							sizeof(newExposure)
						);
					if  ( nRet == IS_SUCCESS )
					{
						sprintf(pi_cam_ids->msg, "+++ is_Exposure: %6.4f", (FLOAT)newExposure);
						pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
					}
					else
					{
						pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Exposure: failed!");
					}


				// ---- update stored range info
					INT nRet = is_Exposure(
							pi_cam_ids->m_hCam,
							IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE,
							(void*)pi_cam_ids->exposure_range,
							sizeof(pi_cam_ids->exposure_range)
						);

					sprintf(pi_cam_ids->msg, "+++ max: %6.4f\n", (FLOAT)pi_cam_ids->exposure_range[1]);
					OutputDebugString(pi_cam_ids->msg);

			}

		}

		#endif 

	return VS_OK;
}


// ---------- CallbackUpdateFramerate ----------
/*!
\brief CallbackUpdateFramerate
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateFramerate()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- auto framerate ?
		INT auto_framerate = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_FRAMERATE,
				&auto_framerate
			);
		if ( auto_framerate )
		{
			// TBD - disable ???
			// return VS_OK;
			;
		}


	// ---- get index
		INT index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::FRAMERATE_INDEX,
				&index
			);
		if ( index == 0 )
		{
			// ignore auto mode
			return VS_OK;
		}


	// ---- get
		FLOAT fps = 0; 
		result = cam_props->GetProperty(
				CamProps::FRAMERATE,
				index,
				CamProps::FPS,
				&fps
			);


	// ---- set
		#ifdef VS_CAM_IDS_VIDEO_SDK

		DOUBLE new_FPS = (DOUBLE)fps;
		INT nRet = is_SetFrameRate(
				pi_cam_ids->m_hCam,
				(DOUBLE)fps,
				&new_FPS
			);
		if ( nRet == IS_SUCCESS )
		{
			sprintf(pi_cam_ids->msg, "+++ is_SetFrameRate: %d ", (INT)fps);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetFrameRate: failed!");
		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateGainColour ----------
/*!
\brief CallbackUpdateGainColour
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateGainColour()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- auto wb ?
		INT auto_wb = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_WB,
				&auto_wb
			);

		if ( auto_wb )
		{
			return VS_OK;
		}


	// ---- get gain master
		INT gm_index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_MASTER_INDEX,
				&gm_index
			);
		gm_index = gm_index == 0 ? 4 : gm_index;
		FLOAT gm_gain = 0;
		result = cam_props->GetProperty(
				CamProps::GAIN_MASTER,
				gm_index,
				CamProps::GAIN,
				&gm_gain
			);


	// ---- get gain colour
		INT gc_index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_COLOUR_INDEX,
				&gc_index
			);
		FLOAT f_gain = 0, f_red = 0, f_green = 0, f_blue = 0;
		result = cam_props->GetProperty(
				CamProps::GAIN_COLOUR,
				gc_index,
				CamProps::GAIN,
				&f_gain
			);
		result = cam_props->GetProperty(
				CamProps::GAIN_COLOUR,
				gc_index,
				CamProps::RED,
				&f_red
			);
		result = cam_props->GetProperty(
				CamProps::GAIN_COLOUR,
				gc_index,
				CamProps::GREEN,
				&f_green
			);
		result = cam_props->GetProperty(
				CamProps::GAIN_COLOUR,
				gc_index,
				CamProps::BLUE,
				&f_blue
			);


	// ---- set

		#ifdef VS_CAM_IDS_VIDEO_SDK

		INT gain  = (INT)(gm_gain * 100 + 0.5f);
		INT red   = (INT)(f_red   * 100 + 0.5f);
		INT green = (INT)(f_green * 100 + 0.5f);
		INT blue  = (INT)(f_blue  * 100 + 0.5f);
		INT nRet = is_SetHardwareGain(
				pi_cam_ids->m_hCam,
				gain,
				red,
				green,
				blue
			);
		if ( nRet == IS_SUCCESS )
		{
			sprintf(pi_cam_ids->msg, "+++ is_SetHardwareGain: (mrgb) %d %d %d %d", gain, red, green, blue);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetHardwareGain: (colour) failed!");
		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateGainMaster ----------
/*!
\brief CallbackUpdateGainMaster
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateGainMaster()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- auto master gain ?
		INT auto_gain = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_GAIN,
				&auto_gain
			);
		if ( auto_gain )
		{
			// TBC -
			result = CallbackUpdateBlackLevel();
			return VS_OK;
		}


	// ---- get index
		INT index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_MASTER_INDEX,
				&index
			);
		if ( index == 0 )
		{
			// ignore auto mode
			return VS_OK;
		}


	// ---- get gain master
		FLOAT f_gain = 0;
		result = cam_props->GetProperty(
				CamProps::GAIN_MASTER,
				index,
				CamProps::GAIN,
				&f_gain
			);


	// ---- set

		#ifdef VS_CAM_IDS_VIDEO_SDK

		INT gain  = (INT)(f_gain  * 100 + 0.5f);
		INT nRet = is_SetHardwareGain(
				pi_cam_ids->m_hCam,
				gain,
				IS_IGNORE_PARAMETER,
				IS_IGNORE_PARAMETER,
				IS_IGNORE_PARAMETER
			);
		if ( nRet == IS_SUCCESS )
		{
			sprintf(pi_cam_ids->msg, "+++ is_SetHardwareGain: (master) %d", gain);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetHardwareGain: (master) failed!");
		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateGamma ----------
/*!
\brief CallbackUpdateGamma
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateGamma()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- note: no auto mode


	// ---- get index
		INT index = 0;
		INT result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAMMA_INDEX,
				&index
			);


	// ---- get
		FLOAT f_factor = 0; 
		result = cam_props->GetProperty(
				CamProps::GAMMA,
				index,
				CamProps::FACTOR,
				&f_factor
			);


	// ---- set

		#ifdef VS_CAM_IDS_VIDEO_SDK

		INT nGamma = (INT)(f_factor * 100 + 0.f);
		INT nRet = is_Gamma(
				pi_cam_ids->m_hCam,
				IS_GAMMA_CMD_SET,
				(void*)&nGamma,
				sizeof(nGamma)
			);
		if ( nRet == IS_SUCCESS )
		{
			sprintf(pi_cam_ids->msg, "+++ is_Gamma: %d", nGamma);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(pi_cam_ids->msg);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Gamma: failed!");
		}

		#endif

	return VS_OK;
}


// ---------- CallbackUpdateRop ----------
/*!
\brief CallbackUpdateRop
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateRop()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;


	// ---- get rop 
		INT h_flip = 0;
		INT result = cam_props->GetProperty(
				CamProps::ROP,
				0,
				CamProps::H_FLIP,
				&h_flip
			);

		INT v_flip = 0;
		result = cam_props->GetProperty(
				CamProps::ROP,
				0,
				CamProps::V_FLIP,
				&v_flip
			);


	#ifdef VS_CAM_IDS_VIDEO_SDK


	// ---- set raster operation (ROP)
		INT rop = ( h_flip ? IS_SET_ROP_MIRROR_LEFTRIGHT : 0 ) | ( v_flip ? IS_SET_ROP_MIRROR_UPDOWN : 0 );

		INT nRet = 0;

		nRet = is_SetRopEffect(
				pi_cam_ids->m_hCam,
				IS_SET_ROP_MIRROR_LEFTRIGHT,
				0,  // disable
				0
			);

		nRet = is_SetRopEffect(
				pi_cam_ids->m_hCam,
				IS_SET_ROP_MIRROR_UPDOWN,
				0,  // disable
				0
			);

		nRet = is_SetRopEffect( 
				pi_cam_ids->m_hCam,
				rop,
				1,  // enable
				0
			);


	// ---- report
		CHAR c[128];
		if (nRet == IS_SUCCESS)
		{
			sprintf(c, "is_SetRopEffect: %d", rop);
			pi_cam_ids->app_dependency->GetSysOutput()->Message(c);
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("is_SetRopEffect: failed!");
		}


	#endif

	return VS_OK;
}


// ---------- CallbackUpdateSetup ----------
/*!
\brief CallbackUpdateSetup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackUpdateSetup()
{

	// ---- NOT use cam props ?
		if ( !pi_cam_ids->use_cam_props_hdl_file ) return VS_ERROR;


	#ifdef VS_CAM_IDS_VIDEO_SDK


	// ---- local
		CamProps *cam_props = pi_cam_ids->cam_props;
		DOUBLE p1 = 0, p2 = 0;
		INT result = 0;
		INT nRet = 0;


	// ---- blacklevel (enable)
		if (pi_cam_ids->camera_id != 3)
		{

			INT nMode = IS_AUTO_BLACKLEVEL_ON;
			nRet = is_Blacklevel(
				pi_cam_ids->m_hCam,
				IS_BLACKLEVEL_CMD_SET_MODE,
				(void*)&nMode, sizeof(nMode)
			);
			if (nRet == IS_SUCCESS)
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Blacklevel: ON");
			else
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_Blacklevel: failed!");

		}

	// ---- colour (na)


	// ---- exposure
		INT auto_exposure = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_EXPOSURE,
				&auto_exposure
			);
		p1 = (DOUBLE)auto_exposure;
		nRet = is_SetAutoParameter(
				pi_cam_ids->m_hCam,
				IS_SET_ENABLE_AUTO_SHUTTER,
				&p1, // 1 == enable & 0 == disable
				&p2
			);
		if (nRet == IS_SUCCESS)
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: exposure ON");
			pi_cam_ids->app_dependency->GetSysOutput()->Message(auto_exposure ? "    Exposure is ON" : "    Exposure is OFF");
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: exposure failed!");
		}

		 
	// ---- framerate (disable)
		INT auto_framerate = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_FRAMERATE,
				&auto_framerate
			);
		p1 = 0; // auto_framerate
		nRet = is_SetAutoParameter(
				pi_cam_ids->m_hCam,
				IS_SET_ENABLE_AUTO_FRAMERATE,
				&p1, // 1 == enable & 0 == disable
				&p2
			);
		if ( nRet == IS_SUCCESS )
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: framerate SET");
		else
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: framerate failed!");


	// ---- gain
		INT auto_gain = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_GAIN,
				&auto_gain
			); 
		p1 = (DOUBLE)auto_gain;
		nRet = is_SetAutoParameter(
				pi_cam_ids->m_hCam,
				IS_SET_ENABLE_AUTO_GAIN,
				&p1, // 1 == enable & 0 == disable
				&p2
			);
		if (nRet == IS_SUCCESS)
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter:");
			pi_cam_ids->app_dependency->GetSysOutput()->Message(auto_gain ? "    Gain is ON" : "    Gain is OFF");
		}
		else
		{
			pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: gain failed!");
		}


	// ---- gamma (na)


	// ---- wb
		if (pi_cam_ids->camera_id != 3)
		{

			INT auto_wb = 0;
			result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::AUTO_WB,
				&auto_wb
			);
			p1 = (DOUBLE)auto_wb;
			nRet = is_SetAutoParameter(
				pi_cam_ids->m_hCam,
				IS_SET_ENABLE_AUTO_WHITEBALANCE,
				&p1, // 1 == enable & 0 == disable
				&p2
			);
			if (nRet == IS_SUCCESS)
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter:");
				pi_cam_ids->app_dependency->GetSysOutput()->Message(auto_wb ? "    WB is ON" : "    WB is OFF");
			}
			else
			{
				pi_cam_ids->app_dependency->GetSysOutput()->Message("+++ is_SetAutoParameter: wb failed!");
			}

		}
	
	#endif


	return VS_OK;
}


// ---------- CallbackWriteIniFile ----------
/*!
\brief CallbackWriteIniFile
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::CallbackWriteIniFile()
{

	// ---- NOT use cam props ?
		if (!pi_cam_ids->use_cam_props_hdl_file) return VS_ERROR;


	// ---- local
		vs_system::SysContext *sys_context = pi_cam_ids->app_dependency->GetSysContext();

		CHAR paf[VS_MAXCHARPATHLEN];
		sprintf(
				paf,
				#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
				"%s/AppData/Local/Observant Innovations/Monitor/data/ids_config.ini",
				#else
				"%s/AppData/Local/Observant Innovations/Patrol/data/ids_config.ini",
				#endif
				sys_context->GetChar(sys_context->PROJECT_FOLDER_PATH)
			);


	#ifdef VS_CAM_IDS_VIDEO_SDK

	// ---- LONG config path/filename
		size_t size = strlen(paf) + 1;
		wchar_t* w_paf = new wchar_t[size];
		mbstowcs(w_paf, paf, size);


	// ---- save parameters to specified file
		INT nRet = is_ParameterSet(
				pi_cam_ids->m_hCam,
				IS_PARAMETERSET_CMD_SAVE_FILE,
				w_paf,
				NULL
			);

	#endif

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////



// ---------- SetCallbackOnCaptureImage ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamIds::SetCallbackOnCaptureImage(
		INT(*callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT [])
	)
{
	pi_cam_ids->callback_on_capture_image = callback_on_capture_image;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
