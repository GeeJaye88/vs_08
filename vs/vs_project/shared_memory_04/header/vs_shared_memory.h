////////////////////////////////////////////////////////////////////////////////

// ---------- vs_shared_memory.h ----------
/*!
\file vs_shared_memory.h
\brief Declaration of shared memory API functions and definitions.
\copyright Copyright 2015 Observant Technology Limited. All rights reserved.
\license This project is released under NDA.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_SHARED_MEMORY
#define VS_SYSTEM_SHARED_MEMORY


// ---- vs definitions----
//#define VS_VIRTUAL_SURFACE


// ---- include OR define ----
#ifdef VS_VIRTUAL_SURFACE
#include "../../../vs_system/header/vs_inc_defs.h"
#include "../../../vs_system/header/vs_inc_dems.h"
#else
#define VS_ERROR 0
#define VS_OK    1
#endif


// ---- Shared Memory definitions ----
// #define VS_SHARED_MEMORY_DEVELOPMENT
// #define VS_SHARED_MEMORY_REPORT


// ---- windows include ----

#ifdef VS_SHARED_MEMORY_DEVELOPMENT
#include <SDKDDKVer.h>  // Defines the #defines that represent each version of Windows, IE, etc.
#include <windows.h>    // Included by "vs_inc_defs.h"
#include <iostream>     // Defines the standard input/output stream objects
#endif

#ifndef VS_VIRTUAL_SURFACE
#include <direct.h>     // Required for mkdir, etc
#include <stdio.h>      // Defines three variable types, several macros, and various functions for performing input and output
#endif

#ifdef VS_SHARED_MEMORY_DEVELOPMENT
#include <chrono>       // Deal with Durations, Time points and Clocks
#include <thread>       // Declares the thread class and the this_thread namespace
#endif


// ---------- Shared Memory structures include ----------
#include "../header/vs_shared_memory_structs.h"


// ---------- namespace ----------
namespace vs_system
{

	// ---------- Shared Memory Functions ----------

	// Initialise
	INT VS_InitialiseSharedMemoryAccess(CHAR*, ULONG, USHORT, USHORT);
	INT VS_InitialiseVirtualCamera(USHORT, USHORT, USHORT, ULONG, FLOAT, FLOAT, FLOAT, USHORT, USHORT);
	INT VS_InitialiseGetSizeOfSharedMemory();
	INT VS_InitialiseSharedMemory(BYTE*);

	// Get
	INT VS_GetControlAccessStatus(USHORT*, BYTE*);
	INT VS_GetControlFPS(UINT*, BYTE*);
	INT VS_GetControlOwnerHeartbeat(INT*, BYTE*);              // Not used
	INT VS_GetControlCameraHeartbeat(INT*, BYTE*);
	INT VS_GetControlMessage(CHAR*, BYTE*);
	INT VS_GetControlProcessStatus(USHORT*, BYTE*);
	INT VS_GetControlTimeStamp(ULONG*, BYTE*);
	INT VS_GetSizeOfSharedMemory(BYTE*);

	// Get Virtual Camera
	INT VS_GetVirtualCamera(VirtualCamera*, UINT, BYTE*);
	INT VS_GetVirtualCameraBufferHeader(BYTE*, UINT, BYTE*);
	INT VS_GetVirtualCameraBufferData(BYTE*, UINT, BYTE*);
	INT VS_GetVirtualCameraStatus(USHORT*, UINT, BYTE*);

	// Increment
	INT VS_IncrementControlOwnerHeartbeat(BYTE *);             // Not used
	INT VS_IncrementControlCameraHeartbeat(BYTE *);

	// Set
	INT VS_SetControlAccessStatus(USHORT, BYTE*);
	INT VS_SetControlFPS(UINT, BYTE*);
	INT VS_SetControlHeartbeat(INT, BYTE *);
	INT VS_SetControlMessage(CHAR*, BYTE*);
	INT VS_SetControlProcessStatus(USHORT, BYTE*);
	INT VS_SetControlTimeStamp(ULONG, BYTE*);

	// Set Virtual Camera
	INT VS_SetVirtualCameraXYF(INT, USHORT, FLOAT, FLOAT, FLOAT, BYTE*);
	INT VS_SetVirtualCameraStatus(INT, USHORT, BYTE*);

	// Session - not currently used...
	INT VS_BeginSharedMemory(CHAR*, DWORD*, BYTE**, HANDLE*);  // Not used

	// Session
	INT VS_CreateSharedMemory(CHAR*, DWORD, BYTE**, HANDLE*);
	INT VS_OpenSharedMemory(CHAR*, DWORD, BYTE**, HANDLE*);
	INT VS_EndSharedMemory(BYTE**, HANDLE*);

	// Test
	INT VS_TestSharedMemory(BYTE*);

	// ---------- Time Functions ----------
	LONG   VS_GetFrameCount();
	DOUBLE VS_GetCurrentTime();
	DOUBLE VS_GetNextTime();
	INT    VS_IncrementTime();
	INT    VS_StartTime(UINT);

}

#endif // --- VS_SYSTEM_SHARED_MEMORY