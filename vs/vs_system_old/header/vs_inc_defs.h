////////////////////////////////////////////////////////////////////////////////

// ---------- vs_inc_defs.h ----------
/*!
\file vs_inc_defs.h
\brief os and vs defintions 
\author Gareth Edwards
\note 1. Definitions for "__unix__" or "linux"
\note 2. Windows C++ if definitions "_WIN32" or "WIN32"
\note 3. Windows STRICT now defined in windef.h
\note 4. See windef.h for windows basic Windows Type Definitions 
*/

#pragma once


// ---- select ----
	#ifdef __unix__
	#define OS_UNIX
	#elif defined(linux)
	#define OS_UNIX
	#elif defined(_WIN32) || defined(WIN32)
		#ifndef OS_WINDOWS
			#define OS_WINDOWS
		#endif
	#endif


// ---- use preprocessor directive to turn OFF MS warnings ----
	#ifdef OS_WINDOWS
	#pragma warning(disable: 4996)
	#endif


// ---- Windows include ---- 
	#ifdef OS_WINDOWS 
	#include <windows.h>
	#include <string.h>
	#include <direct.h>
	#include <math.h>
	#define getcwd _getcwd 
	#define chdir _chrdir
	#endif


// ---- Windows not include ----
	//#include <sys/stat.h>
	//#include <fcntl.h>        // for console
	//#include <fstream>        // for io, etc, ...
	//#include <io.h>           // for console io, etc, ...
	//#include <iostream>       // for io, etc, ...
	//#include <string.h>       // for charUtilities, random, ...
	//#include <windows.h>      // windows
	//#include <io.h>
	//#include <stdlib.h>
	//#include <stdio.h>


// ---- UNIX include ----
	#ifdef OS_UNIX
	#include <unistd.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#endif


// ---- xtra types ----
	#define LLONG   long long
	#define ULLONG  unsigned long long


// ---- windows types ----
	#ifndef OS_WINDOWS
	#define BOOL    bool
	#define CHAR    char
	#define BYTE    unsigned char
	#define UCHAR   unsigned char
	#define DWORD   unsigned long
	#define ULONG   unsigned long
	#define WORD    unsigned short
	#define UINT    unsigned int
	#define INT     int
	#define SHORT   short
	#define LONG    long
	#define FLOAT   float
	#define DOUBLE  double
	#define VOID    void
	#endif


// ---- windows API structs ----
	#ifndef OS_WINDOWS
	typedef struct VS_Rect
	{
		LONG left;
		LONG top;
		LONG right;
		LONG bottom;
	};
	#define RECT VS_Rect
	typedef struct VS_Point {
		LONG x;
		LONG y;
	};
	#define POINT VS_Point
	#endif


// ---- windows NULL value ----
	#ifndef OS_WINDOWS
	#define NULL  0
	#endif


// ---- vs error codes ----
	#define VS_ABORT           -2                   // recommend abort
	#define VS_EXIT            -1                   // recommend exit
	#define VS_ERROR            0                   // failed


// ---- vs success codes ----
	#define VS_OK               1                   // ok
	#define VS_COMPLETED        2                   // ok and completed


// ---- vs success macro ----
	#define VS_SUCCEEDED(i) ((INT)(i)>=VS_ERROR)    // succeeded

