////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_arkive.cpp ----------
/*!
\file vs_img_arkive.cpp
\brief Implementation of the Image Archive class
\author Gareth Edwards
*/


// ---- include
#include "../header/vs_img_arkive.h"


// ---- disable ms deprecation messages ----
#pragma warning (disable : 4996)


////////////////////////////////////////////////////////////////////////////////


// ---------- namespace ----------
using namespace vs_system;


// ---------- ImgArkiveItem ----------
class ImgArkiveItem
{

public:


	// ---- cdtor
		ImgArkiveItem() {};
		virtual ~ImgArkiveItem() {};


	// ---- props
		BOOL   status = TRUE;  // flag'd FALSE when in use

		WORD   width  = 0;     // image
		WORD   height = 0;
		WORD   bpp    = 0;

		#if VS_LIDAR
		UINT  csv_rows    = 0; // csv
		UINT  csv_columns = 0;
		UINT  csv_rows_per_record = 1;
		#endif

		HANDLE handle = 0;     // thread handle
		DWORD  index  = 0;     // stack indice
		DWORD  lid    = 0;     // logical id


	// ---- path components
		std::string s_path[IMAGE_ARCHIVE_PATHTOP];
		std::string s_filename;


	// ---- image buffer properties
		vs_system::ImgBuffer *img_buffer = NULL;
		BOOL img_buffer_created = FALSE;


	// ---- lidar buffer properties
		#if VS_LIDAR
		vs_system::CsvObject *csv_buffer = NULL;
		BOOL csv_buffer_created = FALSE;
		#endif

};


// ---------- private implementation Props class----------
class ImgArkive::Props
{
public:

	// ---- cdtor
		Props();
		virtual ~Props() {};


	// ---- archive properties
		BOOL  save_image  = FALSE;
		BOOL  run_time    = TRUE;
		WORD  extra_stack = ArchiveDefaultParam::MAX_STACK;
		WORD  max_stack   = 0;
		WORD  mode        = ArchiveDefaultParam::MODE;
		WORD  quality     = ArchiveDefaultParam::JPG_QUALITY;
		DWORD write_fail  = 0;
		DWORD read_fail   = 0;


	// ---- archive text
		std::string s_archive_path;
		std::string s_archive_folder_path; 
		std::string s_archive_sub_folder_path;


	// ---- configuration HDL
		std::string s_config_hdl_filepath;
		vs_system::VNode *config_hdl = NULL;


	// ---- fx
		vs_system::ImgLutProcess *img_lut;
		UINT fx_mode  = 0;
		UINT fx_boost = 0;


	// ---- csv
		#if VS_LIDAR
		UINT  csv_rows    = 0;
		UINT  csv_columns = 0;
		UINT  csv_rows_per_record = 1;
		#endif

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Props Constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
vs_system::ImgArkive::Props::Props()
{

	// --- props
		save_image  = FALSE;
		run_time    = TRUE;
		max_stack   = ArchiveDefaultParam::MAX_STACK;
		extra_stack = 0;
		mode        = ArchiveDefaultParam::MODE;
		quality     = ArchiveDefaultParam::JPG_QUALITY;
		write_fail  = 0;
		read_fail   = 0;


	// ---- archive
		s_archive_path = "";
		s_archive_folder_path = "";
		s_archive_sub_folder_path = "";


	// ---- config HDL
		s_config_hdl_filepath = "";
		config_hdl = NULL;


	// ---- fx
		img_lut = new vs_system::ImgLutProcess;
		fx_mode = 0;
		fx_boost = 0;


	// ---- csv
		#if VS_LIDAR
		csv_rows    = 0;
		csv_columns = 0;
		csv_rows_per_record = 1;
		#endif

}


////////////////////////////////////////////////////////////////////////////////


// ---------- global ----------
ImgArkiveItem g_stack[ vs_system::ImgArkive::ArchiveDefaultParam::MAX_STACK ];


// ---------- constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
vs_system::ImgArkive::ImgArkive()
{

	// ---- create props
		props = new Props();


	// ---- initialise stack
		for ( WORD s=0; s<ArchiveDefaultParam::MAX_STACK; s++ )
		{

			g_stack[s].status = FALSE;
			g_stack[s].width  = 0;
			g_stack[s].height = 0;
			g_stack[s].bpp    = 0;
			g_stack[s].handle = NULL;
			g_stack[s].index  = (DWORD)s;
			g_stack[s].lid    = 0;

			g_stack[s].s_filename.assign("");
			for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
				g_stack[s].s_path[i].assign("");

			g_stack[s].img_buffer = NULL;
			g_stack[s].img_buffer_created = FALSE;

			#if VS_LIDAR
			g_stack[s].csv_rows = 0;
			g_stack[s].csv_columns = 0;
			g_stack[s].csv_rows_per_record = 1;
			g_stack[s].csv_buffer = NULL;
			g_stack[s].csv_buffer_created = FALSE;
			#endif

		}
}


// ---------- destructor ----------
/*!
\brief destructor
\author Gareth Edwards
*/
ImgArkive::~ImgArkive()
{
	if ( props != NULL ) { delete props; props = NULL; }
}


// ---------- GET ----------

INT vs_system::ImgArkive::GetFolderPath(CHAR *folder_path)
	{
		if ( props->s_archive_folder_path == "" )
		{
			strcpy(folder_path, "");
			return VS_ERROR;
		}
		else
		{
			strcpy(folder_path, props->s_archive_folder_path.c_str());
		}
		return VS_OK;
	}


INT ImgArkive::GetSubFolderPath(CHAR *sub_folder_path)
	{
		if ( props->s_archive_sub_folder_path == "" )
		{
			strcpy(sub_folder_path, "");
			return VS_ERROR;
		}
		else
		{
			strcpy(sub_folder_path, props->s_archive_sub_folder_path.c_str());
		}
		return VS_OK;
	}

BOOL ImgArkive::GetSave()     { return props->save_image;  }
WORD ImgArkive::GetMaxStack() { return props->max_stack;   }
WORD ImgArkive::GetMode()     { return props->mode;        }
WORD ImgArkive::GetQuality()  { return props->quality;     }


// ---------- SET ----------


INT ImgArkive::SetMaxStack(WORD ms)
	{
		props->max_stack = ms;
		return VS_OK;
	}

INT ImgArkive::SetSave(BOOL s)
	{
		props->save_image  = s;
		return VS_OK;
	}

INT ImgArkive::SetConfigPath(CHAR *config_hdl_filepath)
	{
		props->s_config_hdl_filepath.assign(config_hdl_filepath);
		return VS_OK;
	}

INT ImgArkive::SetFolderPath(CHAR *archive_path)
	{
		props->s_archive_path.assign(archive_path);
		return VS_OK;
	}

INT ImgArkive::SetStackExtra(WORD ms)
	{
		props->extra_stack = ms < 0 ? 0 : ms;
		return VS_OK;
	}

INT ImgArkive::SetMode(WORD m)
	{
		switch ( m )
		{
			case ArchiveMode::SINGLE:
			case ArchiveMode::DAILY:
			case ArchiveMode::RUNTIME:
				props->mode = m;
				break;
			default:
				props->mode = ArchiveDefaultParam::MODE;
				return VS_ERROR;
		}
		return VS_OK;
	}

INT ImgArkive::SetQuality(WORD c)
	{
		props->quality = c <= 100 ? c : c > 20 ? c : 20;
		return VS_OK;
	}


// ---------- ReadSetup ----------
/*!
\brief Configure
\author Gareth Edwards
*/
INT ImgArkive::ReadSetup(
		CHAR *app_setup_config_paf
	)
{

	// ---- read config fle
		vs_system::VNodeIO vio;
		props->config_hdl = vio.Read(app_setup_config_paf);


		#ifdef OS_WINDOWS
			if ( props->config_hdl == NULL )
			{
				INT result = ::MessageBoxA(
						NULL,
						"FAILED to READ the archive configuration parameters file.\n\n"
						"Please select OK to continue.",
						"VS ERROR",
						MB_OK | MB_ICONEXCLAMATION
					);
				return VS_ERROR;
			}
		#endif


	// ---- parse config data
		if ((INT)props->config_hdl != VS_ERROR)
		{
			// local
			vs_system::VNode *p = NULL;

			// ---- "Archive" config data ?
			if ( props->config_hdl->SearchName("Archive") )
			{
				if (p = props->config_hdl->SearchName("Compression"))
					props->quality     = p->GetIntFromValue();
				if (p = props->config_hdl->SearchName("Extra Threads"))
					props->extra_stack = p->GetIntFromValue();
			}
		}


	// ---- maximum stack (thread) size to # of processor cores 
		WORD cores = CountProcessorCores();
		switch (cores)
		{
			// limit excessive stack size - only so many images from camera!!
			case 16:
			case 12:
				props->max_stack = ArchiveDefaultParam::MAX_STACK;
				break;
			case  8:
				props->max_stack = 16;
				break;
			case  4:
				props->max_stack = 8; 
				break;
			case 2:
			default:
				{
					char err[VS_MAXCHARLEN];
					sprintf_s(
							err,
							VS_MAXCHARLEN,
							"ERROR:\n\nUnsupported # of Processor Cores !\n\n"
							"There are only %d Processor Cores.\n"
							"A minimum of 4 is required for this application.\n\n"
							"Do you want to continue?",
							cores
						);
					INT result = ::MessageBoxA(
							NULL,
							err,
							"VS_ERROR",
							MB_YESNO | MB_ICONEXCLAMATION
						);
					if (result == IDNO) exit(VS_ERROR);
				}
				break;
		}


	// ---- add extra stack
		props->max_stack = props->extra_stack + props->max_stack;
		props->max_stack =
			props->max_stack < ArchiveDefaultParam::MAX_STACK
				? props->max_stack
					: ArchiveDefaultParam::MAX_STACK;

	return VS_OK;
}


// ---------- WriteSetup ----------
/*!
\brief framework write setup 
\author Gareth Edwards
*/
INT ImgArkive::WriteSetup()
{

	// ---------- archive folder path ----------


	// IF -- no folder path -- THEN --
		if ( props->s_archive_path == "" )
		{
			INT result = ::MessageBoxA(
					NULL,
					"Error: Archive path has not been declared.",
					"VS ImageArchive Error",
					MB_OK | MB_ICONEXCLAMATION
				);
			return VS_ERROR;
		}


	// ---- copy folder path string into char array
		CHAR archive_path[VS_MAXCHARPATHLEN];
		strcpy(archive_path, props->s_archive_path.c_str());


	// IF -- archive folder path does not exist -- THEN --
		DWORD handle_archive_path = GetFileAttributesA(archive_path);
		if ( handle_archive_path == INVALID_FILE_ATTRIBUTES )
		{
			CHAR message[VS_MAXCHARPATHLEN];
			sprintf(
					message,
					"ERROR: Archive path %s not found!\n\nPlease check, fix and restart.",
					archive_path
				);
			INT result = ::MessageBoxA(
					NULL,
					message,
					"VS ImageArchive Error",
					MB_OK | MB_ICONEXCLAMATION
				);
			return VS_ERROR;
		}

	
	// ---------- archive folder name ----------

	// --- depends on archiving mode...
		switch ( props->mode )
		{

			case ArchiveMode::SINGLE:
				{
					props->s_archive_folder_path = props->s_archive_path + "00000000_000000_Archive";
				}
				break;

			case ArchiveMode::DAILY:
				{
					//
					// "[archive name][year][month]_[day][hour][min]_Archive"
					//
					SYSTEMTIME LocalTime;
					GetLocalTime(&LocalTime);  // NOT GetSystemTime(&SystemTime);

					char ps[4][IMAGE_ARCHIVE_PATHLEN];
					sprintf(ps[0], "%s", "archive");
					sprintf(ps[1], "%4d", LocalTime.wYear);
					sprintf(ps[2], "%2d", LocalTime.wMonth);
					sprintf(ps[3], "%2d", LocalTime.wDay);


					// ---- tidy ----
					for (UINT i = 1; i < 4; i++)
					{
						UINT len = strlen(ps[0]);
						for (UINT j = 0; j<len; j++)
							ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
					}


					// ---- create and store archive folder path ----
					sprintf(  ps[0],
								"%s%s%s%s_000000_Archive",
								archive_path,
								ps[1],
								ps[2],
								ps[3]
							);
					props->s_archive_folder_path.assign(ps[0]);
				}
				break;

			default:
				// Ooooopsssss...... can't reach here!!!
				//
				// But default to...

			case ArchiveMode::RUNTIME:
				{
					//
					// "[archive name][year][month]_[day][hour][min]_Archive"
					//
					SYSTEMTIME LocalTime;
					GetLocalTime(&LocalTime);  // NOT GetSystemTime(&SystemTime);

					char ps[7][IMAGE_ARCHIVE_PATHLEN];
					sprintf(ps[0], "%s", "archive");
					sprintf(ps[1], "%4d", LocalTime.wYear);
					sprintf(ps[2], "%2d", LocalTime.wMonth);
					sprintf(ps[3], "%2d", LocalTime.wDay);
					sprintf(ps[4], "%2d", LocalTime.wHour);
					sprintf(ps[5], "%2d", LocalTime.wMinute);
					sprintf(ps[6], "%2d", LocalTime.wSecond);


					// ---- tidy ----
					for (UINT i = 1; i < 7; i++)
					{
						UINT len = strlen(ps[0]);
						for (UINT j = 0; j<len; j++)
							ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
					}


					// ---- ceate and store archive folder path ----
					sprintf(   ps[0],
								"%s%s%s%s_%s%s%s_Archive",
								archive_path,
								ps[1],
								ps[2],
								ps[3],
								ps[4],
								ps[5],
								ps[6]
							);
					props->s_archive_folder_path.assign(ps[0]);
				}
				break;
		}


	// ---- create and store archive sub folder path ----
		props->s_archive_sub_folder_path = props->s_archive_folder_path + "/archive/";


	// ---------- config file ----------


	// IF -- no config hdl file path -- THEN --
		if ( props->s_config_hdl_filepath == "" )
		{
			INT result = ::MessageBoxA(
						NULL,
						"Error: Config file path has not been declared.",
						"VS ImageArchive Error",
						MB_OK | MB_ICONEXCLAMATION
					);
			return VS_ERROR;
		}


	// ---- copy config file path string into char array ----
		CHAR config_hdl_filepath[VS_MAXCHARPATHLEN];
		strcpy(config_hdl_filepath, props->s_config_hdl_filepath.c_str());


	// IF -- config file does not exist -- THEN --
		DWORD handle_config_hdl_filepath = GetFileAttributesA(config_hdl_filepath);
		if ( handle_config_hdl_filepath == INVALID_FILE_ATTRIBUTES )
		{
			CHAR message[VS_MAXCHARPATHLEN];
			sprintf(
						message,
						"ERROR: Config file %s not found!\n\nPlease check, fix and restart.",
						config_hdl_filepath
					);
			INT result = ::MessageBoxA(
						NULL,
						message,
						"VS ImageArchive Error",
						MB_OK | MB_ICONEXCLAMATION
					);
			return VS_ERROR;
		}
	

	// ---------- max stack size ----------


	// ---- match size to # of processor cores 
		WORD cores = CountProcessorCores();
		WORD max_stack = 8;
		switch (cores)
		{
			case 16:
			case 12: max_stack = ArchiveDefaultParam::MAX_STACK;
				break;
			case  8:
				max_stack = 16;
				break;
			case  4:
				max_stack = 8;
				break;
			default:
				{
					char err[VS_MAXCHARLEN];
					sprintf_s(
								err,
								VS_MAXCHARLEN,
								"ERROR:\n\nUnsupported # of Processor Cores !\n\n"
								"There are only %d Processor Cores.\n"
								"A minimum of 4 is required for this application.\n\n"
								"Do you want to continue?",
								cores
							);
					INT result = ::MessageBox(
								0,
								err,
								"VS ImageArchive Error",
								MB_YESNO | MB_ICONEXCLAMATION
							);
					if (result == IDNO) exit(VS_ERROR);
				}
				break;
		}


	// ---- max_stack
		props->max_stack = props->extra_stack + max_stack;
		props->max_stack =
			props->max_stack < ArchiveDefaultParam::MAX_STACK
				? props->max_stack
				: ArchiveDefaultParam::MAX_STACK;


	// ---- quality
		props->quality =
			props->quality < 10 ? 10 :
				(
					props->quality > 100 ? 100 :
					props->quality
				);

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief framework cleanup 
\author Gareth Edwards
*/
INT ImgArkive::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Setup_Buffers ----------
/*!
\brief
\author Gareth Edwards
*/
int ImgArkive::Setup_Buffers(

		#if VS_LIDAR
		UINT  csv_rows,
		UINT  csv_columns,
		UINT  csv_rows_per_record,
		#endif

		WORD  width,
		WORD  height,
		WORD  bpp
	)
{


	// ---- local
		INT result = 0;


	// ---- store csv param
		#if VS_LIDAR
		props->csv_rows = csv_rows;
		props->csv_columns = csv_columns;
		props->csv_rows_per_record = csv_rows_per_record;
		#endif


	// ---- stack
		for (WORD s = 0; s < ArchiveDefaultParam::MAX_STACK; s++)
		{

			// ---- image
				g_stack[s].width  = width;
				g_stack[s].height = height;
				g_stack[s].bpp    = bpp;

				g_stack[s].img_buffer_created = TRUE;

				g_stack[s].img_buffer = new ImgBuffer(width, height, bpp);
				result = g_stack[s].img_buffer->CreateBuffer();


			// ---- lidar
				#if VS_LIDAR

				g_stack[s].csv_rows    = props->csv_rows;
				g_stack[s].csv_columns = props->csv_columns;
				g_stack[s].csv_rows_per_record = props->csv_rows_per_record;
				g_stack[s].csv_buffer_created = TRUE;

				g_stack[s].csv_buffer = new CsvObject();
				result = g_stack[s].csv_buffer->Initialise(props->csv_rows, props->csv_columns);
				result = g_stack[s].csv_buffer->CreateBuffer();

				#endif

			/*
			if (result == VS_ERROR)
			{
				props->max_stack = s - 1;
				g_stack[s].img_buffer_created = FALSE;
				return VS_ERROR;
			}
			*/
		
		}

	return VS_OK;
}


// ---------- Setup_Csv ----------
/*!
\brief
\author Gareth Edwards
*/
int ImgArkive::Setup_Csv(
	#if VS_LIDAR
	UINT  csv_rows,
	UINT  csv_columns,
	UINT  csv_rows_per_record
	#endif
)
{

	// ---- store csv param
		#if VS_LIDAR
		props->csv_rows = csv_rows;
		props->csv_columns = csv_columns;
		props->csv_rows_per_record = csv_rows_per_record;
		#endif

	return VS_OK;
}


// ---------- Setup_PanConfigHdlFile ----------
/*!
\brief
\author Gareth Edwards
*/
int ImgArkive::Setup_PanConfigHdlFile()
{

	// ---- archive folder
		INT result = mkdir(props->s_archive_folder_path.c_str());
		// TBD Return value ignored: 'mkdir'.


	// ---- archive sub folder
		result = mkdir(props->s_archive_sub_folder_path.c_str());
		// TBD Return value ignored: 'mkdir'.


	// ---- read & write config file
		if ( props->s_config_hdl_filepath.c_str()  != NULL )
		{

			CHAR config_old_file_path[VS_MAXCHARPATHLEN];
			sprintf(config_old_file_path, "%s", props->s_config_hdl_filepath.c_str());

			VNodeIO vnio;
			VNode *config = vnio.Read( config_old_file_path );
			if ( config )
			{
				CHAR config_new_file_path[VS_MAXCHARPATHLEN];
				sprintf(config_new_file_path, "%s/archive.hdl", props->s_archive_folder_path.c_str());
				vnio.Write(config_new_file_path, config);
			}
			else
			{
				INT result = ::MessageBoxA(NULL,
					"ERROR: Read config file failed!\n\nPlease check, fix and restart.",
					"VS ImageArchive Error",
					MB_OK | MB_ICONEXCLAMATION);
				return VS_ERROR;
			}
		}
		else
		{
			INT result = ::MessageBoxA(NULL,
				"ERROR: No archive pano config file!\n\nPlease check, fix and restart.",
				"VS ImageArchive Error",
				MB_OK | MB_ICONEXCLAMATION);
			return VS_ERROR;
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Read ----------
/*!
\brief queue an image to read in archive
\author Gareth Edwards
*/
INT ImgArkive::Read(UINT lid, std::string &filename)
{

	// --- find free slot
	for ( WORD s = 0; s<props->max_stack; s++ )
	{

		// IF -- free -- THEN
		if ( g_stack[s].handle == NULL )
		{

			// ---- get image file header
				ImgBuffer header_image;
				ImgJpegLib img_jpeg_lib;
				CHAR temp[VS_MAXCHARPATHLEN];
				strcpy(temp, filename.c_str());


			// ---- image file does NOT exist ?
				INT file_exists = img_jpeg_lib.GetHeaderInfo(&header_image, temp);
				if ( !file_exists )
					return VS_ERROR;


			// ---- manage buffers
				INT result = VS_OK;
				result = Read_ManageImageBuffer(&header_image, s);

				#if VS_LIDAR
				result = Read_ManageCsvBuffer(
						props->csv_rows,
						props->csv_columns,
						props->csv_rows_per_record,
						s
					);
				#endif


			// ---- file
				g_stack[s].s_filename = filename;
				g_stack[s].lid = lid;


			// ---- create thread to read image
				g_stack[s].index   = (DWORD)s;
				g_stack[s].handle  =
						CreateThread(NULL,
								0,
								ImgArkive::Read_Buffer,
								&g_stack[s].index,
								0,
								0
							);

			return VS_OK;
		}
	}


	{
		CHAR msg[VS_MAXCHARLEN];
		sprintf(
				msg,
				"ERROR: Read file %s,\n\nwith logical id # %d failed!\n\n",
				filename.c_str(),
				lid
			);
		INT result = ::MessageBoxA(
				NULL,
				msg,
				"VS ImageArchive Error",
				MB_OK | MB_ICONEXCLAMATION
				);
	}


	// ---- increment fail counter ----
		props->read_fail++;

	return VS_ERROR;
}


// ---------- Read_ManageImageBuffer ----------
/*!
\brief manage image buffer
\author Gareth Edwards
*/
INT ImgArkive::Read_ManageImageBuffer(
		ImgBuffer *img_buffer,
		WORD index
	)
{

	// --- header image properties
		UINT width  = img_buffer->GetWidth();
		UINT height = img_buffer->GetHeight();
		UINT bytes  = img_buffer->GetBytes();


	// ---- read_image does not exist
		if ( g_stack[index].img_buffer == NULL )
		{
			g_stack[index].img_buffer = new ImgBuffer(width, height, 3);
			g_stack[index].img_buffer->CreateBuffer();
		}


	// ---- read_image buffer does not exist
		else if ( g_stack[index].img_buffer->GetBuffer() == NULL )
		{
			g_stack[index].img_buffer->Initialise(width, height, 3);
			g_stack[index].img_buffer->CreateBuffer();
		}


	// ---- else
		else
		{
			// ---- dimensions DO NOT match
				DWORD header_image_size = width * height;
				DWORD read_image_size =
					g_stack[index].img_buffer->GetWidth() *
					g_stack[index].img_buffer->GetHeight();
				if ( header_image_size != read_image_size )
				{
					g_stack[index].img_buffer->DestroyBuffer();
					g_stack[index].img_buffer->Initialise(width, height, 3);
					g_stack[index].img_buffer->CreateBuffer();
				}
		}

	return VS_OK;
}


// ---------- Read_ManageCsvBuffer ----------
/*!
\brief manage stack csv buffer
\author Gareth Edwards
*/
INT ImgArkive::Read_ManageCsvBuffer(
		UINT csv_rows,
		UINT csv_columns,
		UINT csv_rows_per_record,
		WORD index
	)
{

	#if VS_LIDAR

	// ---- csv buffer does not exist

		if ( g_stack[index].csv_buffer == NULL )
		{
			g_stack[index].csv_buffer = new CsvObject();
			g_stack[index].csv_buffer->Initialise(csv_rows, csv_columns);
			g_stack[index].csv_buffer->SetRowsPerRecord(csv_rows_per_record);
			g_stack[index].csv_buffer->CreateBuffer();
		}

	// ---- else
		else
		{

			CsvObject *csv_buffer = g_stack[index].csv_buffer;

			FLOAT *float_buffer = NULL;
			INT result = csv_buffer->GetBuffer(&float_buffer);

			// ---- dimensions DO NOT match ?
				if ( csv_buffer->GetRows() != csv_rows || csv_buffer->GetColumns() != csv_columns )
				{
					g_stack[index].csv_buffer->DestroyBuffer();
					g_stack[index].csv_buffer->Initialise(csv_rows, csv_columns);
					g_stack[index].csv_buffer->SetRowsPerRecord(csv_rows_per_record);
					g_stack[index].csv_buffer->CreateBuffer();
				}

		}

	#endif

	return VS_OK;
}


// ---------- Read_IsReady ----------
/*!
\brief is there a read image ready
\author Gareth Edwards
*/
BOOL ImgArkive::Read_IsReady(
		UINT lid,
		std::string &filename,
		ImgBuffer *img_buffer
	)
{

	// ---- find ready archive image
		for ( WORD s=0; s<props->max_stack; s++ )
		{

			// IF -- found -- THEN
				if (
						g_stack[s].handle == ((HANDLE)1) &&
						g_stack[s].lid == lid
					)
				{
					UINT width  = g_stack[s].img_buffer->GetWidth();
					UINT height = g_stack[s].img_buffer->GetHeight();
					img_buffer->Initialise(width, height, 4);
					filename = g_stack[s].s_filename;
					return TRUE;
				}
		}

	return FALSE;
}


// ---------- Read_Buffer ----------
/*!
\brief Read image into image stack
\author Gareth Edwards
*/

static ImgJpegLib img_jpeg_lib;
static CsvIO      csv_io;
static CHAR       filename[VS_MAXCHARPATHLEN];

DWORD ImgArkive::Read_Buffer(LPVOID lpParam)
{

	// ---- local
		INT result = VS_OK;


	// ---- source
		INT index = *((INT*)lpParam);


	// ---- image: read "filename.jpg" ( e.g. "20200228_131200_000.jpg" )
		strcpy(filename, g_stack[index].s_filename.c_str());
		result = img_jpeg_lib.Read(g_stack[index].img_buffer, filename);


	// ---- lidar: read "filename.csv" ( e.g. "20200228_131200_000.csv" )
		#if VS_LIDAR

		INT   len  = strlen(filename);
		CHAR *type = "csv";
		for (UINT i = 0; i < 3; i++) filename[len - 3 + i] = type[i];
		result = csv_io.Read(g_stack[index].csv_buffer, filename);
		if ( result == VS_ERROR )
		{
			FLOAT *f = NULL;
			result = g_stack[index].csv_buffer->GetBuffer(&f);
			UINT extent = g_stack[index].csv_buffer->GetExtent();
			memset((VOID*)f, 0, extent * sizeof(FLOAT));
		}
		#endif


	// ---- completed
		g_stack[index].handle = ((HANDLE)1);

	return VS_OK;
}


// ---------- Read_AccessBuffer ----------
/*!
\brief Access an image image buffer
\author Gareth Edwards
*/
INT ImgArkive::Read_AccessBuffer(
		UINT lid,
		ImgBuffer *read_img_buffer
		#if VS_LIDAR
		,
		CsvObject *read_csv_buffer
		#endif
	)
{

	// ---- find ready archive image
		for (WORD index=0; index<props->max_stack; index++)
		{

			// ---- read ok ?
				if (
						g_stack[index].handle == ((HANDLE)1) &&
						g_stack[index].lid == lid
					)
				{

					// ---- close thread handle
						DWORD result = GetThreadId(g_stack[index].handle);
						if ( result != 0 )
							CloseHandle(g_stack[index].handle);


					// ---- reset stack
						g_stack[index].lid = 0;
						g_stack[index].handle = NULL;


					// ---- lut process
						ImgLutProcessBuffer ilb;
						ilb.boost_mode = (WORD)props->fx_boost;
						ilb.lut_mode   = (WORD)props->fx_mode;
						ilb.width      = (WORD)g_stack[index].img_buffer->GetWidth();
						ilb.height     = (WORD)g_stack[index].img_buffer->GetHeight();
						ilb.src_bpp    = (WORD)g_stack[index].img_buffer->GetBytes();
						ilb.dest_bpp   = (WORD)4;
						ilb.src_data   = (BYTE *)g_stack[index].img_buffer->GetBuffer();
						ilb.dest_data  = (BYTE *)read_img_buffer->GetBuffer();
						result         = props->img_lut->PreProcess(&ilb);

					// ---- lidar
						#if VS_LIDAR
						FLOAT *f_src   = NULL;
						result         = g_stack[index].csv_buffer->GetBuffer(&f_src);
						UINT extent    = g_stack[index].csv_buffer->GetExtent();
						FLOAT *f_dest  = NULL;
						result         = read_csv_buffer->GetBuffer(&f_dest);
						memcpy((VOID *)f_dest, (VOID *)f_src, extent * sizeof(FLOAT));

						read_csv_buffer->SetRead(g_stack[index].csv_buffer->GetRead());

						#endif

					return VS_OK;

				}
		}

	return VS_ERROR;
}


// ---------- Read_SetFxBoostMode  ----------
/*!
\brief set read fx image procesing lut lumin mode
\author Gareth Edwards
*/
INT ImgArkive::Read_SetFxBoostMode(UINT fx_boost)
{
	props->fx_boost = fx_boost;
	return VS_OK;
}


// ---------- Read_SetFxColourMode  ----------
/*!
\brief set read fx image procesing lut colour mode
\author Gareth Edwards
*/
INT ImgArkive::Read_SetFxColourMode(UINT fx_mode)
{
	props->fx_mode = fx_mode;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Write ----------
/*!
\brief Create thread to write bitmap RGB data to jpeg file
\author Gareth Edwards
*/
int ImgArkive::Write(
		std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
		std::string &filename,
		WORD  width,
		WORD  height,
		WORD  bpp,
		BYTE *byte_data
	)
{

	// IF -- save mode -- THEN --
	if ( props->save_image )
	{

		// IF -- runtime --THEN -- setup
		if ( props->run_time )
		{
			INT result = 0;
			result = Setup_PanConfigHdlFile();
				if ( result == VS_ERROR ) return VS_ERROR;
			props->run_time = FALSE;
		}


		// ---- find free archive image
		for ( WORD s=0; s<props->max_stack; s++ )
		{

			// ---- free ?
			if ( g_stack[s].status == FALSE )
			{

				// ---- copy path and filename
					g_stack[s].s_filename = filename;
					for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
						g_stack[s].s_path[i] = path[i];


				// ---- copy bitmap data into stack[s] image data
					Write_CopyImage(s, width, height, bpp, byte_data);


				// ---- set quality factor
					g_stack[s].img_buffer->SetQuality(props->quality);


				// ---- set id
					g_stack[s].index = (DWORD)s;


				// ---- close old handle
					if ( g_stack[s].handle != NULL )
					{
						DWORD result = GetThreadId(g_stack[s].handle);
						CloseHandle(g_stack[s].handle);
					}


				// ---- there is a new image
					g_stack[s].status = TRUE;


				// ---- launch thread
					g_stack[s].handle =
						CreateThread(NULL,
							0,
							vs_system::ImgArkive::Write_Buffer,
							&g_stack[s].index,
							0,
							0
						);

				return s + 1;
			}
		}


		// ---- increment fail counter
			props->write_fail++;


		// ---- tell
			CHAR msg[256];
			sprintf(msg, "ImageArchive::Save - failed %s! ( %d so far!) \n", filename.c_str(), (INT)props->write_fail);
			OutputDebugString(msg);


		return VS_ERROR;

	}

	return VS_OK;
}


// ---------- Write_CopyImage ----------
/*!
\brief copy RGB bitmap data into archive image RGB data
\author Gareth Edwards
*/
int ImgArkive::Write_CopyImage(
		WORD  index,
		WORD  width,
		WORD  height,
		WORD  bpp,
		BYTE *byte_data
	)
{

	// ---- set ----
		g_stack[index].bpp = bpp;


	// IF -- image buffer does not exist --
	// THEN -- allocate and create buffer
		if ( g_stack[index].img_buffer_created == FALSE )
		{

#			ifdef IMAGE_ARCHIVE_REPORT
			CHAR msg[256];
			sprintf(msg,"ImageArchive::CopySaveImage - %d image buffer does not exist - should get here ONCE!\n", index);
			OutputDebugString(msg);
#			endif

			g_stack[index].img_buffer = new ImgBuffer(width, height, bpp);
			g_stack[index].img_buffer->CreateBuffer();
			g_stack[index].img_buffer_created = TRUE;
		}

	// ELSE IF -- read_image buffer does not exist --
	//    THEN -- create
		else if ( g_stack[index].img_buffer->GetBuffer() == NULL )
		{

#			ifdef IMAGE_ARCHIVE_REPORT
			CHAR msg[256];
			sprintf(msg, "ImageArchive::CopySaveImage - read_image buffer does not exist - should NEVER get here!\n");
			OutputDebugString(msg);
#			endif

			g_stack[index].img_buffer->Initialise(width, height, bpp);
			g_stack[index].img_buffer->CreateBuffer();
		}

	// ELSE IF -- source dimension != destination dimension --
	//   THEN -- recreate
		else
		{
			DWORD src_image_size = width * height;
			DWORD dest_image_size =
				g_stack[index].img_buffer->GetWidth() *
				g_stack[index].img_buffer->GetHeight();
			if ( src_image_size != dest_image_size )
			{

#				ifdef IMAGE_ARCHIVE_REPORT
				CHAR msg[256];
				sprintf(msg, "ImageArchive::CopySaveImage - source dimension != destination dimension - should NEVER get here!\n");
				OutputDebugString(msg);
#				endif

				g_stack[index].img_buffer->DestroyBuffer();
				g_stack[index].img_buffer->Initialise(width, height, bpp);
				g_stack[index].img_buffer->CreateBuffer();
			}
		}


	// ---- copy
		BYTE *dest = g_stack[index].img_buffer->GetBuffer();
		memcpy(dest, byte_data, height * width * bpp);

	return VS_OK;
}


// ---------- Write_CreateDirectory ----------
/*!
\brief Create write directory from path stack and filename
\author Gareth Edwards
*/
DWORD ImgArkive::Write_CreateDirectory(
		std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
		std::string &filename
	)
{

	// FOR -- each part --
	std::string directory = "";
	for (WORD i=0; i<IMAGE_ARCHIVE_PATHTOP; i++)
	{

		// ---- concatenate
			directory += path[i] + "/";


		// TRY -- directory path
		try
		{

			// ---- get folder handle
				DWORD ftyp = GetFileAttributesA(directory.c_str());
				if (ftyp == INVALID_FILE_ATTRIBUTES)
				{
					char* str = "Create";
					throw(str);
				}

			// Note: You have to use GetLastError() to find out what that failure
			// actually is. If it returns ERROR_PATH_NOT_FOUND, ERROR_FILE_NOT_FOUND,
			// ERROR_INVALID_NAME, or ERROR_BAD_NETPATH then it really does not exist.
			// But if it returns most any other error, then something actually exists
			// at the specified path but the attributes are simply not accessible.

			// Note: if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;   // ...this is a directory!

		}
		catch (char* str)
		{

			// ----create directory
				INT result = mkdir(directory.c_str());
				// TBD Return value ignored: 'mkdir'

				CHAR msg[256];
				sprintf(msg, "   MKDIR - %s %s\n", str, directory.c_str());

#				ifdef IMAGE_ARCHIVE_REPORT_XXX
				OutputDebugString(msg);
#				endif


		}
	}

	return S_OK;
}


// ---------- Write_Buffer ----------
/*!
\brief Write image copied from bitmap data into image stack
\author Gareth Edwards
*/

//static ImgJpegLib jpeg_lib;
//static CHAR filename[VS_MAXCHARPATHLEN];

DWORD WINAPI ImgArkive::Write_Buffer(
		LPVOID lpParam
	)
{

	// ---- source stack[index] image
		int index = *((int*)lpParam);


	// ---- swap R and B, copy G ( & loose alpha if bpp = 4 )
		vs_system::ImgBuffer *img = g_stack[index].img_buffer;
		UINT  bpp = img->GetBytes();
		BYTE *dest = g_stack[index].img_buffer->GetBuffer();
		UINT  height =img->GetHeight();
		UINT  width = img->GetWidth();
		{
			BYTE *src = dest;
			for (UINT y = 0; y < height; y++)
			{
				for (UINT x = 0; x < width; x++)
				{
					BYTE temp   = *(src + 0);  // Store B
					*(dest + 0) = *(src + 2);  // B = R
					*(dest + 1) = *(src + 1);  // G = G
					*(dest + 2) = temp;        // R = B
					dest += 3;
					src += bpp;
				}
			}
		}


	// ---- create archive directory
		Write_CreateDirectory(g_stack[index].s_path, g_stack[index].s_filename);


	// ---- concatenate path components to create directory path
		std::string paf = "";
		for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
		{
			paf += g_stack[index].s_path[i] + ( i== 0 ? "" : "/");
		}

	
	// ----path / filename 
		std::string full_paf = paf + g_stack[index].s_filename;

		//#define IMAGE_SAVE_BIN

		#ifdef IMAGE_SAVE_BIN

		// ---- write image buffer to "filename"
			dest = g_stack[index].img_buffer->GetBuffer();
			strcpy(filename, full_paf.c_str());
			CHAR msg[256];
			strcpy(msg, filename);
			int len = strlen(msg);
			msg[len - 1] = 'n';
			msg[len - 2] = 'i';
			msg[len - 3] = 'b';
			FILE* hFile = fopen(msg, "wb");
			if ( hFile )
			{
				fwrite(dest, sizeof(BYTE), height*width, hFile);
				fclose(hFile);
			}
			else
			{
				CHAR err_msg[512];
				sprintf(err_msg, "Failed to open %s\n"
					"The reason *may* have been %s\n",
					msg, strerror(errno));
				OutputDebugString(err_msg);
			}

		#else

		// ---- write image buffer to "filename"
			img->SetBytes(3);
			strcpy(filename, full_paf.c_str());
			img_jpeg_lib.WriteToDisk(g_stack[index].img_buffer, filename);
			img->SetBytes(bpp);

		#endif

#		ifdef IMAGE_ARCHIVE_REPORT
		CHAR msg[256];
		sprintf(msg, "Saved %s\n", g_stack[index].s_filename.c_str());
		OutputDebugString(msg);
#		endif


	//  ---- finished with this thread
		g_stack[index].status = FALSE;

	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////


// ---------- CountProcessorCores ----------
/*!
\brief Get number of processor cores
\author Gareth Edwards
*/
WORD ImgArkive::CountProcessorCores()
{

	// ---- count
		DWORD_PTR process, system;
		if (
				GetProcessAffinityMask(
							GetCurrentProcess(),
							&process,
							&system
						)
			)
		{
			WORD count = 0;
			for (WORD i = 0; i < 32; i++)
				if (system & (1 << i))
					count++;
			return count;
		}


	// ---- maybe we haven't PROCESS_QUERY_INFORMATION access correctly....
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);

	return (WORD)sysinfo.dwNumberOfProcessors;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- WriteInitPathFilename  ----------
/*!
\brief Create write path stack and filename
\author Gareth Edwards
*/
INT ImgArkive::WriteInitPathFilename(  // ---- not used
		std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
		std::string &filename,
		INT timestamp[]
	)
{

	// ---- local time
		SYSTEMTIME local_time;
		GetLocalTime(&local_time);


	// ---- path : "[archive name]/[year]/[month]/[day]/[hour]/[min]"
		char ps[IMAGE_ARCHIVE_PATHTOP][IMAGE_ARCHIVE_PATHLEN];
		sprintf(ps[1], "%4d", local_time.wYear);
		sprintf(ps[2], "%2d", local_time.wMonth);
		sprintf(ps[3], "%2d", local_time.wDay);
		sprintf(ps[4], "%2d", local_time.wHour);
		sprintf(ps[5], "%2d", local_time.wMinute);


	// ---- tidy
		path[0].assign(props->s_archive_sub_folder_path);
		for ( WORD i = 1; i<6; i++ )
		{
			for (WORD j = 0; j < IMAGE_ARCHIVE_PATHLEN; j++) ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
			path[i].assign(ps[i]);
		}


	// ---- filename : "year|month|day_hour|minute_millisec"
		char f[IMAGE_ARCHIVE_MAXCHAR];
		sprintf(
				f,
				"%4d%2d%2d_%2d%2d%2d_%3d.jpg",
				local_time.wYear,
				local_time.wMonth,
				local_time.wDay,
				local_time.wHour,
				local_time.wMinute,
				local_time.wSecond,
				local_time.wMilliseconds
			);


	// ---- tidy & store
		WORD len_filename = (WORD)strlen(f);
		for ( WORD i = 0; i<len_filename; i++ )
			f[i] = f[i] == ' ' ? '0' : f[i];
		filename.assign(f);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

