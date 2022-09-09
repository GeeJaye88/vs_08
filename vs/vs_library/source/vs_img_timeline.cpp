////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_timeline.cpp ----------
/*!
\file vs_img_timeline.cpp
\brief Implementation of the ImgTimeline class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_img_timeline.h"


// ---- xtra include for high resolution clock ----
#include <chrono>
#include <inttypes.h>


// ---- namespace ----
using namespace vs_system;


/*

std::chrono::high_resolution_clock::time_point hrc_time_start;
std::chrono::high_resolution_clock::time_point hrc_time_end;
LONG hrc_duration     = 0;

hrc_time_start = std::chrono::high_resolution_clock::now();

// code...

hrc_time_end = std::chrono::high_resolution_clock::now();
hrc_duration = std::chrono::high_resolution_clock::duration(hrc_time_end - hrc_time_start).count();

OR:

double start_time     = 0;
double end_time       = 0;
double total_time     = 0;

end_time     = (double)timeGetTime();
total_time   = end_time - start_time;

*/

////////////////////////////////////////////////////////////////////////////////


// ---------- Millisecond class ------------------------------------------------

struct Millisecond
{
	BOOL frame;
	CHAR ss_msc[7];
};


// ---------- ImgTimelineMinute class ------------------------------------------

class ImgTimelineMinute
{
public:

	// Constructor/destructor
	ImgTimelineMinute();
	virtual ~ImgTimelineMinute();

	// Methods
	INT Cleanup();
	INT Initialise();

	// Properties
	CHAR path[VS_MAXCHARLEN];      // "...path/YYYY/MM/DD/HH/MM/" 
	CHAR file_less_ss_msc[21];     // "YYYYMMDD_HHMM"
	Millisecond ms_lut[60000];     // Frame flag, "SS_MSC" and value

	UINT  seconds_lut[60];         // Per seconds frame count
	UINT  ten_ms_frame_lut[6000];  // Per ten millisecond frame #
	UINT  ten_ms_sample_lut[6000]; // Per ten millisecond frame count

	UINT  image_count;              // # of images in folder

	BOOL  start_here;               // Playback starts "inside" minute
	BOOL  stop_here;                // Playback stops "inside" minute

	INT   first_millisecond;        // Of first img
	INT   last_millisecond;         // Of last img
	INT   playback_millisecond;     // Current millisecond

};


// ---------- ImgTimelineMinuteThreads class -----------------------------------

class ImgTimelineMinuteThreads
{
public:

	// Constructor/destructor
	ImgTimelineMinuteThreads();
	virtual ~ImgTimelineMinuteThreads();

	// Returns TRUE if "this" thread completed ELSE FALSE
	BOOL IsThisMinuteAvailable()
	{
		if ( this_minute_thread_completed )
		{
			DWORD result = GetThreadId(this_minute_thread_handle);
			if ( result != 0 ) CloseHandle(this_minute_thread_handle);
			return true;
		}
		return false;
	}

	// Returns TRUE if "next" thread completed ELSE FALSE
	BOOL IsNextMinuteAvailable()
	{
		if ( next_minute_thread_completed )
		{
			DWORD result = GetThreadId(next_minute_thread_handle);
			if ( result != 0 ) CloseHandle(next_minute_thread_handle);
			return true;
		}
		return false;
	}

	// Set "this" thread status
	INT SetThisMinuteStatus(BOOL status)
	{
		if ( this_minute_thread_completed )
		{
			this_minute_thread_completed = status;
			return VS_OK;
		}
		return VS_ERROR;
	}

	// Set "this" thread status
	INT SetNextMinuteStatus(BOOL status)
	{
		if ( next_minute_thread_completed )
		{
			next_minute_thread_completed = status;
			return VS_OK;
		}
		return VS_ERROR;
	}

	// "this" and "next"
	HANDLE  this_minute_thread_handle;
	BOOL    this_minute_thread_completed;
	HANDLE  next_minute_thread_handle;
	BOOL    next_minute_thread_completed;
};


// ---------- ImgTimelineFolders class -----------------------------------------

class ImgTimelineFolders
{
public:

	// Constructor/destructor
	ImgTimelineFolders();
	virtual ~ImgTimelineFolders();

	// Methods
	INT Cleanup();
	INT Cleanup(VNode *);
	INT Initialise(std::string, std::string);

	// Get
	INT GetDateTimeMilliseconds_1_rUpUpL(UINT, UINT *, LLONG *);   // Interface to GetDateTimeMilliseconds_3_rUpL
	INT GetDateTimeMilliseconds_2_rUrLpL(UINT, LLONG, LLONG *);    // Interface to GetDateTimeMilliseconds_3_rUpL
	INT GetDateTimeMilliseconds_3_rUpL(UINT, LLONG *);
	INT GetDateTimeStride(UINT, UINT *);
	INT GetDateTimeList(UINT, UINT *, LLONG[]);
	INT GetFilename(std::string &);
	INT GetThisMinuteHandle(INT *);

	// Get & Set DateTime
	INT  GetDateTime(UINT *);
	INT  SetDateTime(UINT *);

	// Goto
	VNode * GetMinuteFolder(VNode*, UINT, UINT, INT[]);
	VNode * GetMinuteLeafFolder(VNode*);

	// Set
	INT SetDateTimeStack(VNode*);

	// Goto
	INT GotoContextualFirst(UINT);
	INT GotoContextualLast(UINT);

	INT GotoFirstMinuteFirstFrame();
	INT GotoLastMinuteFirstFrame();
	INT GotoLastMinuteLastFrame();

	// Utility
	INT AdvanceMinuteFolders();

	// Thread status test
	BOOL IsTimelineAvailable()
	{
		if ( create_timeline_thread_completed )
		{
			DWORD result = GetThreadId(create_timeline_thread_handle);
			if ( result != 0 ) CloseHandle(create_timeline_thread_handle);
			return true;
		}
		return false;
	}

	// Timeline created status
	BOOL IsTimelineCreated()
	{
		if ( root_folder != NULL )
		{
			return true;
		}
		return false;
	}

	// MS thread properties
	HANDLE  create_timeline_thread_handle;
	BOOL    create_timeline_thread_completed;

	// MS thread properties
	HANDLE  scan_thread_handle;
	BOOL    scan_thread_completed;

	// Properties "passed" forward
	//
	// The image filename returned BEFORE incrementing
	// into a new minute "finds" a new image.
	//
	std::string pass_fwd_filename;
	//
	// The additional "pause" time to correctly wait before
	// incrementing into the minute.
	//
	// TODO - GE
	//
	INT pass_fwd_ms;

	// Properties
	std::string path;
	std::string mask;

	// Properties
	std::string scan_path;
	std::string scan_mask;

	// Counters
	UINT   total_folders;

	// Archive...
	VNode *root_folder;

	// Quick start
	VNode *first_minute_folder;
	VNode *last_minute_folder;

	// Minute buffers
	VNode *this_minute_folder;
	VNode *next_minute_folder;

	// Datetime
	//
	// This is THE home of "where am I"
	//
	// Set by SetDateTimeStack method, each time a "this_minute_folder" is initialised
	// Get timeline list by GetDateTimeList method
 	// Get datetime text by ImgTimeline GetDateTimeValue method 
	//
	VNode *datetime_stack[8];      // 0 to 5: A, Y, M, D, H and M - see GetDateTimeList method

	// Set in:
	//    ImgTimeline::GetDateTimeValue AND
	//    ImgTimeline::Increment
	//
	UINT   datetime_second;        // Used in place of a datetime_stack[6] - see GetDateTimeList method
	UINT   datetime_millisecond;   // Used in place of a datetime_stack[7]

	// Set in:
	//    GetDateTimeMilliseconds
	//
	UINT  datetime_count[8];     // Millisecond count

	// Goto milliseconds
	INT    goto_millisecond;       // Goto millisecond
	INT    delay_millisecond;      // Goto delay millisecond
	BOOL   hunt_millisecond;       // Set TRUE if ONLY seconds supplied

}; 


////////////////////////////////////////////////////////////////////////////////


// ---------- Private implementation PI_Props class----------
class ImgTimeline::PI_Props
{
public:

	// Constructor/destructor
	PI_Props();
	virtual ~PI_Props();

	// ImgTimelineMinute thread methods
	static DWORD WINAPI T_CreateMinute(LPVOID);
	static INT T_CreateMinute(BOOL, UINT, VNode*);
	static INT T_FormatMsLut(BOOL, ImgTimelineMinute *tm);
	static INT T_InitSMLuts(BOOL, ImgTimelineMinute *tm);

	// ImgTimelineFolders create thread methods
	static DWORD WINAPI T_CreateFolders(LPVOID);
	static INT T_ScanFolders();
	static INT T_ScanFolders_01();
	static INT T_AddFolder(BOOL);
	static INT T_CrossLinkFolders(VNode *, UINT, VNode *[]);

	// List methods
	static INT ListAllFolders(VNode*);
	static INT ListCrossLinkFolders(VNode*);
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Create ----------
static ImgTimelineMinute        img_timeline_minute[2];
static ImgTimelineMinuteThreads img_timeline_minute_threads;
static ImgTimelineFolders       img_timeline_folders;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             ImgTimelineMinute                              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ImgTimelineMinute::ImgTimelineMinute()
{
	// Initialise seconds list
	memset(seconds_lut, 0, 60);

	// Initialise millisecond list
	memset(ten_ms_frame_lut, 0, 6000); 
	memset(ten_ms_sample_lut, 0, 6000);

	// Initialise
	Cleanup();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgTimelineMinute::~ImgTimelineMinute()
{
	;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\note thread safe -  test returned value
*/
INT ImgTimelineMinute::Cleanup()
{
	// IF thread completed THEN ok
	path[0]              = 0;
	file_less_ss_msc[0]  = 0;
	image_count          = 0;
	start_here           = false;
	stop_here            = false;
	first_millisecond    = -1;
	last_millisecond     = -1;
	playback_millisecond = -1;
	return VS_OK;
}


// ---------- Initialise ----------
/*!
\brief Cleanup AND Initialise
\author Gareth Edwards
*/
INT ImgTimelineMinute::Initialise()
{
	INT result = Cleanup();
	return result;
}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                         ImgTimelineMinuteThreads                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ImgTimelineMinuteThreads::ImgTimelineMinuteThreads()
{
	this_minute_thread_completed = true;
	this_minute_thread_handle    = NULL;
	next_minute_thread_completed = true;
	next_minute_thread_handle    = NULL;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ImgTimelineMinuteThreads::~ImgTimelineMinuteThreads()
{
	;
}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                             ImgTimelineFolders                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
ImgTimelineFolders::ImgTimelineFolders()
{
	create_timeline_thread_completed = true;
	create_timeline_thread_handle = NULL;
	root_folder = NULL;
	pass_fwd_filename = "";
	pass_fwd_ms = 0;
	path = "";
	mask = "";
	scan_path = "";
	scan_mask = "";

	Cleanup();
}


// ---------- Destructor ----------
/*!
\brief constructor
\author Gareth Edwards
\note NOT thread safe - call Cleanup and test returned value
*/
ImgTimelineFolders::~ImgTimelineFolders()
{
	// Not thread safe!
	// INT result = Cleanup();
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\note thread safe -  test returned value
*/
INT ImgTimelineFolders::Cleanup()
{
	// IF thread available AND created THEN 
	if ( IsTimelineAvailable() && IsTimelineCreated() )
	{
		// The additional "pause" time to correctly wait before
		// incrementing into the minute.
		INT pass_fwd_ms = 0;

		// Clean folder archive...
		if ( root_folder )
		{
			Cleanup(root_folder);
			if (root_folder != NULL)
			{
				delete root_folder;
				root_folder = NULL;
			}
		}

		// Counters
		total_folders = 0;

		// Quick start
		first_minute_folder = NULL;
		last_minute_folder = NULL;

		// Minute buffers
		this_minute_folder = NULL;
		next_minute_folder = NULL;

		// Datetime
		//
		// Note:  Set in GetDateTimeMilliseconds method
		//
		for ( UINT i=0; i<8; i++)
		{
			datetime_count[i] = 0;
			datetime_stack[i] = NULL;
		}
		datetime_second = 0;
		datetime_millisecond = 0;

		// Goto
		goto_millisecond = -1;
		hunt_millisecond = FALSE;

		// Done
		return VS_OK;
	}

	// Error - thread NOT completed
	return VS_ERROR;
}


// ---------- Cleanup ----------
/*!
\brief Recursively delete folders
\author Gareth Edwards
\note thread safe -  test returned value
*/
INT ImgTimelineFolders::Cleanup(VNode *node)
{
	VNode *child = node->GetFirst();
	while ( child )
	{
		VNode *next = child->GetNext();
		Cleanup(child);
		if ( child != NULL )
		{
			delete child;
			child = next;
		}
	}
	return VS_OK;
}


// ---------- Initialise ----------
/*!
\brief Cleanup AND Initialise folders
\author Gareth Edwards
*/
INT ImgTimelineFolders::Initialise(std::string p, std::string m)
{
	// Cleanup EVERYTHING !!!
	INT result = Cleanup();

	//if ( result == VS_OK )
	{
		path = p;
		mask = m;
	}
	return result;
}


// ---------- GetDateTimeMilliseconds_1_rUpUpL ----------
/*!
\brief
\author Gareth Edwards
\note Invoked with an intitialised date time stack

\note Invoke ONLY by  ImgTimeline::GetDateTimeMilliseconds_1() method

*/
INT ImgTimelineFolders::GetDateTimeMilliseconds_1_rUpUpL(
		UINT index,
		UINT *stack,
		LLONG *millisecond
	)
{
	// Initialise a date time stack
	for ( UINT i=0; i<8; i++ )
	{
		datetime_count[i] = stack[i];
	}

	// Now calculate milliseconds
	INT result = GetDateTimeMilliseconds_3_rUpL(index, millisecond);

	// Done
	return result;
}


// ---------- GetDateTimeMilliseconds_2_rUrLpL ----------
/*!
\brief
\author Gareth Edwards
\note Invoked with a value to be assigned to an intialised datetime_count at [index]

\note Invoke ONLY by ImgTimelineFolders::GetDateTimeList() method.

*/
INT ImgTimelineFolders::GetDateTimeMilliseconds_2_rUrLpL(
		UINT index,
		LLONG value,
		LLONG *millisecond
	)
{
	// Initialise a date time stack
	for ( UINT i=0; i<6; i++ )
	{
		datetime_count[i] = atoi(datetime_stack[i]->GetName());
	}
	datetime_count[6] = datetime_second;
	datetime_count[7] = datetime_millisecond;

	// Select initialisation
	switch ( index )
	{
		// Handle Year / Month / Day / Hour / Minute
		default:
			{
				 datetime_count[index] = (UINT)value;
			}
			break;

		// Handle (60) Seconds
		case 6:
			{
				datetime_count[6] = (UINT)value;
				datetime_count[7] = 0;
			}
			break;

		// Handle (100 x 10) Millisecond intervals
		case 7:
			{
				datetime_count[7] = (UINT)value;
			}
			break;
	}

	// Now calculate milliseconds
	INT result = GetDateTimeMilliseconds_3_rUpL(index, millisecond);

	// Done
	return result;
}


// ---------- GetDateTimeMilliseconds ----------
/*!
\brief
\author Gareth Edwards
\note Intialise the datetime_count array

\note invoked by:

1. ImgTimelineFolders::GetDateTimeMilliseconds_1_rUpUpL() method,
2. ImgTimelineFolders::GetDateTimeMilliseconds_2_rUrLpL() method, and
3. ImgTimeline::GetDateTimeMilliseconds_3() method


*/
INT ImgTimelineFolders::GetDateTimeMilliseconds_3_rUpL(
		UINT index,
		LLONG *millisecond
	)
{

	// Local
	UINT day_lut[12] ={ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	UINT year_normal = 365;
	UINT year_leap   = 366;

	// Init total ms
	*millisecond = 0;

	// Add total ms up to "this" year
	LLONG previous_year = datetime_count[1]-1;
	for ( LLONG year=2011; year<=previous_year; year++ )
	{
		unsigned long long days = year % 4 == 0 ? year_leap : year_normal;
		*millisecond += days * 24 * 60 * 60 * 1000;
	}

	// Year
	if ( index == 1 ) return VS_OK;

	// Add total days up to "this" month
	LLONG this_year = datetime_count[1];
	LLONG previous_month = datetime_count[2]-1;
	unsigned long long total_days = 0;
	for ( LLONG month=0; month<previous_month; month++ )
	{
		total_days += this_year % 4 == 0 ? (month == 1 ? 29 : 28) : day_lut[month];
	}

	//for (UINT i = 1; i<8; i++) printf("%d, ", datetime_count[i]);

	// Month
	*millisecond += total_days * 24 * 60 * 60 * 1000;
	if (index == 2)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Day
	*millisecond += datetime_count[3] * 24 * 60 * 60 * 1000;
	if (index == 3)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Hours
	*millisecond += datetime_count[4] * 60 * 60 * 1000;
	if (index == 4)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Minutes
	*millisecond += datetime_count[5] * 60 * 1000;
	if (index == 5)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Seconds
	*millisecond += datetime_count[6] * 1000;
	if (index == 6)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Milliseconds
	*millisecond += datetime_count[7];
	if (index == 7)
	{
		//printf("%I64d\n", *millisecond);
		return VS_OK;
	}

	// Done
	return VS_ERROR;
}


// ---------- GetDateTimeStride ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimelineFolders::GetDateTimeStride(UINT index, UINT *stride)
{
	*stride = 4;
	return VS_OK;
}


// ---------- GetDateTimeList ----------
/*!
\brief Create a DateTime reference LUT for updating timeline
\author Gareth Edwards
\note

LUT is dimensioned 100 * stride (e.g. 4)

LUT is initialised [ 0, -1, -1, -1 ], [ , , , ], etc. per item

3 Modes:

1: Year / Month / Day / Hour / Minute :
      list + 0 == value, e.g. year, month, etc..
		list + 1 == associated node (if any) typecast to INT
		list + 2 == value modulus display_modulus (e.g. 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1 ,2, etc....)

2: Minute:
      list[list_offset]   = second;
		list[list_offset+1] = current_minute->seconds_lut[second];
		list[list_offset+2] = list[list_offset] % 10;
		result = GetDateTimeMilliseconds(index, list[list_offset], &list[list_offset+3]);

3 Millisecond:
      UINT list_offset    = millisecond * stride;
		list[list_offset]   = current_minute->ten_ms_frame_lut[millisecond + offset];
		list[list_offset+1] = current_minute->ten_ms_sample_lut[millisecond + offset];
		list[list_offset+2] = list[list_offset] % 10;
		result = GetDateTimeMilliseconds(index, list[list_offset], &list[list_offset+3]);

Note: list + 3 is unused!

Note: Updated on 7/1/2016 to return correct non-modulus values for % 10 minutes and seconds

*/
INT ImgTimelineFolders::GetDateTimeList(UINT index, UINT *items, LLONG list[])
{
	// IF minute archive folder THEN
	if ( this_minute_folder )
	{
		// Stride - fixed at 4 right now!!!!
		UINT stride;
		INT result = GetDateTimeStride(index, &stride);

		// Initialise:
		for ( UINT item=0; item<100; item++ )
		{
			UINT list_offset    =  item * stride;
			list[list_offset]   =  0;
			list[list_offset+1] = -1;
			list[list_offset+2] = -1;
			list[list_offset+3] = -1;
		}

		/*
		Note: IF this_minute_folder THEN datetime_stack has been initialised!
		*/

		// Select list initialisation
		switch ( index )
		{

			// Handle Year / Month / Day / Hour / Minute
			default:
				{
					// Month day count lut
					UINT day_lut[12]  ={ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

					// Year - and catch leap year...
					CHAR *name = datetime_stack[1]->GetName();
					UINT year = strcmp(name,"") != 0 ? atoi(datetime_stack[1]->GetName()) : 0;
					if ( year % 4 == 0 ) day_lut[1] = 29;

					// Month - Possible BUG - ammended 7/12/15 to ... - 1 : 0;
					name = datetime_stack[2]->GetName();
					UINT month = strcmp(name, "") != 0 ? atoi(datetime_stack[2]->GetName()) - 1 : 0;

					// # items == valkue stored in AYMD MHSM lut
					UINT datetime_lut[8]  = { 0,   10, 12, day_lut[month], 24, 60, 60, 100 };
					*items = datetime_lut[index];

					// Start value for each "type" (e.g. count years from 2011)
					UINT datetime_base[8] = { 0, 2011, 1, 1, 0,  0,  0,  0 };


					// NEW to display correct value
					UINT datetime_mod[8] = { 0, 1, 1, 1, 1,  1, 1, 1 };

					// ... WAS
					//    Display modulus value (e.g. 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1 ,2, etc....)
					//    UINT datetime_mod[8] = { 0, 1, 1, 1, 1, 10, 10, 10 };


					// Assign list values
					//
					// list + 0 == value, e.g. year, month, etc.. 
					// list + 1 == -1
					// list + 2 == value modulus display_modulus 
					//
					for ( UINT item=0; item<*items; item++ )
					{
						UINT list_offset    = item * stride;
						list[list_offset]   = item + datetime_base[index];
						list[list_offset+1] = -1;
						list[list_offset+2] = 
								datetime_mod[index] == 1 ?
								list[list_offset] :
								list[list_offset] % datetime_mod[index];
						result = GetDateTimeMilliseconds_2_rUrLpL(index, list[list_offset], &list[list_offset+3]);

						// NEW only for minutes
						if (index == 5)
						{
							// NEW to display correct value ONLY on % 10
							if (item % 10 == 0) list[list_offset + 2] = item; else list[list_offset + 2] = item % 10;
						}

					}

					// Overlay (in list + 1) associated node (if any) typecast to INT
					VNode *stack_node = datetime_stack[index];
					if ( stack_node != NULL )
					{
						VNode *parent = stack_node->GetParent();
						if ( parent != NULL )
						{
							VNode *node = parent->GetFirst();
							while ( node )
							{
								INT node_index = atoi(node->GetName());
								UINT list_offset = ( node_index - datetime_base[index] ) * stride;
								list[list_offset+1] = (INT)node;
								node = node->GetNext();
							}
						}
					}
					return VS_OK;
				}
				break;

			// Handle (60) Seconds
			case 6:
				{
					ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
					*items = 60;
					for ( UINT second=0; second<60; second++ )
					{
						UINT list_offset    = second * stride;
						list[list_offset]   = second;
						list[list_offset+1] = current_minute->seconds_lut[second];
						list[list_offset+2] = list[list_offset] % 10;
						result = GetDateTimeMilliseconds_2_rUrLpL(index, list[list_offset], &list[list_offset+3]);

						// NEW to display correct value ONLY on % 10
						if (second % 10 == 0) list[list_offset + 2] = second; else list[list_offset + 2] = second % 10;

					}
					return VS_OK;
				}
				break;

			// Handle (100 x 10) Millisecond intervals
			case 7:
				{
					ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
					*items = 100;
					UINT offset = datetime_second * 100;
					LONGLONG ms_max = -1;
					for ( UINT millisecond=0; millisecond<100; millisecond++ )
					{
						UINT list_offset    = millisecond * stride;
						list[list_offset]   = current_minute->ten_ms_frame_lut[millisecond + offset];
						list[list_offset+1] = current_minute->ten_ms_sample_lut[millisecond + offset];
						list[list_offset+2] = list[list_offset] % 10;
						result = GetDateTimeMilliseconds_2_rUrLpL(index, list[list_offset], &list[list_offset+3]);

						list[list_offset + 3] = list[list_offset + 3] < ms_max ? ms_max : list[list_offset + 3];
						ms_max = list[list_offset + 3];
						// printf("%I64d\n", list[list_offset + 3]);

						// NEW to display correct value
						list[list_offset + 2] = millisecond * 10;

					}
					return VS_OK;
				}
				break;
		}
	}

	// No minute folder OR illegal index value
	return VS_ERROR;
}


// ---------- GetFilename ----------
/*!
\brief Get current image file name
\author Gareth Edwards
*/
INT ImgTimelineFolders::GetFilename(std::string &filename)
{
	// IF minute archive folder THEN
	INT result = VS_OK;
	if (this_minute_folder)
	{
		// Current minute = (typecast) minute_folder associated data
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();

		// IF current millisecond less than first millisceond THEN 
		if ( current_minute->playback_millisecond < current_minute->first_millisecond ) 
		{
			// Return passed forward image filename
			filename = pass_fwd_filename;
		}
		// ELSE create and then return image filenmame
		else
		{
			// BUG
			//
			// current_minute
			// 
			// Assemble into temp, then copy into filename
			CHAR temp[256];
			sprintf (temp, "%s%s%s.jpg",
				current_minute->path,
				current_minute->file_less_ss_msc,
				current_minute->ms_lut[current_minute->playback_millisecond].ss_msc);
			filename = temp;
		}
	}
	// ELSE ooops - no this minute - return error
	else
	{
		filename = "";
		result = VS_ERROR;
	}

	// Done
	return result;
}


// ---------- GetMinuteFolder ----------
/*!
\brief 
\author Gareth Edwards
*/
VNode * ImgTimelineFolders::GetMinuteFolder(VNode* node, UINT max, UINT index, INT stack[])
{
	//	IF illegal index THEN reject
	if ( index == -1 ) return NULL;

	// IF match THEN
	if ( atoi(node->GetName()) == stack[index] )
	{
		if ( max <= 4 && max == index )
			return node;

		// IF next index NOT illegal THEN
		if ( stack[index+1] != -1 )
		{
			// For each child
			VNode *child = node->GetFirst();
			while ( child )
			{
				// IF YDMHM THEN
				if ( index+1 <= 5 )
				{
					VNode *result = GetMinuteFolder(child, max, index+1, stack);

					// IF matched THEN return
					if ( result != NULL ) return result;
				}
				child = child->GetNext();
			}
		}

		// ELSE matched
		else
		{
			return node;
		}
	}

	// Done
	return NULL;
}


// ---------- GetMinuteLeafFolder ----------
/*!
\brief
\author Gareth Edwards
*/
VNode * ImgTimelineFolders::GetMinuteLeafFolder(VNode* node)
{
	// IF leaf THEN return
	if ( node->GetFirst() == NULL ) return node;

	// For each child
	VNode *child = node->GetFirst();
	while ( child )
	{
		VNode *result = GetMinuteLeafFolder(child);
		if ( result != NULL ) return result;
		child = child->GetNext();
	}

	// Done
	return NULL;
}


// ---------- GetThisMinuteHandle ----------
/*!
\brief 
\author Gareth Edwards
*/
INT ImgTimelineFolders::GetThisMinuteHandle(INT *handle)
{
	if ( this_minute_folder )
	{
		// NOT this - ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
		*handle = ((INT)this_minute_folder);
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetDateTime ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimelineFolders::GetDateTime(UINT *stack)
{
	// Get
	for (UINT i = 1; i<8; i++) stack[i] = datetime_count[i];

	// Done
	return VS_OK;
}


// ---------- SetDateTime ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimelineFolders::SetDateTime(UINT *stack)
{
	// Get
	for (UINT i = 1; i<8; i++) datetime_count[i] = stack[i];

	// Done
	return VS_OK;
}


// ---------- GotoContextualFirst ----------
/*!
\brief Contextualy goto first
\author Gareth Edwards
*/
INT ImgTimelineFolders::GotoContextualFirst(UINT datetime_index)
{
	// IF no first_minute_folder THEN return error
	if ( this_minute_folder == NULL ) return VS_ERROR;

	UINT datetime_index_tmp = datetime_index - 1;

	// IF AYMDHM THEN
	if (datetime_index_tmp <= 5)
	{
		VNode *node = datetime_stack[datetime_index_tmp];
		VNode *first = node->GetParent()->GetFirst();
		if ( first != NULL )
		{
			this_minute_folder = first;
			next_minute_folder = this_minute_folder->GetLinkS();
			img_timeline_folders.goto_millisecond = -1;
		}
	}
	// ELSE must be seconds
	else
	{
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
		img_timeline_folders.goto_millisecond = current_minute->playback_millisecond / 1000;
	}

	// Done
	return next_minute_folder == NULL ? VS_ERROR : VS_OK;
}


// ---------- GotoContextualLast ----------
/*!
\brief Contextualy goto last frame
\author Gareth Edwards
*/
INT ImgTimelineFolders::GotoContextualLast(UINT datetime_index)
{
	// IF no first_minute_folder THEN return error
	if (this_minute_folder == NULL) return VS_ERROR;

	UINT datetime_index_tmp = datetime_index - 1;

	// IF AYMDHM THEN
	if (datetime_index_tmp <= 5)
	{
		VNode *node = datetime_stack[datetime_index_tmp];
		VNode *last = node->GetParent()->GetLast();
		if ( last != NULL )
		{
			this_minute_folder = last;
			next_minute_folder = this_minute_folder->GetLinkP();
			img_timeline_folders.goto_millisecond = -1;
		}
	}
	// ELSE must be seconds
	else 
	{
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
		img_timeline_folders.goto_millisecond = current_minute->last_millisecond / 1000 * 1000;
	}

	// Done
	return next_minute_folder == NULL ? VS_ERROR : VS_OK;
}


// ---------- GotoFirstMinuteFirstFrame ----------
/*!
\brief Goto first frame of the first minute
\author Gareth Edwards
*/
INT ImgTimelineFolders::GotoFirstMinuteFirstFrame()
{
	// IF no first_minute_folder THEN return error
	if ( first_minute_folder == NULL ) return VS_ERROR;

	// Get Root
	VNode *n = first_minute_folder;
	VNode *root = NULL;
	while ( n != NULL )
	{
		root = n;
		n = n->GetParent();
	}

	// IF root THEN
	if ( root )
	{
		n = root;
		VNode * previous = NULL;
		while ( n )
		{
			previous = n;
			n = n->GetFirst();
		}

		// IF last THEN
		if ( previous )
		{
			this_minute_folder = previous;
			next_minute_folder = previous->GetLinkS();
			return VS_OK;
		}
	}

	// Done
	return VS_ERROR;
}


// ---------- GotoLastMinuteFirstFrame - NOT USED ----------
/*!
\brief Goto first frame of the last minute
\author Gareth Edwards
*/
INT ImgTimelineFolders::GotoLastMinuteFirstFrame()
{
	// IF no first_minute_folder THEN return error
	if (first_minute_folder == NULL) return VS_ERROR;

	// Get Root
	VNode *n = first_minute_folder;
	VNode *root = NULL;
	while ( n != NULL )
	{
		root = n;
		n = n->GetParent();
	}

	// IF root THEN
	if ( root )
	{
		n = root;
		VNode * last = NULL;
		while ( n )
		{
			last = n;
			n = n->GetLast();
		}

		// IF last THEN
		if ( last )
		{
			// Goto Last Minute
			this_minute_folder = last;
			next_minute_folder = NULL;

			// Goto First Frame
			if ( this_minute_folder != NULL )
			{
				ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
				if ( current_minute != NULL )
				{
					img_timeline_folders.goto_millisecond = current_minute->first_millisecond / 1000 * 1000;
				}
			}

			// Done
			return VS_OK;
		}
	}

	// Done
	return VS_ERROR;
}


// ---------- GotoLastMinuteLastFrame ----------
/*!
\brief Goto last frame of the last minute
\author Gareth Edwards
*/
INT ImgTimelineFolders::GotoLastMinuteLastFrame()
{
	// IF no first_minute_folder THEN return error
	if ( first_minute_folder == NULL ) return VS_ERROR;

	// Get Root
	VNode *n = first_minute_folder;
	VNode *root = NULL;
	while ( n != NULL )
	{
		root = n;
		n = n->GetParent();
	}

	// IF root THEN
	if ( root )
	{
		// Goto last minute
		n = root;
		VNode * last = NULL;
		while ( n )
		{
			last = n;
			n = n->GetLast();
		}

		// IF last THEN
		if ( last )
		{
			// Goto Last Minute
			this_minute_folder = last;
			next_minute_folder = NULL;

			// Goto Last Frame
			if ( this_minute_folder != NULL )
			{
				ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
				if ( current_minute != NULL )
				{
					INT goto_millisecond = current_minute->last_millisecond / 1000 * 1000;
						goto_millisecond = goto_millisecond < 0 ? 0 : goto_millisecond;
					img_timeline_folders.goto_millisecond = goto_millisecond;
				}
			}

			// Done
			return VS_OK;
		}
	}

	// Done
	return VS_ERROR;
}


// ---------- SetDateTimeStack ----------
/*!
\brief 
\author Gareth Edwards
\note innvoked from ImgTimeline::PI_Props::T_CreateFolders
and ImgTimeline::PI_Props::T_CreateMinute
*/
INT ImgTimelineFolders::SetDateTimeStack(VNode* minute_folder)
{
	if ( minute_folder != NULL )
	{
		UINT index = 5;
		VNode *node = minute_folder;
		while ( node )
		{
			datetime_stack[ index--] = node;
			node = node->GetParent();
		}
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- AdvanceMinuteFolders ----------
/*!
\brief Swap NOW and NEXT minute folders
\author Gareth Edwards
*/
INT ImgTimelineFolders::AdvanceMinuteFolders()
{
	// Copy NOW minute from NEXT minute
	this_minute_folder = next_minute_folder;

	// IF NEXT minute THEN
	if ( next_minute_folder != NULL )
	{
		next_minute_folder = next_minute_folder->GetLinkS();

		//
		// 15 10 04 - Bug Fix: Stopped playback to last minute!!
		//
		//if ( next_minute_folder != NULL )

		return VS_OK;

	}

	// Done
	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                           ImgTimeline::PI_Props                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// ---------- PI_Props Constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
ImgTimeline::PI_Props::PI_Props()
{
	;
}


// ---------- PI_Props Destructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
ImgTimeline::PI_Props::~PI_Props()
{
	;
}


// ---------- T_CreateMinute ----------
/*!
\brief Create timeline minute
\author Gareth Edwards
\param LPVOID - action
\note action == 0, GOTO - initialise THIS and NEXT AND Goto First Frame
\note action == 1, GOTO - initialise THIS and NEXT AND Goto Last Frame
\note action == 2, INCREMENT - initialise NEXT
\note action == 3, initialise NEXT
*/
DWORD WINAPI vs_system::ImgTimeline::PI_Props::T_CreateMinute(LPVOID lpParam)
{
	// Cast LPVOID to UINT action key
	UINT action = (UINT)lpParam;
	INT result = VS_ERROR;

	/*
		NOTE:

		"this_minute_folder" AND "next_minute_folder" MUST be
		initialised before invoking this method
	*/

	// ----Select action ----
	switch ( action )
	{

		// 1 ---- Initialise both THIS and NEXT minutes ----
		//
		// A. Create  "this_minute_folder"
		// B. Goto in "this_minute_folder" 
		// C. Create  "next_minute_folder"
		// D. Tidy up...
		//
		// Note: goto_millisecond initialised externally - possibly
		// by search or explict instruction (e.g. jog, jump)
		//
		case 0:
		case 1:
			{

				// 1A ---- CREATE THIS minute ----

				img_timeline_minute[0].Initialise();

				if ( img_timeline_folders.this_minute_folder )
					INT result_now = T_CreateMinute(FALSE, 0, img_timeline_folders.this_minute_folder);


				// 1B ---- GOTO in "this_minute_folder" ----

				// IF hunt from "base" goto milliseconds required THEN
				if ( img_timeline_folders.goto_millisecond != -1 )
				{

					// Accelerate "this" minute
					ImgTimelineMinute *this_minute =
						(ImgTimelineMinute *)img_timeline_folders.this_minute_folder->GetAssociated();

					// Hunt from "base" hunt
					BOOL frame_found = FALSE;
					INT gm = img_timeline_folders.goto_millisecond;
					for ( UINT millisecond=gm; millisecond<60000; millisecond++ )
					{
						// If frame found THEN break
						if ( this_minute->ms_lut[millisecond].frame > 0 )
						{
							frame_found = TRUE;
							img_timeline_folders.goto_millisecond = millisecond;
							break;
						}
					}

					// IF goto frame found THEN overwrite initial playback milliseconds
					if ( frame_found )
					{
						this_minute->playback_millisecond = img_timeline_folders.goto_millisecond;
						img_timeline_folders.goto_millisecond = -1;
					}
					// ELSE frame NOT found
					else
					{
						// IF goto less than first frame THEN set playback == first frame
						if ( gm  < this_minute->first_millisecond )
						{
							img_timeline_minute[0].playback_millisecond = this_minute->first_millisecond;
						}
						// ELSE IF goto greater than last frame THEN set playback == last frame
						else if ( gm  > this_minute->last_millisecond )
						{
							this_minute->playback_millisecond = this_minute->last_millisecond;
						}
						// ELSE oooooooooooppssss....
						else
						{
							std::cout << "Error: GOTO did NOT find \"actual\" milliseconds!" << std::endl;
							img_timeline_folders.goto_millisecond = -1;
						}
					}

					// Re-initialise goto
					img_timeline_folders.goto_millisecond = -1;
					img_timeline_folders.hunt_millisecond = FALSE;
				}

				// IF "Goto Last Frame"
				if ( action == 1 )
				{
					// Accelerate "this" minute
					ImgTimelineMinute *this_minute =
						(ImgTimelineMinute *)img_timeline_folders.this_minute_folder->GetAssociated();

					// Update playback_millisecond 
					this_minute->playback_millisecond = this_minute->last_millisecond;
				}

				// 1C ---- CREATE "next_minute_folder"

				img_timeline_minute[1].Initialise();
				if ( img_timeline_folders.next_minute_folder )
					INT result_next = T_CreateMinute(FALSE, 1, img_timeline_folders.next_minute_folder);


				// 1D ---- TIDY UP ----

				// Initialise date time stack
				img_timeline_folders.SetDateTimeStack(img_timeline_folders.this_minute_folder);

				// Threads are completed
				img_timeline_minute_threads.this_minute_thread_completed = true;
				img_timeline_minute_threads.next_minute_thread_completed = true;

				// Done
				result = VS_OK;

			}
			break; // ---- case 0
	

		// 2 ---- Swap minutes ----
		//
		// A. Set NEXT
		// B. CREATE NEXT Minute
		// C. TIDY UP
		//
		case 2:
			{
				// Shortcut
				ImgTimelineFolders *tf = &img_timeline_folders;

				// IF NOT NEXT THEN
				if ( tf->this_minute_folder != NULL )
				{

					// 2A ---- Set THIS equal NEXT ----
					
					// Get "this_minute_folder" minute buffer
					ImgTimelineMinute *tm = (ImgTimelineMinute *)tf->this_minute_folder->GetAssociated();
					
					// Select unused minute buffer
					//
					// If this_minute_folder is using minute 0 THEN
					//		use minute index 1 else index 0
					//
					UINT use_index = tm == &img_timeline_minute[0] ? 1 : 0;

					// 2B ---- Create NEXT minute ----
					img_timeline_minute[use_index].Initialise();
					result = T_CreateMinute(FALSE, use_index, tf->next_minute_folder);

					// 2C ---- TIDY UP ----

					// Millisecond passed forward
					img_timeline_minute[use_index].playback_millisecond = tf->pass_fwd_ms;

					// Initialise date time stack
					img_timeline_folders.SetDateTimeStack(img_timeline_folders.this_minute_folder);

					// Done
					result = VS_OK;
				}
				else
				{
					// Complete result
					result = VS_ERROR;
				}


				// 3C cont. ---- TIDY UP ----

				// Thread has completed
				img_timeline_minute_threads.next_minute_thread_completed = true;
			
			}
			break;


		// 3 ---- Refresh ----
		case 3:
			{
				  // Shortcut
				  ImgTimelineFolders *tf = &img_timeline_folders;

				  // IF there is a "last" minute "previous" minute
				  VNode *prev_min = tf->last_minute_folder->GetLinkP();
				  if ( prev_min != NULL )
				  {
					  // Report P(rev) and S(next) cross links
					  //printf("AAA prev_min = %s\n", prev_min->GetName());
					  //printf("    next_min = %s\n", prev_min->GetLinkS()->GetName());

					  // IF "prev" minute is "this" minute THEN create NEXT minute
					  if ( prev_min == img_timeline_folders.this_minute_folder )
					  {
						  // Get "this_minute"
						  ImgTimelineMinute *tm = (ImgTimelineMinute *)prev_min->GetAssociated();

						  // Set "next" minute buffer index
						  UINT this_index = tm == &img_timeline_minute[0] ? 0 : 1;
						  UINT next_index = this_index == 0 ? 1 : 0;

						  // Create NEXT minute
						  img_timeline_minute[next_index].Initialise();
						  result = T_CreateMinute(
								  FALSE,
								  next_index,
								  tf->last_minute_folder
							  );

						  // Millisecond passed forward
						  img_timeline_minute[next_index].playback_millisecond = tf->pass_fwd_ms;

						  // "next" minute is now the "last" minute
						  tf->next_minute_folder = tf->last_minute_folder;
					  }

					  // Done
					  result = VS_OK;
				  }
				  else
				  {
					  // Complete result
					  result = VS_ERROR;
				  }

			}
			break; // ---- case 3

	}

	// done
	return result;
}


// ---------- T_CreateMinute ----------
/*!
\brief Create or refresh a timeline minute
\author Gareth Edwards
*/
INT ImgTimeline::PI_Props::T_CreateMinute(BOOL refresh, UINT index, VNode *minute_folder)
{
	// NO MINUTE!
	if (minute_folder == NULL) return VS_ERROR;

	// Local
	INT result;

	// Thread NOT completed
	ImgTimelineMinute *tm = &img_timeline_minute[index];

	// Create fullpath
	std::string fullpath = minute_folder->GetName();
	fullpath += "\\*";
	VNode *node = minute_folder->GetParent();
	while ( node != NULL )
	{
		std::string temp = node->GetName();
		fullpath = temp + "\\" + fullpath;    // 160324:REVIEW CRASHES
		node = node->GetParent();
	}

	// Store minute path - and chop off '*' !
	strcpy (tm->path, fullpath.c_str());
		INT len = strlen(tm->path);
		tm->path[len-1] = 0;

	// IF NOT refresh mode THEN set typecast associated minute
	if ( !refresh )
	{
		result = minute_folder->SetAssociated((INT)tm);
	}

	// Initialise milliseconds
	UINT size_of_ms_lut = sizeof(Millisecond)* 60000;
	memset(tm->ms_lut, 0, size_of_ms_lut);

	// Local
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
  
	// Count image files in minute
	tm->image_count = 0;

	// IF valid FindFirstFile THEN
	BOOL set_file_less_ss_msc = true; 
	hFind = FindFirstFile(fullpath.c_str(), &fdata);
	if ( hFind != INVALID_HANDLE_VALUE )
	{
		// DO for all files in directory
		do
		{
			// IF NOT directory files THEN
			if ( strcmp(fdata.cFileName, ".") != 0 &&
				strcmp(fdata.cFileName, "..") != 0)
			{
				// IF a file THEN
				if ( ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 )
				{

					// IF first CHAR NOT a tilda (denoting a temporary file)
					if ( *(fdata.cFileName + 0) != '~' )
					{

						// File naming format is:
						// Position - "01234567_901234_678.012"
						// Layout   - "YYYYMMDD_HHMMSS_MSC.jpg"
						// Example  - "20141231_152312_781.jpg"

						// IF a "jpg" THEN
						//BOOL jpg =
						//	*(fdata.cFileName+20) == 'j' &&
						//	*(fdata.cFileName+21) == 'p' &&
						//	*(fdata.cFileName+22) == 'g';
						//if ( jpg )
						//{

						// Calculate millisecond index
						UINT z = '0';
						UINT millisecond =
							(*(fdata.cFileName+13) - z) * 10000 +   // 10's of seconds 
							(*(fdata.cFileName+14) - z) * 1000 +    // seconds
							(*(fdata.cFileName+16) - z) * 100 +     // 100's of milliseonds
							(*(fdata.cFileName+17) - z) * 10 +      // 10's of milliseconds
							(*(fdata.cFileName+18) - z);            // milliseconcds

						// IF legal millisecond THEN set "SS_MSC" path
						if ( millisecond >= 0 && millisecond <= 59999 )
						{
							// Increment image count
							tm->image_count++;

							// It is a frame - initialise millisecond
							tm->ms_lut[millisecond].frame = TRUE;

							// Store 6 char image access identifier 
							memcpy(
									tm->ms_lut[millisecond].ss_msc,     // "SS_MSC", e.g. "12_034"
									fdata.cFileName+13,                 // In "YYYYMMDD_HHMMSS_MSC.jpg" copy "HHMMSS"
									6
								);


							// IF file "YYYYMMDD_HHMM" part - less "SS_MSC" - to be initialised THEN
							if ( set_file_less_ss_msc ) 
							{
								set_file_less_ss_msc = FALSE;           // DO ONCE set filename part flag FALSE
								
								*(fdata.cFileName+13) = 0;              // Terminate wtih null character at first "S" of second

								memcpy(
										tm->file_less_ss_msc,           // Copy into filename part last 6 characters
										fdata.cFileName,                // + terminating null character at 13
										14
									);
							}

						}
						// ELSE illegal millisecond
						else
						{
							SetLastError((DWORD)VS_ERROR);
							break;
						}

						//} // IF a "jpg" THEN

					} // IF first CHAR NOT a 'x'

				} // IF a file THEN

			} // IF NOT directory files THEN

		}
		while ( FindNextFile(hFind, &fdata ) != 0); // DO for all files in directory

	} // IF valid FindFirstFile

	// Check done
	if ( DWORD error = GetLastError() != ERROR_NO_MORE_FILES )
	{
		FindClose(hFind);
		return error;
	}
 
	// Close
	FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;

	// IF NOT refresh mode THEN link forward and back
	if ( !refresh )
	{
		// Local: Set minute value
		INT minute_value = atoi(minute_folder->GetName());

		// Start here - in this minute
		INT previous_value = minute_value - 2;
		VNode *previous = minute_folder->GetPrevious();
		if (previous &&  atoi(previous->GetName()) == minute_value - 1)
			tm->start_here = false;
		else
			tm->start_here = true;

		// Stop here - in this minute
		INT next_value = minute_value + 2;
		VNode *next = minute_folder->GetNext();
		if (next &&  atoi(next->GetName()) == minute_value + 1)
			tm->stop_here = false;
		else
			tm->stop_here = true;

	}

	// Format minute for millisecond random access
	result = T_FormatMsLut(refresh, tm);

	// Initialise second and millisecond lists
	result = T_InitSMLuts(refresh, tm);

	// done
	return result;
}


// ---------- T_FormatMsLut ----------
/*!
\brief Format Minute millisecond look up table 
\author Gareth Edwards
\note
*/
INT ImgTimeline::PI_Props::T_FormatMsLut(BOOL refresh, ImgTimelineMinute *tm)
{
	// Local
	INT current_millisecond = -1;

	// Initiliase FORWARD from first millisecond
	tm->first_millisecond = -1;
	for (UINT i=0; i<60000; i++)
	{
		// IF frame SET then update current_millisecond
		if ( tm->ms_lut[i].ss_msc[0] != 0 )
		{
			// Set Frame, Last and First
			if ( tm->first_millisecond < 0 ) tm->first_millisecond = i;
			current_millisecond = i;
		}
		// ELSE
		else
		{
			// NOTE: Monster BUG fixed here!
			//
			// Was "if ( current_millisecond > 0 )"
			//
			// IF current_millisecond IS initialised THEN 
			if ( current_millisecond >= 0 )
				memcpy(
					tm->ms_lut[i].ss_msc,
					tm->ms_lut[current_millisecond].ss_msc,
					6
				);
		}
	}

	// Initialise last
	tm->last_millisecond = current_millisecond;

	// IF NOT refresh mode THEN initialise playback
	//
	// BUG ????
	//
	if ( !refresh )
	{
		tm->playback_millisecond = tm->first_millisecond; // WAS tm->start_here ? tm->first_millisecond : 0;
	}

	// Report
	BOOL report = false;
	if (report)
	{
		for (UINT i=0; i<60000; i++)
		{
			if ( tm->ms_lut[i].frame )
				std::cout << tm->ms_lut[i].ss_msc << " - " << i << std::endl;
		}
		std::cout << tm->first_millisecond << std::endl;
		std::cout << tm->last_millisecond << std::endl;
		std::cout << tm->playback_millisecond << std::endl;
	}

	// Done
	return VS_OK;
}


// ---------- T_InitSMLuts ----------
/*!
\brief Initialise Second and Millisecond LUT's 
\author Gareth Edwards
\note
*/
INT ImgTimeline::PI_Props::T_InitSMLuts(BOOL refresh, ImgTimelineMinute *tm)
{
	// FOR each second
	UINT ms_lut_index = 0;
	for (UINT s=0; s<60; s++) // 60 seconds
	{
		// FOR each 10 millisecond interval
		UINT second_frame_count = 0;
		for ( UINT m=0; m<100; m++ ) // 100 x 10 milliseconds intervals 
		{
			// FOR each miilisecond
			UINT millisecond_frame = 0;
			UINT millisecond_frame_count = 0;
			for ( UINT h=0; h<10; h++)
			{
				ms_lut_index = s*1000 + m*10 + h;
				if ( tm->ms_lut[ms_lut_index].frame )
				{
					millisecond_frame = m*10 + h; //ms_lut_index;
					millisecond_frame_count++;
				}
			}

			// Store 10 millisecond interval miilisecond
			tm->ten_ms_frame_lut[s*100 + m] = millisecond_frame;
			tm->ten_ms_sample_lut[s*100 + m] = millisecond_frame_count;

			// Update second_frame_count.....
			second_frame_count += millisecond_frame_count;
		}

		// Store 10 millisecond interval frame count
		tm->seconds_lut[s] = second_frame_count;
	}

	// Done
	return VS_OK;
}


// ---------- T_CreateFolders ----------
/*!
\brief Create timeline folders
\author Gareth Edwards
*/

std::string global_create_archive_path;

DWORD WINAPI vs_system::ImgTimeline::PI_Props::T_CreateFolders(LPVOID lpParam)
{
	// Local
	INT result = 0;
	BOOL refresh = lpParam == (LPVOID)1 ? TRUE : FALSE;

	// Thread NOT completed
	ImgTimelineFolders *tf = &img_timeline_folders;
	tf->create_timeline_thread_completed = FALSE;

	// Store
	global_create_archive_path.assign(tf->path);

	// Local
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;
	std:: string fullpath;
	std::stack<std::string> folders;
	folders.push(tf->path);

	// Reset timeline folders counter
	tf->total_folders = 0;

	// Recurse until no more folders
	while ( !folders.empty() )
	{
		// Init current path from folders stack top
		tf->path = folders.top();

		// Important bit.... build model
		//std::cout << "T : " << tf->path  << std::endl;
		result = T_AddFolder(refresh);

		// Init full directory path..
		fullpath = tf->path + "\\" + tf->mask;

		// Then pop folders stack
		folders.pop();

		// List this folder
		hFind = FindFirstFile(fullpath.c_str(), &fdata);
		if ( hFind != INVALID_HANDLE_VALUE )
		{
			do
			{
				if ( strcmp(fdata.cFileName, ".") != 0 &&
					strcmp(fdata.cFileName, "..") != 0)
				{
					// IF folder THEN push current path onto foldersa stack
					if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
					{
						//if ( refresh ) printf("Folder: %s\n", fdata.cFileName);
						tf->total_folders++;
						folders.push(tf->path + "\\" + fdata.cFileName);
					}
					// ELSE files, which are listed AFTER folders - so exit loop
					else break;
				}
			}
			while ( FindNextFile(hFind, &fdata ) != 0);
		}

	 	// Check this folder listing is complete
		if ( GetLastError() != ERROR_NO_MORE_FILES )
		{
			FindClose(hFind);
			return VS_ERROR;
		}

		// Close this folder
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	// printf("# of Archive folders is %d and took %d ms\n", folder_counter, (INT)total_time);

	// Finally, "cross" link folder nodes
	VNode *link_stack[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
	UINT link_index = 0;
	result = T_CrossLinkFolders(tf->root_folder, link_index, link_stack);

	// Report "cross" linking
	BOOL report = FALSE;
	if ( report )
	{
		VNode *years   = tf->root_folder->GetFirst();
		VNode *months  = years->GetFirst();
		VNode *days    = months->GetFirst();
		VNode *hours   = days->GetFirst();
		VNode *minutes = hours->GetFirst();
		ListCrossLinkFolders(years);
		ListCrossLinkFolders(months);
		ListCrossLinkFolders(days);
		ListCrossLinkFolders(hours);
		ListCrossLinkFolders(minutes);
	}

	// Set last folder
	VNode *last = tf->root_folder;
	while (last != NULL)
	{
		img_timeline_folders.last_minute_folder = last;
		last = last->GetLast();
	}

	// IF NOT refresh THEN initialise date time stack
	if ( !refresh )
	{
		result = img_timeline_folders.SetDateTimeStack(img_timeline_folders.last_minute_folder);
	}

	// Report P(rev) and S(next) cross links
	if ( report )
	{
		// Shortcut
		ImgTimelineFolders *tf = &img_timeline_folders;

		// IF there is a "last" minute "previous" minute
		VNode *prev_min = tf->last_minute_folder->GetLinkP();
		if ( prev_min != NULL )
		{
			printf("KKK prev_min = %s\n", prev_min->GetName());
			printf("    next_min = %s\n", prev_min->GetLinkS()->GetName());
		}
	}


	// Thread has completed
	//
	// Set TRUE before this method is invoked, see
	// INT ImgTimeline::Create(CHAR *) method
	//
	tf->create_timeline_thread_completed = TRUE;

	// Done
	return VS_OK;
}


// ---------- T_ScanFolders ----------
/*!
\brief Scan timeline folders
\author Gareth Edwards
*/
INT vs_system::ImgTimeline::PI_Props::T_ScanFolders_01()
{
	// Local
	INT result = 0;

	INT folder_counter = 0;

	CHAR folder_stack[16][VS_MAXCHARPATHLEN];
	INT  folder_stack_top = -1;

	CHAR folder_store_path[VS_MAXCHARPATHLEN];
	CHAR folder_new_path[VS_MAXCHARPATHLEN];
	CHAR folder_list_path[VS_MAXCHARPATHLEN];

	// Local
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;

	// Accelerate timeline folders
	ImgTimelineFolders *tf = &img_timeline_folders;

	// Initialise folder stack
	strcpy(folder_stack[++folder_stack_top], global_create_archive_path.c_str() );

	// Recurse until no more folders
	while ( folder_stack_top >= 0 )
	{
		// Store folder path
		strcpy(folder_store_path, folder_stack[folder_stack_top]);

		// Create folder listing path
		sprintf(folder_list_path, "%s\\*", folder_store_path);

		// Pop folders stack
		folder_stack_top--;

		// List folder
		hFind = FindFirstFile(folder_list_path, &fdata);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(fdata.cFileName, ".") != 0 &&
					strcmp(fdata.cFileName, "..") != 0)
				{
					// IF folder THEN push onto folders stack
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						folder_counter++;
						sprintf(folder_new_path, "%s\\%s", folder_store_path, fdata.cFileName);
						strcpy(folder_stack[++folder_stack_top], folder_new_path);
					}
					// ELSE files, which are listed AFTER folders - so exit loop
					else break;
				}
			} while (FindNextFile(hFind, &fdata) != 0);
		}

		// Check this folder listing is complete
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);
			printf("Folder Scan: Failed.... %d folders, ERROR_NO_MORE_FILES\n", folder_counter);
			return VS_ERROR;
		}

		// Close FindFirstFile
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	// done
	return folder_counter;
}


// ---------- T_ScanFolders ----------
/*!
\brief Scan timeline folders
\author Gareth Edwards
*/
INT vs_system::ImgTimeline::PI_Props::T_ScanFolders()
{

#	ifdef TIME_SCAN

	std::chrono::high_resolution_clock::time_point hrc_time_start;
	std::chrono::high_resolution_clock::time_point hrc_time_end;
	LONG hrc_duration = 0;

	hrc_time_start = std::chrono::high_resolution_clock::now();

	double start_time = (double)timeGetTime();

	double end_time = 0;
	double total_time = 0;

#endif

	// Local
	INT result = 0;

	// Timeline folders
	ImgTimelineFolders *tf = &img_timeline_folders;
	std::string tf_path;
	tf_path.assign(global_create_archive_path);

	// Local
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;

	std::string fullpath;
	std::stack<std::string> folders;
	folders.push(tf_path);

	UINT folder_counter = 0;

	// Recurse until no more folders
	while (!folders.empty())
	{
		// Init current path from folders stack top
		tf_path = folders.top();

		//std::cout << "T : " << tf_path << std::endl;

		// Init full directory path..
		fullpath = tf_path + "\\" + tf->mask;

		// Then pop folders stack
		folders.pop();

		// List this folder
		hFind = FindFirstFile(fullpath.c_str(), &fdata);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (strcmp(fdata.cFileName, ".") != 0 &&
					strcmp(fdata.cFileName, "..") != 0)
				{
					// IF folder THEN push current path onto foldersa stack
					if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						// printf("Folder: %s\n", fdata.cFileName);
						folder_counter++;
						folders.push(tf_path + "\\" + fdata.cFileName);
					}
					// ELSE files, which are listed AFTER folders - so exit loop
					else break;
				}
			} while (FindNextFile(hFind, &fdata) != 0);
		}

		// Check this folder listing is complete
		if (GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);
			printf("Folder Scan: Failed.... %d folders, ERROR_NO_MORE_FILES\n", folder_counter);
			return VS_ERROR;
		}

		// Close this folder
		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

#	ifdef TIME_SCAN

	hrc_time_end = std::chrono::high_resolution_clock::now();
	hrc_duration = std::chrono::high_resolution_clock::duration(hrc_time_end - hrc_time_start).count();

	end_time = (double)timeGetTime();
	total_time = end_time - start_time;

	printf(
		"Folder Scan: # of Archive folders is %d took %d and %ld\n",
		folder_counter,
		(INT)total_time,
		hrc_duration
		);

#	endif

	// done
	return (INT)folder_counter;
}



// ---------- T_AddFolder  ----------
/*!
\brief Add folder to timeline
\author Gareth Edwards
*/
INT ImgTimeline::PI_Props::T_AddFolder(BOOL refresh)
{
	// Split into path component names
	//
	// Note: original path is comprised of folder/folder/etc.,
	// and by building with backslash this allows the logic
	// that the when split - and there is only one word,
	// then this is the root path.
	//
	CharProc cp;
	CHAR temp[VS_MAXCHARLEN];
	strcpy(temp, img_timeline_folders.path.c_str());
	CHAR *stack[32];
	UINT num_words = cp.Split(stack, 32, '\\', temp);
	
	// IF only one word THEN
	if ( num_words == 1 )
	{
		// IF NOT refresh mode THEN create root folder
		if ( !refresh )
		{
			// Add, allocate and create root node
			UINT len = strlen(stack[0]);
			img_timeline_folders.root_folder = new VNode(stack[num_words - 1]);

		}
	}
	// ELSE add allocate and create folder nodes
	else
	{
		// FOR each hierarchical level on "word" stack except first...
		VNode *parent = img_timeline_folders.root_folder;
		for ( WORD w=1; w<num_words; w++ )
		{
			// Search for matching child name
			VNode *child = parent->GetFirst();
			while ( child )
			{
				CHAR *child_name = child->GetName();
				if ( !strcmp(child_name, stack[w]) )
					break;
				child = child->GetNext();
			}

			// IF last hierarchical level on "word" stack AND
			// search for child by named fails THEN
			// add new node, etc.... AND return
			//
			if ( w==num_words-1 && child==NULL )
			{
				VNode* new_node = NULL;
				if ( refresh )
				{
					new_node = new VNode(stack[w], parent, 1);
					// printf("Append minute %s\n", new_node->GetName());
				}
				else
				{
					new_node = new VNode(stack[w], parent);
				}
				return VS_OK;
			}

			// Next hierarchical level on "word" stack
			parent = child;

		}
	}

	// return ok
    return VS_OK;
}


// ---------- T_CrossLinkFolders ----------
/*!
\brief Recursively cross link folders
\author Gareth Edwards
*/
INT ImgTimeline::PI_Props::T_CrossLinkFolders(VNode *node, UINT link_index, VNode *link[])
{
	// IF leaf THEN set "first_minute_folder"
	if ( node->GetFirst() == NULL )
		if ( img_timeline_folders.first_minute_folder == NULL )
			img_timeline_folders.first_minute_folder = node;

	// IF previous THEN set previous "cross" link to "this" node
	VNode *previous = link[link_index];
	if ( previous != NULL)
	{
		// Link from previous to "this"
		previous->SetLinkS(node);

		// Link from "this back to previous
		node->SetLinkP(previous);

		// Terminate
		node->SetLinkS(NULL);
	}

	// Replace "previous" with "this"
	link[link_index] = node;

	// Recurse
	VNode *child = node->GetFirst();
	while ( child )
	{
		INT result = T_CrossLinkFolders(child, link_index+1, link);
		child = child->GetNext();
	}

	// Done
    return VS_OK;
}


// ---------- ListAllFolders ----------
/*!
\brief List all folders
\author Gareth Edwards
*/
INT ImgTimeline::PI_Props::ListAllFolders(VNode *parent)
{
	VNode *grandparent = parent->GetParent();
	if ( grandparent )
		std::cout << grandparent->GetName() << " <- " << parent->GetName() << std::endl;
	else
		std::cout << parent->GetName() << std::endl;
	VNode *child = parent->GetFirst();
	while ( child )
	{
		INT result = ListAllFolders(child);
		child = child->GetNext();
	}
	return VS_OK;
}


// ---------- ListCrossLinkFolders ----------
/*!
\brief List all folders
\author Gareth Edwards
*/
INT ImgTimeline::PI_Props::ListCrossLinkFolders(VNode *node)
{
	if ( node->GetParent() != NULL ) std::cout << node->GetParent()->GetName() << " : ";
	while ( node )
	{
		VNode *link_p = node->GetLinkP();
		VNode *link_s = node->GetLinkS();

		char temp[256];
		sprintf(
				temp,
				"%s<-%s->%s",
				(link_p == NULL ? "N" : link_p->GetName() ),
				node->GetName(),
				(link_s == NULL ? "N" : link_s->GetName() )
			);

		std::cout << temp << (link_s == NULL ? " " : ", ");
		node = link_s;
	}
	std::cout << std::endl;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                ImgTimeline                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
ImgTimeline::ImgTimeline()
{
	// Allocate and create pi_props
	pi_props = new PI_Props();

}


// ---------- Destructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
ImgTimeline::~ImgTimeline()
{
	if ( pi_props != NULL ) { delete pi_props; pi_props = NULL; }
}


// ---------- IsTimelineCreated ----------
/*!
\brief Return timeline created status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsTimelineCreated()
{
	// Return create thread status
	return img_timeline_folders.IsTimelineCreated();
}


// ---------- IsTimelineAvailable ----------
/*!
\brief Return timeline thread status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsTimelineAvailable()
{
	// Return create thread status
	return img_timeline_folders.IsTimelineAvailable();
}


// ---------- IsThisMinuteAvailable ----------
/*!
\brief Return minute thread status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsThisMinuteAvailable()
{
	return img_timeline_minute_threads.IsThisMinuteAvailable();
}


// ---------- IsNextMinuteAvailable ----------
/*!
\brief Return next minute thread status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsNextMinuteAvailable()
{
	return img_timeline_minute_threads.IsNextMinuteAvailable();
}


// ---------- IsThisMinuteReady ----------
/*!
\brief Return this minute ready status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsThisMinuteReady()
{
	return img_timeline_folders.this_minute_folder ? TRUE : FALSE;
}


// ---------- IsNextMinuteReady ----------
/*!
\brief Return next minute ready status
\author Gareth Edwards
*/
BOOL ImgTimeline::IsNextMinuteReady()
{
	return img_timeline_folders.next_minute_folder ? TRUE : FALSE;
}


// ---------- SetThisMinuteStatus ----------
/*!
\brief Set timeline status
\author Gareth Edwards
*/
INT ImgTimeline::SetThisMinuteStatus(BOOL status)
{
	// Return create thread status
	return img_timeline_minute_threads.SetThisMinuteStatus(status);
}


// ---------- SetNextMinuteStatus ----------
/*!
\brief Set timeline status
\author Gareth Edwards
*/
INT ImgTimeline::SetNextMinuteStatus(BOOL status)
{
	// Return create thread status
	return img_timeline_minute_threads.SetNextMinuteStatus(status);
}


// ---------- Create ----------
/*!
\brief Create timeline folders
\author Gareth Edwards
*/
INT ImgTimeline::Create(CHAR *archive_folder_name)
{
	// Initialise folders
	img_timeline_folders.Initialise(archive_folder_name, "*");

	// Create folders
	img_timeline_folders.create_timeline_thread_completed = FALSE;
	img_timeline_folders.create_timeline_thread_handle  =
		CreateThread(NULL,
				0,
				vs_system::ImgTimeline::PI_Props::T_CreateFolders,
				(LPVOID)0,
				0,
				0
			);

	// Done
	return VS_OK;
}


// ---------- Scan ----------
/*!
\brief Scan folders
\author Gareth Edwards
*/
INT ImgTimeline::Scan(CHAR *archive_folder_name)
{
	// Initialise folders
	//img_timeline_folders.Initialise(archive_folder_name, "*");

	// Scan folders
	INT result = vs_system::ImgTimeline::PI_Props::T_ScanFolders();

	// Done
	return result;
}


// ---------- GetDateTimeMilliseconds 1 ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTimeMilliseconds_1(
		UINT index,
		UINT *stack,
		LLONG *millisecond
	)
{
	// Get
	INT result = img_timeline_folders.GetDateTimeMilliseconds_1_rUpUpL(
			index,
			stack,
			millisecond
		);

	// Done
	return result;
}


// ---------- GetDateTimeMilliseconds 3 ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTimeMilliseconds_3(
		UINT index,
		LLONG *millisecond
	)
{
	// Get
	INT result = img_timeline_folders.GetDateTimeMilliseconds_3_rUpL(
			index,
			millisecond
		);

	// Done
	return result;
}


// ---------- GetDateTimeStride ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTimeStride(UINT index, UINT *stride)
{
	// Get
	INT result = img_timeline_folders.GetDateTimeStride(
			index,
			stride
		);

	// Done
	return result;
}


// ---------- GetDateTimeList ----------
/*!
\brief 
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTimeList(UINT index, UINT *items, LLONG list[])
{
	// Get
	INT result = img_timeline_folders.GetDateTimeList(
			index,
			items,
			list
		);

	// Done
	return result;
}


// ---------- GetDateTimeValue ----------
/*!
\brief 
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTimeValue(UINT *value)
{
	// Get minute folder vnode
	VNode *node = img_timeline_folders.this_minute_folder;


	// Get current_minute
	ImgTimelineMinute *current_minute = (ImgTimelineMinute *)node->GetAssociated();


	// ---- Archive/Year/Month/Day/Hour/Minute ----
	INT index = 5;
	while ( node )
	{
		img_timeline_folders.datetime_stack[index] = node;
		value[index--] = atoi(node->GetName());
		node = node->GetParent();
	}

	// ---- Seconds ----
	img_timeline_folders.datetime_second = current_minute->playback_millisecond / 1000;
	value[6] = img_timeline_folders.datetime_second;


	// ---- Milliseconds ----
	img_timeline_folders.datetime_millisecond =
		current_minute->playback_millisecond -
		(img_timeline_folders.datetime_second * 1000);
	value[7] = img_timeline_folders.datetime_millisecond;

	// Done
	return VS_OK;
}


// ---------- GetFilename ----------
/*!
\brief Get current image filename
\author Gareth Edwards
*/
INT ImgTimeline::GetFilename(std::string &filename)
{
	INT result = img_timeline_folders.GetFilename(filename);
	return result;
}


// ---------- GetTotalFolders ----------
/*!
\brief Get current image filename
\author Gareth Edwards
*/
INT ImgTimeline::GetTotalFolders(UINT *tf)
{
	*tf = img_timeline_folders.total_folders;
	return img_timeline_folders.total_folders > 0 ? VS_OK : VS_ERROR;
}


// ---------- GetPrevNextState ----------
/*!
\brief Getprevious and/or next state
\author Gareth Edwards
*/
INT ImgTimeline::GetPrevNextState(UINT datetime_index)
{
	// IF AYMDHM THEN return bitmask of parent VNODE
	// previous/next status (Note: datetime_index always > 0)
	if ( datetime_index <= 5 )
	{
		VNode *previous = img_timeline_folders.datetime_stack[datetime_index-1]->GetLinkP();
		VNode *next     = img_timeline_folders.datetime_stack[datetime_index-1]->GetLinkS();
		return ( previous == NULL ? 0 : 1 ) + ( next == NULL ? 0 : 2 );
	}

	// ELSE IF S(econds) THEN return bitmask of parent VNODE of
	// this_minute_folder previous/next status
	else if ( datetime_index == 6)
	{
		VNode *minute   = img_timeline_folders.this_minute_folder;
		VNode *previous = minute != NULL ? minute->GetLinkP() : NULL;
		VNode *next     = minute != NULL ? minute->GetLinkS() : NULL;
		return (previous != NULL ? 1 : 0) + (next != NULL ? 2 : 0);
	}

	// ELSE milliseconds
	{
		// Get minute folder vnode
		VNode *node = img_timeline_folders.this_minute_folder;

		// Get current_minute
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)node->GetAssociated();

		// Default - no previous or next!
		BOOL previous = FALSE;
		BOOL next = FALSE;

		// If second greater than min OR previous link TRUE THEN previous TRUE
		UINT this_second = current_minute->playback_millisecond / 1000;
		UINT min_second = current_minute->first_millisecond / 1000;
		if ( this_second > min_second )
		{
			previous = TRUE;
		}
		else
		{
			if ( node->GetLinkP() != NULL )
				previous = TRUE;
		}

		// If second less than max OR next link TRUE THEN next TRUE
		UINT max_second = current_minute->last_millisecond / 1000;
		if ( this_second < max_second )
		{
			next = TRUE;
		}
		else
		{
			if ( node->GetLinkS() != NULL )
				next = TRUE;
		}

		// Return previous/next bitmask
		return (previous ? 1 : 0) + (next ? 2 : 0);
	}

	// Done
	return VS_OK;
}


// ---------- GetFirstLastState ----------
/*!
\brief Getprevious and/or next state
\author Gareth Edwards
*/
INT ImgTimeline::GetFirstLastState()
{
	// Accelerate
	VNode * this_minute_folder  = img_timeline_folders.this_minute_folder;
	VNode * first_minute_folder = img_timeline_folders.first_minute_folder;
	VNode * last_minute_folder  = img_timeline_folders.last_minute_folder;

	// Default - can goto to first and last frame
	BOOL can_goto_first_frame = TRUE;
	BOOL can_goto_last_frame  = TRUE;

	// IF first minute THEN test for first frame
	if (this_minute_folder == first_minute_folder)
	{
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
		if (current_minute != NULL)
		{
			if (current_minute->playback_millisecond <= current_minute->first_millisecond)
			{
				can_goto_first_frame = FALSE;
			}
		}
	}

	// IF last minute THEN test for first frame
	if (this_minute_folder == last_minute_folder)
	{
		// Get current_minute
		ImgTimelineMinute *current_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
		if (current_minute != NULL)
		{
			if (current_minute->playback_millisecond >= current_minute->last_millisecond)
			{
				can_goto_last_frame = FALSE;
			}
		}
	}

	// Return previous/next bitmask (note: there might only be one frame!!!)
	return (can_goto_first_frame ? 1 : 0) + (can_goto_last_frame ? 2 : 0);
}


// ---------- GetDateTime ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimeline::GetDateTime(UINT *stack)
{
	// Get
	INT result = img_timeline_folders.GetDateTime(stack);

	// Done
	return result;
}


// ---------- SetDateTime ----------
/*!
\brief
\author Gareth Edwards
*/
INT ImgTimeline::SetDateTime(UINT *stack)
{
	// Get
	INT result = img_timeline_folders.SetDateTime(stack);

	// Done
	return result;
}


// ---------- GetThisMinuteHandle ----------
/*!
\brief Get current image filename
\author Gareth Edwards
*/
INT ImgTimeline::GetThisMinuteHandle(INT *handle)
{
	INT result = img_timeline_folders.GetThisMinuteHandle(handle);
	return result;
}


// ---------- Goto ----------
/*!
\brief Goto first minute
\author Gareth Edwards
*/
INT ImgTimeline::Goto(UINT index, INT direction, UINT count, INT stack[])
{
	// IF stacked YMDHMSM parameters THEN return error
	if ( count < 1 ) return VS_ERROR;


	// ---- CREATE and INITIALISE YMDHM stack ----
	VNode *root = img_timeline_folders.root_folder;
	INT get_stack[8];
	for ( UINT i=0; i<8; i++ ) get_stack[i] =  i < 6 ? stack[i] : -1;


	// ---- INITIALISE maximum_to_match ----
	UINT maximum_to_match = -1;
	for ( UINT i=0; i<7; i++ )
	{
		if ( get_stack[i+1] == -1 )
		{
			maximum_to_match = i;
			break;
		}
	}


	// ---- MATCH ----

	// IF there is someting to match THEN return error 
	if ( maximum_to_match < 1 ) return VS_ERROR;
		
	// Get "matched" folder
	VNode *matched_folder = img_timeline_folders.GetMinuteFolder(root->GetFirst(), maximum_to_match, 1, get_stack);

	// IF match failed THEN return error
	if ( matched_folder == NULL )
	{ 
		// std::cout << "Error: GOTO failed!" << std::endl;
		return VS_ERROR;
	}


	// ---- HANDLE DIRECTION GOTO ----

	// IF AYMDHM THEN
	if ( maximum_to_match <= 5 && stack[6] == -1 )
	{
		// Zap milli/seconds
		stack[6] = -1;
		stack[7] = -1;
		
		// Select direction
		switch ( direction )
		{
			case GOTO_BACKWARD:
			case GOTO_PREVIOUS:
				{
					VNode *previous = matched_folder->GetLinkP();
					if ( previous )
						matched_folder = previous;
					else
						return VS_ERROR;
					stack[6] = 0; //59;  // Note: Tried defaulting to last second
					stack[7] = 0; //999; // and which did not "feel" right
				}
				break;
			case GOTO_FORWARD:
			case GOTO_NEXT:
				{
					VNode *next = matched_folder->GetLinkS();
					if ( next )
						matched_folder = next;
					else
						return VS_ERROR;
				}
				break;
			default:
				break;
		}
	}


	// ---- HANDLE MILLISECONDS (AYMDHMS) GOTO ----

	// So, if ms AND NOT goto in or out clip points THEN
	//
	if ( index >= 7 && direction != GOTO_CLIP )
	{
		// Select direction
		switch ( direction )
		{
			case GOTO_BACKWARD:
			case GOTO_PREVIOUS:
				{
					stack[6] -= 1;
					stack[7] = 0;
				}
				break;
			case GOTO_FORWARD:
			case GOTO_NEXT:
				{
					stack[6] += 1;
					stack[7] = 0;
				}
				break;
			default:
				break;
		}

		// ---- DECREMENT / INCREMENT ----

		// Get this minute
		ImgTimelineMinute *this_minute = (ImgTimelineMinute *)img_timeline_folders.this_minute_folder->GetAssociated();

		// IF this minute decremented into last minute
		if ( stack[6] == - 1 || stack[6] < this_minute->first_millisecond / 1000 )
		{
			VNode *previous = matched_folder->GetLinkP();
			if ( previous )
				matched_folder = previous;
			else
				return VS_ERROR;
			stack[6] = 59;
			stack[7] = 999;
		}

		// ELSE if this minute incremented into next minute
		else if ( stack[6] > this_minute->last_millisecond / 1000 )
		{
			VNode *next = matched_folder->GetLinkS();
			if ( next )
				matched_folder = next;
			else
				return VS_ERROR;
			stack[6] = 0;
			stack[7] = 0;
		}
	}


	/*
		NOW - TWO CONDITIONAL LOGIC BLOCKS
		1. Handle YMDH or,
		2. YMDHM, YMDHMS or YMDHMSM
	*/

	// 1 ---- IF YMDH (Year/Month/Day/Hour) THEN --
	if ( maximum_to_match <= 4 )
	{
		// Climb up folder tree to find first leaf folder
		//std::cout << "GOTO YDMH: " << matched_folder->GetName() << std::endl;
		VNode *ydmh = img_timeline_folders.GetMinuteLeafFolder(matched_folder);

		// IF leaf folder found THEN
		if ( ydmh != NULL )
		{
			//std::cout << "GOTO : " << ydmh->GetName() << std::endl;

			// Goto Minutes
			img_timeline_folders.this_minute_folder = ydmh;
			img_timeline_folders.next_minute_folder = img_timeline_folders.this_minute_folder->GetLinkS();

			// NO goto miliseconds
			img_timeline_folders.goto_millisecond = -1;
			img_timeline_folders.hunt_millisecond = FALSE;

			// NOW....... "wave of wand".... and lo, create this and next minutes
			img_timeline_minute_threads.this_minute_thread_completed = false;
			img_timeline_minute_threads.next_minute_thread_completed = false;
			img_timeline_minute_threads.this_minute_thread_handle  =
				CreateThread(NULL,
					0,
					vs_system::ImgTimeline::PI_Props::T_CreateMinute,
					(LPVOID)0, // 0: This&Next This First Frame
					0,
					0
				);
		}
		else
		{
			//std::cout << "Error: YMDH GOTO failed!" << std::endl;
			return VS_ERROR;
		}

	} // --- IF YMDH THEN

	// 2 ---- ELSE YMDHM, YMDHMS or YMDHMSM (Year/Month/Day/Hour/Minute/Second/Millisecond) THEN --
	else
	{
		//std::cout << "GOTO Minute: " << matched_folder->GetName() << std::endl;

		// Goto Minutes
		img_timeline_folders.this_minute_folder = matched_folder;
		img_timeline_folders.next_minute_folder = img_timeline_folders.this_minute_folder->GetLinkS();

		// Goto Miliseconds
		img_timeline_folders.goto_millisecond = -1;
		img_timeline_folders.hunt_millisecond = FALSE;

		// IF stacked seconds THEN
		if ( stack[6] >= 0 )
		{
			// Calculate "base" goto_milliseconds
			img_timeline_folders.goto_millisecond = stack[6] * 1000;

			// IF stacked milliseconds THEN add
			if ( stack[7] >= 0 )
			{
				img_timeline_folders.goto_millisecond += stack[7];
			}
			// ELSE flag hunt required AFTER new minute created!!!
			else
			{
				img_timeline_folders.hunt_millisecond = TRUE;
			}
		}

		//std::cout << "GOTO \"base\" milliseconds: " << img_timeline_folders.goto_millisecond << std::endl;

		// NOW....... "wave of wand".... and lo, create this and next minutes
		img_timeline_minute_threads.this_minute_thread_completed = false;
		img_timeline_minute_threads.next_minute_thread_completed = false;
		img_timeline_minute_threads.this_minute_thread_handle  =
			CreateThread(NULL,
					0,
					vs_system::ImgTimeline::PI_Props::T_CreateMinute,
					(LPVOID)0, // 0: This&Next This First Frame
					0,
					0
				);

	} // --- ELSE YMDHM, YMDHMS or YMDHMSM 

	// Return error
	return VS_OK;
}


// ---------- GotoContextualFirst ----------
/*!
\brief Goto first
\author Gareth Edwards
*/
INT ImgTimeline::GotoContextualFirst(UINT datetime_index)
{
	// Initialise goto first folder
	INT result = img_timeline_folders.GotoContextualFirst(datetime_index);

	// Create minutes
	img_timeline_minute_threads.this_minute_thread_completed = false;
	img_timeline_minute_threads.next_minute_thread_completed = false;
	img_timeline_minute_threads.this_minute_thread_handle  =
		CreateThread(NULL,
				0,
				vs_system::ImgTimeline::PI_Props::T_CreateMinute,
				(LPVOID)0, // 0: This&Next This First Frame
				0,
				0
			);

	// Return error code
	return VS_OK;
}


// ---------- GotoContextualLast ----------
/*!
\brief Goto first minute
\author Gareth Edwards
*/
INT ImgTimeline::GotoContextualLast(UINT datetime_index)
{
	// Initialise goto first folder
	INT result = img_timeline_folders.GotoContextualLast(datetime_index);

	// Create minutes
	img_timeline_minute_threads.this_minute_thread_completed = false;
	img_timeline_minute_threads.next_minute_thread_completed = false;
	img_timeline_minute_threads.this_minute_thread_handle =
		CreateThread(NULL,
			0,
			vs_system::ImgTimeline::PI_Props::T_CreateMinute,
			(LPVOID)0, // 0: This&Next This First Frame
			0,
			0
		);

	// Return error code
	return VS_OK;
}


// ---------- GotoFirstMinuteFirstFrame ----------
/*!
\brief Goto first frame of first minute
\author Gareth Edwards
*/
INT ImgTimeline::GotoFirstMinuteFirstFrame()
{
	// Initialise goto first folder
	INT result = img_timeline_folders.GotoFirstMinuteFirstFrame();

	// Create minutes
	img_timeline_minute_threads.this_minute_thread_completed = false;
	img_timeline_minute_threads.next_minute_thread_completed = false;
	img_timeline_minute_threads.this_minute_thread_handle =
		CreateThread(NULL,
		0,
		vs_system::ImgTimeline::PI_Props::T_CreateMinute,
		(LPVOID)0, // 0: This&Next This First Frame
		0,
		0
		);

	// Return error code
	return VS_OK;
}


// ---------- GotoLastMinuteFirstFrame ----------
/*!
\brief Goto last minute first frame
\author Gareth Edwards
*/
INT ImgTimeline::GotoLastMinuteFirstFrame()
{
	// Initialise goto first folder
	INT result = img_timeline_folders.GotoLastMinuteFirstFrame();

	// Create minutes
	img_timeline_minute_threads.this_minute_thread_completed = false;
	img_timeline_minute_threads.next_minute_thread_completed = false;
	img_timeline_minute_threads.this_minute_thread_handle =
		CreateThread(NULL,
				0,
				vs_system::ImgTimeline::PI_Props::T_CreateMinute,
				(LPVOID)0, // 0: This&Next This First Frame
				0,
				0
			);

	// Return error code
	return VS_OK;
}


// ---------- GotoLastMinuteLastFrame ----------
/*!
\brief Goto last minute last frame
\author Gareth Edwards
*/
INT ImgTimeline::GotoLastMinuteLastFrame(UINT delay_millisecond)
{
	// Initialise goto last folder
	INT result = img_timeline_folders.GotoLastMinuteLastFrame();
	if ( result == VS_ERROR ) return VS_ERROR;

	// Set delay
	img_timeline_folders.delay_millisecond = delay_millisecond;

	// Create last minute
	img_timeline_minute_threads.this_minute_thread_completed = false;
	img_timeline_minute_threads.next_minute_thread_completed = false;
	img_timeline_minute_threads.this_minute_thread_handle =
		CreateThread(NULL,
				0,
				vs_system::ImgTimeline::PI_Props::T_CreateMinute,
				(LPVOID)1, // 1: Only create last minute
				0,
				0
			);

	// Return error code
	return VS_OK;
}


// ---------- IsLastMinute ----------
/*!
\brief Is current minute the last minute
\author Gareth Edwards
*/
BOOL ImgTimeline::IsLastMinute()
{
	// Accelerate
	VNode * this_minute_folder = img_timeline_folders.this_minute_folder;
	VNode * last_minute_folder = img_timeline_folders.last_minute_folder;

	// IF last minute THEN
	if ( this_minute_folder == last_minute_folder ) return TRUE;

	// Return error code
	return FALSE;
}


// ---------- GetLastMinuteMSDifference ----------
/*!
\brief If current minute the last minute then returns last minus playback millisecond difference
\author Gareth Edwards
*/
INT ImgTimeline::GetLastMinuteMSDifference(INT *ms_diff)
{
	// Accelerate
	VNode * this_minute_folder = img_timeline_folders.this_minute_folder;
	VNode * last_minute_folder = img_timeline_folders.last_minute_folder;

	// IF last minute THEN
	if (this_minute_folder == last_minute_folder)
	{
		// Shortcut
		ImgTimelineFolders *tf = &img_timeline_folders;

		// IF NOT NEXT THEN
		if (tf->this_minute_folder != NULL)
		{
			// Get "this_minute_folder" minute buffer
			ImgTimelineMinute *tm = (ImgTimelineMinute *)tf->this_minute_folder->GetAssociated();

			// last minus playback millisecond difference 
			*ms_diff = tm->last_millisecond - tm->playback_millisecond;
			// printf("%d %d %d \n", *ms_diff, tm->last_millisecond, tm->playback_millisecond);
			return TRUE;
		}
	}

	// Return error code
	return FALSE;
}


// ---------- GetLastMinteTotalImages ----------
/*!
\brief If current minute the last minute then returns # of images
\author Gareth Edwards
*/
INT ImgTimeline::GetLastMinteTotalImages(UINT *total_images)
{
	// Accelerate
	VNode * this_minute_folder = img_timeline_folders.this_minute_folder;
	VNode * last_minute_folder = img_timeline_folders.last_minute_folder;

	// IF NO first minute THEN return
	if ( this_minute_folder == NULL ) return FALSE;

	// IF last minute THEN
	if ( this_minute_folder == last_minute_folder )
	{
		// Shortcut
		ImgTimelineFolders *tf = &img_timeline_folders;

		// IF NOT NEXT THEN
		if ( tf->this_minute_folder != NULL )
		{
			// Get "this_minute_folder" minute buffer
			ImgTimelineMinute *tm = (ImgTimelineMinute *)tf->this_minute_folder->GetAssociated();

			// Set # of images
			*total_images = tm->image_count;

			// Done
			return TRUE;
		}
	}

	// ELSE NOT last minute THEN
	{
		// Local
		INT stack_index = 5;
		VNode *stack[6];

		// Get Root
		VNode *n = this_minute_folder;
		VNode *root = NULL;
		while (n != NULL)
		{
			root = n;
			stack[stack_index--] = n;
			n = n->GetParent();
		}

		// Build path
		CHAR path[VS_MAXCHARPATHLEN];
		strcpy(path, stack[0]->GetName());
		for (INT i = 1; i < 6; i++)
		{
			strcat(path, stack[i]->GetName());
			strcat(path, "/");
		}
		strcat(path, "*");
		INT len = strlen(path);
		for (INT i = 0; i < len; i++) path[i] = path[i] == '/' ? '\\' : path[i];

		// Local
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;
		INT image_count = 0;

		// IF valid FindFirstFile THEN
		hFind = FindFirstFile(path, &fdata);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			// DO for all files in directory
			do
			{
				// IF NOT directory files THEN
				if (strcmp(fdata.cFileName, ".") != 0 &&
					strcmp(fdata.cFileName, "..") != 0)
				{
					// IF a file THEN
					if ( (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0 )
					{
						// IF a "jpg" THEN
						BOOL jpg =
							*(fdata.cFileName+0 ) >= '2' &&
							*(fdata.cFileName+0 ) <= '9' &&
							*(fdata.cFileName+20) == 'j' &&
							*(fdata.cFileName+21) == 'p' &&
							*(fdata.cFileName+22) == 'g';

						// Increment image counter
						if ( jpg )
						{
							//printf("%s\n", fdata.cFileName);
							image_count++;
						}

					} // IF a file THEN

				} // IF NOT directory files THEN

			} while (FindNextFile(hFind, &fdata) != 0); // DO for all files in directory

		} // IF valid FindFirstFile

		// Check done
		if (DWORD error = GetLastError() != ERROR_NO_MORE_FILES)
		{
			FindClose(hFind);
			return error;
		}

		// Set # of images
		*total_images = image_count;

		// Done
		return TRUE;
	}

	// Return error code
	return FALSE;
}


// ---------- RefreshLastMinute ----------
/*!
\brief Refresh last minute
\author Gareth Edwards
*/
INT ImgTimeline::RefreshLastMinute()
{
	// if (FALSE) return 1;

	// Accelerate
	VNode * this_minute_folder = img_timeline_folders.this_minute_folder;
	VNode * last_minute_folder = img_timeline_folders.last_minute_folder;

	// IF last minute THEN
	if ( this_minute_folder == last_minute_folder )
	{
		// Shortcut
		ImgTimelineFolders *tf = &img_timeline_folders;

		// IF NOT NEXT THEN
		if ( tf->this_minute_folder != NULL )
		{
			// Get "this_minute_folder" minute buffer
			ImgTimelineMinute *tm = (ImgTimelineMinute *)tf->this_minute_folder->GetAssociated();

			// Choose which of the two buffers to use...
			UINT use_index = tm == &img_timeline_minute[0] ? 0 : 1; // was 1 : 0

			// Refresh last minute
			INT result = pi_props->T_CreateMinute(TRUE, use_index, this_minute_folder);

			// Return # of images in minute
			return tm->image_count;
		}
	}

	// Return error code
	return VS_ERROR;
}


// ---------- RefreshCreate ----------
/*!
\brief Refresh created timeline folders
\author Gareth Edwards
*/
INT ImgTimeline::RefreshCreate(CHAR *archive_folder_name)
{
	// Initialise folder
	img_timeline_folders.path = archive_folder_name;
	img_timeline_folders.mask = "*";

	// LOCK
	//
	// Puedo mutex - these invocations are not threaded
	//
	img_timeline_folders.create_timeline_thread_completed = FALSE;

	// Create folders
	LPVOID lpParam = (LPVOID)1;
	vs_system::ImgTimeline::PI_Props::T_CreateFolders(lpParam);

	// Create minute
	lpParam = (LPVOID)3;
	vs_system::ImgTimeline::PI_Props::T_CreateMinute(lpParam);

	// UNLOCK
	img_timeline_folders.create_timeline_thread_completed = TRUE;

	// Done
	return VS_OK;
}


// ---------- Increment ----------
/*!
\brief Increment
\author Gareth Edwards
*/
INT ImgTimeline::Increment(INT milliseconds)
{
	// Local
	INT result = STOP;

	// Accelerate
	VNode *this_minute_folder = img_timeline_folders.this_minute_folder;

	// ---- IF NO this minute folder THEN return THIS_NOT_AVAILABLE ----
	if ( this_minute_folder == NULL )
		return THIS_NOT_AVAILABLE;

	// ---- IF NO this minute THEN also return THIS_NOT_AVAILABLE ----
	ImgTimelineMinute *this_minute = (ImgTimelineMinute *)this_minute_folder->GetAssociated();
	if ( this_minute == NULL )
		return THIS_NOT_AVAILABLE;

	// Forward...............................................................

	// Initialise millisecond value....
	INT new_millisecond_value = this_minute->playback_millisecond + milliseconds;

	// IF new millisecond value less than maximum THEN increment
	if ( new_millisecond_value <= this_minute->last_millisecond )
	{
		this_minute->playback_millisecond = new_millisecond_value;
		result = INCREMENTED;
	}

	// ELSE "try" and increment into "next_minute_folder" ----
	else
	{
		// IF next minute available THEN
		if ( img_timeline_minute_threads.IsNextMinuteAvailable() )
		{
			// IF next minute THEN
			if ( img_timeline_folders.next_minute_folder != NULL )
			{
				// MUST pass forward filename and (adjusted) milliseconds
				img_timeline_folders.GetFilename(img_timeline_folders.pass_fwd_filename);

				// IF play gap between this and next THEN jump to first frame of next ELSE allow timing difference
				img_timeline_folders.pass_fwd_ms = new_millisecond_value < 59999 ? 0 : new_millisecond_value - 59999;

				// Advance NOW and NEXT minute buffers
				INT new_next_minute = img_timeline_folders.AdvanceMinuteFolders();

				// IF new next minute THEN
				//
				// Note: This would be "caught" by the T_CreateMinute method but
				// this test prevents an unrequired thread being created!
				// 
				if ( new_next_minute == VS_OK )
				{
					// Refresh this_minute
					this_minute = (ImgTimelineMinute *)img_timeline_folders.this_minute_folder->GetAssociated();

					// Update playback millisecond
					this_minute->playback_millisecond = this_minute->first_millisecond;

					// (Thread) Create new NEXT minute
					img_timeline_minute_threads.next_minute_thread_completed = false;
					img_timeline_minute_threads.next_minute_thread_handle  =
						CreateThread(NULL,
								0,
								vs_system::ImgTimeline::PI_Props::T_CreateMinute,
								(LPVOID)2, // 2: Next First Frame
								0,
								0
							);

					// NEXT minute initialised and incremented
					result = INCREMENTED;
				}

				// ELSE no "next_minute_folder" THEN LAST_MINUTE_STOP
				else
				{
					result = LAST_MINUTE_STOP;
					//printf("LAST_MINUTE_STOP 3\n");
				}

			}

			// ELSE no "next_minute_folder" THEN LAST_MINUTE_STOP
			else
			{
				result = LAST_MINUTE_STOP;
				//printf("LAST_MINUTE_STOP 2\n");
			}

		}

		// ELSE "next_minute_folder" not ready THEN NEXT_NOT_AVAILABLE
		else
		{
			result = NEXT_NOT_AVAILABLE;
			//printf("LAST_MINUTE_STOP 1\n");
		}

	}

	// FINALLY, set new datetime second AND datetime_millisecond
	img_timeline_folders.datetime_second = this_minute->playback_millisecond / 1000;
	img_timeline_folders.datetime_millisecond = this_minute->playback_millisecond;

	// Return error code
	return result;
}


// ---------- List ----------
/*!
\brief Recursively list
\author Gareth Edwards
*/
INT ImgTimeline::List(CHAR s)
{
	// IF thread available AND created THEN 
	if ( img_timeline_folders.IsTimelineAvailable() && img_timeline_folders.IsTimelineCreated() )
	{
		switch ( s )
		{
			case 'A' : // List "all"
			case 'a' :
				pi_props->ListAllFolders(img_timeline_folders.root_folder);
				break;
			case 'L' : // List "cross" link
			case 'l':
				{
					VNode *archive = img_timeline_folders.root_folder;
					if ( archive != NULL )
					{
						VNode *years   = archive->GetFirst();
						VNode *months  = years->GetFirst();
						VNode *days    = months->GetFirst();
						VNode *hours   = days->GetFirst();
						VNode *minutes = hours->GetFirst();
						pi_props->ListCrossLinkFolders(years);
						pi_props->ListCrossLinkFolders(months);
						pi_props->ListCrossLinkFolders(days);
						pi_props->ListCrossLinkFolders(hours);
						pi_props->ListCrossLinkFolders(minutes);
					}
				}
				break;
		}
		return VS_OK;
	}

	// Timeline NOT completed -  return error
	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


