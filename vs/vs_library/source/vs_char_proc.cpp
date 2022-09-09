////////////////////////////////////////////////////////////////////////////////

// ---------- vs_charproc.cpp ----------
/*!
\file vs_charproc.cpp
\brief Implementation of CHAR PROCesssing class.
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_char_proc.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Find ----------
/*!
\brief Find first occurence of a char str in another char str
\author Gareth Edwards
\param CHAR * - in this string
\param CHAR * - find this
\return INT - - ERROR < 0 < index of first character
\note only tests for first occurence

Reference:

int strcmp ( src , dst )
unsigned char *src;
unsigned char *dst;
{
  int ret = 0 ;

  while( ! (ret = *src - *dst) && *dst)
    ++src, ++dst;

  if ( ret < 0 )
    ret = -1 ;
  else if ( ret > 0 )
    ret = 1 ;

  return( ret );
}

*/
INT CharProc::Find(CHAR *inStr, CHAR *matchStr)
{
	// Local
	INT matchLen = strlen(matchStr);
	INT inLen = strlen(inStr);

	// Silly
	if ( matchLen > inLen ) return -1;

	// Test
	INT startIndex = -1;
	INT testIndex = 0;
	BOOL testing = false;
	for (int i=0; i<inLen; i++)
	{
		if ( testing )
		{
			if ( *(inStr+i) == *(matchStr+testIndex) )
			{
				testIndex++;
				if (testIndex == matchLen)
					return startIndex;
			}
			else
			{
				testing = false;
				testIndex = 0;
				startIndex = -1;
			}
		}
		else
		{
			if ( *(inStr+i) == matchStr[0] )
			{
				testing = true;
				testIndex = 1;
				startIndex = i;
			}
		}
	}

	// Catch last char or partial match trap
	if ( testIndex+1 != matchLen ) startIndex = -1;

	// Done
	return startIndex;
}


// ---------- Replace ----------
/*!
\brief Replace occurence of a char str in another char str
\author Gareth Edwards
\param CHAR * - in to this string
\param CHAR * - with this string
\param INT - starting at this index
\param CHAR * - from this string
\param CHAR * - with this string
\return INT - ERROR <= 0 < VS_OK
*/
INT CharProc::Replace (CHAR *too, CHAR *tooStr, INT startIndex, CHAR *from, CHAR *fromStr)
{
	// Local
	INT tooIndex = 0;
	INT fromIndex = 0;

	// First copy from up to start index
	for (fromIndex=0; fromIndex<startIndex; fromIndex++)
		*(too+tooIndex++) = *(from+fromIndex);

	// Then copy tooStr
	INT tooLen = strlen(tooStr);
	for (int i=0; i<tooLen; i++)
		*(too+tooIndex++) = *(tooStr+i);

	// Then jump ahead fromStr
	fromIndex += strlen(fromStr);

	// Then copy rest of from
	while (*(from+fromIndex) != 0)
		*(too+tooIndex++) = *(from+fromIndex++);

	// Terminate
	*(too+tooIndex++) = 0;

	// Done
	return VS_OK;
}


// ---------- Snip ----------
/*!
\brief Snip
\author Gareth Edwards
\param CHAR * - pointer to destination string
\param INT - size of destination string
\param INT - snip start
\param INT - snip width
\param CHAR * - pointer source string
\param CHAR * - pointer tail string
\return INT - ERROR <= 0 < length of snipped string
\note Returned length cannot exceed size of destination string minus one
as the last character is a '\0'.
*/
INT CharProc::Snip (CHAR *dest, INT destSize, INT start, INT width, CHAR *src, CHAR *tail)
{
	// Local
	INT len = strlen(src);
	INT tailLen = 0;
	INT destMaxLen = destSize;
	if ( tail != 0 )
	{
		tailLen = strlen(tail);
		destMaxLen = destMaxLen - tailLen;
	}

	// Snip
	INT end = start + width;
	INT dindex = 0;
	for (int i=0; i<len; i++)
	{
		if (i+1 >= start && i+1 < end)
		{
			if ( dindex+1 < destMaxLen )
				dest[dindex++] = src[i];
			else
				break;
		}
	}

	// IF tail THEN add tail
	if ( tailLen > 0 )
		for (int t=0; t<tailLen; t++)
			dest[dindex++] = tail[t];
	dest[dindex] = '\0';

	// Done
	return dindex;
}


// ---------- SnipSpaces ----------
/*!
\brief Snip spaces
\author Gareth Edwards
\param CHAR * - result string
\param CHAR * - original string
\param UINT - length of original string
\return INT - ERROR <= 0 < VS_OK
*/
INT CharProc::SnipSpaces (CHAR *r, CHAR *o, UINT otop)
{
	*r = 0;
	if (otop)
	{
		INT i = 0;
		INT start = 0;
		INT end = 0;
		while (i<(INT)otop)
		{
			if ( *(o+i) != ' ' )
			{
				start = i;
				i = (INT)otop+1;
			}
			else
			{
				i++;
			}
		}
		i = (INT)otop-1;
		while (i>=0)
		{
			if ( *(o+i) != ' ' )
			{
				end = i;
				i = -1;
			}
			else
			{
				i--;
			}
		}
		if ( end-start >= 0 ) {
			INT j=0;
			for (i=start;i<=end;i++)
			{
				*(r+j++) = *(o+i);
			}
			*(r+j) = 0;
			return VS_OK;
		}
	}
	return VS_ERROR;
}


// ---------- Split ----------
/*!
\brief Split string and return word count
\author Gareth Edwards
\param CHAR * - result array string - maximum size is second parameter
\param UINT - max words to return
\param CHAR - spliting string
\param CHAR * - original string
\return INT - ERROR <= 0 < number of new words
*/
INT CharProc::Split(CHAR **pword, UINT maxwords, CHAR splitChar, CHAR *splitStr)
{
	if ( pword == NULL ) return VS_ERROR;
	if ( splitStr == NULL ) return VS_ERROR;
	UINT len = strlen(splitStr);
	UINT words = 0;
	BOOL penup = true;
	for (UINT i=0; i<len; i++)
	{
		if ( *(splitStr+i) == splitChar )
		{
			penup = true;
			*(splitStr+i) = '\0';
		}
		else
		{
			if ( penup )
			{
				if (words < maxwords)
					*(pword+words) = splitStr+i;
				words++;
			}
			penup = false;
		}
	}
	return words;
}


// ---------- Substitute ----------
/*!
\brief Substitute all instances of a char for another
\author Gareth Edwards
\param CHAR - replace
\param CHAR - with this
\param CHAR * - in this string
\return INT - ERROR <= 0 < VS_OK 
*/
INT CharProc::Substitute(CHAR out, CHAR in, CHAR *str)
{
	if ( str == NULL ) return VS_ERROR;
	INT len = strlen(str);
	for (INT i=0; i<len; i++)
		if (*(str+i) == out) *(str+i) = in;
	return VS_OK;
}


// ---------- Toupper ----------
/*!
\brief Convert to upper case
\author Gareth Edwards
\param CHAR * - pointer to destination string
\param UINT - size of destination string
\param CHAR * - pointer source string
\return INT - ERROR <= 0 < len of destination string
\note If the size of the destination string is less than
the length of the source string then only the size of
the destination characters - less one for the termination
end of string character - will be converted to upper case.
 */
INT CharProc::Toupper(CHAR *dest, UINT destSize, CHAR *src)
{
	if ( dest == NULL ) return VS_ERROR;
	if ( src == NULL ) return VS_ERROR;
	UINT len = strlen(src);
	len = len < destSize ? len : destSize -1;
	for (UINT i=0; i<len; i++)
		dest[i] = ::toupper(src[i]);
	dest[len] = '\0';
	return len;
}
