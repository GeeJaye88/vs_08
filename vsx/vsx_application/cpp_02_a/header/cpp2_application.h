////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_application.h ----------
/*!
\file cpp2_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++

//string str;
//getline(std::cin, str);
//printf(str.c_str());
//printf("\n");


*/


#pragma once


// ---- include
#include <stdio.h>   // FILE, 
#include <string.h>  // printf,

#include <string>

#include <ctype.h>   // toupper, 
#include <iostream>


// ---- system


#include "../../../../vs/vs_system/header/vs_inc_defs.h"
#include "../../../../vs/vs_system/header/vs_app_dependency.h"


// ---------- namespace ----------
namespace cpp_02
{

	// ---------- application class ----------
	class application
	{

	public:

		// ---- c/dtor
			application();
			virtual ~application();


		// ---- framework
			INT Setup(vs_system::AppDependency *);
			INT Display();
			INT Cleanup();
			

		// ---- flow control
			VOID cpp_1_1_if_original();
			VOID cpp_1_2_if();
			VOID cpp_1_3_for();
			VOID cpp_1_4_loop();
			VOID cpp_1_5_break();
			VOID cpp_1_6_continue();
			VOID cpp_1_7_goto();


		// ---- functions
			VOID cpp_2_1_function_lib();
			VOID cpp_2_2_function_user();
			VOID cpp_2_3_no_arg_no_return();
			VOID cpp_2_3_no_arg();
			VOID cpp_2_3_arg();
			VOID cpp_2_3_arg_and_return();


	private:

		// ----p rivate implementation
		class PI_Application; static PI_Application *pi_app;

	};


}