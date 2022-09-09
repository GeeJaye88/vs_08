////////////////////////////////////////////////////////////////////////////////

// ---------- vs_reference.cpp ----------
/*!
\file vs_reference.cpp
\brief Reference patterns and techniques
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_reference.h"


using namespace vs_system;


// ---------- Private IMPLementation class----------
class RefObject::PIMPL
{
public:
	int i;
};

// ---------- Constructor ----------
RefObject::RefObject()
{
    pimpl = new PIMPL;
}

// ---------- Constructor ----------
RefObject::~RefObject()
{
    if (pimpl)
        delete pimpl;
}

// ---------- Method referencing by pimpl ----------
void RefObject::test()
{
	pimpl->i = 5;
}
