////////////////////////////////////////////////////////////////////////////////

// ---------- vs_dot_ini.cpp ----------
/*!
\file vs_dot_ini.cpp
\brief Implementation of the DotIni class
\author Gareth Edwards
\note Vanilla C++
*/


// ---------- include
	#include <string>
	#include <iostream>


// ---------- vs include
	#include "../header/vs_dot_ini.h"


// ---------- namespace ----------
using namespace vs_system;


// ---------- Private Implementation of PI_Application class ----------
class DotIni::PI_DotIni
{

	public:

		// ---- constructor/destructor
			PI_DotIni() { };
			~PI_DotIni() { };

		// ---- data
			CHAR  filename[VS_MAXCHARLEN] = "";
			VNode *root = NULL;
};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
DotIni::DotIni()
{
	pi_dotini = new PI_DotIni();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
DotIni::~DotIni()
{
	delete pi_dotini;
}


// ---------- Create ----------
/*!
\brief create dot ini root
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT DotIni::Initialise(
		CHAR *filename
	)
{

	pi_dotini->root = new VNode("DotIni");
	if ( pi_dotini->root == NULL ) return VS_ERROR;

	INT len = (INT)strlen(filename);
	if ( len < 1 ) return VS_ERROR;

	strcpy_s(pi_dotini->filename, VS_MAXCHARLEN, filename);

	return VS_OK;
}


// ---------- Read ----------
/*!
\brief read [name.ini] file
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

	if ( lc != ' ' && c != ' ' )
	{
		line[dindex++] = c;
	}
	else if ( lc != ' ' && c == ' ' )
	{
		line[dindex++] = c;
	}
	else if ( c != ' ')
	{
		line[dindex++] = c;
	}

	if ( lc != ' ' )
	{
		line[dindex++] = c;
	}
	else if ( c != ' ')
	{
		line[dindex++] = c;
	}


*/
INT DotIni::Read()
{

	// ---- need StrSplitIntoWords
		#include <../vs_07/vs/vs_system/lambda/vs_auto_str.h>


	// ---- lambda: line into word split
		auto split = [&](CHAR *line, CHAR *word[])
		{

			// ---- local
				INT windex = 0;
				CHAR *word_start = line;
				CHAR c = 0;
				

			// ---- remove leading, trailing and double spaces...
				INT cindex = 0;
				INT dindex = 0;
				CHAR lc = line[0];
				while ( (c = line[cindex]) != 0 )
				{
					if ( lc != ' ' || c != ' ' )
					{
						line[dindex++] = c;
					}
					lc = c;
					cindex++;
				}
				line[dindex] = '\0';


			// ----split
				cindex = 0;
				while ( (c = line[cindex]) != 0 )
				{
					switch (c)
					{
						case ' ':
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

		INT   data_count = 0;
		CHAR *data[128];
		CHAR *data_line[5] =
		{
			"   fred  is  a  bad  boy   ",
			"B A C",
			"A",
			"B A C",
			" bad boy"
		};
		for (UINT i = 0; i < 5; i++)
		{
			CHAR text[256];
			strcpy(text, data_line[i]);
			INT data_count = split(text, data);
			float fred = 2;
		}


	// ---- root ?
		if ( pi_dotini->root == NULL ) return VS_ERROR;


	// ---- open file ?
		BOOL test = FALSE;
		std::ifstream myfile(pi_dotini->filename);
		if ( myfile.is_open() )
		{

			if ( test) printf(" +++ Reading : %s \n", pi_dotini->filename);

			// ---- node
				VNode *sect = NULL;
				VNode *prop = NULL;


			// ---- local
				INT   data_count = 0;
				CHAR *data[128];
				CHAR  data_line[VS_MAXCHARLINELEN];

				INT line_count = 0;
				while ( !myfile.getline(data_line, VS_MAXCHARLINELEN).eof() )
				{

					INT data_count = split(data_line, data);
					if (test) printf(" +++ %s [%d]\n", data_line, data_count);

					// ---- section ?
						if ( data_count == 1 )
						{

								size_t len = strlen(data[0]);
								if (
										*(data[0]) == '[' &&
										*(data[0] + len - 1) == ']'
									)
								{
										if ( len > 3 )
										{
												data[0]++;
												*(data[0] + len - 2) = 0;
												sect = pi_dotini->root->SearchName(data[0]);
												if ( sect != NULL )
												{
													OutputDebugString("Section : ");
													OutputDebugString(sect->GetName());
													OutputDebugString("\n");
													if (test) printf(" +++ %s \n", sect->GetName());
												}
												else
												{
													if (test) printf(" +++ section not found \n");
													return VS_ERROR; // section not found
												}
										}
										else
										{
											if (test) printf(" +++ section illegal length \n");
											return VS_ERROR; // section illegal length
										}
								}
								else
								{
									if ( !strcmp(data[0], "") )
									{
										; // ok, empty line
									}
									else
									{
										if (test) printf(" +++ section  badly formed \n");
										return VS_ERROR; // section badly formed
									}
								}

						}

					// ---- property
						else if ( data_count == 2 )
						{
							if ( sect == NULL )
							{
								return VS_ERROR; // no section
							}
							prop = sect->SearchName(data[0]);
							if ( prop != NULL )
							{
								OutputDebugString("Property : ");
								OutputDebugString(prop->GetName());
								OutputDebugString("\n");
								prop->SetValue(data[1]);
								if (test) printf(" +++ %s %s\n",prop->GetName(), prop->GetValue());
							}
							else
							{
								return VS_ERROR; // property not found
							}
						}

					// ---- what ?
						else
						{
							return VS_ERROR; // line badly formed
						}

				}

			// ---- close (note: ifstream could do this out of scope)
				myfile.close();

			// ---- yay
				return VS_OK;

		}

	// ---- baaaaaad
		return VS_ERROR;

}


// ---------- Write ----------
/*!
\brief write [name.ini] file
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT DotIni::Write()
{

	// ---- root ?
		if ( pi_dotini->root == NULL ) return VS_ERROR;


	// ---- open file ?
		std::ofstream myfile(pi_dotini->filename);
		if ( myfile.is_open() )
		{

			// ---- write ?
				CHAR  msg[VS_MAXCHARLEN];
				CHAR *name = "", *value = "";
				VNode *prop = NULL, *sect = pi_dotini->root->GetFirst();
				while ( sect )
				{

					if ( strcmp(name, "") != 0)
						myfile.write("\n", strlen("\n"));

					name = sect->GetName();
					sprintf(msg, "[%s]\n", name);
					OutputDebugString(msg);
					myfile.write(msg,strlen(msg));

					prop = sect->GetFirst();
					while ( prop )
					{
						name  = prop->GetName();
						value = prop->GetValue();
						INT len = (INT)strlen(value) - 1;
						while ( len > 2 && *(value+len) == '0' )
						{
							*(value + len) = 0;
							len--;
						}
						sprintf(msg, "%s %s\n", name, value);
						OutputDebugString(msg);
						myfile.write(msg, strlen(msg));
						prop = prop->GetNext();
					}

					sect = sect->GetNext();

				}

			// ---- close (note: ifstream could do this out of scope)
				myfile.close();

			// ---- yay
				return VS_OK;

		}

	return VS_OK;
}


// ---------- Get ----------

VNode * DotIni::GetRoot() { return pi_dotini->root; }

