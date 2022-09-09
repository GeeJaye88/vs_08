////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_io.h ----------
/*!
\file s1_system_io.h
\brief Interface for the SystemIO class.
\author Gareth Edwards
*/

#pragma once


// ---- include ----
#include <windows.h>
#include <string.h>
#include <iostream>
#include <fstream>


// ---- system ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"


////////////////////////////////////////////////////////////////////////////////


namespace vs_system
{

	// ----------  csv buffer ----------
	class CsvObject
	{

		public:

		// ---- cdtor
			CsvObject();
			virtual ~CsvObject();


		// ---- housekeeping 
			INT  CreateBuffer();
			INT  DestroyBuffer();
			INT  Initialise(UINT columns, UINT rows);

			INT  GetBuffer(FLOAT **buffer);

			UINT GetColumns();
			UINT GetExtent();
			BOOL GetRead();
			UINT GetRowsPerRecord();
			UINT GetRows();

			INT  SetRowsPerRecord(UINT record_row_length);

			INT  SetKey(UINT column, CHAR *key);
			INT  SetFloat(UINT row, UINT column, CHAR *float_text);
			INT  SetRead(BOOL read);

		private:

			class PI_CsvObject; PI_CsvObject *pi_csv_object = NULL;

	};


	// ---------- csv io ----------
	class CsvIO
	{

		public:

		// ---- cdtor
			CsvIO();
			virtual ~CsvIO();

		// ---- io
			INT Read(CsvObject *, CHAR *);

		private:

			class PI_CsvIO; PI_CsvIO *pi_system_io;

	};

}


////////////////////////////////////////////////////////////////////////////////
