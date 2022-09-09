////////////////////////////////////////////////////////////////////////////////

// ---------- vs_node.h ----------
/*!
\file vs_node.h
\brief Interface for the Node class.
\author Gareth Edwards
*/

#pragma once


// ---- include ----
#include <stdio.h>        // 
#include <ctype.h>        // 
#include <string>         // std::string
#include <iostream>       // std::cout
#include <stack>          // std::stack
#include <vector>         // std::vector
#include <deque>          // std::deque


// ---- vs include ----
#include "../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


// ---------- Node Class ----------
class Node
{

public:

	// ---------- Constructor ----------
	/*!
	\brief Constructor
	\author Gareth Edwards
	*/
	Node::Node()
	{
		;
	}

	// ---------- Destructor ----------
	/*!
	\brief Constructor
	\author Gareth Edwards
	*/
	Node::~Node()
	{
		;
	}

private:

	// ---------- Properties ----------

	CHAR name[VS_MAXCHARLEN]= "";
	Node* parent   = NULL;
	Node* first    = NULL;
	Node* last     = NULL;
	Node* back     = NULL;
	Node* forward  = NULL;
	Node* previous = NULL;
	Node* next     = NULL;

public:

	// ---------- Methods ----------

	// Get
	CHAR  *GetName()         { return name;     };
	Node  *GetParent()       { return parent;   };
	Node  *GetFirstChild()   { return first;    };
	Node  *GetLastChild()    { return last;     };
	Node  *GetBack()         { return back;     };
	Node  *GetForward()      { return forward;  };
	Node  *GetPrevious()     { return previous; };
	Node  *GetNext()         { return next;     };

	// Set
	INT SetName(CHAR *n)     { strcpy(name, n); };
	INT SetParent(Node *p)   { parent   = p;    };
	INT SetFirst(Node *f)    { first    = f;    };
	INT SetLast(Node *l)     { last     = l;    };
	INT SetBack(Node *b)     { back     = b;    };
	INT SetForward(Node *f)  { forward  = f;    };
	INT SetPrevious(Node *p) { previous = p;    };
	INT SetNext(Node *n)     { next     = n;    };

};


} // vs_system namespace



///////////////////////////////////////////////////////////////////////////////
