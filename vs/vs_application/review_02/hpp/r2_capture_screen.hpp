////////////////////////////////////////////////////////////////////////////////

// ---------- r2_capture_screen.hpp ----------
/*!
\file r2_capture_screen.hpp
\brief
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


int Model::CaptureAnImage(HWND hWnd)
{

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
				MessageBox(hWnd, "CreateCompatibleDC has failed","Failed", MB_OK);
				goto done;
			}


    // --- this is the best stretch mode
		SetStretchBltMode(hdcWindow,HALFTONE);


	// ---- source top left corner (currently same as window)
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD src_x_offset = sys_context->GetDWord(vs_system::SysContext::X_OFFSET);
		DWORD src_y_offset = sys_context->GetDWord(vs_system::SysContext::Y_OFFSET);


	// ---- select rect
		//#define SCREEN_1024
		#define SCREEN_1920

		#if defined(SCREEN_1024)
		DWORD src_width  = 1024;
		DWORD src_height = 576;
		#else
		DWORD src_width  = 1920;
		DWORD src_height = 1080;
		#endif

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

				#if defined(SCREEN_1024)
				src_x_offset = 448;
				src_y_offset = 124;
				#else
				src_x_offset = 0;
				src_y_offset = 0;
				#endif

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
			MessageBox(hWnd, "StretchBlt has failed","Failed", MB_OK);
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
			MessageBox(hWnd, "CreateCompatibleBitmap Failed","Failed", MB_OK);
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
			MessageBox(hWnd, "BitBlt has failed", "Failed", MB_OK);
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


INT Model::SaveBMP(HWND hWnd, HDC hdcWindow, HBITMAP hbmScreen)
{

	// ---- IF runtime THEN create directory name
		if ( pi_model->capture_image_runtime_flag )
		{

			// ---- zap
				pi_model->capture_image_runtime_flag = FALSE;


			// ---- directory name components
				SYSTEMTIME LocalTime;
				GetLocalTime(&LocalTime);  // NOT GetSystemTime(&SystemTime);
				char ps[6][32];
					sprintf(ps[0], "%4d", LocalTime.wYear);
					sprintf(ps[1], "%2d", LocalTime.wMonth);
					sprintf(ps[2], "%2d", LocalTime.wDay);
					sprintf(ps[3], "%2d", LocalTime.wHour);
					sprintf(ps[4], "%2d", LocalTime.wMinute);
					sprintf(ps[5], "%2d", LocalTime.wSecond);


			// ---- tidy components
				for (UINT i = 0; i < 6; i++)
				{
					UINT len = strlen(ps[0]);
					for (UINT j = 0; j<len; j++)
						ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
				}


			// ---- create name
					sprintf(
						pi_model->capture_image_dir_name,
						"%s%s%s_%s%s%s_Bitmap",
						ps[0],
						ps[1],
						ps[2],
						ps[3],
						ps[4],
						ps[5]
					);


			// ---- create directory
				if (CreateDirectory(pi_model->capture_image_dir_name, NULL) ||
					ERROR_ALREADY_EXISTS == GetLastError())
				{
					; // ok...
				}
				else
				{
					CHAR err_fail[64];
					sprintf(err_fail, "Create Directory %s has failed", pi_model->capture_image_dir_name);
					MessageBox(hWnd, err_fail, "Failed", MB_OK);
					goto done;
				}

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


	// ---- create filename
		CHAR filename[64];
		sprintf(
				filename,
				"%5d.bmp",
				pi_model->capture_image_number
			);
		pi_model->capture_image_number =
			pi_model->capture_image_number > 99999 ?
				1 : pi_model->capture_image_number + 1 ;
		for (UINT i = 0; i < 5; i++) filename[i] = filename[i] == ' ' ? '0' : filename[i];


	// ---- create directory/filename
		CHAR create_filename[64];
		sprintf(
			create_filename,
				"%s/%s",
				pi_model->capture_image_dir_name,
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


/*
	// ---- local 
		BOOL use_jpeg_encoding = FALSE;


	// ---- choose encoding
		if ( use_jpeg_encoding )
		{

			// ---- get the BITMAP from the HBITMAP
				BITMAP  bmpScreen;
				GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

			// ---- alocate
				INT bitCount = 32;
				DWORD dwBmpSize = ((bmpScreen.bmWidth * bitCount + 31) / 32) * 4 * bmpScreen.bmHeight;
				HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
				char  *lpbitmap = (char *)GlobalLock(hDIB);

			// ---- header
				BITMAPFILEHEADER   bmfHeader;
				BITMAPINFOHEADER   bi;

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


		}

		else
*/


////////////////////////////////////////////////////////////////////////////////
