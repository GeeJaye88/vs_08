////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_io.cpp ----------
/*!
\file s1_system_io.cpp
\brief Implementation of the CsvIO class
\author Gareth Edwards
*/

#include "../header/vs_csv.h"


// ---------- namespace ----------
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of CsvObject class ----------

class CsvObject::PI_CsvObject
{

public:

	// ---- ctor
		PI_CsvObject();
		~PI_CsvObject();


	// ---- properties
		UINT   _rows    = 0;
		UINT   _columns = 0;
		UINT   _extent  = 0;
		UINT   _rows_per_record = 1;
		BOOL   _read    = FALSE;
		CHAR   _key[128][VS_MAXCHARNAME];
		FLOAT *_float_buffer = NULL;

};


CsvObject::PI_CsvObject::PI_CsvObject()
{
	;
}


CsvObject::PI_CsvObject::~PI_CsvObject()
{
	;
}


////////////////////////////////////////////////////////////////////////////////



CsvObject::CsvObject()
{
	pi_csv_object = new PI_CsvObject();
}


CsvObject::~CsvObject()
{
	if ( pi_csv_object != NULL )
	{
		INT result = DestroyBuffer();
		delete pi_csv_object;
		pi_csv_object = NULL;
	}
}


INT CsvObject::CreateBuffer()
{
	pi_csv_object->_float_buffer = new FLOAT[pi_csv_object->_extent];
	return VS_OK;
}


INT CsvObject::DestroyBuffer()
{
	if ( pi_csv_object->_float_buffer != NULL )
	{
		delete [] pi_csv_object->_float_buffer;
		pi_csv_object->_float_buffer = NULL;
	}
	return VS_OK;
}


INT CsvObject::Initialise(UINT rows, UINT columns)
{
	pi_csv_object->_columns = columns;
	pi_csv_object->_rows    = rows;
	pi_csv_object->_extent  = columns * rows;
	for (UINT c = 0; c < columns; c++) strcpy(pi_csv_object->_key[c], "NULL");
	return VS_OK;
}


INT CsvObject::GetBuffer(FLOAT **buffer)
{
	
	if ( pi_csv_object->_float_buffer != NULL )
	{
		*buffer = &pi_csv_object->_float_buffer[0];
		return VS_OK;
	}

	return VS_ERROR;
}


UINT CsvObject::GetColumns()
{
	return pi_csv_object->_columns;
}


UINT CsvObject::GetExtent()
{
	return pi_csv_object->_extent;
}


BOOL CsvObject::GetRead()
{
	return pi_csv_object->_read;
}


UINT CsvObject::GetRowsPerRecord()
{
	return pi_csv_object->_rows_per_record;
}


UINT CsvObject::GetRows()
{
	return pi_csv_object->_rows;
}


INT CsvObject::SetRowsPerRecord(UINT record_row_length)
{
	return pi_csv_object->_rows_per_record = record_row_length;
}


INT CsvObject::SetKey(UINT column, CHAR *key)
{
	if ( column <= pi_csv_object->_columns)
	{
		strcpy(pi_csv_object->_key[column], key);
		return VS_OK;
	}
	return VS_ERROR;
}


INT CsvObject::SetFloat(UINT row, UINT column, CHAR *float_text)
{
	if ( row <= pi_csv_object->_rows )
	{
		if ( column <= pi_csv_object->_columns )
		{
			pi_csv_object->_float_buffer[pi_csv_object->_columns * row + column] = (FLOAT)atof(float_text);
			return VS_OK;
		}
	}
	return VS_ERROR;
}


INT CsvObject::SetRead(BOOL read)
{
	pi_csv_object->_read = read;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ConfigFiles class ----------

class CsvIO::PI_CsvIO
{

public:

	// ---- ctor
		PI_CsvIO();
		~PI_CsvIO();


};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CsvIO::PI_CsvIO::PI_CsvIO()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CsvIO::PI_CsvIO::~PI_CsvIO()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
CsvIO::CsvIO()
{
	pi_system_io = new PI_CsvIO();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CsvIO::~CsvIO()
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
INT CsvIO::Read(CsvObject *csv_object, CHAR *filename)
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


	// ---- local
		INT result = 0;
		csv_object->SetRead(FALSE);


	// ---- open file ?
		std::ifstream myfile(filename);
		if ( myfile.is_open() )
		{

			// ---- column
				UINT  column_count = 0;
				CHAR *column[128];
				CHAR  column_line[VS_MAXCHARLINELEN];


			// ---- line usage:
				// 0  : key
				// 1+ : data
				INT row = 0;
				while ( !myfile.getline(column_line, VS_MAXCHARLINELEN).eof() )
				{
					column_count = split(column_line, column);
					switch ( row )
					{
						case 0:
							for (UINT c = 0; c < column_count; c++)
							{
								csv_object->SetKey(c, column[c]);
							}
							break;
						default:
							for (UINT c = 0; c < column_count; c++)
							{
								csv_object->SetFloat(row-1, c, column[c]);
							}
							break;
					}
					row++;
				}


			// ---- close (note: ifstream could do this out of scope)
				myfile.close();
			

			// ---- yay
				csv_object->SetRead(TRUE);
				return VS_OK;

		}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
