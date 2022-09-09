////////////////////////////////////////////////////////////////////////////////

// ---------- e1_system_object.h ----------
/*!
\file e1_system_object.h
\brief Interface for the SystemObject class.
\author Gareth Edwards
*/

#pragma once


////////////////////////////////////////////////////////////////////////////////


namespace surface_01
{

	// ---------- space object ----------
	class SystemObject
	{

		public:



		// ---- object type
			enum Type
			{
				UnKnown   = 0,
				Star      = 1,
				Planet    = 2,
				GasGiant  = 3,
				Dwarf     = 4,
				Moon      = 5,
				Asteroid  = 6,
				AstRing   = 7,
				Meteroid  = 8,
				Starfield = 9
			};


		// ---- cdtor
			SystemObject();
			SystemObject(SystemObject *parent, CHAR *n);
			virtual ~SystemObject();


		// ---- framework
			INT Setup();


		// ---- get/set properties
			INT   Set(CHAR*, FLOAT);
			INT   Set(CHAR*, CHAR*);
			INT   SetIndex(INT);
			FLOAT Get(CHAR*);
			UINT  GetIndex();
			CHAR *GetType(INT);
			CHAR *GetGroup();
			CHAR *GetName();


		// ---- get/set links
			SystemObject* GetParent();
			SystemObject* GetFirst();
			SystemObject* GetNext();

			SystemObject *Search(SystemObject *, CHAR *);

	private:

		// ---- private implementation
			class  PI_SystemObject;
			PI_SystemObject *pi_system_object;

	};

}


////////////////////////////////////////////////////////////////////////////////
