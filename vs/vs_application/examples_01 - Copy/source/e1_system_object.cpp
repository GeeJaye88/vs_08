////////////////////////////////////////////////////////////////////////////////

// ---------- e1_system_object.cpp ----------
/*!
\file e1_system_object.cpp
\brief Implementation of the SystemObject class
\author Gareth Edwards
*/

#include "../header/e1_system_object.h"

using namespace examples_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation of Portal class ----------
class SystemObject::PI_SystemObject
{

public:

	// ---- cdtor
		PI_SystemObject();
		~PI_SystemObject();


	// ---- properties
		CHAR  name[32];
		CHAR  group[32];
		UINT  type                 = Type::UnKnown;
		FLOAT mass                 = 0;
		FLOAT diameter             = 0;
		FLOAT density              = 0;
		FLOAT gravity              = 0;
		FLOAT escape_velocity      = 0;
		FLOAT rotation_period      = 0;
		FLOAT day_length           = 0;
		FLOAT distance             = 0;
		FLOAT radius_min           = 0;
		FLOAT radius_max           = 1;
		FLOAT perihelion           = 0;
		FLOAT aphelion             = 0;
		FLOAT orbital_period       = 0;
		FLOAT orbital_velocity     = 0;
		FLOAT orbital_inclination  = 0;
		FLOAT orbital_ecentricity  = 0;
		FLOAT orbital_obliquity    = 0;
		FLOAT temperature          = 0;
		FLOAT surface_pressure     = 0;
		UINT  moons                = 0;
		BOOL  ring_system          = FALSE;
		BOOL  magnetic_field       = FALSE;


	// ---- default graphical properties
		FLOAT gfx_diameter         = 0;
		FLOAT gfx_day_length       = 0;
		FLOAT gfx_distance         = 0;
		FLOAT gfx_radius_min       = 0;
		FLOAT gfx_radius_max       = 1;
		FLOAT gfx_orbital_period   = 0;
		FLOAT gfx_red              = 1;
		FLOAT gfx_green            = 1;
		FLOAT gfx_blue             = 1;
		UINT  gfx_page             = 0;
		UINT  gfx_section          = 1;
		

	// ---- derived
		FLOAT day_length_start         = 0;
		FLOAT orbital_period_start     = 0;
		FLOAT gfx_day_length_start     = 0;
		FLOAT gfx_orbital_period_start = 0;


	// ---- assigned
		UINT  index = 0;


	// ---- link
		SystemObject *parent   = NULL;
		SystemObject *previous = NULL;
		SystemObject *next     = NULL;
		SystemObject *first    = NULL;
		SystemObject *last     = NULL;

};


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemObject::PI_SystemObject::PI_SystemObject()
{
	;
}


// ---------- dtor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemObject::PI_SystemObject::~PI_SystemObject()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemObject::SystemObject()
{
	pi_system_object = new PI_SystemObject();
	strcpy(pi_system_object->name, "System Object");
	strcpy(pi_system_object->group, "None");
}


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemObject::SystemObject(
		SystemObject *parent,
		CHAR *name
	)
{

	pi_system_object = new PI_SystemObject();

	if ( parent == NULL )
	{
		;
	}
	else
	{

		PI_SystemObject *parent_pi = parent->pi_system_object;
		pi_system_object->parent = parent;
		if ( parent_pi->first != NULL )
		{
			parent_pi->last->pi_system_object->next = this;
			pi_system_object->previous = parent_pi->last;
			parent_pi->last = this;
		}
		else
		{
			parent_pi->first = this;
			parent_pi->last = this;
		}

	}

	strcpy(pi_system_object->name, name == 0 ? "No Name" : name);
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
SystemObject::~SystemObject()
{
	delete pi_system_object;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemObject::Setup()
{

	// ---- local
		INT result = 0;


	return VS_OK;
}


// ---------- Link ----------


SystemObject* SystemObject::GetParent() { return pi_system_object->parent; }
SystemObject* SystemObject::GetFirst()  { return pi_system_object->first;  }
SystemObject* SystemObject::GetNext()   { return pi_system_object->next;   }


// ---------- GetIndex ----------
/*!
\brief GetIndex
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
UINT SystemObject::GetIndex()
{
	return pi_system_object->index;
}


// ---------- GetGroup ----------
/*!
\brief GetGroup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
CHAR *SystemObject::GetGroup()
{
	return pi_system_object->group;
}


// ---------- GetName ----------
/*!
\brief GetName
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
CHAR *SystemObject::GetName()
{
	return pi_system_object->name;
}


// ---------- Get ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
FLOAT SystemObject::Get(CHAR *name)
{

	switch (name[0])
	{
		case 'A':
			return pi_system_object->aphelion;
			break;
		case 'D':
			if      (!strcmp(name, "Diam")   ) return pi_system_object->diameter;
			else if (!strcmp(name, "Dens")   ) return pi_system_object->density;
			else if (!strcmp(name, "DayLen") ) return pi_system_object->day_length;
			else if (!strcmp(name, "DaySta") ) return pi_system_object->day_length_start;
			else if (!strcmp(name, "Dist")   ) return pi_system_object->distance;
			break;
		case 'E':
			if      (!strcmp(name, "EscVel") ) return pi_system_object->escape_velocity;
			break;
		case 'G':
			if      (!strcmp(name, "Grav")   ) return pi_system_object->gravity;

			else if (!strcmp(name, "GfxDia")    ) return pi_system_object->gfx_diameter;
			else if (!strcmp(name, "GfxDayLen") ) return pi_system_object->gfx_day_length;
			else if (!strcmp(name, "GfxDaySta") ) return pi_system_object->gfx_day_length_start;
			else if (!strcmp(name, "GfxDis")    ) return pi_system_object->gfx_distance;
			else if (!strcmp(name, "GfxRadMin") ) return pi_system_object->gfx_radius_min;
			else if (!strcmp(name, "GfxRadMax") ) return pi_system_object->gfx_radius_max;
			else if (!strcmp(name, "GfxOrbPer") ) return pi_system_object->gfx_orbital_period;
			else if (!strcmp(name, "GfxOrbSta") ) return pi_system_object->gfx_orbital_period_start;
			else if (!strcmp(name, "GfxRed")    ) return pi_system_object->gfx_red;
			else if (!strcmp(name, "GfxGre")    ) return pi_system_object->gfx_green;
			else if (!strcmp(name, "GfxBlu")    ) return pi_system_object->gfx_blue;
			else if (!strcmp(name, "GfxPag")    ) return (FLOAT)pi_system_object->gfx_page;
			else if (!strcmp(name, "GfxSec")    ) return (FLOAT)pi_system_object->gfx_section;

			break;
		case 'M':
			if      (!strcmp(name, "MagFie") ) return pi_system_object->magnetic_field == TRUE ? 1.0f : 0.0f;
			else if (!strcmp(name, "Mass")   ) return pi_system_object->mass;
			else if (!strcmp(name, "Moon")   ) return (FLOAT)pi_system_object->moons;
			break;
		case 'N':
			break;
		case 'O':
			if      (!strcmp(name, "OrbEcc") ) return pi_system_object->orbital_ecentricity;
			else if (!strcmp(name, "OrbInc") ) return pi_system_object->orbital_inclination;
			else if (!strcmp(name, "OrbObl") ) return pi_system_object->orbital_obliquity;
			else if (!strcmp(name, "OrbPer") ) return pi_system_object->orbital_period;
			else if (!strcmp(name, "OrbSta") ) return pi_system_object->orbital_period_start;
			else if (!strcmp(name, "OrbVel") ) return pi_system_object->orbital_velocity;
			break;
		case 'P':
			if      (!strcmp(name, "Peri")   ) return pi_system_object->perihelion;
			break;
		case 'R':
			if      (!strcmp(name, "Ring")   ) return pi_system_object->ring_system == TRUE ? 1.0f : 0.0f;
			else if (!strcmp(name, "RadMin") ) return pi_system_object->radius_min;
			else if (!strcmp(name, "RadMax") ) return pi_system_object->radius_max;
			else if (!strcmp(name, "RotPer") ) return pi_system_object->rotation_period;
			break;
		case 'S':
			if      (!strcmp(name, "SurPre") ) return  pi_system_object->surface_pressure ;
			break;
		case 'T':
			if      (!strcmp(name, "Type")   ) return (FLOAT)pi_system_object->type;
			else if (!strcmp(name, "Temp")   ) return pi_system_object->temperature;

			break;
	}

	return VS_ERROR;
}


// ---------- Set ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemObject::Set(CHAR *name, FLOAT data)
{
	CHAR c[VS_MAXCHARLEN];
	sprintf(c, "%f", data);
	INT result = Set(name, c);
	return result;
}


// ---------- Set ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemObject::Set(CHAR *name, CHAR *data)
{

	switch (name[0])
	{
		case 'A':
			if      (!strcmp(name, "Ap")     ) pi_system_object->aphelion   = (FLOAT)atof(data);
			break;

		case 'D':
			if      (!strcmp(name, "Diam")   ) pi_system_object->diameter   = (FLOAT)atof(data);
			else if (!strcmp(name, "Dens")   ) pi_system_object->density    = (FLOAT)atof(data);
			else if (!strcmp(name, "DayLen") )
				{

					pi_system_object->day_length = (FLOAT)atof(data);

					INT mod_val = (INT)
									(
										fabs(pi_system_object->day_length) < 0.00001 ?
											1 : fabs(pi_system_object->day_length)
									);

					pi_system_object->day_length_start =
									(
										(FLOAT)(rand() % mod_val) / (FLOAT)mod_val * 
											pi_system_object->day_length
									);

				}
			else if (!strcmp(name, "Dist")   ) pi_system_object->distance   = (FLOAT)atof(data);
			break;

		case 'E':
			if      (!strcmp(name, "EscVel") ) pi_system_object->escape_velocity = (FLOAT)atof(data);
			break;

		case 'G':
			if      (!strcmp(name, "Grav")   ) pi_system_object->gravity         = (FLOAT)atof(data);

			else if (!strcmp(name, "GfxDia")    ) pi_system_object->gfx_diameter = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxDayLen"))
				{

					pi_system_object->gfx_day_length = (FLOAT)atof(data);

					INT mod_val = (INT)
									(
										fabs(pi_system_object->gfx_day_length) < 0.00001 ?
											1 : fabs(pi_system_object->gfx_day_length)
									);

					pi_system_object->gfx_day_length_start =
									(
										(FLOAT)(rand() % mod_val) / (FLOAT)mod_val * 
											pi_system_object->gfx_day_length
									);

				}
			else if (!strcmp(name, "GfxDis")   ) pi_system_object->gfx_distance   = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxRadMin")) pi_system_object->gfx_radius_min = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxRadMax")) pi_system_object->gfx_radius_max = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxOrbPer"))
				{

					pi_system_object->gfx_orbital_period = (FLOAT)atof(data);

					INT mod_val = (INT)
									(
										fabs(pi_system_object->gfx_orbital_period) < 0.00001 ?
											1 : fabs(pi_system_object->gfx_orbital_period)
									);

					pi_system_object->gfx_orbital_period_start = pi_system_object->orbital_period_start;

				}
			else if (!strcmp(name, "GfxRed")    ) pi_system_object->gfx_red          = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxGre")    ) pi_system_object->gfx_green        = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxBlu")    ) pi_system_object->gfx_blue         = (FLOAT)atof(data);
			else if (!strcmp(name, "GfxPag")    ) pi_system_object->gfx_page         = (UINT)atoi(data);
			else if (!strcmp(name, "GfxSec")    ) pi_system_object->gfx_section      = (UINT)atoi(data);
			break;

		case 'M':
			if      (!strcmp(name, "MagFie") ) pi_system_object->magnetic_field = data[0] == 'N' ? FALSE : TRUE;
			else if (!strcmp(name, "Mass")   ) pi_system_object->mass  = (FLOAT)atof(data);
			else if (!strcmp(name, "Moon")   ) pi_system_object->moons = (UINT)atoi(data);
			break;

		case 'N':
			if      (!strcmp(name, "Name")   ) strcpy(pi_system_object->name,data);
			break;

		case 'O':
			if      (!strcmp(name, "OrbEcc") ) pi_system_object->orbital_ecentricity = (FLOAT)atof(data);
			else if (!strcmp(name, "OrbInc") ) pi_system_object->orbital_inclination = (FLOAT)atof(data);
			else if (!strcmp(name, "OrbObl") ) pi_system_object->orbital_obliquity   = (FLOAT)atof(data);
			else if (!strcmp(name, "OrbPer") )
				{

					pi_system_object->orbital_period = (FLOAT)atof(data);

					INT mod_val = (INT)
									(
										fabs(pi_system_object->orbital_period) < 0.00001 ?
											1 : fabs(pi_system_object->orbital_period)
									);

					pi_system_object->orbital_period_start =
									(
										(FLOAT)(rand() % mod_val) / (FLOAT)mod_val * 
											pi_system_object->orbital_period
									);

				}
			else if (!strcmp(name, "OrbVel") ) pi_system_object->orbital_velocity    = (FLOAT)atof(data);
			break;

		case 'P':
			if      (!strcmp(name, "Peri")   ) pi_system_object->perihelion          = (FLOAT)atof(data);
			else if (!strcmp(name, "Grou")   ) strcpy(pi_system_object->group, data);
			break;

		case 'R':
			if      (!strcmp(name, "Ring")   ) pi_system_object->ring_system = data[0] == 'N' ? FALSE : TRUE;
			else if (!strcmp(name, "RotPer") ) pi_system_object->rotation_period     = (FLOAT)atof(data);
			else if (!strcmp(name, "RadMin") ) pi_system_object->radius_min          = (FLOAT)atof(data);
			else if (!strcmp(name, "RadMax") ) pi_system_object->radius_max          = (FLOAT)atof(data);
			break;

		case 'S':
			if      (!strcmp(name, "SurPre") ) pi_system_object->surface_pressure    = (FLOAT)atof(data);
			break;

		case 'T':
			if (!strcmp(name, "Type"))
			{
				if      (!strcmp(data, "UnKnown") ) pi_system_object->type = Type::UnKnown;
				else if (!strcmp(data, "Star")    ) pi_system_object->type = Type::Star;
				else if (!strcmp(data, "Planet")  ) pi_system_object->type = Type::Planet;
				else if (!strcmp(data, "GasGiant")) pi_system_object->type = Type::GasGiant;
				else if (!strcmp(data, "Dwarf")   ) pi_system_object->type = Type::Dwarf;
				else if (!strcmp(data, "Moon")    ) pi_system_object->type = Type::Moon;
				else if (!strcmp(data, "Asteroid")) pi_system_object->type = Type::Asteroid;
				else if (!strcmp(data, "AstRing") ) pi_system_object->type = Type::AstRing;
				else if (!strcmp(data, "Meteroid")) pi_system_object->type = Type::Meteroid;
			}
			else if (!strcmp(name, "Temp"))
			{
				pi_system_object->temperature = (FLOAT)atof(data);
			}
			break;
	}

	return VS_OK;
}


// ---------- SetIndex ----------
/*!
\brief SetIndex
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemObject::SetIndex(INT index)
{
	pi_system_object->index = index;
	return VS_OK;
}


// ---------- Search ----------
/*!
\brief Search
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
SystemObject * SystemObject::Search(SystemObject *sys_obj, CHAR *search_name)
{
	CHAR *name = sys_obj->GetName();

	if (!strcmp(name, search_name)) return sys_obj;

	SystemObject *s = sys_obj->GetFirst();
	while (s)
	{
		SystemObject *result = Search(s, search_name);
		if (result != NULL) return result;
		s = s->GetNext();
	}

	return NULL;
};



////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation of System Paremeters class ----------
class SystemParam::PI_SystemParam
{

public:

	// ---- cdtor
		PI_SystemParam();
		~PI_SystemParam();


	// ---- scaling
		DOUBLE scale_global = 0.00001f;
		DOUBLE scale_object = 1;


	// ---- gfx
		UINT gfx_mode = SystemObject::ILLUSTRATION;


	// --- pause
		INT    pause_mode          = 0;
		DOUBLE paused_start_time   = 0;
		DOUBLE paused_elapsed_time = 0;


	// ---- moons, asteroids, meteoroids, etc.
		DOUBLE object_scale    = 1;


	// ---- moons with parent
		DOUBLE orbital_scalar  = 1;


	// ---- to speed everything up (yawn!!!)
		DOUBLE time_scalar   = 1;
		DOUBLE daylen_scalar = 1;


	// ---- time
		LONG   ms_offset = 0;

};


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::PI_SystemParam::PI_SystemParam()
{
	;
}


// ---------- dtor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::PI_SystemParam::~PI_SystemParam()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::SystemParam()
{
	pi_system_param = new PI_SystemParam();
}


// ---------- dtor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::~SystemParam()
{
	if ( pi_system_param != NULL ) { delete pi_system_param;  pi_system_param = NULL; }
}


// ---------- mode ----------
	UINT SystemParam::GetGfxMode()           { return pi_system_param->gfx_mode; }
	VOID SystemParam::SetGfxMode(UINT gm)    { pi_system_param->gfx_mode = gm;   }

	UINT SystemParam::IncGfxMode()
	{
		pi_system_param->gfx_mode += 1;
		pi_system_param->gfx_mode =
			pi_system_param->gfx_mode > SystemObject::ILLUSTRATION ?
				SystemObject::NORMAL :
					pi_system_param->gfx_mode;
		return pi_system_param->gfx_mode;
	}


// ---------- mode ----------
	INT    SystemParam::GetPauseMode()                  { return pi_system_param->pause_mode; }
	VOID   SystemParam::SetPauseMode(INT pm)            { pi_system_param->pause_mode = pm;   }
	DOUBLE SystemParam::GetPauseTimeStart()             { return pi_system_param->paused_start_time;   }
	VOID   SystemParam::SetPauseTimeStart(DOUBLE pst)   { pi_system_param->paused_start_time = pst;    }
	DOUBLE SystemParam::GetPauseTimeElapsed()           { return pi_system_param->paused_elapsed_time; }
	VOID   SystemParam::SetPauseTimeElapsed(DOUBLE pet) { pi_system_param->paused_elapsed_time = pet;  }


// ---------- get ----------
	DOUBLE SystemParam::GetGlobalScale()     { return pi_system_param->scale_global;    }
	DOUBLE SystemParam::GetObjectScale()     { return pi_system_param->scale_object;    }
	DOUBLE SystemParam::GetTimeScalar()      { return pi_system_param->time_scalar;     }
	DOUBLE SystemParam::GetDaylenScalar()    { return pi_system_param->daylen_scalar;   }
	LONG   SystemParam::GetMsOffset()        { return pi_system_param->ms_offset;       }
	FLOAT  SystemParam::GetTargetDistScalar()  { return (FLOAT)pi_system_param->scale_object * 0.1f; }

// ---------- set ----------
	INT SystemParam::SetScale(DOUBLE s)           { pi_system_param->scale_global    = s; return VS_OK; }
	INT SystemParam::SetObjectScale(DOUBLE s)     { pi_system_param->scale_object    = s; return VS_OK; }

	INT SystemParam::SetTimeScalar(DOUBLE s)      { pi_system_param->time_scalar     = s; return VS_OK; }
	INT SystemParam::SetDaylenScalar(DOUBLE s)    { pi_system_param->daylen_scalar   = s; return VS_OK; }
	INT SystemParam::SetMsOffset(LONG l)          { pi_system_param->ms_offset       = l; return VS_OK; }


////////////////////////////////////////////////////////////////////////////////


// ---------- Read ----------
/*!
\brief Read
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::Read(SystemObject *system)
{

	// ---- lambda: line into word split
		auto split = [&](CHAR *line, CHAR *word[])
		{

			// ---- local
				INT windex = 0;
				CHAR *word_start = line;

			// ----split
				CHAR c;
				INT cindex = 0;
				while ( (c = line[cindex]) != 0 )
				{
					switch (c)
					{
						case ' ':
							break;
						case ',':
							{
								word[windex++] = word_start;
								line[cindex] = '\0';
								word_start = &line[cindex + 1];
							}
							break;
						default:
							break;
					}
					cindex++;
				}

			// ---- last word
				word[windex++] = word_start;

			// ---- return word count
				return windex;
		};


	// ---- lambda: add system object
		auto add = [&](INT name_count, CHAR *name[], CHAR *data[])
		{

			// ---- So:
				//   data[0] is type
				//   data[1] is name
				//   data[2] is group name


			// ---- search for parent group
				SystemObject *parent = system;
				if ( strcmp("System", data[2]) )
				{
					parent = system->Search(system, data[2]);

					if (parent == NULL)
					{
						OutputDebugString("+++ ERROR: parent group not found!!\n");
						parent = system;
					}
					else
					{
						CHAR msg[128];
						sprintf(msg, "+++ Found %s by search for %s \n", parent->GetName(), data[2]);
						OutputDebugString(msg);
					}
				}


			// ---- add
				SystemObject *object = new SystemObject(parent, data[1]);


			// ---- initialise
				for (INT i = 0; i < name_count; i++)
				{
					object->Set(name[i], data[i]);
				}
		};


	// ---- debug or release path/filename ?
		CHAR *filename = "data/examples_01/csv/solar_system.csv";
		if (IsDebuggerPresent())
		{
			filename = "../data/examples_01/csv/solar_system.csv";
		}


	// ---- local
		INT result = 0;


	// ---- open file ?
		std::ifstream myfile(filename);
		if (myfile.is_open())
		{

			// ---- key names
				INT   key_count = 0;
				CHAR *key[128];
				CHAR  key_line[4096];

				INT   data_count = 0;
				CHAR *data[128];
				CHAR  data_line[4096];

			// ---- line usage:
				// 0  : comments
				// 1  : key notes
				// 2  : key
				// 3+ : data
				INT lc = 0;
				while (!myfile.getline(lc > 2 ? data_line : key_line, 4096).eof())
				{
					if ( lc > 2 )
					{
						INT data_count = split(data_line, data);
						add(key_count, key, data);
					}
					else
					{
						key_count = split(key_line, key);
					}
					lc++;
				}

			// ---- close
				myfile.close();

		}

	return VS_OK;
}
