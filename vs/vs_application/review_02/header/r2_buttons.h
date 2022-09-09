////////////////////////////////////////////////////////////////////////////////

// ---------- r2_buttons.h ----------
/*!
\file r2_buttons.h
\brief Button lists, structures and arrays. 
\author Gareth Edwards
*/

// ---- application variants ----
#include "../header/r2_variant.h"


// ---- include ----
#include "../header/r2_model.h"


// ---- namespace ----
using namespace review_02;


////////////////////////////////////////////////////////////////////////////////
//
// Notes on display lists
// ======================
//
// Content is associated with a VS panel item ( see VS documentation ) by a
// Display List "display_list_name".
//
// Display Lists are added to added to the the graphics interface via the
// GfxInterface method "AddList":
//
//    INT handle_display_list = gfx->AddDisplayList("display_list_name");
//
// The display name is also used to "handle" the display in the Model class
// method "Display":
//
// 	if ( !strcmp(display_name, "display_list_name") ) 
//    {
//    	INT display_handle = handle_display_list;
//
//			.../* See details in Display method */
//
//    }
//
// A "display_name" is passed from VS, via the application Display method, to
// the Model Display method from SDL view configuration data ingesteed at
// run-time (See VS View class and VS SDL view configuration file info).
//
//
////////////////////////////////////////////////////////////////////////////////
//
// Notes on button display lists
// =============================
//
// The order of button lists is determined by preprocessor definitions defined
// in the "r2_define.h" definitons file. 
//
// These are:
//    BUTTON_MAX_LISTS - total number of button lists
//    TITLE_BUTTON_INDEX, etc.,, - index of each button list
//
// The list indices are important as they denote the order in which the button
// reference arrays are intialised.
//   
// To add a button list - and in order:
//
//    1. Update BUTTON_MAX_LISTS to new total #
//    2. Add new *_BUTTON_INDEX definition
//    2. Add button_element_descriptor e.g. "title_button_list[] = {", etc....
//    3. Update button_list_size array
//    4. Update button_element_descriptor pointer list
//    5. Update handles list
//    6. Update font_list list
//    7. In Setup method, and where "Title" is the graphics display list
//       name, e.g. "handle_dspl_title_button = gfx->AddDisplayList("Title");"
//    8. In setup method Initialise HANDLES list
//


// ---- button struct
	struct button_element_descriptor
		{
			std::string name;
			std::string text;
			UINT  behaviour;
			UINT  sub_behaviour;
			UINT  switched_on;
			UINT  visible;
			UINT  group_id;
			UINT  id;

			BYTE  version_bitmask;
			FLOAT u_width;
			FLOAT u_space;
			UINT  u_block;

			UINT  tex_atlas_page;
			UINT  tex_background_index;
			UINT  tex_icon_row;
			UINT  tex_icon_col;

			INT   handle_element_context[3];
		};


// ---- "title" button list
	//#define VS_HYBRID
	button_element_descriptor title_button_list[] =
		{
			{ "INFO",     "9",     BECL, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      9,                CLBM, ICWD*4, ZEWD, 0,   3, 0, 0, 4,   0, 0, 0 }, //'P'atrol
			{ "SPACE",    "",      BENO, 0, 0, 1, TITLE_BUTTON_GROUP_ID,       0,                0,    ZEWD,   ZEWD, 1,   2, 0, 0, 0,   0, 0, 0 }, // Space (width selected by u_block == 1)
			
			#if defined(VS_HYBRID)
			{ "INFO",     "9",     BECL, 0, 0, 1, TITLE_BUTTON_GROUP_ID,       TITLE_EXIT,       CLBM, ICWD*4, ZEWD, 0,   3, 0, 5, 4,   0, 0, 0 }  // Exit application
			#else
			{ "EXIT",     "",      BECL, 0, 0, 1, TITLE_BUTTON_GROUP_ID,       TITLE_EXIT,       BTBM, ICWD,   ZEWD, 0,   2, 0, 1, 7,   0, 0, 0 }  // Exit application
			#endif
		};


// ---- REVIEW_02
	button_element_descriptor review_02_frames_button_list[] =
		{
			{ "F1",       "1",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      1,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 0,   0, 0, 0 }, // Spherical / Pano
			{ "F2",       "2",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      2,                CLBM, ICWD,   GPWD, 0,   2, 0, 5, 1,   0, 0, 0 }, // Nona / Pano

			{ "4",        "4",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      4,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 3,   0, 0, 0 }, // Cylinder / Pano
			{ "5",        "5",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      5,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 4,   0, 0, 0 }, // Spherical
			{ "6",        "6",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      6,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 5,   0, 0, 0 }, // Front Pano / Back Pano
			{ "7",        "7",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      7,                CLBM, ICWD,   GPWD, 0,   2, 0, 6, 6,   0, 0, 0 }, // Annular

			#if VS_LIDAR
			{ "8",        "8",     BECL, 0, 0, 1, LIDAR_BUTTON_GROUP_ID,       8,                CLBM, ICWD,   BLWD, 0,   2, 0, 1, 0,   0, 0, 0 }, // Lidar
			{ "9",        "9",     BECL, 0, 0, 1, LIDAR_BUTTON_GROUP_ID,       9,                CLBM, ICWD,   GPWD, 0,   3, 0, 3, 1,   0, 0, 0 }, // Lidar
			#endif

			{ "I",        "I",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_ZOOM_IN,      CLBM, ICWD,   BLWD, 0,   2, 0, 1, 3,   0, 0, 0 }, // Zoom In
			{ "O",        "O",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_ZOOM_OUT,     CLBM, ICWD,   BLWD, 0,   2, 0, 1, 4,   0, 0, 0 }, // Zoom Out
			{ "R",        "R",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_RESET,        CLBM, ICWD,   GPWD, 0,   2, 0, 1, 5,   0, 0, 0 }, // Reset

			{ "S",        "S",     BECK, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID,  NAVIGATION_PANO,  CLBM, ICWD,   GPWD, 0,   2, 0, 7, 5,   0, 0, 0 }, // Pano pic

			{ "FX",       "N",     BERA, 0, 1, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_COLOUR,       CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 4,   0, 0, 0 }, // FX normal colour
			{ "FX",       "M",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_MONO,         CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 5,   0, 0, 0 }, // FX monochrome
			{ "FX",       "S",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_SEPIA,        CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 6,   0, 0, 0 }, // FX sepia wash
			{ "FX",       "G",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_GREEN,        CLBM, ICWD,   GPWD, 0,   2, 0, 3, 7,   0, 0, 0 }, // FX green night

			{ "BoostBox", "BB",    BECL, 0, 0, 1, IPP_BUTTON_BOOST_GROUP_ID,   IPP_BOOST_BOX,    CLBM, ICWD,   ZEWD, 0,   2, 0, 4, 5,   0, 0, 0 }, // Boost

		}; 


// ---- REVIEW_04
	button_element_descriptor review_04_frames_button_list[] =
		{
			{ "F1",       "1",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      1,                0,    0,      0,    0,   1, 0, 6, 0,   0, 0, 0 }, // 
			{ "F2",       "2",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      2,                CLBM, ICWD,   BLWD, 0,   1, 0, 5, 0,   0, 0, 0 }, // 
			{ "F3",       "3",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      3,                CLBM, ICWD,   BLWD, 0,   1, 0, 6, 0,   0, 0, 0 }, // Spherical / Annular / Pano

			{ "5",        "5",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      5,                CLBM, ICWD,   GPWD, 0,   2, 0, 6, 4,   0, 0, 0 }, // 

			{ "FX",       "N",     BERA, 0, 1, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_COLOUR,       CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 4,   0, 0, 0 }, // FX normal colour
			{ "FX",       "M",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_MONO,         CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 5,   0, 0, 0 }, // FX monochrome
			{ "FX",       "S",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_SEPIA,        CLBM, ICWD,   ZEWD, 0,   2, 0, 3, 6,   0, 0, 0 }, // FX sepia wash
			{ "FX",       "G",     BERA, 0, 0, 1, IPP_BUTTON_COLOR_GROUP_ID,   IPP_GREEN,        CLBM, ICWD,   GPWD, 0,   2, 0, 3, 7,   0, 0, 0 }, // FX green night

			{ "BoostBox", "BB",    BECL, 0, 0, 1, IPP_BUTTON_BOOST_GROUP_ID,   IPP_BOOST_BOX,    CLBM, ICWD,   ZEWD, 0,   2, 0, 4, 5,   0, 0, 0 }, // Boost

		}; 


// ---- CAMERA_03
	//#define VS_QUADVIEW
	button_element_descriptor camera_03_frames_button_list[] =
		{

			{ "F1",       "1",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      1,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 0,   0, 0, 0 }, // Spherical / Pano
			{ "F2",       "2",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      2,                CLBM, ICWD,   GPWD, 0,   2, 0, 5, 1,   0, 0, 0 }, // Nona / Pano

			#if defined(VS_QUADVIEW)
			{ "3",        "3",     BECL, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      3,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 2,   0, 0, 0 }, // Spherical / Annular / Pano
			#endif

			{ "4",        "4",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      4,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 3,   0, 0, 0 }, // Cylinder / Pano
			{ "5",        "5",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      5,                CLBM, ICWD,   BLWD, 0,   2, 0, 6, 4,   0, 0, 0 }, // Spherical
			{ "6",        "6",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      6,                CLBM, ICWD,   GPWD, 0,   2, 0, 6, 5,   0, 0, 0 }, // Front Pano / Back Pano

			{ "I",        "I",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_ZOOM_IN,      CLBM, ICWD,   BLWD, 0,   2, 0, 1, 3,   0, 0, 0 }, // Zoom In
			{ "O",        "O",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_ZOOM_OUT,     CLBM, ICWD,   BLWD, 0,   2, 0, 1, 4,   0, 0, 0 }, // Zoom Out
			{ "R",        "R",     BECL, 0, 0, 1, PTF_BUTTON_GROUP_ID,         PTF_RESET,        CLBM, ICWD,   GPWD, 0,   2, 0, 1, 5,   0, 0, 0 }, // Reset

			#ifdef VS_CAMERA_03_ARCHIVE
			{ "10",      "10",     BESE, 0, 0, 1, CAPTURE_BUTTON_GROUP_ID,     CAPTURE_SAVE,     CLBM, ICWD,   GPWD, 0,   2, 0, 7, 4,   0, 0, 0 }, // Archive On/Off
			#endif
	
			#ifdef VS_CAMERA_03_STREAMING
			{ "ST",       "ST",    BESE, 0, 0, 1, STREAM_BUTTON_GROUP_ID,      STREAM_VIEW,      CLBM, ICWD,   BLWD, 0,   2, 0, 4, 1,   0, 0, 0 }, // Stream
			#endif

			{ "7",        "7",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      7,                CLBM, ICWD,   BLWD, 0,   2, 0, 4, 2,   0, 0, 0 }, // Settings
			{ "8",        "8",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      8,                CLBM, ICWD,   BLWD, 0,   2, 0, 4, 4,   0, 0, 0 }, // Dashboard

		};


// ---- CAMERA_05
	button_element_descriptor camera_05_frames_button_list[] =
		{

			{ "F1",       "1",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      1,                0,    0,      0,    0,   1, 0, 6, 0,   0, 0, 0 }, // 
			{ "F2",       "2",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      2,                CLBM, ICWD,   BLWD, 0,   1, 0, 5, 0,   0, 0, 0 }, // 
			{ "F3",       "3",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      3,                CLBM, ICWD,   BLWD, 0,   1, 0, 6, 0,   0, 0, 0 }, // Spherical / Annular / Pano
			{ "5",        "5",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      5,                CLBM, ICWD,   GPWD, 0,   2, 0, 6, 4,   0, 0, 0 }, // 

			#ifdef VS_CAMERA_03_ARCHIVE
			{ "10",      "10",     BESE, 0, 0, 1, CAPTURE_BUTTON_GROUP_ID,     CAPTURE_SAVE,     CLBM, ICWD,   GPWD, 0,   2, 0, 7, 4,   0, 0, 0 }, // Archive On/Off
			#endif

			#ifdef VS_CAMERA_03_STREAMING
			{ "ST",       "ST",    BESE, 0, 0, 1, STREAM_BUTTON_GROUP_ID,      STREAM_VIEW,      CLBM, ICWD,   BLWD, 0,   2, 0, 4, 1,   0, 0, 0 }, // Stream
			#endif

			{ "7",        "7",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      7,                CLBM, ICWD,   BLWD, 0,   2, 0, 4, 2,   0, 0, 0 }, // Settings
			{ "8",        "8",     BERA, 0, 0, 1, FRAMES_BUTTON_GROUP_ID,      8,                CLBM, ICWD,   BLWD, 0,   2, 0, 4, 4,   0, 0, 0 }, // Dashboard

		};


// ---- "context" button list
	button_element_descriptor context_button_list[] =
		{
			{ "LAST",     "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,     CONTEXT_PREVIOUS, BTBM, ICWD,   SPWD, 0,   2, 0, 2, 3,   0, 0, 0 }, // Last
			{ "SPACE",    "",      BENO, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,     0,                OVBM, ZEWD,   SPWD, 2,   2, 0, 0, 0,   0, 0, 0 }, // Space (width selected by u_block == 2)
			{ "NEXT",     "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,     CONTEXT_NEXT,     BTBM, ICWD,   ZEWD, 0,   2, 0, 2, 4,   0, 0, 0 }, // Next
		};


// ---- "timeline" button list
	button_element_descriptor timeline_button_list[100];


// ---- "clipline" button list 
	button_element_descriptor clipline_button_list[100];


// ---- "navigation" button list
//
// NOTE: Minute is parent of Seconds!
//
// Minute initialised as selected (see /****/ below) to match initialisation of 
// timeline_datetime_index = "seconds", in r2_application.cpp Setup method.
//
// Selecting another initial value for timeline_datetime_index requires that the
// following table be updated!!
//
	button_element_descriptor navigation_button_list[] =
	{
		{ "ARCHIVE",  "",      BECL, 0, 0, 1, CREATE_BUTTON_GROUP_ID,     CREATE_TIMELINE,         BTBM, ICWD,   BLWD*2, 0,   2, 0, 2, 0,   0, 0, 0 }, // Archive

		{ "DECADE",   "",      BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_DECADE,         BTBM, NUWD,   ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Decade
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "YEAR",     "0000",  BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_YEAR,           TLBM, NUWD*3, ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Year
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "MONTH",    "00",    BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_MONTH,          TLBM, NUWD*2, ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Month
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "DAY",      "00",    BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_DAY,            TLBM, NUWD*2, BLWD*2, 0,   2, 0, 0, 0,   0, 0, 0 }, // Day

		{ "HOUR",     "00",    BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_HOUR,           TLBM, NUWD*2, ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Hour
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "MIN",/****/"00",    BERA, 0, 1, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_MINUTE,         TLBM, NUWD*2, ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Minute
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "SEC",      "00",    BERA, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_SECOND,         TLBM, NUWD*2, ZEWD,   0,   2, 0, 0, 0,   0, 0, 0 }, // Second
		{ ":",        ":",     BENO, 0, 0, 1, 0,                          0,                       CLBM, CLWD,   ZEWD,   0,   2, 0, 2, 1,   0, 0, 0 },
		{ "MSEC",     "000",   BENO, 0, 0, 1, DATETIME_BUTTON_GROUP_ID,   DATETIME_MSEC,           MSBM, NUWD*2, BLWD*3, 0,   2, 0, 7, 0,   0, 0, 0 }, // Millisecond
	
		{ "FIRST",    "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,    CONTEXT_FIRST,           BTBM, ICWD,   SPWD,   0,   2, 0, 2, 2,   0, 0, 0 }, // First Minute First Frame
		{ "LAST",     "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,    CONTEXT_PREVIOUS,        BTBM, ICWD,   SPWD,   0,   2, 0, 2, 3,   0, 0, 0 }, // Contextual Last
		{ "PLAY",     "",      BESE, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_PLAY,         BTBM, ICWD,   SPWD,   0,   2, 0, 7, 4,   0, 0, 0 }, // Play OR Pause
		{ "SPEED",    "",      BECL, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_SPEED,        BTBM, ICWD,   SPWD,   0,   2, 0, 0, 4,   0, 0, 0 }, // 
		{ "NEXT",     "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,    CONTEXT_NEXT,            BTBM, ICWD,   SPWD,   0,   2, 0, 2, 4,   0, 0, 0 }, // Contextual Next
		{ "LAST",     "",      BECL, 0, 0, 1, CONTEXT_BUTTON_GROUP_ID,    CONTEXT_LAST,            BTBM, ICWD,   BLWD*2, 0,   2, 0, 2, 5,   0, 0, 0 }, // Last Minute First Frame

		{ "CLIP_IN",  "",      BECK, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_START,   TGBM, ICWD,   SPWD,   0,   2, 0, 3, 0,   0, 0, 0 }, //
		{ "CLIP_OUT", "",      BECK, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_END,     TGBM, ICWD,   SPWD,   0,   2, 0, 3, 1,   0, 0, 0 }, // 
		{ "CLIP_CLR", "",      BECL, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_CLEAR,   TGBM, ICWD,   SPWD,   0,   2, 0, 3, 2,   0, 0, 0 }, // 
		{ "CLIP_LOP", "",      BECK, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_LOOP,    TGBM, ICWD,   SPWD,   0,   2, 0, 3, 3,   0, 0, 0 }, // 
		{ "CLIP_EXP", "",      BECK, 0, 0, 1, NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_ARCHIVE, TGBM, ICWD,   ZEWD,   0,   2, 0, 7, 3,   0, 0, 0 }, // 

	};


// ---- list sizes ( # of elements )
	UINT button_list_size[BUTTON_MAX_LISTS] =
	{
		sizeof(title_button_list) / sizeof(button_element_descriptor),       // "Title"
		NULL,                                                                // "Frames"
		sizeof(context_button_list) / sizeof(button_element_descriptor),     // "Context"
		sizeof(timeline_button_list) / sizeof(button_element_descriptor),    //
		sizeof(clipline_button_list) / sizeof(button_element_descriptor),    //
		sizeof(navigation_button_list) / sizeof(button_element_descriptor),  //
	};


// ---- button_element_descriptor pointer list
	button_element_descriptor *button_list[BUTTON_MAX_LISTS] =
	{
		title_button_list,
		NULL,
		context_button_list,
		timeline_button_list,
		clipline_button_list,
		navigation_button_list
	};


// ---- graphics element handle list
	INT handles[BUTTON_MAX_LISTS];


// ---- font structure
	struct font_info
	{
		std::string name;
		std::string type;
		UINT height;
		UINT weight;
	};


// ---- associated font list
	std::string font_caption = "Roboto Lt";
	std::string font_label   = "Roboto Lt";
	std::string font_normal  = "Roboto";
	font_info font_list[] =
	{
		{ "Title",       font_normal,  36, 1 },
		{ "Frames",      font_normal,  36, 1 },
		{ "Context",     font_normal,  36, 1 },
		{ "Timeline",    font_normal,  36, 1 },
		{ "Clipline",    font_normal,  36, 1 },
		{ "Navigation",  font_caption, 36, 1 },
		{ "GpsData",     font_caption, 36, 1 },
		{ "About",       font_caption, 36, 1 },
		{ "Stream",      font_caption, 36, 1 },
		{ "RTSP",        font_normal,  36, 1 },
		{ "Dbz_Caption", font_caption, 36, 1 },
		{ "Dbz_Label",   font_label,   36, 1 },
		{ "Dbz_Normal",  font_normal,  36, 1 },
		{ "Ov_Button",   font_normal,  36, 1 },
		{ "Mv_Button",   font_normal,  36, 1 },
		{ "Vm_Button",   font_normal,  36, 1 },
	};


////////////////////////////////////////////////////////////////////////////////
