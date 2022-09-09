////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_archive.cpp ----------
/*!
\file vs_img_archive.cpp
\brief Implementation of the ImgArchive class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_img_archive.h"
#include "../header/vs_img_lut.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Definitions ----------


// ---------- TBD - add immediate mode ----------
//
//#define IMAGE_ARCHIVE_IMMEDIATE


// ---------- Maximum depth of archive ----------
//
// Path stack "[archive name]/[year]/[month]/[day]/[hour]/[min]"
// Filename "year|month|day_hour|minute_millisec"
//
// Note - set explicitly in header file to [6]
//
#define IMAGE_ARCHIVE_PATHTOP     6


// ---------- MS Non thread handle values ----------
#define HANDLE_EMPTY       NULL
#define HANDLE_COMPLETED ((HANDLE)1)


////////////////////////////////////////////////////////////////////////////////


// ---------- Private implementation PI_Props class----------
class ImgArchive::PI_Props
{

public:

	// ---- c/dtor ----
	PI_Props();
	virtual ~PI_Props();

	// ---- configured ----
	BOOL   write_roi;
	WORD   compression;
	WORD   max_stack;
	WORD   extra_threads;
	UINT   mode;

	// ---- properties ----
	SysOutput *sys_output;
	DWORD      archive_fail;

	// ---- config HDL parameters ----
	vs_system::VNode *config_hdl;

	// ---- allocated ----
	vs_system::ImgLutProcess *img_lut;
	UINT fx_mode;
	UINT fx_boost;

	// Image
	//ImgJpegLib img_jpeg_lib;
	//CHAR img_filename[VS_MAXCHARPATHLEN];
};


// ---------- ctor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
vs_system::ImgArchive::PI_Props::PI_Props()
{

	// ---- configured ----
	write_roi     = false;
	compression   = ArchiveDefaultParam::JPG_COMPRESSION;
	archive_fail  = 0;
	max_stack     = ArchiveDefaultParam::MAX_STACK;
	extra_threads = 0;
	mode          = ArchiveDefaultParam::MODE;

	sys_output = NULL;

	// ---- allocated ----
	config_hdl = NULL;
	img_lut    = new vs_system::ImgLutProcess;

	// ---- fx ----
	fx_mode = 0;
	fx_boost = 0;

}


// ---------- dtor ----------
/*!
\brief destructor
\author Gareth Edwards
*/
vs_system::ImgArchive::PI_Props::~PI_Props()
{
	if ( img_lut != NULL ) { delete img_lut; img_lut = NULL; }
}

////////////////////////////////////////////////////////////////////////////////


// ---------- ImgArchiveItem ----------
class ImgArchiveItem
{

public:

	// ---- c/dtor ----
	ImgArchiveItem() {};
	virtual ~ImgArchiveItem() {};

	// ---- stack ----
	HANDLE  handle;
	DWORD   index;

	// ---- read ----
	ImgBuffer *img_buffer;
	UINT       img_index;

	// ---- write ----
	WORD quality;

	// ---- string ----
	std::string s_path[IMAGE_ARCHIVE_PATHTOP];
	std::string s_filename;

};


// ---------- global archive stack ----------
ImgArchiveItem g_stack[ ImgArchive::ArchiveDefaultParam::MAX_STACK ];


// ---------- ctor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
vs_system::ImgArchive::ImgArchive()
{

	// ---- allocate and create pi_props ----
	pi_props = new PI_Props();

	// ---- initialise stack ----
	for ( WORD s=0; s<ArchiveDefaultParam::MAX_STACK; s++ )
	{
		// ---- stack ----
		g_stack[s].index = (DWORD)s;
		g_stack[s].handle = HANDLE_EMPTY;

		// ---- read ----
		g_stack[s].img_buffer = NULL;
		g_stack[s].img_index = 0;

		// ---- write ----
		g_stack[s].quality = pi_props->compression;

		// ---- string ----
		g_stack[s].s_filename.assign("");
		for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
			g_stack[s].s_path[i].assign("");

	}
}


// ---------- dtor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
vs_system::ImgArchive::~ImgArchive()
{
	if ( pi_props != NULL ) { delete pi_props; pi_props = NULL; }
}


// ---------- Configure ----------
/*!
\brief Configure
\author Gareth Edwards
*/
INT ImgArchive::Configure(CHAR *app_setup_config_paf)
{

	printf(
		"IMaGe ARCHIVE configuration:--------------------------------------------------\n");

	// ========== Read config fle ==========
	vs_system::VNodeIO vio;
	pi_props->config_hdl = vio.Read(app_setup_config_paf);

#	ifdef OS_WINDOWS
	if ( pi_props->config_hdl == NULL )
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
#	endif

	// IF config data READ THEN
	if ((INT)pi_props->config_hdl != VS_ERROR)
	{
		// local
		vs_system::VNode *p = NULL;

		// IF "Archive" config data THEN
		if (pi_props->config_hdl->SearchName("Archive") )
		{
			//if (p = pi_props->config_hdl->SearchName("ROI"))     pi_props->write_roi      = p->GetBoolFromValue();
			if (p = pi_props->config_hdl->SearchName("Compression"))   pi_props->compression   = p->GetIntFromValue();
			if (p = pi_props->config_hdl->SearchName("Extra Threads")) pi_props->extra_threads = p->GetIntFromValue();
		}
	}

	// ========= SET MAXIMUM PROCESS STACK SIZE =========

	// Get # of processor core
	WORD cores = CountProcessorCores();

	// Match maximum stack (thread) size to # of processor cores  
	switch (cores)
	{
		// Limit excessive stack size - only so many images from camera!!
		case 16:
		case 12:
			pi_props->max_stack = ArchiveDefaultParam::MAX_STACK;
			break;
		case  8:
			pi_props->max_stack = 16;
			break;
		case  4:
			pi_props->max_stack = 8; 
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


	// ========= SET max_stack + config hdl file specified extra threads =========
	pi_props->max_stack = pi_props->extra_threads + pi_props->max_stack;
	pi_props->max_stack =
		pi_props->max_stack < ArchiveDefaultParam::MAX_STACK
			? pi_props->max_stack
				: ArchiveDefaultParam::MAX_STACK;
	printf("   Image archive stack size is %d (including %d extra).\n", pi_props->max_stack, pi_props->extra_threads);


	// ========= SET MINIMUM/MAXIMUM IMAGE COMPRESSION SIZE =========
	pi_props->compression =
		pi_props->compression < 10 ? 10 :
		(
			pi_props->compression > 100 ? 100 :
			pi_props->compression
		);
	printf("   Image archive write compression is %d.\n", pi_props->compression);

	printf("End:--------------------------------------------------------------------------\n");

	// Return
	return VS_OK;
}


// ---------- Set ----------
INT ImgArchive::SetMode(UINT m)             { pi_props->mode          =   m; return VS_OK; }
INT ImgArchive::SetMaxStack(UINT ms)        { pi_props->max_stack     =  ms; return VS_OK; }
INT ImgArchive::SetWriteROI(BOOL roi)       { pi_props->write_roi     = roi; return VS_OK; }
INT ImgArchive::SetWriteQuality(UINT wq)    { pi_props->compression   = wq; return VS_OK; }

INT ImgArchive::SetSysOutput(SysOutput *so)
{
	pi_props->sys_output    =  so; return VS_OK;
}


// ---------- FlushStack ----------
/*!
\brief Read image into image stack
\author Gareth Edwards
*/
INT vs_system::ImgArchive::FlushStack()
{
	for (WORD index=0; index<pi_props->max_stack; index++)
	{
		g_stack[index].img_index = 0;
		g_stack[index].handle    = HANDLE_EMPTY;
	}
	return VS_OK;
}


// ---------- QueueImage ----------
/*!
\brief Queue an image to read in archive
\author Gareth Edwards
*/
INT vs_system::ImgArchive::QueueImage(UINT img_index, std::string &filename)
{

	// Find free archive image
	for (WORD index=0; index<pi_props->max_stack; index++)
	{
		// IF free THEN set, thread and return
		if ( g_stack[index].handle == HANDLE_EMPTY )
		{
			// Get image file header
			ImgBuffer header_image;
			ImgJpegLib img_jpeg_lib;
			CHAR temp[VS_MAXCHARPATHLEN];
			strcpy(temp, filename.c_str());

			// IF file does NOT exist THEN return error
			INT file_exists = img_jpeg_lib.GetHeaderInfo(&header_image, temp);
			//std::cout << "Queuing : " << temp << " " << file_exists << std::endl;
			if ( !file_exists )
				return VS_ERROR;

			// Set
			ReadSetupImageInStack(&header_image, index);
			g_stack[index].s_filename = filename;
			g_stack[index].img_index = img_index;

			// Create thread to read image
			g_stack[index].index   = (DWORD)index;
			g_stack[index].handle  =
				CreateThread(NULL,
					0,
					vs_system::ImgArchive::ReadImage,
					&g_stack[index].index,
					0,
					0);

			// OK
			return VS_OK;
		}
	}

	if ( pi_props->sys_output )
		{
		CHAR text[VS_MAXCHARLEN];
		sprintf(text, "Could not queue %d!", img_index);
		pi_props->sys_output->Message(text);
	}

	// Increment write fail counter
	pi_props->archive_fail++;


	// Return
	return VS_ERROR;
}


// ---------- GetImageCount ----------
/*!
\brief Count, and then return # of images in archive stack
\author Gareth Edwards
*/
INT vs_system::ImgArchive::GetImageCount()
{
	// Local
	INT image_count = 0;

	// Find ready archive image
	for (WORD index = 0; index<pi_props->max_stack; index++)
	{
		if (g_stack[index].img_index != 0) image_count++;
	}

	// Return # of images
	return image_count;
}


// ---------- IsReadImageReady ----------
/*!
\brief Is there a read image ready to receive from archive
\author Gareth Edwards
*/
BOOL vs_system::ImgArchive::IsReadImageReady(UINT img_index, std::string &filename, ImgBuffer *read_img_buffer)
{
	// Find ready archive image
	for (WORD index=0; index<pi_props->max_stack; index++)
	{
		// IF read archived in THEN
		if ( g_stack[index].handle == HANDLE_COMPLETED &&
			g_stack[index].img_index == img_index)
		{
			UINT width  = g_stack[index].img_buffer->GetWidth();
			UINT height = g_stack[index].img_buffer->GetHeight();
			read_img_buffer->Initialise(width, height, 4);
			filename = g_stack[index].s_filename;
			return TRUE;
		}
	}

	// Return error code
	return FALSE;
}


// ---------- ReadImage ----------
/*!
\brief Read an image from archive
\author Gareth Edwards
*/
INT vs_system::ImgArchive::ReadImage(
		UINT img_index,
		ImgBuffer *read_img_buffer
	)
{

	// ---- find ready archive image ----
	for (WORD index=0; index<pi_props->max_stack; index++)
	{

		// IF -- read ok -- THEN --
		if (
				g_stack[index].handle == HANDLE_COMPLETED &&
				g_stack[index].img_index == img_index
			)
		{

			// ---- close thread handle ----
			DWORD result = GetThreadId(g_stack[index].handle);
			if ( result != 0 ) CloseHandle(g_stack[index].handle);


			// ---- reset stack ----
			g_stack[index].img_index = 0;
			g_stack[index].handle = HANDLE_EMPTY;


			// ---- lut process ----
			ImgLutProcessBuffer ilb;
			ilb.boost_mode = (WORD)pi_props->fx_boost;
			ilb.lut_mode   = (WORD)pi_props->fx_mode;
			ilb.width      = (WORD)g_stack[index].img_buffer->GetWidth();
			ilb.height     = (WORD)g_stack[index].img_buffer->GetHeight();
			ilb.src_bpp    = (WORD)g_stack[index].img_buffer->GetBytes();
			ilb.dest_bpp   = (WORD)4;
			ilb.src_data   = (BYTE *)g_stack[index].img_buffer->GetBuffer();
			ilb.dest_data  = (BYTE *)read_img_buffer->GetBuffer();
			result = pi_props->img_lut->PreProcess(&ilb);

			return VS_OK;

		}
	}

	return VS_ERROR;
}


// ---------- ReadSetupImageInStack ----------
/*!
\brief Initialise image in archive stack
\author Gareth Edwards
*/
INT vs_system::ImgArchive::ReadSetupImageInStack(
	ImgBuffer *read_img_buffer,
	WORD index)
{
	// Header image properties
	UINT width  = read_img_buffer->GetWidth();
	UINT height = read_img_buffer->GetHeight();
	UINT bytes  = read_img_buffer->GetBytes();

	// IF read_image does not exist THEN allocate and create buffer
	if ( g_stack[index].img_buffer == NULL )
	{
		g_stack[index].img_buffer = new ImgBuffer(width, height, 3);
		g_stack[index].img_buffer->CreateBuffer();
	}

	// ELSE IF read_image buffer does not exist THEN create
	else if ( g_stack[index].img_buffer->GetBuffer() == NULL )
	{
		g_stack[index].img_buffer->Initialise(width, height, 3);
		g_stack[index].img_buffer->CreateBuffer();
	}

	// ELSE
	else
	{
		// ---- IF dimensions DO NOT match THEN RESIZE (create image)
			DWORD header_image_size = width * height;
			DWORD read_image_size = g_stack[index].img_buffer->GetWidth() * g_stack[index].img_buffer->GetHeight();
			if (header_image_size != read_image_size)
			{
				g_stack[index].img_buffer->DestroyBuffer();
				g_stack[index].img_buffer->Initialise(width, height, 3);
				g_stack[index].img_buffer->CreateBuffer();
			}
	}

	// Done
	return VS_OK;
}


// ---------- ReadImage ----------
/*!
\brief Read image into image stack
\author Gareth Edwards
*/

static ImgJpegLib img_jpeg_lib;
static CHAR img_filename[VS_MAXCHARPATHLEN];

DWORD WINAPI vs_system::ImgArchive::ReadImage(LPVOID lpParam)
{

	// ---- source stack[index] image
		int index = *((int*)lpParam);


	// ---- read image buffer from "filename".jpg
		strcpy(img_filename, g_stack[index].s_filename.c_str());
		img_jpeg_lib.Read(g_stack[index].img_buffer, img_filename);


	// ---- completed thread
		g_stack[index].handle = HANDLE_COMPLETED;


	return VS_OK;
}


// ---------- Write  ----------
/*!
\brief Write images to archive
\author Gareth Edwards
*/
INT vs_system::ImgArchive::Write(ImgBuffer *img_buffer)
{
	// Create write path stack and filename
	std::string path[IMAGE_ARCHIVE_PATHTOP];
	std::string filename;
	WriteCreateFilename(path, filename);

	// Find free archive image
	for (WORD s=0; s<pi_props->max_stack; s++)
	{
		// IF archived THEN empty
		if ( g_stack[s].handle == HANDLE_COMPLETED )
		{				
			g_stack[s].handle = HANDLE_EMPTY;
		}

		// IF free THEN
		if ( g_stack[s].handle == HANDLE_EMPTY )
		{
			// Copy path and filename
			g_stack[s].s_filename = filename;
			for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
				g_stack[s].s_path[i] = path[i];

			// Copy image into stack
			WriteImageIntoStack(img_buffer, s);

			// Create thread to write image
			g_stack[s].index   = (DWORD)s;
			g_stack[s].quality = pi_props->compression;
			g_stack[s].handle  =
				CreateThread(NULL,
					0,
					vs_system::ImgArchive::WriteImage,
					&g_stack[s].index,
					0,
					0);

			// Return successful code
			return VS_OK;
		}
	}

	if ( pi_props->sys_output )
	{
		CHAR text[VS_MAXCHARLEN];
		sprintf(text, "Failed to write %s!", filename.c_str());
		pi_props->sys_output->Message(text);
	}

	// Increment fail counter
	pi_props->archive_fail++;

	// Return error code
	return VS_ERROR;
}


// ---------- WriteCreateFilename  ----------
/*!
\brief Create save path stack and filename
\author Gareth Edwards
*/
INT vs_system::ImgArchive::WriteCreateFilename(std::string(&path)[IMAGE_ARCHIVE_PATHTOP], std::string &filename)
{
	// Create image filename
	SYSTEMTIME SystemTime;
	// GetSystemTime(&SystemTime);
	GetLocalTime(&SystemTime);

	// Create path stack "[archive name]/[year]/[month]/[day]/[hour]/[min]"
	char ps[IMAGE_ARCHIVE_PATHTOP][VS_MAXCHARPATHLEN];
	sprintf(ps[0], "%s", "archive");
	sprintf(ps[1], "%4d", SystemTime.wYear);
	sprintf(ps[2], "%2d", SystemTime.wMonth);
	sprintf(ps[3], "%2d", SystemTime.wDay);
	sprintf(ps[4], "%2d", SystemTime.wHour);
	sprintf(ps[5], "%2d", SystemTime.wMinute);
	for (WORD i = 0; i < IMAGE_ARCHIVE_PATHTOP; i++)
	{
		for (WORD j = 0; j < VS_MAXCHARPATHLEN; j++) ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
		path[i].assign(ps[i]);
	}

	// Create image filename "year|month|day_hour|minute_millisec"
	char f[VS_MAXCHARLEN];
	sprintf(f, "%4d%2d%2d_%2d%2d%2d_%3d.jpg",
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond,
		SystemTime.wMilliseconds);
	WORD len_filename = (WORD)strlen(f);
	for (WORD i=0; i<len_filename; i++)
		f[i] = f[i] == ' ' ? '0' : f[i];
	filename.assign(f);

	// Done
	return VS_OK;
}

 
// ---------- WriteImageIntoStack ----------
/*!
\brief Copy image into archive stack
\author Gareth Edwards
*/
INT vs_system::ImgArchive::WriteImageIntoStack(ImgBuffer *img_buffer, WORD index)
{
	// ROI: Region Of Interest - BUG : Needs offset
	WORD roi_height = img_buffer->GetHeight();
	WORD roi_width  = img_buffer->GetWidth();
	WORD offset     = 0;
	if ( pi_props->write_roi )
	{
		roi_width = roi_height;
		offset = (roi_width - roi_height) / 2;
	}

	// IF read_image does not exist THEN allocate and create buffer
	if ( g_stack[index].img_buffer == NULL )
	{
		g_stack[index].img_buffer = new ImgBuffer(roi_width, roi_height, 3);
		g_stack[index].img_buffer->CreateBuffer();
	}

	// ELSE IF read_image buffer does not exist THEN create
	else if ( g_stack[index].img_buffer->GetBuffer() == NULL )
	{
		g_stack[index].img_buffer->Initialise(roi_width, roi_height, 3);
		g_stack[index].img_buffer->CreateBuffer();
	}

	// ELSE
	else
	{
		// IF dimensions DO NOT match THEN RESIZE (create image)
		DWORD roi_image_size = roi_width * roi_height;
		DWORD write_image_size = g_stack[index].img_buffer->GetWidth() * g_stack[index].img_buffer->GetHeight();
		if (roi_image_size != write_image_size)
		{
			g_stack[index].img_buffer->DestroyBuffer();
			g_stack[index].img_buffer->Initialise(roi_width, roi_height, 3);
			g_stack[index].img_buffer->CreateBuffer();
		}
	}

	// Accelerate
	UCHAR *dest  = g_stack[index].img_buffer->GetBuffer();;
	UCHAR *src   = img_buffer->GetBuffer();
	UINT   src_bytes = img_buffer->GetBytes();

	// Copy SRC into DEST - BUG BUG BUG - sort out offset!!!
	for (UINT y=0; y<roi_height; y++)
	{
		for (UINT x=0; x<roi_width; x++)
		{
			*(dest)   = *(src);
			*(dest+1) = *(src+1);
			*(dest+2) = *(src+2);
			dest += 3; // Forced !
			src += src_bytes;
		}
	}

	// Done
	return VS_OK;
}


// ---------- WriteImage ----------
/*!
\brief Write image to disk from image stack
\author Gareth Edwards
*/
DWORD WINAPI vs_system::ImgArchive::WriteImage(LPVOID lpParam)
{
	// Source stack[index] image
	int index = *((int*)lpParam);

	// Create archive directory
	WriteCreateDirectory(g_stack[index].s_path, g_stack[index].s_filename);

	// Concatenate path components to create directory path
	std::string paf = "";
	for (WORD i = 0; i<IMAGE_ARCHIVE_PATHTOP; i++)
		paf += g_stack[index].s_path[i] + "/";

	// Append filename
	paf += g_stack[index].s_filename;

	
	// Write image buffer to "filename"
	ImgJpegLib jpeg_lib;
	CHAR filename[VS_MAXCHARPATHLEN];
	strcpy (filename, paf.c_str());
	jpeg_lib.WriteToDisk(g_stack[index].img_buffer, filename);

	// Completed thread
	g_stack[index].handle = HANDLE_COMPLETED;

	// done
	return VS_OK;
}


// ---------- WriteCreateDirectory ----------
/*!
\brief Create write directory from path stack and filename
\author Gareth Edwards
*/
DWORD vs_system::ImgArchive::WriteCreateDirectory(
	std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
	std::string &filename)
{
	// FOR each path component
	std::string directory = "";
	for (WORD i=0; i<IMAGE_ARCHIVE_PATHTOP; i++)
	{
		// Concatenate
		directory += path[i] + "/";

		// TRY directory path
		try
		{
			// Get folder handle
			DWORD ftyp = GetFileAttributesA(directory.c_str());
			if (ftyp == INVALID_FILE_ATTRIBUTES)
				throw("Create");

			// Note: You have to use GetLastError() to find out what that failure
			// actually is.If it returns ERROR_PATH_NOT_FOUND, ERROR_FILE_NOT_FOUND,
			// ERROR_INVALID_NAME, or ERROR_BAD_NETPATH then it really does not exist.
			// But if it returns most any other error, then something actually exists
			// at the specified path but the attributes are simply not accessible.

			// Note: if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;   // ...this is a directory!
		}
		catch (char* throw_str)
		{
			// copy throw string
			CHAR str[VS_MAXCHARNAME];
			strcpy (str, throw_str);

			// Create directory
			INT result = mkdir(directory.c_str()); // TBD Return value ignored: 'mkdir'.
		}
	}

	// Done
	return S_OK;
}


// ---------- CountProcessorCores ----------
/*!
\brief Get number of processor cores
\author Gareth Edwards
*/
WORD vs_system::ImgArchive::CountProcessorCores()
{
	// Count

	// 64 BIT CHANGE from DWORD to size_t
	DWORD_PTR process, system;

	if ( GetProcessAffinityMask(GetCurrentProcess(), &process, &system ))
	{
		WORD count = 0;
		for (WORD i = 0; i < 32; i++)
			if (system & (1 << i))
				count++;
		return count;
	}

	// Maybe we haven't PROCESS_QUERY_INFORMATION access correctly....
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	return (WORD)sysinfo.dwNumberOfProcessors;
}


// ---------- ReadCreateFilename  ----------
/*!
\brief Create read filename
\author Gareth Edwards
*/
INT vs_system::ImgArchive::ReadCreateFilename(
	std::string &filename,
	VsDateTime &read_image_time)
{
	// Create image filename withg spaces
	char filename_with_spaces[VS_MAXCHARLEN];
	sprintf(
		
		filename_with_spaces,
		"archive/%4d/%2d/%2d/%2d/%2d/%4d%2d%2d_%2d%2d%2d_%3d.jpg",
		
		read_image_time.year,     // Folder structure
		read_image_time.month,
		read_image_time.day,
		read_image_time.hour,
		read_image_time.minute,
		
		read_image_time.year,     // File name
		read_image_time.month,
		read_image_time.day,
		read_image_time.hour,
		read_image_time.minute,
		read_image_time.second,
		read_image_time.milliseconds

		);

	// Replace spaces with zero's
	WORD len_filename = (WORD)strlen(filename_with_spaces);
	for (WORD i=0; i<len_filename; i++)
		filename_with_spaces[i] = filename_with_spaces[i] == ' ' ? '0' : filename_with_spaces[i];

	// Store in string
	filename.assign(filename_with_spaces);

	// Done
	return VS_OK;
}


// ---------- SetFxBoost  ----------
/*!
\brief Set fx image procesing lut mode
\author Gareth Edwards
*/
INT vs_system::ImgArchive::SetFxBoost(UINT fx_boost)
{
	pi_props->fx_boost = fx_boost;
	return VS_OK;
}


// ---------- SetFxMode  ----------
/*!
\brief Set fx image procesing lut mode
\author Gareth Edwards
*/
INT vs_system::ImgArchive::SetFxMode(UINT fx_mode)
{
	pi_props->fx_mode = fx_mode;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


