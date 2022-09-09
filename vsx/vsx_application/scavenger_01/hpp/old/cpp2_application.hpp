////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_application.hpp ----------
/*!
\file cpp2_application.hpp
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

using namespace cpp_02;


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

public:

	// ---- c/dtor
		PI_Application();
		~PI_Application();


	// ---- methods
		INT Setup(vs_system::AppDependency *);
		INT Usage(CHAR);


	//---- pointers
		vs_system::AppDependency *app_dependency;
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- argument count and values
		CHAR argv[VS_MAXARGV][VS_MAXCHARLEN];
		UINT argc = 0;


	// ---- version stuff
		CHAR *vs_sys_build = NULL;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{

	// ---- application dependency pointers
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

	// ---- inject application dependency
		app_dependency = ad;


	// ---- accelerate
		sys_context = ad->GetSysContext();
		sys_output = ad->GetSysOutput();


	/*

	// ---- catch overflow
		argc = app_dependency->GetArgc();
		if (argc >= VS_MAXARGV)
		{
			printf("Error: Exceeded maximum [%d] # of command line arguments.\n", VS_MAXARGV);
			printf("\n");
			return VS_ABORT;
		}


	// ---- development
		//#define TEST

		#ifdef TEST
		argc = 3;
		strcpy(argv[0], "set_version");
		strcpy(argv[1], "-b");
		strcpy(argv[2], "1422");
		#else
		for ( UINT i = 0; i < argc; i++ )
		{
			strcpy(argv[i], app_dependency->GetArgv(i));
		}
		#endif


	// ---- options

		UINT arg = 1;
		while ( arg < argc )
		{
			if ( !strcmp("-b", argv[arg] ))
			{
				if ( arg + 1 < argc )
				{
					vs_sys_build = argv[arg + 1];
					arg += 2;
				}
			}
			else
			{
				printf("Error: Unrecognised command line option %s.\n", argv[arg]);
				printf("\n");
				Usage('m');
				return VS_ABORT;
			}
		}


		if ( vs_sys_build == NULL )
		{
			printf("Error: No command line options.\n");
			printf("\n");
			Usage('m');
			return VS_ABORT;
		}

		*/

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

	printf("Usage: app_version [app prefix] [...]\n");
	printf("\n");

	printf("Argument 1: \"app_version\" OR \"app_version.exe\"\n");
	printf("\n");

	printf("Command line options:\n");
	printf("\n");
	printf("         -b [version #] initialise the application BUILD version #.\n");

	printf("\n");

	return VS_COMPLETED;
}

