////////////////////////////////////////////////////////////////////////////////

// ---------- r2_empty.h ----------
/*!
\file r2_empty.h
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_application.h"

using namespace review_02;


// ---------- Panophoto ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT application::Panophoto(vs_system::ImgBuffer *image)
{

	// ---- local
		INT result = 0;
		CHAR msg[VS_MAXCHARLEN];
		vs_system::ObjConfig *obj_config = model->GetObjConfig();


	// ---- pano
		FLOAT min_angle = 0;
		FLOAT max_angle = 0;
		result = obj_config->GetPanoAngles(
				&min_angle,
				&max_angle
			);

		FLOAT min_radius = 0.5;
		FLOAT max_radius = 1;
		result = obj_config->GetPanoRadii(
				&min_radius,
				&max_radius
			);

		FLOAT u_offset = 0;
		FLOAT v_offset = 0;
		result = obj_config->GetPanoUVOffset(
				&u_offset,
				&v_offset
			);


	// ---- annular
		UINT annular_width = image->GetWidth();
		UINT annular_height = image->GetHeight();
		UINT bytes_per_pixel = image->GetBytes();


	// ---- initialise annular image buffer same size as source
		vs_system::ImgBuffer annular_image;
		annular_image.Initialise(
				annular_width,
				annular_height,
				bytes_per_pixel
			);


	// ---- inject source annular buffer byte data
		annular_image.SetBuffer(image->GetBuffer());


	// ---- image process ----
		vs_system::ImgProcess img_process;


	// ---- calculate panorama dimensions
		INT panorama_height = 0;
		INT panorama_width = 0;
		if ( pi_props->panophoto_fullsize )
		{
			INT ok = img_process.GetPanoDimensions(
					(INT)annular_width,
					pi_props->panophoto_basis,
					min_angle,
					max_angle,
					min_radius,
					max_radius,
					&panorama_width,
					&panorama_height
				);
		}
		else
		{
			panorama_width = (INT)pi_props->panophoto_width;
			panorama_height =
				img_process.GetPanoHeight(
					panorama_width,
					min_angle,
					max_angle
				);

		}
		sprintf(msg, "PANOPHOTO: Output dimensions are %d x %d.",
				panorama_width,
				panorama_height
			);
		pi_props->sys_output->Message(msg);


	// ---- initialise panorama		
		vs_system::ImgBuffer panorama_image;
		panorama_image.Initialise(
				panorama_width,
				panorama_height,
				3
			);
		panorama_image.CreateBuffer();


	// ---- process
		pi_props->sys_output->Message("PANOPHOTO: Processing.");
		INT res = img_process.PanoFromAnnular(
				&panorama_image,
				u_offset,
				v_offset,
				min_radius,
				max_radius,
				pi_props->panophoto_sample,
				&annular_image
			);
		res = img_process.SwapRB(&panorama_image);
		res = img_process.Luminance(&panorama_image, 1.2f, 1);


	// ---- create path/filename
		//
		// use only the last image archived filename
		//
		CHAR   filename[VS_MAXCHARLEN];
		CHAR *pfilename = filename;
		strcpy(filename, pi_props->archive_read_filename.c_str());
		INT len = strlen(filename);
		for (UINT i = len; i >= 0; i--)
		{
			if (filename[i] == '/' || filename[i] == '\\')
			{
				pfilename = &filename[i + 1];
				break;
			}
		}


	// ---- use: "pi_props->app_archive_paf" initialised at runtime
		CHAR   paf[VS_MAXCHARLEN];
		CHAR *ppaf = paf;
		sprintf(paf, "%s/%s", pi_props->app_archive_paf, pfilename);


	// ---- (sharpen &) save
		BOOL sharpen = TRUE;
		if ( sharpen )
		{
			vs_system::ImgBuffer filter_image;
			filter_image.Initialise(
					panorama_width,
					panorama_height,
					3
				);
			filter_image.CreateBuffer();

			DOUBLE z = pi_props->panophoto_sharpness;
			DOUBLE filter[] = {
				 0,        -z,  0,
				-z, 1 + 4 * z, -z,
				 0,        -z,  0
			};

			res = img_process.Filter(&filter_image,
					1.0f,
					filter,
					3,
					&panorama_image
				);
			pi_props->sys_output->Message("PANOPHOTO: Sharpened.");

			// ---- save
				vs_system::ImgJpegLib jlib;
				filter_image.SetQuality(95);
				jlib.WriteToDisk(&filter_image, ppaf);
					sprintf(msg, "PANOPHOTO: Saved %s.", ppaf);
					pi_props->sys_output->Message(msg);

				filter_image.DestroyBuffer();

		}
		else
		{
			// ---- save
				vs_system::ImgJpegLib jlib;
				panorama_image.SetQuality(95);
				jlib.WriteToDisk(&panorama_image, ppaf);
					sprintf(msg, "PANOPHOTO: Saved %s.", ppaf);
					pi_props->sys_output->Message(msg);
		}


	// ---- cleanup
		panorama_image.DestroyBuffer();


	return VS_OK;
}


// ---------- CreatePanophotoThread ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT application::CreatePanophotoThread(vs_system::ImgBuffer *annular_image)
{

	// ---- pano photo parameters ----
		pi_panophoto_data.fullsize  = pi_props->panophoto_fullsize;
		pi_panophoto_data.basis     = pi_props->panophoto_basis;
		pi_panophoto_data.gamma     = pi_props->panophoto_gamma;
		pi_panophoto_data.sharpness = pi_props->panophoto_sharpness;
		pi_panophoto_data.sample    = pi_props->panophoto_sample;
		pi_panophoto_data.width     = pi_props->panophoto_width;


	// ---- pano photo image ----
		pi_panophoto_data.annular_image.Initialise(
				annular_image->GetWidth(),
				annular_image->GetHeight(),
				annular_image->GetBytes()
			);
		pi_panophoto_data.annular_image.CreateBuffer();
		memcpy(
				pi_panophoto_data.annular_image.GetBuffer(),
				annular_image->GetBuffer(),
				pi_panophoto_data.annular_image.GetExtent()
			);


	// ---- annular archive path/filename
		strcpy(pi_panophoto_data.filename, pi_props->archive_read_filename.c_str());
		strcpy(pi_panophoto_data.path, pi_props->app_archive_paf);


	// ---- pano model
		vs_system::ObjConfig *obj_config = model->GetObjConfig();

		pi_panophoto_data.min_angle = 0;
		pi_panophoto_data.max_angle = 0;
		obj_config->GetPanoAngles(
				&pi_panophoto_data.min_angle,
				&pi_panophoto_data.max_angle
			);

		pi_panophoto_data.min_radius = 0.5;
		pi_panophoto_data.max_radius = 1;
		obj_config->GetPanoRadii(
				&pi_panophoto_data.min_radius,
				&pi_panophoto_data.max_radius
			);

		pi_panophoto_data.u_offset = 0;
		pi_panophoto_data.v_offset = 0;
		obj_config->GetPanoUVOffset(
				&pi_panophoto_data.u_offset,
				&pi_panophoto_data.v_offset
			);


	// ---- create pano thread
		pi_panophoto_data.thread_complete = FALSE;
		pi_props->pano_thread_handle =
			CreateThread(NULL,
				0,
				application::CreatePanophoto,
				(LPVOID)0,
				0,
				0
			);

	return VS_OK;
}


// ---------- CreatePanophoto ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
DWORD WINAPI application::CreatePanophoto(
		LPVOID lpParam
	)
{

	// ---- annular
		UINT annular_width  = pi_panophoto_data.annular_image.GetWidth();
		UINT annular_height = pi_panophoto_data.annular_image.GetHeight();


	// ---- image process
		vs_system::ImgProcess img_process;


	// ---- calculate panorama dimensions
		INT panorama_height = 0;
		INT panorama_width = 0;

		if ( pi_panophoto_data.fullsize )
		{
			INT ok = img_process.GetPanoDimensions(
					(INT)annular_width,
					pi_panophoto_data.basis,
					pi_panophoto_data.min_angle,
					pi_panophoto_data.max_angle,
					pi_panophoto_data.min_radius,
					pi_panophoto_data.max_radius,
					&panorama_width,
					&panorama_height
				);
		}
		else
		{
			panorama_width = (INT)pi_panophoto_data.width;
			panorama_height =
				img_process.GetPanoHeight(
					pi_panophoto_data.width,
					pi_panophoto_data.min_angle,
					pi_panophoto_data.max_angle
				);

		}
		/*
		printf("PANOPHOTO: Output dimensions are %d x %d.\n",
				panorama_width,
				panorama_height
			);
		*/


	// ---- create panorama image
		vs_system::ImgBuffer panorama_image;
		panorama_image.Initialise(
				panorama_width,
				panorama_height,
				3
			);
		panorama_image.CreateBuffer();


	// ---- process
		// printf("PANOPHOTO: Processing.\n");
		INT res = img_process.PanoFromAnnular(
				&panorama_image,
				pi_panophoto_data.u_offset,
				pi_panophoto_data.v_offset,
				pi_panophoto_data.min_radius,
				pi_panophoto_data.max_radius,
				pi_panophoto_data.sample,
				&pi_panophoto_data.annular_image
			);
		res = img_process.SwapRB(&panorama_image);
		res = img_process.Luminance(&panorama_image, 1.2f, 1);


	// ---- create path/filename
		CHAR *pfilename = pi_panophoto_data.filename;
		INT len = strlen(pi_panophoto_data.filename);
		for ( UINT i = len; i >= 0; i-- )
		{
			if (pi_panophoto_data.filename[i] == '/' || pi_panophoto_data.filename[i] == '\\')
			{
				pfilename = &pi_panophoto_data.filename[i + 1];
				break;
			}
		}


	// ---- from "pi_props->app_archive_paf" initialised at runtime
		CHAR  paf[VS_MAXCHARLEN];
		CHAR *ppaf = paf;
		sprintf(paf, "%s/%s", pi_panophoto_data.path, pfilename);


	// ---- (sharpen &) save
		BOOL sharpen = FALSE;
		if ( sharpen )
		{
			vs_system::ImgBuffer filter_image;
			filter_image.Initialise(
					panorama_width,
					panorama_height,
					3
				);
			filter_image.CreateBuffer();

			DOUBLE z = pi_panophoto_data.sharpness;
			DOUBLE filter[] = {
				0, -z, 0,
				-z, 1 + 4 * z, -z,
				0, -z, 0
			};

			res = img_process.Filter(&filter_image,
					1.0f,
					filter,
					3,
					&panorama_image
				);
			//printf("PANOPHOTO: Sharpened.\n");


			// ---- save filter image
				vs_system::ImgJpegLib jlib;
				filter_image.SetQuality(95);
				jlib.WriteToDisk(&filter_image, ppaf);
				//printf("PANOPHOTO: Saved filter image %s.\n", ppaf);


			// ---- destroy filter image
				filter_image.DestroyBuffer();

		}
		else
		{
			// ---- save
				vs_system::ImgJpegLib jlib;
				panorama_image.SetQuality(95);
				jlib.WriteToDisk(&panorama_image, ppaf);
				//printf("PANOPHOTO: Saved image %s.\n", ppaf);
		}


	// ---- destroy panorama image
		panorama_image.DestroyBuffer();


	// ---- completed
		pi_panophoto_data.thread_complete = TRUE;

	return VS_OK;
}


