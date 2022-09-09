////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_timeline.h ----------
/*!
\file vs_img_timeline.cpp
\brief ImgTimeline definitions, declarations and class interfaces
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_IMG_TIMELINE
#define VS_SYSTEM_IMG_TIMELINE


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


// ---- vs library include ----
#include "../header/vs_vnode.h"
#include "../header/vs_char_proc.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- ImgTimeline ----------
class ImgTimeline
{
public:

	// Constructor/destructor
	ImgTimeline();
	virtual ~ImgTimeline();

	// Timeline constants
	enum {

		// Values returned from Increment method

		THIS_NOT_AVAILABLE = 1,  // BAD : Should never, ever, reach this event
		NEXT_NOT_AVAILABLE = 2,  // BAD : Ditto
		CLIP_END_STOP      = 3,  // Good : Natural clip end stop
		LAST_MINUTE_STOP   = 4,  // Good : Natural last minute stop 
		STOP               = 5,  // Good : Selected stop
		INCREMENTED        = 6,  // Increment ok

		// Values passed to Goto methods

		GOTO_PREVIOUS      = 7, 
		GOTO_NEXT          = 8,
		GOTO               = 9,
		GOTO_BACKWARD      = 10, // Probably not needed
		GOTO_FORWARD       = 11, // Ditto

		GOTO_FIRST_MINUTE  = 12,
		GOTO_LAST_MINUTE   = 13,
		GOTO_LAST_FRAME    = 14,

		GOTO_CLIP          = 20, // Goto clip IN OR OUT
		GOTO_CLIP_INPOINT  = 1,
		GOTO_CLIP_OUTPOINT = 2

	};

	// Methods that return status
	BOOL IsTimelineCreated();
	BOOL IsTimelineAvailable();
	BOOL IsThisMinuteAvailable();
	BOOL IsNextMinuteAvailable();
	BOOL IsThisMinuteReady();
	BOOL IsNextMinuteReady();

	// Methods that set status
	INT  SetThisMinuteStatus(BOOL);
	INT  SetNextMinuteStatus(BOOL);

	// Command
	INT  Create(CHAR *);
	INT  Scan(CHAR *);

	// Get
	INT  GetDateTimeMilliseconds_1(UINT, UINT *, LLONG *);  // NOT USED
	INT  GetDateTimeMilliseconds_3(UINT, LLONG *);
	INT  GetDateTimeStride(UINT, UINT *);
	INT  GetDateTimeList(UINT, UINT *, LLONG[]);
	INT  GetFilename(std::string &);
	INT  GetThisMinuteHandle(INT *);
	INT  GetTotalFolders(UINT *);
	INT  GetPrevNextState(UINT);
	INT  GetFirstLastState();

	// Get
	INT  GetDateTimeValue(UINT *);

	// Get & Set DateTime
	INT  GetDateTime(UINT *);
	INT  SetDateTime(UINT *);

	// Goto
	INT  Goto(UINT, INT, UINT, INT[]);

	// Goto contextual
	INT  GotoContextualFirst(UINT);     // NOT USED
	INT  GotoContextualLast(UINT);      // NOT USED

	// Goto frame
	INT  GotoFirstMinuteFirstFrame();
	INT  GotoLastMinuteFirstFrame();    // NOT USED
	INT  GotoLastMinuteLastFrame(UINT);

	// Last minute stuff
	BOOL IsLastMinute();
	INT  GetLastMinuteMSDifference(INT *);
	INT  GetLastMinteTotalImages(UINT *);
	INT  RefreshCreate(CHAR *);
	INT  RefreshLastMinute();

	// Time
	INT  Increment(INT);

	// Utility
	INT  List(CHAR);

private:

	// Private implementation
	class PI_Props; PI_Props *pi_props;

}; // ImgTimeline


} // namespace


#endif // VS_SYSTEM_IMG_TIMELINE

////////////////////////////////////////////////////////////////////////////////