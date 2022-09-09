////////////////////////////////////////////////////////////////////////////////

// ---------- vs_reference.h ----------
/*!
\file vs_reference.h
\brief Reference patterns and techniques
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_REFERENCE
#define VS_SYSTEM_REFERENCE


// ---- include ----
#include <string>


// ---------- namespace ----------
namespace vs_system
{


// ---------- PIMPL ----------
class RefObject
{

public:
	RefObject();
	~RefObject();

private:
	std::string m_name;
	class PIMPL;          // Forward declaration.
	PIMPL *pimpl;         // The opaque pointer.

private:
	void test();

}; // RefObject class


}  /// namespace


#endif // VS_SYSTEM_REFERENCE

