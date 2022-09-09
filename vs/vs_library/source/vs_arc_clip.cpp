////////////////////////////////////////////////////////////////////////////////

// ---------- vs_arc_clip.cpp ----------
/*!
\file vs_arc_clip.cpp
\brief Implementation of the Arch(ive) Util(ity) class.
\author Gareth Edwards 
*/


// ---- include ----

#include "../header/vs_arc_clip.h"


// ---- namespace ----
using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ArchTime class ----------

class ArchTime::PI_ArchTime
{

public:

	PI_ArchTime()
	{
		_year    = 0;
		_month   = 0;
		_day     = 0;
		_hour    = 0;
		_minute  = 0;
		_second  = 0;
		_msecond = 0;
	};

	~PI_ArchTime()
	{
		;
	};

	UINT _year;
	UINT _month;
	UINT _day;
	UINT _hour;
	UINT _minute;
	UINT _second;
	UINT _msecond;
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of ArchTime class ----------


// ---------- Constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
ArchTime::ArchTime()
{
	_pi_arch_time = new PI_ArchTime();
}


// ---------- Destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
ArchTime::~ArchTime()
{
	if ( _pi_arch_time != NULL ) { delete _pi_arch_time; _pi_arch_time = NULL; }
}


// ---------- Get ----------
/*!
\brief Get....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
UINT ArchTime::Get(
		UINT index
	)
{
	return *(&_pi_arch_time->_year + index);
}


// ---------- GetClipName ----------
/*!
\brief Set....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

\note for UTC, use GetSystemTime(&SystemTime);

*/
INT ArchTime::GetClipName(
		CHAR *name
	)
{

	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);

	// ---- created date time ----
	char cdt[16];
	sprintf(cdt, "%4d%2d%2d_%2d%2d%2d",
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond
		);


	// ---- in point date time + "_Clip_" + created date time ----
	sprintf(
			name,
			"%s_Clip_%4d%2d%2d_%2d%2d%2d",
			cdt,
			_pi_arch_time->_year,
			_pi_arch_time->_month,
			_pi_arch_time->_day,
			_pi_arch_time->_hour,
			_pi_arch_time->_minute,
			_pi_arch_time->_second
		);


	// ---- tidy ----
	UINT len = strlen(name);
	for ( UINT i = 0; i < len; i++ )
	{
		*(name + i) = *(name + i) == ' ' ? '0' : *(name + i);
	}

	return VS_OK;
}


// ---------- EqualMinute ----------
/*!
\brief Set....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchTime::EqualMinute(ArchTime* adt)
{
	if (_pi_arch_time->_year == adt->_pi_arch_time->_year)
		if (_pi_arch_time->_month == adt->_pi_arch_time->_month)
			if (_pi_arch_time->_day == adt->_pi_arch_time->_day)
				if (_pi_arch_time->_hour == adt->_pi_arch_time->_hour)
					if (_pi_arch_time->_minute == adt->_pi_arch_time->_minute)
						return TRUE;
	return FALSE;
}


// ---------- EqualTo ----------
/*!
\brief Set....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchTime::EqualTo(ArchTime* adt, UINT index)
{
	if ( index <= MSECOND )
	{
		for (UINT i = 0; i <= index; i++)
		{
			if ( *(&_pi_arch_time->_year + i) != *(&adt->_pi_arch_time->_year + i) )
				return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}


// ---------- Set ----------
/*!
\brief Set....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchTime::Set(
	UINT index,
	UINT value
	)
{
	if ( index <= MSECOND )
	{
		*(&_pi_arch_time->_year + index) = value;
		_time[index] = value;
		return VS_OK;
	}
	return -1;
}


// ---------- Set ----------
/*!
\brief Set....
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchTime::Set(
		UINT year,
		UINT month,
		UINT day,
		UINT hour,
		UINT minute,
		UINT second,
		UINT msecond
	)
{
	_pi_arch_time->_year = year;
	_pi_arch_time->_month = month;
	_pi_arch_time->_day = day;
	_pi_arch_time->_hour = hour;
	_pi_arch_time->_minute = minute;
	_pi_arch_time->_second = second;
	_pi_arch_time->_msecond = msecond;
	_time[YEAR]    = year;
	_time[MONTH]   = month;
	_time[DAY]     = day;
	_time[HOUR]    = hour;
	_time[MINUTE]  = minute;
	_time[SECOND]  = second;
	_time[MSECOND] = msecond;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ArchClip class ----------

class ArchClip::PI_ArchClip
{

public:

	PI_ArchClip() { ; };
	~PI_ArchClip() {; };

	INT CopyClip(UINT, CHAR *,ArchTime *,ArchTime *,CHAR *);
	INT CopyMinute(UINT, CHAR *,ArchTime *,ArchTime *,CHAR *,CHAR *);
	INT GetClipPath(CHAR *, ArchTime *, ArchTime *, CHAR *);
	INT GetMilliseconds(LLONG *, ArchTime *);
	INT GetStartEnd(UINT, UINT, ArchTime *, ArchTime *, UINT *, UINT *);

	INT *_total_file_count     = NULL;
	INT *_total_file_processed = NULL;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of PI_ArchClip class ----------

#ifdef OS_WINDOWS
#undef OS_WINDOWS
#endif

#include "atlstr.h"


// ---------- CopyClip ----------
/*!
\brief Create a clip
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchClip::PI_ArchClip::CopyClip(
		UINT      copy_pass,
		CHAR     *destination,
		ArchTime *in,
		ArchTime *out,
		CHAR     *source
	)
{
	
	// ---- local ----
	INT      result = VS_OK;
	CHAR     path[6][VS_MAXCHARPATHLEN];
	ArchTime now;
	INT      file_count = 0;


	// ---- local []_s(tart)/[]_e(nd) ----
	UINT ye_s, ye_e, ye_i = (UINT)ArchTime::YEAR;
	UINT mo_s, mo_e, mo_i = (UINT)ArchTime::MONTH;
	UINT da_s, da_e, da_i = (UINT)ArchTime::DAY;
	UINT ho_s, ho_e, ho_i = (UINT)ArchTime::HOUR;
	UINT mi_s, mi_e, mi_i = (UINT)ArchTime::MINUTE;


	// ---- lambda ------------------------------------------------------------
		auto copy_archive_hdl = [&]()
		{
			if (copy_pass == 0) return VS_OK;

			try
			{
				CHAR src[VS_MAXCHARPATHLEN];
				sprintf(src, "%sarchive.hdl", source);

				DWORD dwAttrib = GetFileAttributesA(src);
				if (dwAttrib != INVALID_FILE_ATTRIBUTES)
				{
					throw(src);
				}
			}
			catch (char *src)
			{

				// ---- source image file ----
				TCHAR tc_sif[512]; _tcscpy(tc_sif, A2T(src));

				// --- destination image file ----
				CHAR dest[VS_MAXCHARPATHLEN];
				sprintf(dest, "%sarchive.hdl", destination);
				TCHAR tc_dif[512]; _tcscpy(tc_dif, A2T(dest));

				// ---- copy ----
				if ( CopyFileA(tc_sif, tc_dif, false) != 0 )
				{
					DWORD gle = GetLastError();
					if (gle == ERROR_FILE_NOT_FOUND)
					{
						printf("CopyFileA: %s file not found.\n", src);
					}
					else if (gle != 0)
					{
						printf("CopyFileA failed with error # %d.\n", (INT)gle);
					}
					else
					{
						return VS_OK;
					}
				}

			}

			return VS_ERROR;
		};


	// ---- lambda ------------------------------------------------------------
		auto copy_minute = [&](ArchTime *in, ArchTime *out, CHAR *min_path)
		{
			INT file_count = CopyMinute
				(
					copy_pass,
					destination,
					in,
					out,
					min_path,
					source
				);
			return file_count;
		};


	// ---- lambda ------------------------------------------------------------
		auto make_path = [](CHAR *dest_path, BOOL start, UINT unit, CHAR *src_path)
		{
			if (start)
			{
				sprintf(dest_path, "archive/%4d/", unit);
			}
			else
			{
				sprintf(dest_path, "%s%2d/", src_path, unit);
			}
			UINT len = strlen(dest_path);
			for (UINT l = 0; l < len; l++)
				dest_path[l] = dest_path[l] == ' ' ? '0' : dest_path[l];
			printf("%s\n", dest_path);
		};


	// ---- lambda ------------------------------------------------------------
		auto make_directory = [&](CHAR *path)
		{
			if ( copy_pass == 0 ) return VS_OK;

			CHAR temp[VS_MAXCHARPATHLEN];
			sprintf(temp, "%s%s", destination, path);
			try
			{
				DWORD dwAttrib = GetFileAttributesA(temp);
				if (dwAttrib == INVALID_FILE_ATTRIBUTES)
				{
					throw("make");
				}
			}
			catch (char *throw_str)
			{
				if ( _mkdir(temp) == 0 )
				{
					printf("%s %s succeeded.\n", throw_str, path);
					return VS_OK;
				}
				else
				{
					printf("make_directory: _mkdir %s failed!\n", path);
					return VS_ERROR;
				}
			}
			printf("make_directory: GetFileAttributesA %s failed!\n", path);
			return VS_ERROR;
		};


	// ---- lambda ------------------------------------------------------------
		auto remove_directory = [&]()
		{
			try
			{
				DWORD dwAttrib = GetFileAttributesA(destination);
				if (
					dwAttrib != INVALID_FILE_ATTRIBUTES &&
						( dwAttrib & FILE_ATTRIBUTE_DIRECTORY )
					)
				{
					throw("remove");
				}
			}
			catch (char *throw_str)
			{
				if ( _rmdir(destination) == 0 )
				{
					printf("%s %s succeeded.\n", throw_str, destination);
					return VS_OK;
				}
				else
				{
					printf("remove_directory: _rmdir %s failed!\n", destination);
					return VS_ERROR;
				}
			}
			printf("remove_directory: GetFileAttributesA %s failed!\n", destination);
			return VS_ERROR;
		};


	// ---- create clip folder ----
	result = make_directory("");


	// ---- copy clip "archive.hdl" file ----
	result = copy_archive_hdl();


	// ---- create archive folder ----
	{
		result = make_directory("archive/");

		// FOR -- each year --
		ye_s = in->Get(ye_i);
		ye_e = out->Get(ye_i);
		for (UINT year = ye_s; year <= ye_e; year++)
		{
			now.Set(ye_i, year);
			make_path(path[0], TRUE, year, path[0]);
			result = make_directory(path[0]);

			// FOR -- each month --
			result = GetStartEnd(year, mo_i, in, out, &mo_s, &mo_e);
			for (UINT month = mo_s; month <= mo_e; month++)
			{
				now.Set(mo_i, month);
				make_path(path[1], FALSE, month, path[0]);
				result = make_directory(path[1]);

				// FOR -- each day --
				result = GetStartEnd(month, da_i, in, out, &da_s, &da_e);
				for (UINT day = da_s; day <= da_e; day++)
				{
					now.Set(da_i, day);
					make_path(path[2], FALSE, day, path[1]);
					result = make_directory(path[2]);

					// FOR -- each hour --
					result = GetStartEnd(day, ho_i, in, out, &ho_s, &ho_e);
					for (UINT hour = ho_s; hour <= ho_e; hour++)
					{
						now.Set(ho_i, hour);
						make_path(path[3], FALSE, hour, path[2]);
						result = make_directory(path[3]);

						// FOR -- each minute --
						result = GetStartEnd(hour, mi_i, in, out, &mi_s, &mi_e);
						for (UINT minute = mi_s; minute <= mi_e; minute++)
						{
							now.Set(mi_i, minute);
							make_path(path[4], FALSE, minute, path[3]);
							result = make_directory(path[4]);

							// IF -- equal to -- THEN -- [blah[ -- ELSE -- [blah]
							if ( in->EqualTo(&now, ArchTime::MINUTE) && out->EqualTo(&now, ArchTime::MINUTE) )
								file_count += copy_minute(in, out, path[4]);

							else if ( in->EqualTo(&now, ArchTime::MINUTE) )
								file_count += copy_minute(in, NULL, path[4]);

							else if ( out->EqualTo(&now, ArchTime::MINUTE) )
								file_count += copy_minute(NULL, out, path[4]);

							else
								file_count += copy_minute(NULL, NULL, path[4]);

						}
					}
				}
			}
		}
	}


	if ( copy_pass == 0 )
	{
		*_total_file_count = file_count;
		printf("Total # of files is %d.\n", *_total_file_count);
	}

	return VS_OK;
}


// ---------- CopyMinute ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/

INT ArchClip::PI_ArchClip::CopyMinute(
		UINT      copy_pass,
		CHAR     *destination,
		ArchTime *in,
		ArchTime *out,
		CHAR     *minute,
		CHAR     *source
	)
{

	printf("CopyMinuteFolder\n");


	// ---- lambda ------------------------------------------------------------
		auto get_arch_time = [](ArchTime *now, CHAR *filename)
		{
			// yyyymmdd_hhmmss_mss.jpg
			// 012345678901234567890123
			//
			UINT begin[7] = { 0, 4, 6, 9, 11, 13, 16 };
			UINT end[7] = { 4, 6, 8, 11, 13, 15, 19 };
			CHAR name[24];
			strcpy(name, filename);
			for (UINT i = 0; i < 7; i++)
			{
				CHAR bi = begin[i];
				CHAR ei = end[i];
				CHAR ec = name[ei];
				name[ei] = 0;
				UINT v = atoi(&name[bi]);
				name[ei] = ec;
				now->Set(i, v);
			}
		};


	// ---- local ----
	INT result = VS_OK;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA fdata;


	// ---- source ----
	CHAR src[256]; strcpy(src, source); strcat(src, minute); strcat(src, "*");
	for (UINT i = 0; i < strlen(src); i++) src[i] = src[i] == '/' ? '\\' : src[i];
 

	// ---- destination ----
	CHAR dest[256]; strcpy(dest, destination); strcat(dest, minute);
	for (UINT i = 0; i < strlen(dest); i++) dest[i] = dest[i] == '/' ? '\\' : dest[i];


	// ---- ms ----
	LLONG in_ms, out_ms;
	if ( in  != NULL ) result = GetMilliseconds(&in_ms, in);
	if ( out != NULL ) result = GetMilliseconds(&out_ms, out);


	// ---- counter ----
	INT file_count = *_total_file_processed;


	// ---- for  ---- all files in this folder ----
	hFind = FindFirstFile(src, &fdata);
	if ( hFind != INVALID_HANDLE_VALUE )
	{

		// ---- zap '*' wildcard ----
		src[strlen(src) - 1] = 0;

		do
		{
			if (strcmp(fdata.cFileName, ".") != 0 &&
				strcmp(fdata.cFileName, "..") != 0)
			{
				if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					;
				}
				else
				{

					//  ---- it is a file ----
					INT len = strlen(fdata.cFileName);
					if ( len == 23 )
					{

						BOOL copy_image = TRUE;

						// ============================================================

						ArchTime now;
						LLONG now_ms;
						if ( in != NULL || out != NULL )
						{
							get_arch_time(&now, fdata.cFileName);
							result = GetMilliseconds(&now_ms, &now);
						}

						if ( in != NULL )
						{
							if ( now_ms < in_ms ) copy_image = FALSE;
						}

						if ( copy_image != FALSE && out != NULL)
						{
							if ( now_ms > out_ms) copy_image = FALSE;
						}

						// ============================================================

						if (copy_image == TRUE)
						{

							if ( copy_pass == 0 )
							{
								//printf("%4d : %s\n", file_count+1, fdata.cFileName);
								//Sleep(10);
								file_count++;
							}
							else
							{
								// ---- source image file ----
								CHAR sif[256]; strcpy(sif, src); strcat(sif, fdata.cFileName);
								TCHAR tc_sif[512]; _tcscpy(tc_sif, A2T(sif));


								// --- destination image file ----
								CHAR dif[256]; strcpy(dif, dest); strcat(dif, fdata.cFileName);
								TCHAR tc_dif[512]; _tcscpy(tc_dif, A2T(dif));


								// ---- copy ----
								if ( CopyFileA(tc_sif, tc_dif, false) != 0 )
								{
									DWORD gle = GetLastError();
									if ( gle == ERROR_FILE_NOT_FOUND )
									{
										printf("CopyFileA: %s file not found.\n", fdata.cFileName);
									}
									else if ( gle != 0 )
									{
										printf("CopyFileA failed with error # %d.\n", (INT)gle);
									}
								}

								
									file_count++;
									*_total_file_processed = file_count;
							}
						}

						// ============================================================
				
					}
				}
			}

		} while (FindNextFile(hFind, &fdata) != 0);
	}


	// ---- check that this folder listing is complete ----
	if ( GetLastError() != ERROR_NO_MORE_FILES )
	{
		FindClose(hFind);
		return VS_ERROR;
	}


	// ---- close this folder ----
	FindClose(hFind);
	hFind = INVALID_HANDLE_VALUE;

	return file_count;
}


// ---------- GetClipPath ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchClip::PI_ArchClip::GetClipPath(
	CHAR     *destination,
	ArchTime *in,
	ArchTime *out,
	CHAR     *source
	)
{

	// ---- local ----
	CHAR archive_folder[VS_MAXCHARLEN];


	// ---- create destination folder name from in_point ----
	in->GetClipName(archive_folder);


	// ---- copy destination path/folder paf from source ----
	strcpy_s(destination, VS_MAXCHARLEN, source);


	// ---- clean out '\\' ----
	INT len = strlen(destination);
	for (INT i = 0; i < len; i++)
	{
		*(destination + i) == '\\' ?
			'/' :
			*(destination + i);
	}


	// ---- strip off source folder name ----
	INT index = len - 2; // Note: ignore last '/' char
	while (index >= 0)
	{
		if (*(destination + index) == '/')
		{
			*(destination + index + 1) = 0;
			index = 0; // Exit while ( index....
		}
		index--;
	}


	// ---- replace with destination folder name ----
	strcat(destination, archive_folder);


	// ---- tidy ----
	strcat(destination, "/");

	return VS_OK;
}


// ---------- GetMilliseconds ----------
/*!
\brief
\author Gareth Edwards
*/
INT ArchClip::PI_ArchClip::GetMilliseconds(
		LLONG *millisecond,
		ArchTime *adt
	)
{

	// ---- unpack ----
	UINT year   = adt->Get(ArchTime::YEAR);
	UINT month  = adt->Get(ArchTime::MONTH);
	UINT day    = adt->Get(ArchTime::DAY);
	UINT hour   = adt->Get(ArchTime::HOUR);
	UINT minute = adt->Get(ArchTime::MINUTE);
	UINT second = adt->Get(ArchTime::SECOND);
	UINT milli  = adt->Get(ArchTime::MSECOND);


	// ---- local ----
	UINT day_lut[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	UINT year_normal = 365;
	UINT year_leap = 366;
	UINT index = 99;


	// ---- init total ms ----
	*millisecond = 0;


	// ---- total ms up to "this" year ----
	LLONG previous_year = year - 1;
	for (LLONG year = 2011; year <= previous_year; year++)
	{
		unsigned long long days = year % 4 == 0 ? year_leap : year_normal;
		*millisecond += days * 24 * 60 * 60 * 1000;
	}
	if (index == 1) return VS_OK;


	// ---- total days up to "this" month ----
	LLONG this_year = year;
	LLONG previous_month = month - 1;
	unsigned long long total_days = 0;
	for (LLONG month = 0; month<previous_month; month++)
	{
		total_days += this_year % 4 == 0 ? (month == 1 ? 29 : 28) : day_lut[month];
	}


	// ---- month ----
	*millisecond += total_days * 24 * 60 * 60 * 1000;
	if (index == 2) return VS_OK;


	// ---- day ----
	*millisecond += day * 24 * 60 * 60 * 1000;
	if (index == 3) return VS_OK;


	// ---- hours ----
	*millisecond += hour * 60 * 60 * 1000;
	if (index == 4) return VS_OK;


	// ---- minutes ----
	*millisecond += minute * 60 * 1000;
	if (index == 5) return VS_OK;


	// ---- seconds ----
	*millisecond += second * 1000;
	if (index == 6) return VS_OK;


	// ---- milliseconds ----
	*millisecond += milli;
	if (index == 7) return VS_OK;

	return VS_ERROR;
}


// ---------- GetStartEnd ----------
/*!
\brief Get 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ArchClip::PI_ArchClip::GetStartEnd(
		UINT      parent_value,
		UINT      datetime_index,
		ArchTime *in,
		ArchTime *out,
		UINT     *start_value,
		UINT     *end_value
	)
{
	// ---- local ----
	INT result = VS_OK;


	// ---- local parent minmax ----
	UINT min_parent_value, max_parent_value;
	min_parent_value = in->Get(datetime_index - 1);
	max_parent_value = out->Get(datetime_index - 1);


	// ---- local child minmax ----
	UINT min_child_value, max_child_value;
	min_child_value = in->Get(datetime_index);
	max_child_value = out->Get(datetime_index);


	// ---- process based upon datetime index ----
	switch (datetime_index)
	{
		case ArchTime::MONTH:
			{
				if (parent_value == min_parent_value && parent_value == max_parent_value)
				{
					*start_value = min_child_value;
					*end_value = max_child_value;
				}
				else if (parent_value == min_parent_value)
				{
					*start_value = min_child_value;
					*end_value = 12;
				}
				else if (parent_value == max_parent_value)
				{
					*start_value = 1;
					*end_value = max_child_value;
				}
				else
				{
					*start_value = 1;
					*end_value = 12;
				}
			}
			break;

		case ArchTime::DAY:
			{

				UINT day_lut[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
				day_lut[1] = parent_value % 4 == 0 ? 29 : 28;

				if (parent_value == min_parent_value && parent_value == max_parent_value)
				{
					*start_value = min_child_value;
					*end_value = max_child_value;
				}
				else if (parent_value == min_parent_value)
				{
					*start_value = min_child_value;
					*end_value = day_lut[min_parent_value];
				}
				else if (parent_value == max_parent_value)
				{
					*start_value = 1;
					*end_value = max_child_value;
				}
				else
				{
					*start_value = 1;
					*end_value = day_lut[min_parent_value];
				}
			}
			break;

		case ArchTime::HOUR:
			{
				if (parent_value == min_parent_value && parent_value == max_parent_value)
				{
					*start_value = min_child_value;
					*end_value = max_child_value;
				}
				else if (parent_value == min_parent_value)
				{
					*start_value = min_child_value;
					*end_value = 24;
				}
				else if (parent_value == max_parent_value)
				{
					*start_value = 1;
					*end_value = max_child_value;
				}
				else
				{
					*start_value = 1;
					*end_value = 24;
				}
			}
			break;

		case ArchTime::MINUTE:
		case ArchTime::SECOND:
		{
				if (parent_value == min_parent_value && parent_value == max_parent_value)
				{
					*start_value = min_child_value;
					*end_value = max_child_value;
				}
				else if (parent_value == min_parent_value)
				{
					*start_value = min_child_value;
					*end_value = 60;
				}
				else if (parent_value == max_parent_value)
				{
					*start_value = 1;
					*end_value = max_child_value;
				}
				else
				{
					*start_value = 1;
					*end_value = 60;
				}
			}
			break;

		default:
			return VS_ERROR;
	}

	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of ArchClip class ----------


// ---------- Constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
ArchClip::ArchClip()
{
	_pi_arch_util = new PI_ArchClip();
}


// ---------- Destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
ArchClip::~ArchClip()
{
	if ( _pi_arch_util != NULL ) { delete _pi_arch_util; _pi_arch_util = NULL; }
}


// ---------- CopyClip ----------
/*!
\brief Copy a sub archive clip from an existing archive
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT ArchClip::CopyClip(
		ArchTime *in,
		ArchTime *out,
		CHAR     *source,
		INT      *total_files,
		INT      *total_processed
	)
{

	// ---- local ----
	INT result = VS_OK;
	CHAR destination[VS_MAXCHARLEN];


	// ---- zap ----
	_pi_arch_util->_total_file_count = total_files;
	_pi_arch_util->_total_file_processed = total_processed;


	// ---- on the tin ----
	result = _pi_arch_util->GetClipPath
		(
			destination,
			in,
			out,
			source
		);


	// ---- copy ----
	{
		for (UINT pass = 0; pass < 2; pass++)
		{
			result =
				_pi_arch_util->CopyClip(
					pass,
					destination,
					in,
					out,
					source
				);
		}
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
