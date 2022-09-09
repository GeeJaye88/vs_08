////////////////////////////////////////////////////////////////////////////////

// ---------- c3_variant.h ----------
/*!
\file c3_variant.h
\brief Include for variants of c3 based applications.
\author Gareth Edwards
*/

#ifndef CAMERA_03_VARIANT
#define CAMERA_03_VARIANT


// ---- flags ----

// So it's...

//#define VS_CAMERA_03_SHARED_MEMORY


// Or it's....

	#define VS_CAMERA_03_SCAVENGER
	#define VS_CAMERA_03_STREAMING
	#define VS_CAMERA_03_ARCHIVE
	#define VS_CAMERA_03_GPS


// ---- logic ----

#ifdef VS_CAMERA_03_SHARED_MEMORY

	#ifdef VS_CAMERA_03_ARCHIVE
	#undef VS_CAMERA_03_ARCHIVE
	#endif

	#ifdef VS_CAMERA_03_GPS
	#undef VS_CAMERA_03_GPS
	#endif

	#ifdef VS_CAMERA_03_STREAMING
	#undef VS_CAMERA_03_STREAMING
	#endif

#elif defined(VS_CAMERA_03_STREAMING)

	#ifdef VS_CAMERA_03_SHARED_MEMORY
	#undef VS_CAMERA_03_SHARED_MEMORY
	#endif

#endif


#endif // CAMERA_03_VARIANT


////////////////////////////////////////////////////////////////////////////////