////////////////////////////////////////////////////////////////////////////////

// ---------- r2_definitions.h ----------
/*!
\file r2_defintions.h
\brief application definitions
\author Gareth Edwards

\todo Will change these for enum and const when I get a few mins.

\note Happy to leave as if for now...24/3/18

*/

#ifndef REVIEW_02_DEFINITIONS
#define REVIEW_02_DEFINITIONS


// ---- GROUP ID ----
	#define GROUP_UNDEFINED               0


// ---- pano element id
	#define GROUP_ELEM_PANO_ANNULAR       1
	#define GROUP_ELEM_PANO_SPHERICAL     2
	#define GROUP_ELEM_PANO_CYLIDRICAL    4
	#define GROUP_ELEM_PANO_SPH_SOLO      5
	#define GROUP_ELEM_PANO_FULL          7
	#define GROUP_ELEM_PANO_FRONT         10
	#define GROUP_ELEM_PANO_BACK          11


// ---- image element id
	#define GROUP_ELEM_IMAGE              16


// ---- stream id
	#define GROUP_STREAM                  15


// ---- commands: control & timeline ----

	#define COMMAND_BUFFER_MAX            8

	#define COMMAND_NONE                  0
	#define COMMAND_PLAY_START            1
	#define COMMAND_PLAY_STOP             2
	#define COMMAND_GOTO                  3
	#define COMMAND_GOTO_BOUNCE           38
	#define COMMAND_PANO_SAVE             39

	#define COMMAND_GOTO_FIRST            4        // GotoContextualFirst
	#define COMMAND_GOTO_LAST             37       // GotoContextualLast
	#define COMMAND_GOTO_FIRST_MINUTE     5        // GotoFirstMinuteFirstFrame
	#define COMMAND_GOTO_LAST_MINUTE      6        // GotoLastMinuteFirstFrame - NOT USED
	#define COMMAND_GOTO_LAST_FRAME       36       // GotoLastMinuteLastFrame

	#define COMMAND_GOTO_PREVIOUS         7
	#define COMMAND_GOTO_NEXT             8
	#define COMMAND_GOTO_FORWARD          9
	#define COMMAND_GOTO_BACKWARD         10                   

	#define COMMAND_DATETIME_DECADE       11       // Note: command = DATETIME_DECADE
	#define COMMAND_DATETIME_YEAR         12       // + COMMAND_DATETIME_DECAD
	#define COMMAND_DATETIME_MONTH        13
	#define COMMAND_DATETIME_DAY          14
	#define COMMAND_DATETIME_HOUR         15
	#define COMMAND_DATETIME_MINUTE       16
	#define COMMAND_DATETIME_SECOND       17
	#define COMMAND_DATETIME_MILLISECOND  18
	#define COMMAND_DATETIME_GO_DOWN      19

	#define COMMAND_DATETIME_RUNTIME      ( COMMAND_DATETIME_SECOND - COMMAND_DATETIME_DECADE )

	#define COMMAND_CREATE_TIMELINE       29

	#define COMMAND_CLIP_INPOINT          20
	#define COMMAND_CLIP_OUTPOINT         21
	#define COMMAND_CLIP_CLEAR            22
	#define COMMAND_CLIP_LOOP             23
	#define COMMAND_CLIP_ARCHIVE          24

	#define COMMAND_LIVE_START            26
	#define COMMAND_LIVE_STOP             27

	#define COMMAND_BOOST_COLOR_FX        40


// ---- IS bitmask
	#define IS_FIRST_MINUTE               1
	#define IS_LAST_MINUTE                2


// ---- BUTTON list indices
	#define BUTTON_MAX_LISTS              6
	#define TITLE_BUTTON_INDEX            0   // "Title" 
	#define FRAMES_BUTTON_INDEX           1   // "Frames
	#define CONTEXT_BUTTON_INDEX          2   // "Context"
	#define TIMELINE_BUTTON_INDEX         3   // "Timeline"
	#define CLIPLINE_BUTTON_INDEX         4   // "ClipLine"
	#define NAVIGATION_BUTTON_INDEX       5   // "Navigation" 


// ---- BUTTON groups
	#define TITLE_BUTTON_GROUP_ID         100
	#define FRAMES_BUTTON_GROUP_ID        200
	#define CAPTURE_BUTTON_GROUP_ID       250
	#define CONTEXT_BUTTON_GROUP_ID       300
	#define TIMELINE_BUTTON_GROUP_ID      400
	#define CLIPLINE_BUTTON_GROUP_ID      450
	#define CREATE_BUTTON_GROUP_ID        500
	#define DATETIME_BUTTON_GROUP_ID      600
	#define NAVIGATION_BUTTON_GROUP_ID    700

	#define LIDAR_BUTTON_GROUP_ID         750

	#define PTF_BUTTON_GROUP_ID           800
	#define IPP_BUTTON_BOOST_GROUP_ID     860
	#define IPP_BUTTON_COLOR_GROUP_ID     870

	#define MONITOR_BUTTON_GROUP_ID       900
	#define IMG_BUTTON_GROUP_ID           920
	#define NEST_BUTTON_GROUP_ID          930
	#define STREAM_BUTTON_GROUP_ID        940


// ---- TITLE button
	#define TITLE_LOGO              1
	#define TITLE_EXIT              2


// ---- CAPTURE buttons
	#define CAPTURE_LIVE            1
	#define CAPTURE_SAVE            2
	#define CAPTURE_REVIEW          3
	#define CAPTURE_IR              4
	#define CAPTURE_PANO            5


// ---- CONTEXT buttons
	#define CONTEXT_PREVIOUS        1
	#define CONTEXT_NEXT            2
	#define CONTEXT_FIRST           5
	#define CONTEXT_LAST            6


// ---- CREATE button
	#define CREATE_TIMELINE         8


// ---- DATETIME button indices
	#define DATETIME_MAX_STACK      8
	#define DATETIME_DECADE         0
	#define DATETIME_YEAR           1
	#define DATETIME_MONTH          2
	#define DATETIME_DAY            3 
	#define DATETIME_HOUR           4
	#define DATETIME_MINUTE         5
	#define DATETIME_SECOND         6
	#define DATETIME_MSEC           7


// ---- GPSDATA buttons
	#define GPSDATA_YEAR            1
	#define GPSDATA_MONTH           2
	#define GPSDATA_DAY             3 
	#define GPSDATA_HOUR            4
	#define GPSDATA_MINUTE          5
	#define GPSDATA_SECOND          6
	#define GPSDATA_MSEC            7
	#define GPSDATA_LONGITUDE       8
	#define GPSDATA_LATITUDE        9


// ---- NAVIGATION button actions
	#define NAVIGATION_GOTO_FIRST   1
	#define NAVIGATION_PLAY         2
	#define NAVIGATION_CLIP_START   3
	#define NAVIGATION_CLIP_END     4
	#define NAVIGATION_CLIP_CLEAR   5
	#define NAVIGATION_CLIP_LOOP    6
	#define NAVIGATION_CLIP_ARCHIVE 7
	#define NAVIGATION_LAST_MINUTE  8
	#define NAVIGATION_PANO         9
	#define NAVIGATION_LIVE         10
	#define NAVIGATION_SPEED        11


// ---- PTF actions
	#define PTF_ZOOM_IN             1
	#define PTF_TILT_UP             2
	#define PTF_ZOOM_OUT            3
	#define PTF_PAN_LEFT            4
	#define PTF_RESET               5
	#define PTF_PAN_RIGHT           6
	#define PTF_AUTOPAN_LEFT        7
	#define PTF_TILT_DOWN           8
	#define PTF_AUTOPAN_RIGHT       9
	#define PTF_SAVE               10


// ---- PTF actions
	#define PTF_TILT_UP             2


// ---- GPSDATA actions
	#define GPSDATA_VIEW            1
	#define GPSDATA_CLOSE           2


// ---- COLOR & BOOST FX actions
	#define IPP_COLOUR              0
	#define IPP_MONO                1
	#define IPP_SEPIA               2
	#define IPP_GREEN               3
	#define IPP_BOOST_01            4
	#define IPP_BOOST_02            5
	#define IPP_BOOST_03            6
	#define IPP_BOOST_BOX           7
	#define IPP_COLOUR_BOX          8


// ---- STREAM actions
	#define STREAM_VIEW             1
	#define STREAM_PLAY             2
	#define STREAM_ONVIF_VPTZ		3		
	#define STREAM_1                4
	#define STREAM_2                5
	#define STREAM_3                6
	#define STREAM_RTSP             8
	#define STREAM_CLOSE            7


// ---- Button: bitmasks
	#define BACKROUND_VERSION       1
	#define FOREGROUND_VERSION      2
	#define OUTLINE_VERSION         4


// ---- Button bitmask ----
	#define BTBM BACKROUND_VERSION | FOREGROUND_VERSION | OUTLINE_VERSION


// ---- Button: timeline bitmask
	#define OVBM OUTLINE_VERSION
	#define TLBM BACKROUND_VERSION | OUTLINE_VERSION
	#define MSBM FOREGROUND_VERSION | OUTLINE_VERSION


// ---- Button: frames, etc., bitmask
	#define CLBM FOREGROUND_VERSION


// ---- Button:  play ON/OFF  bitmask
	#define PLBM FOREGROUND_VERSION | OUTLINE_VERSION


// ---- Button: tag ON/OFF  bitmask
	#define TGBM FOREGROUND_VERSION | OUTLINE_VERSION


// ---- Button: behaviour
	#define BENO vs_system::ElmContext::BEHAVIOUR_NONE
	#define BECL vs_system::ElmContext::BEHAVIOUR_CLICK
	#define BECK vs_system::ElmContext::BEHAVIOUR_CLUNK
	#define BERA vs_system::ElmContext::BEHAVIOUR_RADIO
	#define BESE vs_system::ElmContext::BEHAVIOUR_SWITCH
	#define BELO vs_system::ElmContext::BEHAVIOUR_UNCLUNK


// ---- Button: U widths
	#define CLWD 0.1f    // Colon width
	#define ICWD 1.0f    // Icon width
	#define NUWD 0.6f    // Number width


// ---- Button: U spaces
	#define GPWD 1.0f    // Gap width
	#define BLWD 0.15f   // Block width
	#define SPWD 0.1f    // Space width
	#define ZEWD 0.0f    // Zero width


#endif // REVIEW_02_DEFINITIONS

////////////////////////////////////////////////////////////////////////////////
