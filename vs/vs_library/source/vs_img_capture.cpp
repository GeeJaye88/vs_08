////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_capture.cpp ----------
/*!
\file vs_img_capture.cpp
\brief Implementation of the ImgCapture class
\author Gareth Edwards
*/

#include <vs/vs_library/header/vs_img_capture.h>


using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ImgAnalysis class ----------
class ImgCapture::PI_ImgCapture
{

public:

	PI_ImgCapture() {};
	~PI_ImgCapture() {};

	INT Capture(HWND hWnd);
	INT SaveBMP(HWND hWnd, HDC hdcWindow, HBITMAP hbmScreen);

	vs_system::AppDependency *app_dependency = NULL;

	CHAR directory[VS_MAXCHARPATHLEN];
	INT  event_id = 0;
	INT  image_conter = 0;

};


////////////////////////////////////////////////////////////////////////////////


// ---- constructor ----
ImgCapture::ImgCapture()
{
	pi_img_capture = new PI_ImgCapture();
	strcpy(pi_img_capture->directory, "");
};


// ---- destructor ----
ImgCapture::~ImgCapture()
{
	if ( pi_img_capture != NULL )
	{
		delete pi_img_capture;
		pi_img_capture = NULL;
	}
};


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ImgCapture::Setup(
		vs_system::AppDependency *app_dependency,
		CHAR *directory,
		INT event_id
	)
{

	pi_img_capture = new PI_ImgCapture();
	pi_img_capture->app_dependency = app_dependency;
	strcpy(pi_img_capture->directory, directory);
	pi_img_capture->event_id = event_id;

	return VS_OK;
};


// ---------- Capture ----------
/*!
\brief Capture
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ImgCapture::Capture()
{

	vs_system::SysContext *sys_context = pi_img_capture->app_dependency->GetSysContext();

	INT event_id = sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);
	if ( event_id == pi_img_capture->event_id )
	{
		HWND hWnd = (HWND)sys_context->GetDWord(vs_system::SysContext::WINDOW_HANDLE);
		INT result = pi_img_capture->Capture(hWnd);
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT ImgCapture::PI_ImgCapture::Capture(
		HWND hWnd
	)
{

	// ---- local shortcuts
		vs_system::SysContext *sys_context = app_dependency->GetSysContext();
		vs_system::SysOutput  *sys_output  = app_dependency->GetSysOutput();


		// ---- local
		HDC     hdcScreen;
		HDC     hdcWindow;
		HDC     hdcMemDC  = NULL;
		HBITMAP hbmScreen = NULL;


    // ---- retrieve the handle to a display device context for
	//      the client area of the window. 
		hdcScreen = GetDC(NULL);
		hdcWindow = GetDC(hWnd);


    // ---- create a compatible DC which is used in a BitBlt from the window DC
		hdcMemDC = CreateCompatibleDC(hdcWindow);
			if( !hdcMemDC )
			{
				sys_output->MessageError("CreateCompatibleDC has failed","Failed", "ERROR");
				goto done;
			}


	// --- this is the best stretch mode
		SetStretchBltMode(hdcWindow, HALFTONE);


	// ---- source top left corner (currently same as window)
		DWORD src_x_offset = sys_context->GetDWord(vs_system::SysContext::X_OFFSET);
		DWORD src_y_offset = sys_context->GetDWord(vs_system::SysContext::Y_OFFSET);


	// ---- select rect
		DWORD src_width  = 1024;
		DWORD src_height = 576;
		UINT  rect_mode = 1;
		RECT  rct;
		switch ( rect_mode )
		{
			case 0: // window - needs fixing
				GetWindowRect(hWnd, &rct);
				break;

			case 1: // client
				GetClientRect(hWnd, &rct);
				break;

			case 2: // rect
				src_x_offset = 448;
				src_y_offset = 124;
				rct.top     = src_y_offset;
				rct.left    = src_x_offset;
				rct.bottom  = rct.top  + src_height;
				rct.right   = rct.left + src_width;
				break;
		}
		src_width  = rct.right - rct.left;
		src_height = rct.bottom - rct.top;


	// ---- target width and height (currently same as window)
		DWORD tgt_width  = src_width;
		DWORD tgt_height = src_height;


    // ----source DC is the a part of the screen (currently same as window) and
	// the destination DC is the current window (HWND)
		if (
				!StretchBlt(
					   hdcWindow,
					   0,0, 
					   tgt_width,
					   tgt_height,
					   hdcScreen, 
					   src_x_offset,
					   src_y_offset,
					   src_width,
					   src_height,
					   SRCCOPY
					)
			)
		{
			sys_output->MessageError("StretchBlt has failed", "Failed", "ERROR");
			goto done;
		}
    

    // ---- create a compatible bitmap from the Window DC
		hbmScreen = CreateCompatibleBitmap(
				hdcWindow,
				tgt_width,
				tgt_height
			);
		if( !hbmScreen )
		{
			sys_output->MessageError("CreateCompatibleBitmap has failed", "Failed", "ERROR");
			goto done;
		}


    // ---- select the compatible bitmap into the compatible memory DC
		SelectObject(hdcMemDC, hbmScreen);
    

    // ---- bit block transfer into our compatible memory DC.
		if ( !BitBlt(
					hdcMemDC,
					0,0, 
					tgt_width,
					tgt_height,
					hdcWindow,
					0,0,
					SRCCOPY
				)
			)
		{
			sys_output->MessageError("BitBlt has failed", "Failed", "ERROR");
			goto done;
		}


	// --- hey hey
		INT save_result = SaveBMP(hWnd, hdcWindow, hbmScreen);


    // --- clean up
	done:
		DeleteObject(hbmScreen);
		DeleteObject(hdcMemDC);
		ReleaseDC(NULL,hdcScreen);
		ReleaseDC(hWnd,hdcWindow);

    return 1;
}


INT ImgCapture::PI_ImgCapture::SaveBMP(HWND hWnd, HDC hdcWindow, HBITMAP hbmScreen)
{

	// ---- local shortcuts
		vs_system::SysContext *sys_context = app_dependency->GetSysContext();
		vs_system::SysOutput  *sys_output  = app_dependency->GetSysOutput();


	// ---- dev mode ?
		BOOL development_mode = sys_context->GetBool(sys_context->DEVELOPMENT);
		CHAR *app_src_path = NULL;
		if ( development_mode )
			app_src_path = "..\\";
		else
			app_src_path = "";


	// ---- create directory
		CHAR app_directory[VS_MAXCHARPATHLEN];
		sprintf(app_directory, "%s%s", app_src_path, directory);
		if ( CreateDirectory(app_directory, NULL ) ||
			ERROR_ALREADY_EXISTS == GetLastError())
		{
			;//sys_output->MessageError("Directory already exists", "Warning", app_directory);
		}
		else
		{
			sys_output->MessageError("Create Directory has failed", "Failed", app_directory);
			goto done;
		}


	// ---- get the BITMAP from the HBITMAP
		BITMAP  bmpScreen;
		GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);


	// ---- header
		BITMAPFILEHEADER   bmfHeader;
		BITMAPINFOHEADER   bi;
		bi.biSize          = sizeof(BITMAPINFOHEADER);
		bi.biWidth         = bmpScreen.bmWidth;
		bi.biHeight        = bmpScreen.bmHeight;
		bi.biPlanes        = 1;
		bi.biBitCount      = 32;
		bi.biCompression   = BI_RGB;
		bi.biSizeImage     = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed       = 0;
		bi.biClrImportant  = 0;


	// ---- size
		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;


	// ---- starting with 32-bit Windows, GlobalAlloc and LocalAlloc are
	//      implemented as wrapper functions that call HeapAlloc using a
	//      handle to the process's default heap. Therefore, GlobalAlloc
	//      and LocalAlloc have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
		char  *lpbitmap = (char *)GlobalLock(hDIB);


	// ---- gets the "bits" from the bitmap and copies them into a buffer 
	//      which is pointed to by lpbitmap.
		GetDIBits(
				hdcWindow,
				hbmScreen,
				0,
				(UINT)bmpScreen.bmHeight,
				lpbitmap,
				(BITMAPINFO *)&bi,
				DIB_RGB_COLORS
			);

	// ---- add the size of the headers to the size of the bitmap to get the total file size
		DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


	// ---- offset to where the actual bitmap bits start.
		bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);


	// ---- size of the file
		bmfHeader.bfSize = dwSizeofDIB;


	// ---- bfType must always be BM for Bitmaps
		bmfHeader.bfType = 0x4D42; //BM   


	// ---- create numbered filename
		image_conter++;
		image_conter = image_conter >= 99999 ? 1 : image_conter;
		CHAR filename[64];
		sprintf(
				filename,
				"%5d.bmp",
				image_conter
			);


	// ---- tidy
		for (UINT i = 0; i < 5; i++) filename[i] = filename[i] == ' ' ? '0' : filename[i];


	// ---- create directory/filename
		CHAR create_filename[64];
		sprintf(
				create_filename,
				"%s/%s",
				app_directory,
				filename
			);


	// ---- create file
		HANDLE hFile = CreateFile(
				create_filename,
				GENERIC_WRITE,
				0,
				NULL,
				CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL
			);


	// ---- write bitmap data
		DWORD dwBytesWritten = 0;
		WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
		WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);


	// ---- close the handle for the file that was created
		CloseHandle(hFile);


	// ---- unlock and Free the DIB from the heap
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);


	// --- clean up
	done:

		return 1;
}


////////////////////////////////////////////////////////////////////////////////

