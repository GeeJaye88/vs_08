////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_arkive.h ----------
/*!
\file vs_img_arkive.cpp
\brief Image Archive definitions, declarations and class interfaces
\author Gareth Edwards
*/

#pragma once


// ---------- definitions ----------

//#define IMAGE_ARCHIVE_IMMEDIATE

// #define IMAGE_ARCHIVE_REPORT          --- NOT USED
// #define IMAGE_ARCHIVE_ROI             --- NOT USED

#define IMAGE_ARCHIVE_PATHTOP   6
#define IMAGE_ARCHIVE_PATHLEN   128
#define IMAGE_ARCHIVE_MAXCHAR   256

// #define IMAGE_ARCHIVE_MAXTHREAD 20    --- NOT USED


// ---- include ----
#include <windows.h>
#include <string.h>


// ---- system ----
#include "../../vs_system/header/vs_inc_defs.h"


// ---- library ----
#include "../header/vs_vnode.h"
#include "../header/vs_csv.h"
#include "../header/vs_img_buffer.h"
#include "../header/vs_img_jpeglib.h"
#include "../header/vs_img_lut.h"
#include "../header/vs_img_col_process.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- namespace ----------
namespace vs_system
{


// ---------- ImgArkive ----------
class ImgArkive
{

public:

	// ---- enumerated archive modes ( default is SINGLE )
		enum ArchiveMode {
				SINGLE  = 1,
				DAILY   = 2,
				RUNTIME = 3
			};


	// ---- enumerated archive parameters
		enum ArchiveDefaultParam {
				MODE        = RUNTIME,
				JPG_QUALITY = 80,
				MAX_STACK   = 8,
			};


	// --- cdtor
		ImgArkive();
		virtual ~ImgArkive();


	// ---- get
		INT  GetFolderPath(CHAR *);
		INT  GetSubFolderPath(CHAR *);
		BOOL GetSave();
		WORD GetMaxStack();
		WORD GetMode();
		WORD GetQuality();


	// ---- set
		INT SetMaxStack(WORD);
		INT SetSave(BOOL);
		INT SetConfigPath(CHAR *);
		INT SetFolderPath(CHAR *);
		INT SetStackExtra(WORD);
		INT SetMode(WORD);
		INT SetQuality(WORD);


	// ---- framework
		INT ReadSetup(CHAR *);
		INT WriteSetup();
		INT Cleanup();

	
	// ---- setup
		INT Setup_PanConfigHdlFile();

		INT Setup_Buffers(

				#if VS_LIDAR
				UINT csv_rows,
				UINT csv_columns,
				UINT csv_rows_per_record,
				#endif

				WORD width,
				WORD height,
				WORD bpp
			);

		INT Setup_Csv(
				#if VS_LIDAR
				UINT csv_rows,
				UINT csv_columns,
				UINT csv_rows_per_record
				#endif
			);


	// ---- read
		INT  Read(
					UINT img_index,
					std::string &filename
				);

		INT  Read_ManageImageBuffer(
					ImgBuffer *img_buffer,
					WORD index
				);

		INT  Read_ManageCsvBuffer(
					UINT csv_rows,
					UINT csv_columns,
					UINT csv_rows_per_record,
					WORD index
				);

		BOOL Read_IsReady(
					UINT index,
					std::string &filename,
					ImgBuffer *img_buffer
				);

		static DWORD WINAPI Read_Buffer(
					LPVOID lpParam
				);

		INT Read_AccessBuffer(
					UINT img_index,
					ImgBuffer *read_img_buffer
					#if VS_LIDAR
					,
					CsvObject *read_csv_buffer
					#endif
				);

		INT Read_SetFxBoostMode(UINT);

		INT Read_SetFxColourMode(UINT);


	// ---- write
		INT Write(
					std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
					std::string &filename,
					WORD  width,
					WORD  height,
					WORD  bpp,
					BYTE *byte_data
				);

		INT Write_CopyImage(
					WORD  t,
					WORD  width,
					WORD  height,
					WORD  bpp,
					BYTE *byte_data
				);

		static DWORD Write_CreateDirectory(
					std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
					std::string &filename
				);

		static DWORD WINAPI Write_Buffer(
					LPVOID lpParam
				);

	// ---- housekeeping
		WORD CountProcessorCores();


	// ---- write - not used
		INT WriteInitPathFilename(
				std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
				std::string &filename,
				INT *timestamp
			);


private:

	// ---- private implementation
		class Props; Props *props;

};


} // namespace
