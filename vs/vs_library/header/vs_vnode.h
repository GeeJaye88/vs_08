////////////////////////////////////////////////////////////////////////////////

// ---------- vs_vnode.h ----------
/*!
\file vs_vnode.h
\brief Interfaces for VNODE classes.
\author Gareth Edwards
\note This library supports VNode document type 4 (DOCTYPE HDL4)
*/

#ifndef VS_SYSTEM_VNODE
#define VS_SYSTEM_VNODE


// ---- include ----
#include "../header/vs_char_proc.h"
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_inc_dems.h"
#include <fstream> // For VNodeIO


namespace vs_system
{

// ---------- VNode ----------
class VNode
{

public:

	// ---- constructor
		VNode();
		VNode(CHAR *);
		VNode(const CHAR *);
		VNode(CHAR *, CHAR *);
		VNode(VNode *);
		VNode(VNode *, CHAR *);
		VNode(CHAR *, VNode *);
		VNode(CHAR *, VNode *, UINT);
		VNode(VNode *, const CHAR *);
		VNode(VNode *, CHAR *, CHAR *);

	// ---- destructor
		virtual ~VNode();

	// ---- add
		VNode *Add();
		VNode *Add(CHAR *);
		VNode *Add(const CHAR *);
		VNode *Add(CHAR *, CHAR *);

	// ---- append
		VNode *Append(VNode*, CHAR *);

	// ---- delete
		INT Delete();

	// ---- get property
		CHAR  *GetName();
		CHAR  *GetValue();
		BOOL   GetQuotes();
		CHAR   GetType();
		VNode *GetParent();
		VNode *GetFirst();
		VNode *GetLast();
		VNode *GetLinkP();
		VNode *GetLinkS();
		INT    GetAssociated();
		VNode *GetPrevious();
		VNode *GetNext();

	// ---- get
		VNode *GetListItem(CHAR *, CHAR *);
		BOOL   GetBoolFromValue();
		FLOAT  GetFloatFromValue();
		INT    GetIntFromValue();

	// ---- set
		INT    SetParent(VNode *);
		INT    SetLinkP(VNode *);
		INT    SetLinkS(VNode *);
		INT    SetAssociated(INT);

	// ---- search
		VNode *SearchName(CHAR *);
		VNode *SearchThis(CHAR *);
		INT    SearchValue(CHAR *, INT, CHAR *);

	// ---- set
		VOID   SetQuotes(BOOL q);
		VOID   SetType(CHAR t);
		VOID   SetValue(CHAR  *);
		VOID   SetValue(FLOAT);
		VOID   SetValue(INT);

private:

	// ---- private implementation
		class PNode; PNode *pnode;

};


// ---------- VNodeIO ----------
class VNodeIO
{

	public:
		VNode *Copy(VNode*, VNode*);
		VNode *Read (CHAR*);
		INT    Write (CHAR*, VNode*);
		INT    Write (std::ofstream*, VNode*, INT*, INT*);

};


}  /// namespace


#endif // VS_SYSTEM_VNODE

