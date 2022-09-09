////////////////////////////////////////////////////////////////////////////////

// ---------- e1_system_object.h ----------
/*!
\file e1_system_object.h
\brief Interface for the SystemObject class.
\author Gareth Edwards
*/

#pragma once

#include "../header/e1_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace examples_01
{

	// ---------- space object ----------
	class SystemObject
	{

		public:

		// ---- gfx mode
			enum Mode
			{
				UNKNOWN      = 0,
				NORMAL       = 1,
				LARGE        = 2,
				ILLUSTRATION = 3
			};

		// ---- object type
			enum Type
			{
				UnKnown  = 0,
				Star     = 1,
				Planet   = 2,
				GasGiant = 3,
				Dwarf    = 4,
				Moon     = 5,
				Asteroid = 6,
				AstRing  = 7,
				Meteroid = 8,
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


	// ---------- system parameters ----------
	class SystemParam
	{

	public:

		// ---- gfx mode
		enum
		{
			MAX_SYS_OBJECT = 128
		};

		// ---- cdtor
			SystemParam();
			virtual ~SystemParam();


		// ---- gfx mode
			UINT  GetGfxMode();
			UINT  IncGfxMode();
			VOID  SetGfxMode(UINT);


		// ---- pause mode
			INT    GetPauseMode();
			VOID   SetPauseMode(INT);
			DOUBLE GetPauseTimeStart();
			VOID   SetPauseTimeStart(DOUBLE);
			DOUBLE GetPauseTimeElapsed();
			VOID   SetPauseTimeElapsed(DOUBLE);


		// ---- get
			DOUBLE GetGlobalScale();
			DOUBLE GetObjectScale();
			DOUBLE GetTimeScalar();
			DOUBLE GetDaylenScalar();
			LONG   GetMsOffset();
			FLOAT  GetTargetDistScalar();


		// ---- set
			INT SetScale(DOUBLE);
			INT SetObjectScale(DOUBLE);
			INT SetTimeScalar(DOUBLE);
			INT SetDaylenScalar(DOUBLE);
			INT SetMsOffset(LONG);

	private:

		// ---- private implementation
			class  PI_SystemParam;
			PI_SystemParam *pi_system_param;

	};


	// ---------- system object io ----------
	class SystemIO
	{

		public:

		// ---- cdtor
			SystemIO() {};
			virtual ~SystemIO() {};

		// ---- methods
			INT Read(SystemObject *);
	};

}


////////////////////////////////////////////////////////////////////////////////
