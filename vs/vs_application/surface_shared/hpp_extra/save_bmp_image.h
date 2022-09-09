////////////////////////////////////////////////////////////////////////////////

// ---------- save_bmp_image.h ----------
/*!
\file save_bmp_image.h
\brief Extra feaures: save a bmp image
\author Gareth Edwards
*/


// ---- add to the include ".h" file
/*

	#include "../../../vs_library/header/vs_img_capture.h"

*/


// ---- add to the ".h" file
/*

	// ---- objects pointers managed by app private implmentation (pimp)
		vs_system::ImgCapture *img_capture = NULL;
		UINT  img_capture_frame_number = 0;
		UINT  img_capture_event_id     = 666;
		FLOAT img_capture_elapsed_ms   = 0;

*/


// ---- add to the pimp destructor method in ".cpp" file
/*

// ---- delete objects managed by app pimp
		if (img_capture != NULL) { delete model;   img_capture = NULL; }

*/


// ---- add to the Setup method in ".cpp" file
/*

	// ---- setup image capture
		pi_application->img_capture = new vs_system::ImgCapture();
		result = pi_application->img_capture->Setup(
					app_dependency,
					"image",
					pi_application->img_capture_event_id
				);

		pi_application->model->SetImgCaptureEventId(
					pi_application->img_capture_event_id
				);

*/


// ---- add to the Display method in ".cpp" file
/*

	// ---- capture and save part of screen ?
		result = pi_application->img_capture->Capture();

*/
