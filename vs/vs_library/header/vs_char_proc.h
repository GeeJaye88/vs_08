////////////////////////////////////////////////////////////////////////////////

// ---------- vs_char_proc.h ----------
/*!
\file vs_char_proc.h
\brief Interfaces for CHAR PROCesssing class.
\author Gareth Edwards
*/

#ifndef VS_LIBRARY_CHARPROC
#define VS_LIBRARY_CHARPROC


// ---- include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include <fstream> // For VNodeIO


// ---------- namespace ----------
namespace vs_system
{


// ---------- CharProc ----------
class CharProc
{
public:
	INT Find       (CHAR*,  CHAR*);                                  // TODO TEST and UINT
	INT Replace    (CHAR*,  CHAR*,  INT,   CHAR*,  CHAR*);           // TODO TEST and UINT
	INT Snip       (CHAR*,  INT,    INT,   INT,    CHAR*,   CHAR*);  // TODO TEST and UINT
	INT SnipSpaces (CHAR*,  CHAR*,  UINT);                           // TESTED
	INT Split      (CHAR**, UINT,   CHAR,  CHAR*);                   // TODO TEST
	INT Substitute (CHAR,   CHAR,   CHAR*);                          // TODO TEST
	INT Toupper	   (CHAR*,  UINT,   CHAR*);                          // TODO TEST
}; // CharProc class


}  // vs_system namespace


#endif // VS_LIBRARY_CHARPROC

