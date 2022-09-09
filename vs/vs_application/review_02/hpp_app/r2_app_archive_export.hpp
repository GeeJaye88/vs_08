////////////////////////////////////////////////////////////////////////////////

// ---------- r2_app_archive_export.hpp ----------
/*!
\file r2_app_archive_export.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- CreateArchiveExportThread ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT application::CreateArchiveExportThread()
{
	// ---- local ----
	INT result = VS_OK;


	// ---- initialise ----
	pi_props->archive_export_data.application     = this;
	pi_props->archive_export_data.thread_complete = FALSE;
	pi_props->archive_export_data.total_files     = 0;
	pi_props->archive_export_data.total_processed = 0;
	for (UINT i = 0; i < 2; i++)
	{
		for (UINT index = 1; index < DATETIME_MAX_STACK; index++)
		{
			result = pi_props->archive_export_data.clip_datetime[i].Set(
					index - 1,
					pi_props->clip_datetime_stack[i][index]
				);
		}
	}


	// note: button will remain off (clunked)
	//       until thread_complete = TRUE
	pi_props->archive_export_data.button_on = FALSE;


	// ---- go ----
	pi_props->pano_thread_handle =
		CreateThread(NULL,
			0,
			application::ArchiveExport,
			(LPVOID)this,
			0,
			0
		);

	return VS_OK;
}


// ---------- ArchiveExport ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
DWORD WINAPI application::ArchiveExport(
		LPVOID lpParam
	)
{

	// ---- local ----
	application *application = (review_02::application *)lpParam;
	ArchData *archive_export_data = &application->pi_props->archive_export_data;


	// ---- clip ----
	archive_export_data->
		arch_clip.
			CopyClip
				(
					&archive_export_data->clip_datetime[0],
					&archive_export_data->clip_datetime[1],
					application->pi_props->archive_paf,
					&archive_export_data->total_files,
					&archive_export_data->total_processed
				);


	// ---- completed ----
	archive_export_data->total_files = 0;
	archive_export_data->total_processed = 0;
	archive_export_data->thread_complete = TRUE;

	return VS_OK;
}


