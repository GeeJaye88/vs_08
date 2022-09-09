////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_archive.h ----------
/*!
\file vs_img_archive.cpp
\brief ImgArchive definitions, declarations and class interfaces
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_ARCHIVE
#define VS_SYSTEM_IMG_ARCHIVE


// ---- include ----
#include <windows.h>
#include <string>


// ---- include for Timeline ----
#include <vector>
#include <stack>
#include <iostream>


// ---- vs system include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_sys_output.h"


// ---- vs library include ----------
#include "../header/vs_img_buffer.h"
#include "../header/vs_img_jpeglib.h"
#include "../header/vs_vnode.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- VS_ImgArchive ----------
class ImgArchive
{
public:

	enum ArchiveMode {
		SINGLE  = 1,
		DAILY   = 2,
		RUNTIME = 3
	};

	enum ArchiveDefaultParam {
		MODE            = RUNTIME,
		JPG_COMPRESSION = 80,
		MAX_STACK       = 16
	};

	// Constructor/destructor
	ImgArchive();
	virtual ~ImgArchive();

	// Configure
	INT Configure(CHAR *);

	// Set
	INT  SetMode(UINT);
	INT  SetMaxStack(UINT);
	INT  SetWriteROI(BOOL);
	INT  SetWriteQuality(UINT);
	INT  SetSysOutput(SysOutput *);

	// Stack
	INT  FlushStack();

	// Read
	INT  QueueImage(UINT, std::string &);
	INT  GetImageCount();
	BOOL IsReadImageReady(UINT, std::string &, ImgBuffer *);
	INT  ReadImage(UINT, ImgBuffer *);

	// (Re)Setup in QueueImage
	INT  ReadSetupImageInStack(ImgBuffer *, WORD);

	// Read - thread methods
	static DWORD WINAPI ReadImage(LPVOID);

	// Write
	INT  Write(ImgBuffer *);
	INT  WriteCreateFilename(std::string (&)[6], std::string &);
	INT  WriteImageIntoStack(ImgBuffer *, WORD);

	// Write - thread methods
	static DWORD WINAPI WriteImage(LPVOID);
	static DWORD WriteCreateDirectory(std::string(&)[6], std::string &);

	// Housekeeping
	WORD CountProcessorCores();
	INT  ReadCreateFilename(std::string &, VsDateTime &);

	// ---- FX ----
	INT SetFxBoost(UINT);
	INT SetFxMode(UINT);

private:

	// Private implementation
	class PI_Props; PI_Props *pi_props;

}; // ImgArchive


} // namespace


#endif // VS_SYSTEM_IMG_ARCHIVE

////////////////////////////////////////////////////////////////////////////////