////////////////////////////////////////////////////////////////////////////////

// ---------- vs_vnode.cpp ----------
/*!
\file vs_vnode.cpp
\brief Implementation of VNODE classes.
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_vnode.h"


// ---- definitions ----
#define DOCTYPE "HDL4"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private implimentation Node class----------
class VNode::PNode
{

public:

	// Window
	VNode *vnode;
	CHAR name[VS_MAXCHARLEN];
	CHAR value[VS_MAXCHARLEN];
	CHAR type;
	CHAR format;
	BOOL quotes;
	BOOL touched;
	BOOL visible;

	// Hierarchical links
	PNode*	parent;
	PNode*	previous;
	PNode*	next;
	PNode*	first_child;
	PNode*	last_child;

	// Non-hierarchical links
	VNode*	link_p;
	VNode*	link_s;

	// Associated integer data
	int	    associated;

	// Constructor
	PNode(VNode*);
	PNode(VNode*, CHAR*);
	PNode(VNode*, const CHAR*);
	PNode(VNode*, CHAR*, CHAR*);
	PNode(VNode*, PNode*);
	PNode(VNode*, PNode*, CHAR*);
	PNode(VNode*, CHAR*, PNode*);
	PNode(VNode*, CHAR*, PNode*, UINT);
	PNode(VNode*, PNode*, const CHAR*);
	PNode(VNode*, PNode*, CHAR*, CHAR*);

	// Destructor
	virtual ~PNode();

	// Initialise
	void Initialise (VNode*);

	// Add
	PNode *Add(VNode*);
	PNode *Add(VNode*, CHAR*);
	PNode *Add(VNode*, const CHAR*);
	PNode *Add(VNode*, CHAR*, CHAR*);

	// Append
	PNode *Append(VNode*, CHAR*);

	// Get
	PNode *GetRoot();
	PNode *GetRootList(CHAR *, CHAR *);

	// Search
	enum SEARCH { NAME=1, VALUE };
	PNode *Search(CHAR*, UINT);
	PNode *SearchThis(CHAR*, UINT);

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
*/
VNode::PNode::PNode(VNode *v)
{
	Initialise(v);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param CHAR * - name
*/
VNode::PNode::PNode(VNode *v, CHAR *name)
{
	Initialise(v);
	strcpy_s (this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param const CHAR * - name
*/
VNode::PNode::PNode(VNode *v, const CHAR *name)
{
	Initialise(v);
	strcpy_s (this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param CHAR * - name
\param CHAR * - value
*/
VNode::PNode::PNode(VNode *v, CHAR *name, CHAR *value)
{
	Initialise(v);
	strcpy_s (this->name, VS_MAXCHARLEN, name);
	strcpy_s (this->value, VS_MAXCHARLEN, value);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
*/
VNode::PNode::PNode(VNode *v, PNode *p)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->first_child != 0)
		{
			parent->last_child->next = this;
			previous = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			previous = 0;
		}
	}
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
\param CHAR * - name
*/
VNode::PNode::PNode(VNode *v, PNode *p, CHAR *name)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->first_child != 0)
		{
			parent->last_child->next = this;
			previous = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			previous = 0;
		}
	}
	strcpy_s (this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
\param CHAR * - name
*/
VNode::PNode::PNode(VNode *v, CHAR *name, PNode *p)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->first_child != 0)
		{
			parent->first_child->previous = this;
			next = parent->first_child;
			parent->first_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			previous = 0;
		}
	}
	strcpy_s (this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
\param CHAR * - name
*/
VNode::PNode::PNode(VNode *v, CHAR *name, PNode *p, UINT sense)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->last_child != 0)
		{
			parent->last_child->next = this;
			previous = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			next = 0;
		}
	}
	strcpy_s(this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
\param const CHAR * - name
*/
VNode::PNode::PNode(VNode *v, PNode *p, const CHAR *name)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->first_child != 0)
		{
			parent->last_child->next = this;
			previous = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			previous = 0;
		}
	}
	strcpy_s (this->name, VS_MAXCHARLEN, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - owner
\param PNode * - parent
\param CHAR * - name
\param CHAR * - value
*/
VNode::PNode::PNode(VNode *v, PNode *p, CHAR *name, CHAR *value)
{
	Initialise(v);
	parent = p;
	if (parent != 0)
	{
		if (parent->first_child != 0)
		{
			parent->last_child->next = this;
			previous = parent->last_child;
			parent->last_child = this;
		}
		else
		{
			parent->first_child = this;
			parent->last_child = this;
			previous = 0;
		}
	}
	strcpy_s (this->name, VS_MAXCHARLEN, name);
	strcpy_s (this->value, VS_MAXCHARLEN, value);
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
VNode::PNode::~PNode ()
{}


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
\param VNode * - owner
*/
void VNode::PNode::Initialise(VNode *v)
{
	// store pointer to parent VNode
	vnode = v;

	// name and value, type, etc.
	name[0]  = 0;
	value[0] = 0;
	type     = 'N';
	format   = 0;
	quotes   = 0;
	touched  = false;
	visible  = true;

	// Hierarchical links
	parent   = NULL;
	previous = NULL;
	next     = NULL;
	first_child = NULL;
	last_child  = NULL;

	// Non-hierarchical links
	link_p = NULL;
	link_s = NULL;

	// Parameters
	//param = 0;

	// Page view
	//view = 0;

	// Associated
	associated = 0;
}


// ---------- Add PNode ----------
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param VNode * - owner
*/
VNode::PNode *VNode::PNode::Add(VNode *v)
{
	PNode *p = new PNode(v);
	return p;
}


//  ---------- Add PNode ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param VNode * - owner
\param CHAR * - name
*/
VNode::PNode *VNode::PNode::Add(VNode *v, CHAR *name)
{
	PNode *p = new PNode(v, name);
	return p;
}


//  ---------- Add PNode ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param VNode * - owner
\param const CHAR * - name
*/
VNode::PNode *VNode::PNode::Add(VNode *v, const CHAR *name)
{
	PNode *p = new PNode(v, name);
	return p;
}


//  ---------- Add PNode ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param VNode * - owner
\param CHAR * - name of new instance
\param CHAR * - value of new instance
*/
VNode::PNode *VNode::PNode::Add(VNode *v, CHAR *name, CHAR *value)
{
	PNode *p = new PNode(v, name, value);
	return p;
}


// ---------- Search ----------
/*!
\brief Search recursively for name or value
\author Gareth Edwards
\param CHAR * - search string
\param UINT * - search token (e.g. NAME == 1, VALUE == 2;
*/
VNode::PNode *VNode::PNode::Search(CHAR *str, UINT token)
{
	if ( !strcmp( name,  str) ) return this;
	PNode *found = NULL;
	PNode *p = first_child;
	while (p)
	{
		if ( found = p->Search( str, token) )
			return found;
		p = p->next;
	}
	return VS_ERROR;
}


// ---------- SearchThis ----------
/*!
\brief Search only this recursively for name or value
\author Gareth Edwards
\param CHAR * - search string
\param UINT * - search token (e.g. NAME == 1, VALUE == 2;
*/
VNode::PNode *VNode::PNode::SearchThis(CHAR *str, UINT token)
{
	PNode *p = first_child;
	while (p)
	{
		switch (token)
		{
			case NAME  :
				if ( !strcmp( p->name,  str) )
					return p;
				else break;
			case VALUE :
				if ( !strcmp( p->value, str) )
					return p;
				else break;
			default:
				return VS_ERROR;
		}
		p = p->next;
	}
	return VS_ERROR;
}


// ---------- GetRoot ----------
/*!
\brief Search recursively for root node
\author Gareth Edwards
*/
VNode::PNode *VNode::PNode::GetRoot()
{
	PNode* node = this;
	while (node)
	{
		PNode* parent = node->parent;
		if ( parent == NULL )
			return node;
		else
			node = parent;
	}
	return NULL;
}


// ---------- GetRootList ----------
/*!
\brief Search for "name" root list
\author Gareth Edwards
*/
VNode::PNode *VNode::PNode::GetRootList(CHAR *list, CHAR *name)
{
	if ( list == NULL ) return NULL;
	if ( name == NULL ) return NULL;
	if ( PNode *root = GetRoot() )
		if ( PNode *define = root->Search(list, NAME) )
			if ( PNode *definition = root->Search(name, NAME) )
				return definition;
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
VNode::VNode()
{
	pnode = new PNode(this);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name
*/
VNode::VNode(CHAR *name)
{
	pnode = new PNode(this, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name
*/
VNode::VNode(const CHAR *name)
{
	pnode = new PNode(this, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name
*/
VNode::VNode(CHAR *name, CHAR *value)
{
	pnode = new PNode(this, name, value);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - parent
*/
VNode::VNode(VNode *v)
{
	pnode = new PNode(this, v->pnode);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - parent
\param CHAR * - name
*/
VNode::VNode(VNode *v, CHAR *name)
{
	pnode = new PNode(this, v->pnode, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name
\param VNode * - parent
*/
VNode::VNode(CHAR *name, VNode *v)
{
	pnode = new PNode(this, name, v->pnode);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name
\param VNode * - parent
*/
VNode::VNode(CHAR *name, VNode *v, UINT sense)
{
	pnode = new PNode(this, name, v->pnode, sense);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - parent
\param const CHAR * - name
*/
VNode::VNode(VNode *v, const CHAR *name)
{
	pnode = new PNode(this, v->pnode, name);
}


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param VNode * - parent
\param CHAR * - name
*/
VNode::VNode(VNode *v, CHAR *name, CHAR *value)
{
	pnode = new PNode(this, v->pnode, name, value);
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
VNode::~VNode()
{
	if ( pnode != NULL ) { delete pnode; pnode = NULL; }
}


// ---------- Add ----------
/*!
\brief Add child to 'this'
\author Gareth Edwards
*/
VNode *VNode::Add()
{
	VNode *v = new VNode(this);
	return v;
}


// ---------- Add ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param CHAR * - name
*/
VNode *VNode::Add(CHAR *name)
{
	VNode *v = new VNode(this, name);
	return v;
}


// ---------- Add ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param const CHAR * - name
*/
VNode *VNode::Add(const CHAR *name)
{
	VNode *v = new VNode(this, name);
	return v;
}


// ---------- Add ---------- 
/*!
\brief Add child to 'this'
\author Gareth Edwards
\param CHAR * - name
\param CHAR * - value
*/
VNode *VNode::Add(CHAR *name, CHAR *value)
{
	VNode *v = new VNode(this, name, value);
	return v;
}


// ---------- Append ---------- 
/*!
\brief Append child to 'this'
\author Gareth Edwards
\param CHAR * - name
*/
VNode *VNode::Append(VNode* parent, CHAR *name)
{
	VNode *v = new VNode(parent, name);
	return v;
}


// ---------- Delete ---------- 
/*!
\brief Recursively delete from and including 'this' 
\author Gareth Edwards
*/
INT VNode::Delete()
{
	VNode *node = this;
	while (node)
	{
		VNode *next = node->GetNext();
		VNode *first = node->GetFirst();
		if (first != NULL) first->Delete();
		delete node;
		node = next;
	}
	return VS_OK;
}


// ---------- Get property ----------
CHAR * VNode::GetName()        {
	return pnode->name[0]     == NULL ? NULL : 
	pnode->name;
}
CHAR * VNode::GetValue()       { return pnode->value[0]    == NULL ? NULL :  pnode->value; }
BOOL   VNode::GetQuotes()      { return pnode->quotes; }
CHAR   VNode::GetType()        { return pnode->type; }
VNode *VNode::GetParent()      { return pnode->parent      == NULL ? NULL : (VNode*)pnode->parent     ->vnode; }
VNode *VNode::GetFirst()       { return pnode->first_child == NULL ? NULL : (VNode*)pnode->first_child->vnode; }
VNode *VNode::GetLast()        { return pnode->last_child  == NULL ? NULL : (VNode*)pnode->last_child ->vnode; }
VNode *VNode::GetLinkP()       { return pnode->link_p; }
VNode *VNode::GetLinkS()       { return pnode->link_s; }
INT    VNode::GetAssociated()  { return pnode->associated; }
VNode *VNode::GetPrevious()    { return pnode->previous    == NULL ? NULL : (VNode*)pnode->previous   ->vnode; }
VNode *VNode::GetNext()        { return pnode->next        == NULL ? NULL : (VNode*)pnode->next       ->vnode; }


// ---------- GetListItem ----------
VNode *VNode::GetListItem(CHAR *list, CHAR *item)
{
	if ( list == NULL ) return NULL;
	if ( PNode *root = pnode->GetRoot() )
		if ( PNode *l = root->Search(list, pnode->NAME) )
			if ( PNode *i = l->Search(item, pnode->NAME) )
			return i->vnode;
	return NULL;
}


// ---------- SetParent ----------
INT VNode::SetParent(VNode *node)
{ 
	if ( node == NULL) return VS_ERROR;
	node->pnode->parent = NULL;
	return VS_OK;
}


// ---------- SetLinkP ----------
INT VNode::SetLinkP(VNode *node)
{
	pnode->link_p = node;
	return VS_OK;
}


// ---------- SetLinkS ----------
INT VNode::SetLinkS(VNode *node)
{ 
	pnode->link_s = node;
	return VS_OK;
}


// ---------- SetAssociated ----------
INT VNode::SetAssociated(INT associated)
{ 
	pnode->associated = associated;
	return VS_OK;
}


// ---------- SearchName ---------- 
VNode *VNode::SearchName(CHAR *str)
{
	VNode::PNode *p = pnode->Search(str, pnode->NAME);
	if ( p ) return (VNode*)p->vnode;
	return NULL;
}


// ---------- SearchThis ----------
VNode *VNode::SearchThis(CHAR *str)
{
	VNode::PNode *p = pnode->SearchThis(str, pnode->NAME);
	if (p) return (VNode*)p->vnode;
	return NULL;
}


// ---------- SearchByNameToGetValue ----------
INT VNode::SearchValue(CHAR *dest, INT maxlen, CHAR *str)
{
	VNode::PNode *p = pnode->Search(str, pnode->VALUE);
	if ( p )
	{
		strcpy_s(dest, maxlen, p->value);
		return VS_OK;
	}
	return 0;
}

// ---------- GetBoolFromValue ----------
BOOL VNode::GetBoolFromValue()
{
	PNode* p = pnode;
	if ( pnode->value[0] == '$' )
		p = pnode->GetRootList("define", &pnode->value[1]);
	return p != NULL && p->value[0] == '0' ? false : true;
}


// ---------- GetFloatFromValue ----------
FLOAT VNode::GetFloatFromValue()
{
	PNode* p = pnode;
	if ( pnode->value[0] == '$' )
		p = pnode->GetRootList("define", &pnode->value[1]);
	return p != NULL && p->value[0] != NULL ? (FLOAT)atof(p->value) : 0;
}


// ---------- GetIntFromValue ----------
INT VNode::GetIntFromValue()
{
	PNode* p = pnode;
	if ( pnode->value[0] == '$' )
		p = pnode->GetRootList("define", &pnode->value[1]);
	return p != NULL && p->value[0] != NULL ? (INT)atoi(p->value) : 0;
}


// ---------- Set property ----------
VOID VNode::SetQuotes(BOOL q)  { pnode->quotes=q; }
VOID VNode::SetType(CHAR t)    { pnode->type=t; }
VOID VNode::SetValue(CHAR  *v) { strcpy(pnode->value, v); }
VOID VNode::SetValue(FLOAT  v) { CHAR c[64]; sprintf_s(c, 64, "%f", v); strcpy(pnode->value, c); }
VOID VNode::SetValue(INT    v) { CHAR c[64]; sprintf_s(c, 64, "%d", v); strcpy(pnode->value, c); }


////////////////////////////////////////////////////////////////////////////////


// ---------- SnipSpaces ----------
/*!
\brief Snip spaces
\author Gareth Edwards
\param CHAR * - result string
\param CHAR * - original string
\param UINT - length of original string
\return INT - ERROR <= 0 < VS_OK
*/
INT SnipSpaces(CHAR *r, CHAR *o, UINT otop)
{
	*r = 0;
	if (otop)
	{
		INT i = 0;
		INT start = 0;
		INT end = 0;
		while (i<(INT)otop)
		{
			if (*(o + i) != ' ')
			{
				start = i;
				i = (INT)otop + 1;
			}
			else
			{
				i++;
			}
		}
		i = (INT)otop - 1;
		while (i >= 0)
		{
			if (*(o + i) != ' ')
			{
				end = i;
				i = -1;
			}
			else
			{
				i--;
			}
		}
		if (end - start >= 0) {
			INT j = 0;
			for (i = start; i <= end; i++)
			{
				*(r + j++) = *(o + i);
			}
			*(r + j) = 0;
			return VS_OK;
		}
	}
	return VS_ERROR;
}


// ---------- Copy ----------
/*!
\brief Copy vnode data set
\author Gareth Edwards
\param 
\return vNode * - If VS_ERROR then failed
*/
VNode *VNodeIO::Copy(VNode* dest, VNode* src)
{
	 
	VNode *src_child = src->GetFirst();
	while ( src_child )
	{
		VNode *dest_child = NULL;
		if ( src_child->GetValue() != NULL )
			dest_child = dest->Add(src_child->GetName(), src_child->GetValue());
		else
			dest_child = dest->Add(src_child->GetName());
		if (src_child->GetFirst()) this->Copy(dest_child, src_child);
		src_child = src_child->GetNext();
	}

	return VS_ERROR;
}


// ---------- Read ----------
/*!
\brief Read data file
\author Gareth Edwards
\param CHAR *(filename)
\return vNode * - If VS_ERROR then failed
\note TODO memory leak on error due to non-cleanup of allocated nodes
*/
VNode *VNodeIO::Read (CHAR *filename)
{

    // IF -- stream open -- THEN -- read
	CHAR line[VS_MAXCHARLEN];
	std::ifstream myfile (filename);
	if ( myfile.is_open() )
	{

		// ---- keyword/value pair ----
		CHAR keyword[VS_MAXCHARLEN];
		CHAR value[VS_MAXCHARLEN];


		// ---- iit keyword/value pair ----
		keyword[0] = 0;
		value[0]   = 0;


		// ---- init Stack - TODO: Catch stack overflow ----
		VNode *stack[64];
		for (UINT i=0; i<32; i++) stack[i] = NULL;
		INT stackTop = 0;
		INT stackMax = 64;


		// ---- init  ----
		VNode *root = new VNode("root");
		stack[0]    = root;


		// ---- status ----
		BOOL comment;
		BOOL quote, quotes;
		BOOL ok;
		BOOL visible = true;


		// ---- parse ----
		INT   c;
		INT   lineIndex;
		CHAR  cStack[VS_MAXCHARLEN];
		INT   cTop = 0;
		cStack[cTop] = '\0';


		// FOR -- each line
		INT lineCount = 0;
		while( !myfile.getline(line, VS_MAXCHARLEN).eof() )
		{
			// ---- parse loop ----
			comment   = false;
			quote     = false;
			quotes    = false;
			ok        = true;
			lineIndex = 0;
			cTop      = 0;
			while ( (c=line[lineIndex++]) != 0 )
			{
				switch ( c )
				{

				case '\t' :	// ---- ignore tab ----
					break;

				case '#' :	// ---- comment ----
					//if (lineIndex == 1)
					comment = true;
					break;

				case '"' :	// ---- quote ----
					if ( quote )
					{
						quote = false;
						quotes = true;
					}
					else
					{
						quote = true;
					}
					break;

				case ':' :	// ---- snip keyword ----

					// IF -- NOT comment AND NOT "between quotes" -- THEN -- begin value
					if ( !comment && !quote )
					{
						if ( ok = SnipSpaces(keyword, cStack, cTop) ) {
							cTop = 0;
						} else {
							keyword[0] = 0;
							continue;
						}
					}
					else cStack[cTop++] = c;
					break;

				case ';' :	//  snip value & end pair ----

					// IF -- NOT comment AND NOT "between quotes" -- THEN -- end value pair
					if ( !comment && !quote )
					{
						if ( ok = SnipSpaces(value, cStack, cTop) )
						{

							// IF -- no keyword -- THEN -- it is a parameter
							//
							// TBD the readable size is '256' bytes, but '-4' bytes may be read.
							//
							VNode *n = 0;
							if (keyword[0] == 0)
								n = stack[stackTop]->Add(value,"");  // TBD !!!
							else
								n = stack[stackTop]->Add(keyword,value);

							// ---- catch text type ----
							if ( quotes )
							{
								n->SetType('T');
								n->SetQuotes(true);
							}

							// ---- re-init ----
							keyword[0] = 0;
							value[0] = 0;
							cTop = 0;

						}
					
						// ---- re-init ----
						else
						{
							keyword[0] = 0;
							continue;
						}
					}
					break;

				case '{' :	// ---- push/begin ----

					// IF -- NOT comment AND NOT "between quotes" -- THEN -- push
					if ( !comment && !quote )
					{

						// IF - not overflow -- THEN
						if ( stackTop < VS_MAXCHARLEN )
						{

							// IF -- keyword THEN -- snip value and begin {
							VNode *lst = 0;
							if ( keyword[0] )
							{
								ok = SnipSpaces(value, cStack, cTop);
								lst = stack[stackTop]->Add(keyword, value);
							}

							// ELSE -- begin {
							else
							{
								ok = SnipSpaces(keyword, cStack, cTop);
								lst = stack[stackTop]->Add(keyword);
							}

							// ---- catch text ----
							if ( quotes ) stack[stackTop]->SetType('T');

							// ---- stack ----
							stack[++stackTop] = lst;

						}

						// ---- re-init
						cStack[cTop] = '\0';
						cTop = 0;
						keyword[0] = 0;
						value[0] = 0;
					}
					break;

				case '}' :	// ---- pop/end ----

					// IF -- NOT comment AND NOT "between quotes" -- THEN -- pop
					if ( !comment && !quote )
					{
						stackTop--;
						cStack[cTop] = '\0';
						cTop = 0;
					}
					break;

				default : // ---- accumulate ----
					cStack[cTop++] = c;
					break;

				}
			}


			// IF --comment -- THEN
			if ( comment )
			{

				// IF -- first line -- THEN
				if ( lineCount==0 )
				{
					// IF -- NOT HDL filetype 4 -- THEN -- return error
					CHAR doctype[32];
					sprintf(doctype, "#DOCTYPE %s", DOCTYPE);
					if ( strcmp(doctype, line) )
					{
						root->Delete();
						return (VNode*)VS_VNODE_READERROR;
					}
				}
			}

			// ---- reset comment ----
			comment = false;

			// ---- increment line count ----
			lineCount++;
	
			// IF -- error -- THEN -- report
			if ( !ok )
			{
				root->Delete();
				return (VNode*)VS_VNODE_READERROR;
			}
		}


		// IF -- parse not complete -- THEN -- check for final pop
		if ( myfile.gcount() > 0 ) {
			if ( ok = SnipSpaces(keyword, line, (int)myfile.gcount()) )
			{
				if (keyword[0] == '}')
				{
					stackTop--;
				}
			}
		}


		// ---- close stream ----
		myfile.close();


		// ---- exceptions ----
		try
		{
			if ( stackTop < 0 )
				throw (VS_VNODE_UNDERFLOW);
			else if ( stackTop > 0 )
				throw (VS_VNODE_OVERFLOW);
			else if ( lineCount == 0 || stack[1] == 0 )
				throw (VS_VNODE_EMPTYFILE);
		}
		catch (INT error)
		{
			root->Delete();
			return (VNode*)error;
		}


		// ---- cleanup ----
		stack[1]->SetParent(NULL);

		// ---- done ----
		return stack[1];
	}

	// ELSE -- stream NOT open
	else
	{
		return (VNode*)VS_ERROR;
	}

    return (VNode*)VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write data file
\author Gareth Edwards
\param CHAR * - filename
\param VNode * - data
\param BOOL - if TRUE then write touched
\param BOOL - if TRUE then write parameters
\return INT - error code
*/
INT VNodeIO::Write(CHAR *filename, VNode *data)
{ 

	// ---- set flags ----
	// _writeTouched = touched;
	// _writeParam = param;


    // IF -- open THEN write --
	std::ofstream myfile (filename);
	if (myfile.is_open())
	{

		// ---- write VNode document type ----
		myfile << "#DOCTYPE " << DOCTYPE << "\n";

		// ---- recursively write ----
		int count = 0;
		int indent = 0;
		Write(&myfile, data, &count, &indent);
		myfile.close();
		return true;

	}
	// ELSE -- stream NOT open
	else
	{
		return VS_VNODE_OPENFILEFAILED;
	}

    return VS_OK;
}


// ---------- Write ----------
/*!
\brief Write data file
\author Gareth Edwards
\param CHAR * - filename
\param VNode * - data
\param INT * - line counter
\param INT * - indent
\return INT - error code
*/
INT VNodeIO::Write(
		std::ofstream *myfile,
		VNode *data,
		INT *count,
		INT *indent
	)
{

	// IF -- write touched AND this NOT touched -- THEN -- return
	// if (_writeTouched && !data->touched)
	//	 return false;


	// ---- local ----
	//
	// TBD Possibly incorrect order of operations:  dereference ignored.
	//
		INT index;
		*count++; 


	// ---- indent
		CHAR t[VS_MAXCHARLEN];
		for ( index = 0; index < *indent; index++ ) t[index] = '\t';


	// ---- name
		CHAR c = 0;
		if ( data->GetName() != 0 )
		{
			INT o = 0;
			while ( c = *(data->GetName()+o) != 0 )
			{
				t[index++] = *(data->GetName()+o++);
			}
		}


	// ---- value
		if ( data->GetValue() != 0 )
		{

			// ---- seperator
				if ( data->GetName() != 0 ) t[index++] = ':';

			// ---- begin quotes
				BOOL isQuoted = data->GetQuotes();
				if ( isQuoted ) t[index++] = '"';

			//	---- acc
				INT len = strlen(data->GetValue());

			// ---- snip trailing 0's
				INT len_value = len;
				if ( len > 2 )
				{

					// ---- local
						CHAR *v = data->GetValue();
						INT max_len = len - 1;

					// -- float ?
						BOOL isFloat= FALSE;
						for (INT i = 0; i < max_len; i++)
						{
							if (*(v + i) == '.')
							{
								isFloat = TRUE;
								break;
							}
						}

					// ---- float ?
						if ( isFloat )
						{
							for (INT i = max_len; i > 0; i--)
							{
								CHAR c = *(v+i);
								if ( c == '0' )
								{
									CHAR d = *(v+i-1);
									if ( d != '0' )
									{
										switch (d)
										{
											case '.': 
											case '-':
												len_value = i + 1;
												break;
											default:
												len_value = i + 1;
												break;
										}
										break;
									}
								}
								else
								{
									len_value = i+1;
									break;
								}
							}
						}

				}

			// ---- add
				INT o = 0;
				while ( o < len_value )
				{
					t[index++] = *(data->GetValue()+o);
					o++;
				}

			// ---- end quotes
				if ( isQuoted ) t[index++] = '"';

		}


	// ---- push or terminate ----
		bool push = data->GetParent() == 0 || data->GetFirst();


	// ---- push
		if ( push )
		{
			t[index++] = '{';
			t[index] = 0;
		}
		else
		{
			t[index++] = ';';
			t[index++] = 0;
		}


	// ---- write
		*myfile << t;
		*myfile << "\n";


	// ---- push & indent
		*indent += 1;


	// ---- children
		VNode *child = data->GetFirst();
		while ( child ) {
			Write(myfile, child, count, indent);
			child = child->GetNext();
		}

	// ---- pop & indent
		*indent -= 1;


	// ---- push?
		if ( push )
		{
			for (index = 0; index<*indent; index++)
			{
				t[index] = '\t';
			}
			t[index++] = '}';
			t[index]   = 0;
			*myfile << t;
			*myfile << "\n";
	}

    return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////