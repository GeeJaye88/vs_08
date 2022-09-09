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
			VOID cpp_2_4_default_arg();
			VOID cpp_2_5_local_var();
			VOID cpp_2_5_global_var();
			VOID cpp_2_5_static_var();
			VOID cpp_2_5_register_var();
			VOID cpp_2_5_thread_local();
			VOID cpp_2_6_recursion();
			VOID cpp_2_7_pass_by_ref();
			VOID cpp_2_8_pass_by_val();
			VOID cpp_2_9_ret_by_ref();


		// ---- arrays & string
			VOID cpp_3_1_arrays();
			VOID cpp_3_2_2D_array_display();
			VOID cpp_3_2_2D_array_store();
			VOID cpp_3_2_3D_array();
			VOID cpp_3_3_pass_array_to_function();
			VOID cpp_3_3_pass_multi_array_to_function();
			VOID cpp_3_4_c_string();
			VOID cpp_3_4_c_string_get();
			VOID cpp_3_4_str_string();
			VOID cpp_3_4_pass_str_to_function();

		// ---- structures


		// ---- object & class


		// ---- arrays & strings


		// ---- inheritance


	private:

		// ---- private implementation
		class PI_Application; static PI_Application *pi_app;

	};


}