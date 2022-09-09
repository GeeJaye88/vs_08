////////////////////////////////////////////////////////////////////////////////

// ---------- se1_application.cpp ----------
/*!
\file se1_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note https://msdn.microsoft.com/en-us/library/windows/desktop/ms533832(v=vs.85).aspx
*/

#define _CRT_SECURE_NO_WARNINGS


// ---------- Application include ----------
#include "../header/se1_application.h"


// ---------- namespace ----------
using namespace set_exif_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

public:

	// Constructor/destructor
	PI_Application();
	~PI_Application();

	// Methods
	INT Setup(vs_system::AppDependency *);

	// Application dependency pointers
	vs_system::AppDependency *app_dependency;
	vs_system::SysContext    *sys_context;
	vs_system::SysOutput     *sys_output;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{
	// Application dependency pointers
	app_dependency = NULL;
	sys_context = NULL;
	sys_output = NULL;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::~PI_Application()
{
	;
}


// ---------- Setup ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
INT application::PI_Application::Setup(
		vs_system::AppDependency *ad
	)
{

	// Inject application dependency
	app_dependency = ad;

	// Accelerate
	sys_context = ad->GetSysContext();
	sys_output = ad->GetSysOutput();

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Application declaration for static properties ----------
application::PI_Application  * application::pi_app = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	// Allocate and construct private properties
	pi_app = new PI_Application();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	// Delete private properties
	if ( pi_app ) delete pi_app;
}


// -------------------- Framework --------------------


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param INT [in] number of arguments
\param CHAR* [in] argument list
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dependency)
{
	// Local
	INT result = VS_OK;

	// Store application dependency reference
	result = pi_app->Setup(app_dependency);
	if ( result <= VS_ERROR ) return result;

	// Done
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Process
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	INT result = E3();

	Sleep(2000);

	// Not just VS_OK but completed
	return VS_COMPLETED;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{

	// Done
	return VS_OK;
}


// ---------- E1 ----------
/*!
\brief E1
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::E1()
{
	// Scopw == method
	using namespace Gdiplus;

	// Initialize <tla rid="tla_gdiplus"/>.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	Gdiplus::Status status = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT    size = 0;
	UINT    count = 0;
	Bitmap* bitmap = new Bitmap(L"rocks.jpg");

	bitmap->GetPropertySize(&size, &count);

	printf("There are %d pieces of metadata in the file.\n", count);
	printf("The total size of the metadata is %d bytes.\n", size);

	int s = sizeof(bitmap);

	delete bitmap;
	GdiplusShutdown(gdiplusToken);


	// Done
	return VS_OK;
}


HRESULT PropertyTypeFromWORD(WORD , WCHAR* , UINT );


// ---------- E2 ----------
/*!
\brief E2
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::E2()
{
	// Scope == method
	using namespace Gdiplus;

	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	UINT  size = 0;
	UINT  count = 0;

#define MAX_PROPTYPE_SIZE 30

	WCHAR strPropertyType[MAX_PROPTYPE_SIZE] = L"";

	Bitmap* bitmap = new Bitmap(L"rocks.jpg");

	bitmap->GetPropertySize(&size, &count);
	printf("There are %d pieces of metadata in the file.\n\n", count);

	// GetAllPropertyItems returns an array of PropertyItem objects.
	// Allocate a buffer large enough to receive that array.

	Gdiplus::PropertyItem* pPropBuffer = (Gdiplus::PropertyItem*)malloc(size);

	// Get the array of PropertyItem objects.
	bitmap->GetAllPropertyItems(size, count, pPropBuffer);

	// For each PropertyItem in the array, display the id, type, and length.
	for (UINT j = 0; j < count; ++j)
	{
		// Convert the property type from a WORD to a string.
		PropertyTypeFromWORD(
			pPropBuffer[j].type, strPropertyType, MAX_PROPTYPE_SIZE);

		printf("Property Item %d\n", j);
		printf("  id: 0x%x\n", pPropBuffer[j].id);
		wprintf(L"  type: %s\n", strPropertyType);
		printf("  length: %d bytes\n\n", pPropBuffer[j].length);
	}

	free(pPropBuffer);
	delete bitmap;
	GdiplusShutdown(gdiplusToken);
	
	// Done
	return VS_OK;
}


// Helper function
HRESULT PropertyTypeFromWORD(
		WORD index,
		WCHAR* string,
		UINT maxChars
	)
{
	HRESULT hr = E_FAIL;

	WCHAR* propertyTypes[] = {
		L"Nothing",                   // 0
		L"PropertyTagTypeByte",       // 1
		L"PropertyTagTypeASCII",      // 2
		L"PropertyTagTypeShort",      // 3
		L"PropertyTagTypeLong",       // 4
		L"PropertyTagTypeRational",   // 5
		L"Nothing",                   // 6
		L"PropertyTagTypeUndefined",  // 7
		L"Nothing",                   // 8
		L"PropertyTagTypeSLONG",      // 9
		L"PropertyTagTypeSRational" }; // 10

	hr = StringCchCopyW(string, maxChars, propertyTypes[index]);
	return hr;
}


// ---------- Declarations ----------

INT GetEncoderClsid(CONST WCHAR* format, CLSID* pClsid);
INT GetPropertyLabel(CONST PROPID id, CHAR *);
INT SetBitMapProperties(Gdiplus::Bitmap *);


// ---------- E3 ----------
/*!
\brief E3
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::E3()
{
	// Initialize <tla rid="tla_gdiplus"/>
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Get the CLSID of the JPEG encoder
	CLSID clsid;
	GetEncoderClsid(L"image/jpeg", &clsid);

	// Set Bitmap properties
	Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(L"rocks.jpg");
	INT result = SetBitMapProperties(bitmap);
	Gdiplus::Status stat = bitmap->Save(L"rocks2.jpg", &clsid, NULL);
	if ( stat != Gdiplus::Ok ) return VS_ERROR;
	printf("FakePhoto2.jpg saved successfully.\n");

	// Cleanup
	delete bitmap;
	Gdiplus::GdiplusShutdown(gdiplusToken);

	// Success
	return VS_OK;
}


INT GetEncoderClsid(
		const WCHAR* format,
		CLSID* pClsid
	)
{
	UINT  num  = 0;  // Number of image encoders
	UINT  size = 0;  // Size of the image encoder array in bytes
	Gdiplus::ImageCodecInfo *pImageCodecInfo = NULL;
	
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return VS_ERROR;  // Failure
	
	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return VS_ERROR;  // Failure
	
	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			* pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}
	
	free(pImageCodecInfo);
	return VS_ERROR;  // Failure
}


INT GetPropertyLabel(
		CONST PROPID id,
		CHAR *propertyLabel
	)
{
	CHAR *label = "Not Defined";
	switch (id)
	{
		// ========== IMAGE ==========

		case PropertyTagImageWidth           :   // 0x0100
			label = "Image Width";
			break;
		case PropertyTagImageHeight          :   // 0x0101
			label = "Image Height";
			break;
		case PropertyTagBitsPerSample        :   // 0x0102 
			label = "Image Bits Per Sample";
			break;
		case PropertyTagCompression          :   // 0x0103 
			label = "Image Compression";
			break;

		// ========== DESCRIPTION ==========

		case PropertyTagImageDescription     :   // 0x010E
			label = "Image Description";
			break;
		case PropertyTagEquipMake            :   // 0x10f
			label = "Equipment Make";       
			break;
		case PropertyTagEquipModel           :   // 0x110 
			label = "Equipment Model";
			break;

		// ========== GPS ==========

		case PropertyTagGpsVer               :   // 0x0000
			label = "Gps Ver";
			break;
		case PropertyTagGpsLatitudeRef       :   // 0x0001 
			label = "Gps Latitude Ref";
			break;
		case PropertyTagGpsLatitude          :   // 0x0002
			label = "Gps Latitude";
			break;
		case PropertyTagGpsLongitudeRef      :   // 0x0003
			label = "Gps Longitude Ref";
			break;
		case PropertyTagGpsLongitude         :   // 0x0004
			label = "Gps Longitude";
			break;
		case PropertyTagGpsAltitudeRef       :   // 0x0005
			label = "Gps Altitude Ref";
			break;
		case PropertyTagGpsAltitude          :   // 0x0006
			label = "Gps Altitude";
			break;
		case PropertyTagGpsGpsTime           :   // 0x0007
			label = "Gps Time";
			break;
		case PropertyTagGpsGpsSatellites     :   // 0x0008
			label = "Gps Satellites";
			break;
		case PropertyTagGpsGpsStatus         :   // 0x0009
			label = "Gps Status";
			break;
		case PropertyTagGpsGpsMeasureMode    :   // 0x00A
			label = "Gps Measure Mode";
			break;
		case PropertyTagGpsGpsDop            :   // 0x000B -  Measurement precision
			label = "Gps Dop";
			break;
		case PropertyTagGpsSpeedRef          :   // 0x000C
			label = "Gps Spee dRef";
			break;
		case PropertyTagGpsSpeed             :   // 0x000D
			label = "Gps Speed";
			break;
		case PropertyTagGpsTrackRef          :   // 0x000E
			label = "Gps TrackRef";
			break;
		case PropertyTagGpsTrack             :   // 0x000F
			label = "Gps Track";
			break;
		case PropertyTagGpsImgDirRef         :   // 0x0010
			label = "Gps Img Dir Ref";
			break;
		case PropertyTagGpsImgDir            :   // 0x0011
			label = "Gps Img Dir";
			break;
		case PropertyTagGpsMapDatum          :   // 0x0012
			label = "Gps Map Datum";
			break;
		case PropertyTagGpsDestLatRef        :   // 0x0013
			label = "Gps Dest Lat Ref";
			break;
		case PropertyTagGpsDestLat           :   // 0x0014
			label = "Gps Dest Lat";
			break;
		case PropertyTagGpsDestLongRef       :   // 0x0015
			label = "Gps Dest Long Ref";
			break;
		case PropertyTagGpsDestLong          :   // 0x0016
			label = "Gps Dest Long";
			break;
		case PropertyTagGpsDestBearRef       :   // 0x0017
			label = "Gps Dest Bear Ref";
			break;
		case PropertyTagGpsDestBear          :   // 0x0018
			label = "Gps Dest Bear";
			break;
		case PropertyTagGpsDestDistRef       :   // 0x0019
			label = "Gps Dest Dist Ref";
			break;
		case PropertyTagGpsDestDist          :   // 0x001A
			label = "Gps Dest Dist";
			break;
		case PropertyTagGpsProcessingMethod  :   // 0x001B
			label = "Gps Processing Method";
			break;
		case PropertyTagGpsAreaInformation   :   // 0x001C
			label = "Gps Area Information";
			break;
		case PropertyTagGpsDate              :   // 0x001D
			label = "Gps Date";
			break;
		case PropertyTagGpsDifferential      :   // 0x001E
			label = "Gps Differential";
			break;

		default:
			break;
	}
	strcpy(propertyLabel, label);
	return VS_OK;
}


INT SetBitMapProperties(
		Gdiplus::Bitmap* bitmap
		
	)
{
	// Ambiguous, requires explict namespace
	Gdiplus::PropertyItem* propertyItem = new Gdiplus::PropertyItem;
	CHAR *value = NULL;

	// Description
	propertyItem->id     = PropertyTagImageDescription;
	value                = "360 Degree Annular";
	propertyItem->length = strlen(value)+1;
	propertyItem->type   = PropertyTagTypeASCII;
	propertyItem->value  = value;
	bitmap->SetPropertyItem(propertyItem);

	// Make
	propertyItem->id     = PropertyTagEquipMake;
	value                = "Observant Innovations";
	propertyItem->length = strlen(value) + 1;
	propertyItem->type   = PropertyTagTypeASCII;
	propertyItem->value  = value;
	bitmap->SetPropertyItem(propertyItem);

	// Model
	propertyItem->id     = PropertyTagEquipModel;
	value                = "Patrol 18";
	propertyItem->length = strlen(value) + 1;
	propertyItem->type   = PropertyTagTypeASCII;
	propertyItem->value  = value;
	bitmap->SetPropertyItem(propertyItem);

	// Success
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////