////////////////////////////////////////////////////////////////////////////////

// ---------- vs_arc_clip.h ----------
/*!
\file vs_arc_clip.h
\brief Interface for the Arch(ive) Util(ity) class.
\author Gareth Edwards
*/


#pragma once

#include <ctime>
#include <stdio.h>
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"


// ---------- namespace ----------
namespace review_02
{


// ---------- ArchTime class ----------
class ArchTime
{

	public:

		enum
		{
			YEAR    = 0,
			MONTH   = 1,
			DAY     = 2,
			HOUR    = 3,
			MINUTE  = 4,
			SECOND  = 5,
			MSECOND = 6
		};

	public:
	 
		// ---- c/dtor ----
		ArchTime();
		virtual ~ArchTime();
		UINT Get(UINT);
		INT  GetClipName(CHAR *);
		INT  GetMilliseconds(LLONG *);
		INT  EqualMinute(ArchTime*);
		INT  EqualTo(ArchTime*, UINT);
		INT  Set(UINT, UINT);
		INT  Set(UINT, UINT, UINT, UINT, UINT, UINT, UINT);

		// ---- no interface shortcut, e.g. _time[YEAR], etc. ----
		UINT _time[7]; 

	private:

		class PI_ArchTime; PI_ArchTime *_pi_arch_time;

};


// ---------- ArchClip class ----------
class ArchClip
{

	public:
	 
		// ---- c/dtor ----
		ArchClip();
		virtual ~ArchClip();
		INT CopyClip(
				ArchTime *,
				ArchTime *,
				CHAR *,
				INT *,
				INT *
			);


	private:

		class PI_ArchClip; PI_ArchClip *_pi_arch_util;

};


// ---------- ArchData struct ----------

class application;

struct ArchData
{
	application *application = NULL;
	ArchTime clip_datetime[2];
	BOOL button_on = TRUE;
	BOOL thread_complete = TRUE;
	INT  total_files = 0;
	INT  total_processed = 0;
	ArchClip arch_clip;
};


}
