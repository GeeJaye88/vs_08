////////////////////////////////////////////////////////////////////////////////

// ---------- vs_parameter.h ----------
/*!
\file vs_parameter.h
\brief Interface for the Parameter class.
\author Gareth Edwards
*/


#pragma once


#include<iostream> 
#include<string.h>
#include "../../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- Parameter class ----------
	class Parameter
	{

		public:

			enum
			{
				MAX_PARAM_GROUP  = 16,
				MAX_PARAM_ARRAY  = 128,
				MAX_PARAM_CHAR   = 32,
				PARAM_ARRAY_SIZE = 10,
				PARAM_LID        = 0,
				PARAM_VALUE      = 1,
				PARAM_DEFAULT    = 2,
				PARAM_INCREMENT  = 3
			};

		public:

			// ---- c/dtor ----
				Parameter();
				virtual ~Parameter();


			// ---- methods included for facade
				#include "vs_parameter_methods.h"


		private:

			class PI_Parameter; PI_Parameter *_pi_parameter;

	};

};

