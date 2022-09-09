////////////////////////////////////////////////////////////////////////////////

// ---------- mount_cal_01.hpp ----------
/*!
\file mount_cal_01.hpp
\brief
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS


// ---------- application ----------
//#include "../header/mc1_application.h"


// ---------- namespace ----------
using namespace mount_calc_01;


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

	public:

		// ---- constructor/destructor
			PI_Application();
			~PI_Application();


		// ---- methods
			INT Setup(vs_system::AppDependency *);
			INT Mount();


		// ---- support methods
			INT Report();
			INT Usage(CHAR);


		// ---- dot ini methods
			INT DotIni_Initialise(CHAR *);
			INT DotIni_Read();
			INT DotIni_Write();


		// ---- dot ini object
			vs_system::DotIni dot_ini;


		// ---- application dependency pointers
			vs_system::AppDependency *_app_dependency = NULL;
			vs_system::SysContext    *_sys_context    = NULL;
			vs_system::SysOutput     *_sys_output     = NULL;


		// ---- default dimensions
			enum
			{
				DEFAULT_PAPER_SIZE     = 4,
				DEFAULT_IMAGE_WIDTH    = 100,
				DEFAULT_IMAGE_HEIGHT   = 100,

				DEFAULT_OUTLINE_MAX    = 10,
				DEFAULT_OUTLINE_COUNT  = 1,
				DEFAULT_OUTLINE_DEPTH  = 1,
				DEFAULT_OUTLINE_GAP    = 5,

				DEFAULT_BORDER_DEPTH   = 5,
				DEFAULT_BORDERT_MIN    = 1,

				DEFAULT_PIXEL_VALUE    = 128,

				DEFAULT_MEASURE        = 0,
				DEFAULT_REPORT         = 1

			};


		// ---- required args
			FLOAT _image_width  = DEFAULT_IMAGE_WIDTH;
			FLOAT _image_height = DEFAULT_IMAGE_HEIGHT;


		// ---- (-f) filename
			CHAR _filename[VS_MAXCHARNAME];


		// ---- total used
			FLOAT _total_width_used  = 0;
			FLOAT _total_height_used = 0;
			FLOAT _total_chrome = 0;

		// ---- minimum & maximum
			FLOAT _minimax_left   = 0;
			FLOAT _minimax_right  = 0;
			FLOAT _minimax_top    = 0;
			FLOAT _minimax_bottom = 0;

		// ---- available 
			FLOAT _available_width = 0;
			FLOAT _available_height = 0;


		// ---- paper info
			struct a_dimension
			{
				FLOAT width;
				FLOAT height;
			};

			a_dimension _paper_dimension[6] =
			{
				{   0,   0 },
				{ 594, 841 },
				{ 420, 594 },
				{ 297, 420 },
				{ 210, 297 },
				{ 148, 210 }
			};

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{
	
	_app_dependency = NULL;
	_sys_context    = NULL;
	_sys_output     = NULL;

	strcpy(_filename, "mcalc.jpg");

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
INT application::PI_Application::Setup(vs_system::AppDependency *ad)
{

	// ---- local
		INT result = VS_ERROR;


	// ---- properties
		#include "../hpp/mc1_properties_include.hpp"


	// ---- inject application dependency
		_app_dependency = ad;


	// ---- accelerate
		_sys_context = ad->GetSysContext();
		_sys_output  = ad->GetSysOutput();


	// ---- dev
		if ( _sys_context->GetBool(_sys_context->DEVELOPMENT) )
		{

			// ---- set required args
				_image_width  = 189;
				_image_height = 274;


			// ---- test
			 // vsx 100 150 -ps 4 -pr Y -bd 8 -oc 5 -od 0.2 -og 2 -cv 0.75 -f my_a4_mount.jpg
				CHAR *test_argv[5] =
				{
					"vsx",
					"100",
					"100",
					"-bd",   // paper
					"5",
					/*
					"-pm",
					"y",
					"-pr",
					"y",
					"-bd",   // border
					"8",
					"-oc",   // outline
					"5",
					"-od",
					"0.2",
					"-og",
					"2",
					"-cv",   // colour
					"0.75",
					"-f",    // file
					"my_a4_mount.jpg"
					*/
				};
				INT test_argc = 5;
				_app_dependency->SetArgc(test_argc);
				for (INT i = 0; i < test_argc; i++)
				{
					_app_dependency->SetArgv(i,test_argv[i]);
				}

		}
		else
		{

			// ---- overflow
				INT argc = _app_dependency->GetArgc();
				if ( argc >= VS_MAXARGV )
				{
					printf("\n");
					printf("Error: Exceeded maximum [%d] # of command line arguments.", VS_MAXARGV);
					printf("\n");
					return VS_EXIT;
				} 


			// ---- usage
				if ( argc == 2 )
				{
					if ( !strcmp(_app_dependency->GetArgv(1), "-e") )
						result = Usage('e');
					else
						result = Usage('u');
					return VS_EXIT;
				}


			// ---- less than minimum args
				if ( argc < 3 )
				{
					printf("\n");
					printf("Error: %d - too few command line arguments.\n", argc);
					result = Usage('u');
					return VS_EXIT;
				}


			// ---- process required args
				_image_width  = (FLOAT)atof(_app_dependency->GetArgv(1));
				_image_height = (FLOAT)atof(_app_dependency->GetArgv(2));

		}

	// ---- init param
		INT arg  = 3; // min arguments
		INT argc = _app_dependency->GetArgc();
		while ( arg < argc )
		{

				if ( arg + 1 < argc)
				{

					// ---- shortcut
						CHAR *argv_name  = _app_dependency->GetArgv(arg);
						CHAR *argv_value = _app_dependency->GetArgv(arg+1);

					// ---- border depth
						if ( !strcmp("-bd", argv_name) )
						{
							border_depth = (FLOAT)atof(argv_value);
							border_depth = border_depth < DEFAULT_BORDERT_MIN ? DEFAULT_BORDERT_MIN : border_depth;
							vn_border_depth->SetValue(border_depth);
							arg += 2;
						}

					// ---- colour value
						else if ( !strcmp("-cv", argv_name) )
						{
							FLOAT v = (FLOAT)atof(argv_value);
								v = v < 0 ? 0 : (v > 1 ? 1 : v);
							colour_value = v;
							vn_colour_value->SetValue(colour_value);
							arg += 2;
						}

					// ---- filename
						else if ( !strcmp("-f", argv_name) )
						{
							strcpy_s(_filename, VS_MAXCHARNAME, argv_value);
							arg += 2;
						}

					// ---- number of outlines
						else if ( !strcmp("-oc", argv_name) )
						{
							outline_count = atoi(argv_value);
							outline_count =
								outline_count < DEFAULT_OUTLINE_MAX ?
									outline_count : DEFAULT_OUTLINE_MAX;
							vn_outline_count->SetValue(outline_count);
							arg += 2;
						}

					// ---- outline depth
						else if ( !strcmp("-od", argv_name) )
						{
							outline_depth = (FLOAT)atof(argv_value);
							outline_depth = outline_depth < 0 ? 0 : outline_depth;
							vn_outline_depth->SetValue(outline_depth);
							arg += 2;
						}

					// ---- outline gap
						else if ( !strcmp("-og", argv_name) )
						{
							outline_gap = (FLOAT)atof(argv_value);
							outline_gap = outline_gap < 0 ? 0 : outline_gap;
							vn_outline_gap->SetValue(outline_gap);
							arg += 2;
						}

					// ---- paper measurement
						else if ( !strcmp("-pm", argv_name) )
						{
							INT c = (INT)*(argv_value + 0);
							c = toupper(c);
							vn_paper_measure->SetValue((CHAR)c == 'Y' ? "Y" : "N");
							arg += 2;
						}

					// ---- report
						else if ( !strcmp("-pr", argv_name) )
						{
							INT c = (INT)*(argv_value + 0);
							c = toupper(c);
							vn_paper_report->SetValue( (CHAR)c == 'Y' ? "Y" : "N");
							arg += 2;
						}

					// ---- paper size
						else if ( !strcmp("-ps", argv_name) )
						{
							paper_size = atoi(argv_value);
							if ( paper_size < 1 ? 1 : paper_size > 5 )
							{
								printf("Error: Unrecognised Paper Size %d.", paper_size);
								printf("\n");
								return VS_EXIT;
							}
							vn_paper_size->SetValue(paper_size);
							arg += 2;
						}

					// ---- printer correction
						else if (!strcmp("-pc", argv_name))
						{
							printer_correction = (FLOAT)atof(argv_value);
							vn_printer_correction->SetValue(argv_value);
							arg += 2;
						}

					// ---- printer horizontal
						else if (!strcmp("-ph", argv_name))
						{
							printer_horizontal = (FLOAT)atof(argv_value);
							vn_printer_horizontal->SetValue(argv_value);
							arg += 2;
						}

					// ---- printer vertical
						else if (!strcmp("-pv", argv_name))
						{
							printer_vertical = (FLOAT)atof(argv_value);
							vn_printer_vertical->SetValue(argv_value);
							arg += 2;
						}
					// ---- oops....
						else
						{
							printf("\n");
							printf("Error: Unrecognised command line option %s.", argv_name);
							printf("\n");
							return VS_EXIT;
						}

			}
			else
			{
				printf("\n");
				printf("Error: Unmatched command line option %s.", _app_dependency->GetArgv(arg));
				printf("\n");
				return VS_EXIT;
			}
		}


	// ---- check that everything fits

		_total_chrome = 
			outline_count <= 0 ? 0 :  
				( border_depth +
					outline_count * outline_depth +
						(outline_count - 1) * outline_gap );


	// ---- total width

		_total_width_used =
			_image_width +
				_total_chrome * 2;

		_total_width_used    *= printer_correction;

		FLOAT paper_width_by2 = _paper_dimension[paper_size].width / 2;
		FLOAT total_width_by2 = _total_width_used / 2;
		_minimax_left         =  paper_width_by2 - total_width_by2 - printer_horizontal;
		_minimax_right        = _minimax_left + _total_width_used;

		if ( _total_width_used > _paper_dimension[paper_size].width )
		{
			printf("\n");
			printf("Error: Width (Image + Border + Outline) greater (%5.2f) than available width\n", _total_width_used);
			printf("\n");
			Report();
			return VS_EXIT;
		}

		if ( _minimax_left < 0 )
		{
			printf("\n");
			printf("Error: Outline left (%5.2f) outside left minimum\n", _minimax_left);
			printf("\n");
			Report();
			return VS_EXIT;
		}

		if ( _minimax_right > _paper_dimension[paper_size].width )
		{
			printf("\n");
			printf("Error: Outline right (%5.2f) outside right maximum\n", _minimax_right);
			printf("\n");
			Report();
			return VS_EXIT;
		}


	// ---- total height

		_total_height_used =
			_image_height +
				_total_chrome * 2;

		_total_height_used    *= printer_correction;

		FLOAT paper_height_by2 = _paper_dimension[paper_size].height / 2;
		FLOAT total_height_by2 = _total_height_used / 2;
		_minimax_top           = paper_height_by2 - total_height_by2 - printer_vertical;
		_minimax_bottom        = _minimax_top + _total_height_used;

		if ( _total_height_used > _paper_dimension[paper_size].height )
		{
			printf("\n");
			printf("Error: Height (Image + Border + Outline) greater (%5.2f) than availabe height\n", _total_height_used);
			printf("\n");
			Report();
			return VS_EXIT;
		}

		if ( _minimax_top < 0 )
		{
			printf("\n");
			printf("Error: Outline top (%5.2f) outside top minimum\n", _minimax_top);
			printf("\n");
			Report();
			return VS_EXIT;
		}

		if ( _minimax_bottom > _paper_dimension[paper_size].height )
		{
			printf("\n");
			printf("Error: Outline bottom (%5.2f) outside bottom maximum\n", _minimax_bottom);
			printf("\n");
			Report();
			return VS_EXIT;
		}


	// ---- available width

		_available_width = 
			_paper_dimension[paper_size].width -
				( _total_chrome * 2 + 0.5f ) ;

		_available_width /= printer_correction;
	
	// ---- available height

		_available_height =
			_paper_dimension[paper_size].height -
				( _total_chrome * 2 + 0.5f );

		_available_height /= printer_correction;


	return VS_OK;
}


// ---------- Mount ----------
/*!
\brief Mount
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Mount()
{

	// ---- dots per mm (roughly equiv to 300 dpi)
		FLOAT dpmm = 118.11f / 10;


	// ---- local
		vs_system::ImgJpegLib jpeg_lib;
		vs_system::ImgBuffer  img_buffer;
		struct Pixel { BYTE red; BYTE green; BYTE blue; };


	// ---- properties
		#include "../hpp/mc1_properties_include.hpp"


	// ---- pixel colour required by lambda
		Pixel pixel =
			{
				(BYTE)(colour_value * 255 + 0.5f),
				(BYTE)(colour_value * 255 + 0.5f),
				(BYTE)(colour_value * 255 + 0.5f),
			};


	// ---- lambda img_buffer plot
		auto plot = [&img_buffer, pixel]( UINT x, UINT y)
		{
			INT row_offset = img_buffer.GetPitch()   * y;
			INT col_offset = img_buffer.GetStride()  * x;
			INT offset     = row_offset + col_offset;
			BYTE *buffer   = img_buffer.GetBuffer() + offset;
			*(buffer   )  = pixel.red;
			*(buffer + 1) = pixel.green;
			*(buffer + 2) = pixel.blue;
		};


	// ----- paper conversion
		UINT pixel_paper_width   = (UINT)(_paper_dimension[paper_size].width  * dpmm + 0.5f);
		UINT pixel_paper_height  = (UINT)(_paper_dimension[paper_size].height * dpmm + 0.5f);
		UINT pixel_paper_wby2    = pixel_paper_width  / 2;
		UINT pixel_paper_hby2    = pixel_paper_height / 2;


	// ---- printer conversion
		UINT pixel_printer_horizontal = (UINT)(printer_horizontal * dpmm + 0.5f);
		UINT pixel_printer_vertical   = (UINT)(printer_vertical   * dpmm + 0.5f);


	// ---- other
		UINT pixel_border_depth  = (UINT)(border_depth  * dpmm * printer_correction + 0.5f);
		UINT pixel_outline_depth = (UINT)(outline_depth * dpmm * printer_correction + 0.5f);
		UINT pixel_outline_gap   = (UINT)(outline_gap   * dpmm * printer_correction + 0.5f);


	// ---- image conversion
		UINT pixel_image_width  = (UINT)(_image_width  * dpmm * printer_correction + 0.5f);
		UINT pixel_image_height = (UINT)(_image_height * dpmm * printer_correction + 0.5f);
		UINT pixel_image_wby2   = pixel_image_width / 2;
		UINT pixel_image_hby2   = pixel_image_height / 2;


	// ---- create img_buffer
		img_buffer.Initialise(pixel_paper_width, pixel_paper_height,3);
		img_buffer.CreateBuffer();
		img_buffer.SetQuality(100);
		BYTE *buffer = img_buffer.GetBuffer();
		memset(buffer, 255, img_buffer.GetExtent());


	// ---- convert mm dimensions to pixel
		UINT pixel_image_left   = pixel_paper_wby2 - pixel_image_wby2 + pixel_printer_horizontal;
		UINT pixel_image_right  = pixel_paper_wby2 + pixel_image_wby2 + pixel_printer_horizontal;
		UINT pixel_image_top    = pixel_paper_hby2 - pixel_image_hby2 + pixel_printer_vertical;
		UINT pixel_image_bottom = pixel_paper_hby2 + pixel_image_hby2 + pixel_printer_vertical;


	// ----- # of outlines (plus one for image)
		UINT pixel_outline_count = outline_count + 1;


	// ---- build pixel rect table
		struct pixel_rect
		{
			UINT left, right, top, bottom;
		};
		pixel_rect rect[DEFAULT_OUTLINE_MAX+2]; // plus image & paper
		rect[0] = { pixel_image_left, pixel_image_right, pixel_image_top, pixel_image_bottom };
		for (UINT o = 1; o < pixel_outline_count; o++)
		{

			UINT pixel_outdent =
				pixel_border_depth +
					pixel_outline_depth * o +
						pixel_outline_gap * (o - 1);

			rect[o] = {
					rect[0].left   - pixel_outdent,
					rect[0].right  + pixel_outdent,
					rect[0].top    - pixel_outdent,
					rect[0].bottom + pixel_outdent
			};

		}


	// ---- pm ?
		if ( *paper_measure == 'Y' )
		{

			// outline
			rect[pixel_outline_count++] = { 0, pixel_paper_width-1, 0, pixel_paper_height-1 };

			// 5 mm lines, ...
			UINT yoff = (UINT)(dpmm * 5 + 0.5f);

			// centred +/- 50 mm, at...
			UINT  left = pixel_paper_wby2 - (UINT)(dpmm * 50 + 0.5f);

			// 10 mm intervals
			for (UINT b = 0; b <= 10; b++)
			{
				UINT x = left + b * (UINT)(dpmm * 10 + 0.5f);
				for (UINT y = pixel_paper_height - yoff; y < pixel_paper_height; y++)
				{
					plot(x, y);
					plot(x+1, y);
				}
			}

		}


	// ---- outline
		for (UINT o = 0; o < pixel_outline_count; o++)
		{

			UINT left   = rect[o].left;
			UINT right  = rect[o].right;
			UINT top    = rect[o].top;
			UINT bottom = rect[o].bottom;

			UINT depth = o == 0 ? 0 : pixel_outline_depth;

			// ---- outline top
				for (UINT y = top; y <= top + depth; y++)
				{
					for (UINT x = left; x <= right; x++)
					{
						plot(x, y);
					}
				}

			// ---- outline bottom
				for (UINT y = bottom-depth; y <= bottom; y++)
				{
					for (UINT x = left; x <= right; x++)
					{
						plot(x, y);
					}
				}

			// ---- outline sides
				for (UINT y = top; y <= bottom; y++)
				{
					for (UINT x = left; x <= left+depth; x++) plot(x, y);
					for (UINT x = right-depth; x <= right; x++) plot(x, y);
				}

		}


	// ---- save
		jpeg_lib.WriteToDisk(&img_buffer, _filename);


	// ---- destroy
		img_buffer.DestroyBuffer();

	return VS_OK;
}


// ---------- Report ----------
/*!
\brief Report
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Report()
{

	// ---- properties
		#include "../hpp/mc1_properties_include.hpp"


	// ---- report ?
		if ( *paper_report == 'Y' )
		{

			printf("Report:\n");

			printf("\n");

			printf("Provided by CLA OR \"mcalc.ini\" file:\n");

				printf("   Border:\n");
				printf("      Depth      - %.2f\n",  border_depth);

				printf("   Colour:\n");
				printf("      Value      - %.2f\n",  colour_value);

				printf("   Outline:\n");
				printf("      Count      - %d\n",    outline_count);
				printf("      Depth      - %.2f\n",  outline_depth);
				printf("      Gap        - %.2f\n",  outline_gap);

				printf("   Paper:\n");
				printf("      Size       - %d\n",    paper_size);
				printf("      Width      - %.2f\n", _paper_dimension[paper_size].width);
				printf("      Height     - %.2f\n", _paper_dimension[paper_size].height);

				printf("   Printer:\n");
				printf("      Correction - %.2f\n",  printer_correction);
				printf("      Horizontal - %.2f\n",  printer_horizontal);
				printf("      Vertical   - %.2f\n",  printer_vertical);

			printf("\n");

			printf("Required CLA:\n");

				printf("   Image:\n");
				printf("      Width      - %.2f\n", _image_width);
				printf("      Height     - %.2f\n", _image_height);

			printf("\n");

			printf("Calculated:\n");

				printf("   Image Outline Dimension:\n");
				printf("      Width      - %.2f\n", _total_width_used);
				printf("      Height     - %.2f\n", _total_height_used);

				printf("   Image  Outline Minimax:\n");
				printf("      Left       - %.2f\n", _minimax_left);
				printf("      Right      - %.2f\n", _minimax_right);
				printf("      Top        - %.2f\n", _minimax_top);
				printf("      Bottom     - %.2f\n", _minimax_bottom);

				printf("   Image Dimension Available:\n");
				printf("      Width      - %d.00\n", (INT)_available_width);
				printf("      Height     - %d.00\n", (INT)_available_height);
		}

	return VS_OK;
}


// ---------- Usage ----------
/*!
\brief Usage
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Usage(CHAR mode)
{

	if ( mode == 'u' )
	{

		printf("Usage: mcalc [#.#] [#.#] [...], or\n");
		printf("       mcalc -u, or\n");
		printf("       mcalc -e\n");
		printf("\n");

		printf("Argument 1: \"mcalc\" OR \"mcalc.exe\"\n");
		printf("         2: [image width] e.g. \"100\", \"150\".\n");
		printf("         3: [image height] e.g. \"200\", \"400\".\n");
		printf("\n");

		printf("Command Line Arguments (CLA):\n");
		printf("\n");
		printf("         -bd [#.#]  mm border depth (default %d)\n",            DEFAULT_BORDER_DEPTH);
		printf("         -cv [#.#]  colour value (default %4.2f)\n",     (FLOAT)DEFAULT_PIXEL_VALUE / 255);
		printf("         -e         example (e.g., \"mcalc -e\").\n");
		printf("         -f  [text] output image file (default is \"mcalc.jpg\")\n");
		printf("         -oc [#]    outline count (default %d)\n",              DEFAULT_OUTLINE_COUNT);
		printf("         -od [#.#]  mm outline depth (default %d)\n",           DEFAULT_OUTLINE_DEPTH);
		printf("         -og [#.#]  mm outline gap (default %d)\n",             DEFAULT_OUTLINE_GAP);
		printf("         -ps [#]    international paper size (default %d)\n",   DEFAULT_PAPER_SIZE);
		printf("         -pm [Y/N]  paper measurement (default N(o))\n");
		printf("         -pr [Y/N]  report (default N(o)\n");
		printf("         -pc [#.#]  printer scalar correction (default 1)\n");
		printf("         -ph [#.#]  printer horizontal offset (default 0)\n");
		printf("         -pv [#.#]  printer vertical offset (default 0)\n");
		printf("         -u         usage (e.g., \"mcalc -u\").\n");

		printf("\n");

	}
	else if ( mode == 'e' )
	{

		printf("Command Line Arguments (CLA) Examples:\n");
		printf("mcalc 140 200 ( note: creates image file \"mcalc.jpg\")\n");
		printf("mcalc 100 150 -ps 4 -pr Y -bd 8 -oc 5 -od 0.2 -og 2 -cv 0.75 -f my_a4_mount.jpg\n");
		printf("mcalc 140 280 -ps 3 -bd 8 -oc 5 -od 0.2 -og 2 -cv 0.75 -f my_a3_mount.jpg\n");
		printf("\n");
		printf("      If a CLA is specified then the CLA value is used, and the\n");
		printf("      value in the \"mcalc.ini\" initialisation file updated.\n");
		printf("\n");
		printf("      If a CLA is NOT specified then the CLA value in the\n");
		printf("      \"mcalc.ini\" initialisation file is used.\n");
		printf("\n");
		printf("      A default \"mcalc.ini\" is created if not found.\n");
		printf("\n");
	
	}

	return VS_COMPLETED;
}


// ---------- Setup ----------
/*!
\brief setup configuration data
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::DotIni_Initialise(CHAR *filename)
{

	// ---- file
		#include <../vs_07/vs/vs_system/lambda/vs_auto_folder.h>
		if ( FileExists(filename) )
		{
			dot_ini.Initialise(filename);
		}
		else
		{
			return VS_ERROR;
		}


	// ----- local misc
		using namespace vs_system;
		CHAR text[VS_MAXCHARLEN];
		UINT tsize = VS_MAXCHARLEN;


	// ---- local node
		VNode *root = dot_ini.GetRoot();
		VNode *section = NULL;
		VNode *property = NULL;


	// ---- create
		section = root->Add("Paper");

			snprintf(text, tsize, "%d",    DEFAULT_PAPER_SIZE);
			property = section->Add("Size",    text);

			snprintf(text, tsize, "%c",    DEFAULT_MEASURE ? 'Y' : 'N');
			property = section->Add("Measure", text);

			snprintf(text, tsize, "%c",    DEFAULT_REPORT ? 'Y' : 'N');
			property = section->Add("Report",  text);
			
		section = root->Add("Printer");
			property = section->Add("Correction", "1");
			property = section->Add("Horizontal", "0");
			property = section->Add("Vertical",   "0");

		section = root->Add("Border");

			snprintf(text, tsize, "%4.2f", (FLOAT)DEFAULT_BORDER_DEPTH);
			property = section->Add("Depth",   text);

		section = root->Add("Outline");

			snprintf(text, tsize, "%d",    DEFAULT_OUTLINE_COUNT);
			property = section->Add("Count",   text);

			snprintf(text, tsize, "%4.2f", (FLOAT)DEFAULT_OUTLINE_DEPTH);
			property = section->Add("Depth",   text);
			
			snprintf(text, tsize, "%4.2f", (FLOAT)DEFAULT_OUTLINE_GAP);
			property = section->Add("Gap",     text);

		section = root->Add("Colour");

			snprintf(text, tsize, "%4.2f", (FLOAT)DEFAULT_PIXEL_VALUE / 255);
			property = section->Add("Value",   text);


	return VS_OK;
}


// ---------- Read ----------
/*!
\brief read configuration file
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::DotIni_Read()
{
	INT result = dot_ini.Read();
	return VS_OK;
}


// ---------- ConfigWrite ----------
/*!
\brief write configuration file
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::DotIni_Write()
{
	INT result = dot_ini.Write();
	return VS_OK;
}


