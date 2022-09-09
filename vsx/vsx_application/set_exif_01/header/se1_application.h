////////////////////////////////////////////////////////////////////////////////

// ---------- se1_application.h ----------
/*!
\file se1_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once;


// ---------- System include ----------

#include <stdio.h>   // Defines the structures, values, macros, and functions used by the level 2 "standard I/O" routines
#include <string.h>  // Function declarations for the string manipulation functions
#include <ctype.h>   // Defines macros for character classification/conversion 
#include <iostream>  // Microsoft I/O stream standard header 

#include <windows.h>           // Required for Gdiplus defintions, etc...
#include <gdiplus.h>           // GDI+ public header file
#include <Strsafe.h>           // Required for StringCchCopyW in PropertyTypeFromWORD

// recommended but not needed...
// commeted out 1/11/17 to avoid D2Derr defintions
// #include <D2d1helper.h>     // PixelFormat ( Helper files over the D2D interfaces and APIs )

#include <GdiPlusImaging.h>    // GDI+ Imaging GUIDs
#include <GdiPlusImageCodec.h> // GDI+ Codec Image APIs


#pragma comment(lib,"gdiplus.lib")


// ---------- VS include ----------
#include "../../../../vs/vs_system/header/vs_inc_defs.h"
#include "../../../../vs/vs_system/header/vs_app_dependency.h"



// ---------- namespace ----------
namespace set_exif_01
{

	// ---------- application class ----------
	class application
	{

	public:

		// Constructor/Destructor
		application();
		virtual ~application();

		// Framework methods
		INT Setup(vs_system::AppDependency *);
		INT Display();
		INT Cleanup();

		INT E1();
		INT E2();
		INT E3();

	private:

		// Private implementation
		class PI_Application; static PI_Application *pi_app;

	}; // application class


} // se1_application namespace

