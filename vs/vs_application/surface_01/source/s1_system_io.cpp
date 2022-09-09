////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_io.cpp ----------
/*!
\file s1_system_io.cpp
\brief Implementation of the SystemIO class
\author Gareth Edwards
*/

#include "../header/s1_system_io.h"

using namespace surface_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ConfigFiles class ----------

class SystemIO::PI_SystemIO
{

public:

	// ---- ctor
		PI_SystemIO();
		~PI_SystemIO();

	// ---- methods
		INT WriteLine(std::ofstream *, UINT);
		INT Write(std::ofstream *, SystemObject *);
		
	// ---- filenames
		CHAR _solar_system_csv_filename[VS_MAXCHARPATHLEN];

	// ---- note
		CHAR  _note_line[VS_MAXCHARLINELEN];

	// ---- key descriptor names
		INT   _desc_count = 0;
		CHAR *_desc[128];
		CHAR  _desc_line[VS_MAXCHARLINELEN];

	// ---- key names
		INT   _key_count = 0;
		CHAR *_key[128];
		CHAR  _key_line[VS_MAXCHARLINELEN];

	// ---- parent root object
		SystemObject * _system = NULL;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemIO::PI_SystemIO::PI_SystemIO()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
SystemIO::PI_SystemIO::~PI_SystemIO()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemIO::SystemIO()
{
	pi_system_io = new PI_SystemIO();

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
SystemIO::~SystemIO()
{
	if ( pi_system_io != NULL )
	{
		delete pi_system_io;
		pi_system_io = NULL;
	}
}


// ---------- Read ----------
/*!
\brief Read
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::Read(SystemObject *system, CHAR *filename)
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
							if (c < 0)
							{
								float fred = 2;
							}
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
		auto add = [&](INT name_count, CHAR *key[], CHAR *data[])
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

					if ( parent == NULL )
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
					object->Set(key[i], data[i]);
				}
		};


	// ---- local
		INT result = 0;


	// ---- open file ?
		std::ifstream myfile(filename);
		if ( myfile.is_open() )
		{

			// ---- store
				strcpy(pi_system_io->_solar_system_csv_filename, filename);

			// ---- data
				INT   data_count = 0;
				CHAR *data[128];
				CHAR  data_line[VS_MAXCHARLINELEN];

			// ---- line usage:
				// 0  : comments
				// 1  : key notes
				// 2  : key
				// 3+ : data
				INT lc = 0;
				while ( !myfile.getline(data_line, VS_MAXCHARLINELEN).eof() )
				{
					switch (lc)
					{
						case 0:
							strcpy(pi_system_io->_note_line,data_line);
							break;
						case 1:
							strcpy(pi_system_io->_desc_line, data_line);
							pi_system_io->_desc_count = split(pi_system_io->_desc_line, pi_system_io->_desc);
							break;
						case 2:
							strcpy(pi_system_io->_key_line,data_line);
							pi_system_io->_key_count = split(pi_system_io->_key_line, pi_system_io->_key);
							break;
						default:
							{
								INT data_count = split(data_line, data);
								add(
										pi_system_io->_key_count,
										pi_system_io->_key,
										data
									);
							}
							break;
					}
					lc++;
				}

			// ---- close (note: ifstream could do this out of scope)
				myfile.close();

			// --- success, so store...
				pi_system_io->_system = system;
			
			// ---- yay
				return VS_OK;

		}

	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::Write()
{

	// ---- local
		INT result = VS_OK;

	// ---- open file ?
		std::ofstream myfile("tmp.csv");
		if ( myfile.is_open() )
		{

			// ---- note, descriptors and keys
				for (INT i = 0; i < 3; i++)
				{
					result = pi_system_io->WriteLine(&myfile, i);
				}

			// ---- objects
				pi_system_io->Write(&myfile, pi_system_io->_system);


			// ---- close (note: ofstream could do this out of scope)
				myfile.close();

			return VS_OK;
		}

	return VS_ERROR;
}


// ---------- WriteLine ----------
/*!
\brief WriteLine
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::PI_SystemIO::WriteLine(std::ofstream *myfile, UINT line)
{

	switch (line)
	{
		case 0:
			myfile->write(_note_line, strlen(_note_line));
			OutputDebugString(_note_line);
			for (INT i = 1; i < _key_count; i++)
			{
				myfile->write(",", 1);
				OutputDebugString(",");
			}
			break;
		case 1:
		case 2:
			for (INT i = 0; i < _key_count; i++)
			{
				myfile->write(line == 1 ? _desc[i] : _key[i], strlen(line == 1 ? _desc[i] : _key[i]));
				OutputDebugString(line == 1 ? _desc[i] : _key[i]);
				myfile->write(",", 1);
				OutputDebugString(",");
			}
			break;
		default:
			break;
	}
	myfile->write("\n", 1);
	OutputDebugString("\n");

	return VS_OK;
}


// ---------- Write ----------
/*!
\brief Write
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::PI_SystemIO::Write(std::ofstream *myfile, SystemObject *parent)
{

	// ---- thx teppic on stack overflow
		auto convert = [](char *buf, float x)
		{
			char *p;
			sprintf(buf, "%.6f", x);
			p = buf + strlen(buf) - 1;
			while (*p == '0' && *p-- != '.');
				*(p + 1) = '\0';
			if (*p == '.') *p = '\0';
		};

	// ---- text data
		CHAR *type = parent->GetType((INT)parent->Get("Type"));
			myfile->write(type, strlen(type));
			OutputDebugString(type);
			myfile->write(",", 1);
			OutputDebugString(",");

		CHAR *name = parent->GetName();
			myfile->write(name, strlen(name));
			OutputDebugString(name);
			myfile->write(",", 1);
			OutputDebugString(",");

		CHAR *group = parent->GetGroup();
			myfile->write(group, strlen(group));
			OutputDebugString(group);
			myfile->write(",", 1);
			OutputDebugString(",");

	// ---- # data ( to SIX decimal places )
		CHAR buf[VS_MAXCHARLEN];
		INT last = _key_count - 1;
		for (INT i = 3; i < _key_count; i++)
		{
			convert(buf, parent->Get(_key[i]));
			myfile->write(buf, strlen(buf));
				OutputDebugString(buf);
			myfile->write(i == last ? "\n" : ",", 1);
				OutputDebugString(i == last ? "\n" : ",");
		}

	// ---- children
		SystemObject *child = parent->GetFirst();
		while (child)
		{
			INT result = Write(myfile, child);
			child = child->GetNext();
		}

	return VS_OK;
}


// ---------- GetKeyCount ----------
/*!
\brief GetKeyCount
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT SystemIO::GetKeyCount()
{
	return pi_system_io->_key_count;
}


// ---------- GetKey ----------
/*!
\brief GetKey
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
CHAR * SystemIO::GetKey(INT index, CHAR *key)
{
	if ( index >= 0 && index < pi_system_io->_key_count )
	{
		return pi_system_io->_key[index];
	}
	return NULL;
}


// ---------- GetDescriptor ----------
/*!
\brief GetDescriptor
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
CHAR * SystemIO::GetDescriptor(INT index, CHAR *key)
{
	if ( index >= 0 && index < pi_system_io->_key_count )
	{
		return pi_system_io->_desc[index];
	}
	return NULL;
}


// ---------- GetKeyDescriptor ----------
/*!
\brief GetKeyDescriptor
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
CHAR * SystemIO::GetKeyDescriptor(CHAR *key)
{
	INT key_count = pi_system_io->_key_count;
	for (INT i = 0; i < key_count; i++)
	{
		if (*pi_system_io->_key[i] == key[0])
		{
			if (!strcmp(pi_system_io->_key[i], key))
			{
				return pi_system_io->_key[i];
			}
		}
	}
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////
