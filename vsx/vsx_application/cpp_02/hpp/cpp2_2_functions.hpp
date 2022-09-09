////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_2_functions.cpp ----------
/*!
\file cpp2_2_functions.cpp
\brief Implementation of the flow control examples
\author Gareth Edwards
\note Vanilla C++
*/


////////////////////////////////////////////////////////////////////////////////


// -------- Functions --------

using namespace std;

// -------- libray function --------
/*!
\brief using libray function
\author Gareth Edwards
*/

INT   my_sqrt(INT);
FLOAT my_sqrt(FLOAT);
CHAR  my_sqrt(CHAR);

VOID application::cpp_2_1_function_lib()
{

	// ---- input
	    double number, squareRoot;
		cout << "Enter a number: ";
		cin >> number;


    // ---- sqrt() is a library function to calculate square root
	  squareRoot = sqrt(number);
	  cout << "Square root of " << number << " = " << squareRoot << endl;


	// ---- function overloading
		  INT   int_square_root = my_sqrt((INT)number);
		  FLOAT float_square_root = my_sqrt((FLOAT)number);
		  CHAR  silly_square_root = my_sqrt((CHAR)((INT)number));
 

	// ---- note: camela case v '_' seperated


	// ---- note: google style c++ guide


	return;
}

INT my_sqrt(INT i)
{
	return (INT)sqrt((FLOAT)i);
}

FLOAT my_sqrt(FLOAT f)
{
	return sqrt(f);
}

CHAR my_sqrt(CHAR c)
{
	FLOAT result = sqrt((FLOAT)c);
	return (CHAR)((INT)result);
}


// -------- user function defintion --------
/*!
\brief User defined function
\author Gareth Edwards
*/

int add(int, int);

VOID application::cpp_2_2_function_user()
{

	// ---- input
		int num1, num2, sum;
		cout << "Enters two numbers to add: ";
		cin >> num1 >> num2;


	// ---- function call
		sum = add(num1, num2);
		cout << "Sum = " << sum << endl;

	return;
}


// -------- add function definition --------
int add(int a, int b)
{

	// ---- do something...
		int add;
		add = a + b;


	// ---- return statement
		return add;
}


// -------- functions: Example 1 --------
/*!
\brief No arguments passed and no return value
\author Gareth Edwards
*/

void prime1();

VOID application::cpp_2_3_no_arg_no_return()
{

	// ---- no argument is passed to prime()
		prime1();

	return;
}


void prime1() // Note: Return type of function is void because value is not returned.
{

	// ---- local
		int num, i, flag = 0;


	// ---- input
		cout << "Enter a positive integer enter to check: ";
		cin >> num;


	// ---- do something...
		for(i = 2; i <= num/2; ++i)
		{
			if( num % i == 0 )
			{
				flag = 1; 
				break;
			}
		}

		if (flag == 1)
		{
			cout << num << " is not a prime number.";
		}
		else
		{
			cout << num << " is a prime number.";
		}
}


// -------- functions: Example 2 --------
/*!
\brief No arguments passed but a return value
\author Gareth Edwards
*/

int prime2();

VOID application::cpp_2_3_no_arg()
{

	// ---- local
		int num, i, flag = 0;


    // ---- no argument is passed to prime2()
		num = prime2();
		for (i = 2; i <= num/2; ++i)
		{
			if ( num%i == 0 )
			{
				flag = 1;
				break;
			}
		}

		if (flag == 1)
		{
			cout<<num<<" is not a prime number.";
		}
		else
		{
			cout<<num<<" is a prime number.";
		}

	return;
}


int prime2() // Note: Return type of function is int
{

	// ---- local
		int n;


	// ---- input
		printf("Enter a positive integer to check: ");
		cin >> n;

	return n;
}


// -------- functions: Example 3 --------
/*!
\brief Arguments passed but no return value
\author Gareth Edwards
*/

void prime3(int);

VOID application::cpp_2_3_arg()
{

	// ---- local
		int num;


	// ---- input
		cout << "Enter a positive integer to check: ";
		cin >> num;


	// ---- argument num is passed to the function prime()
		prime3(num);

	return;
}


void prime3(int n) // There is no return value to calling function. Hence, return type of function is void. */
{
		
	// ---- do it
		int i, flag = 0;
		for (i = 2; i <= n/2; ++i)
		{
			if (n%i == 0)
			{
				flag = 1;
				break;
			}
		}

		if (flag == 1)
		{
			cout << n << " is not a prime number.";
		}
		else {
			cout << n << " is a prime number.";
		}
}


// -------- functions --------
/*!
\brief Example 4: Arguments passed and a return value.
\author Gareth Edwards
*/

int prime4(int);

VOID application::cpp_2_3_arg_and_return()
{

	// ---- local
		int num, flag = 0;


	// ----input
		cout << "Enter positive integer to check: ";
		cin >> num;


    // ---- argument num is passed to check() function
		flag = prime4(num);

		if(flag == 1)
			cout << num << " is not a prime number.";
		else
			cout<< num << " is a prime number.";

	return;
}


int prime4(int n) // This function returns integer value.
{

	int i;
	for (i = 2; i <= n / 2; ++i)
	{
		if (n % i == 0)
			return 1;
	}

	return 0;
}


// -------- default argument --------
/*!
\brief Demonstrate working of default argument
\author Gareth Edwards
*/

void my_default_arg(char = '*', int = 1);

VOID application::cpp_2_4_default_arg()
{

    cout << "No argument passed:\n";
	my_default_arg();
    

    cout << "\nFirst argument passed:\n";
	my_default_arg('#');
    

    cout << "\nBoth argument passed:\n";
	my_default_arg('$', 5);

    return;
}

void my_default_arg(char c, int n)
{

	// ---- do something....
		for(int i = 1; i <= n; ++i)
		{
			cout << c;
		}
		cout << endl;

}


// -------- Storage Class --------
/*!

\brief Demonstrate working of local variable
\author Gareth Edwards

\notes

	Every variable in C++ has two features: type and storage class.

	Type specifies the type of data that can be stored in a variable.
	For example: int, float, char etc.

	And, storage class controls two different properties of a variable:
	lifetime (determines how long a variable can exist) and scope
	(determines which part of the program can access it).

	Depending upon the storage class of a variable, it can be divided
	into 5 major types:

	1. Local variable
	2. Global variable
	3. Static local variable
	4. Register Variable
	5. Thread Local Storage

\notes

	A variable defined inside a function (defined inside function body
	between braces) is called a local variable or automatic variable.

	Its scope is only limited to the function where it is defined.
	In simple terms, local variable exists and can be accessed only
	inside a function.

	The life of a local variable ends (It is destroyed) when the
	function exits.

*/

void my_local_var_test();

VOID application::cpp_2_5_local_var()
{

	// ---- local variable to main()
		int var = 5;
		my_local_var_test();


	// ---- illegal: var1 not declared inside main()
		//var1 = 9;


	// ---- wait....
		cin >> var;

	return;
}

void my_local_var_test()
{

	// ---- local variable to test()
		int var1;
		var1 = 6;

	// ---- illegal: var not declared inside test()
		//cout << var;
}


// -------- global variable --------
/*!
\brief Demonstrate working of global variable
\author Gareth Edwards

\notes

	If a variable is defined outside all functions, then
	it is called a global variable.

	The scope of a global variable is the whole program.
	This means, It can be used and changed at any part of
	the program after its declaration.

	Likewise, its life ends only when the program ends.

*/

void my_global_var_test();
int my_global_var = 12;

VOID application::cpp_2_5_global_var()
{

	++my_global_var;

	cout << my_global_var << endl;  // outputs 13

	my_global_var_test();


	// ---- wait....
		int var = 0;
		cin >> var;

	return;
}

void my_global_var_test()
{

	++my_global_var;
	cout << my_global_var << endl;  // outputs 14

}


// -------- static variable --------
/*!
\brief Demonstrate working of static variable
\author Gareth Edwards

\notes

	A static local variable exists only inside a function
	where it is declared (similar to a local variable) but
	its lifetime starts when the function is called and
	ends only when the program ends.

	The main difference between local variable and static
	variable is that, the value of static variable persists
	the end of the program.

*/

void my_static_var_test();

VOID application::cpp_2_5_static_var()
{

	my_static_var_test();
	my_static_var_test();


	// ---- wait....
		int var = 0;
		cin >> var;

	return;
}

void my_static_var_test()
{
	static int var = 0;

	++var;

	/*

	static BOOL runtime = TRUE;

	if (runtime)
	{
		float fred = 1;
		runtime = FALSE;
	}

	*/

}


// -------- register variable --------
/*!
\brief Demonstrate working of register variable
\author Gareth Edwards

\notes

	Keyword register is used for specifying register variables.

	Register variables are similar to automatic variables and
	exists inside a particular function only. It is supposed
	to be faster than the local variables.

	If a program encounters a register variable, it stores the
	variable in processor's register rather than memory if
	available. This makes it faster than the local variables.

	However, this keyword was deprecated in C++11 and should not
	be used.

*/
VOID application::cpp_2_5_register_var()
{

	register int var = 12;

	return;
}


// -------- thread local variable --------
/*!
\brief Demonstrate working of register variable
\author Gareth Edwards

\notes

	Thread-local storage is a mechanism by which variables are
	allocated such that there is one instance of the variable
	per extant thread.

	Keyword thread_local is used for this purpose.

*/
VOID application::cpp_2_5_thread_local()
{

	thread_local int var = 12;

	return;
}


// -------- recursion --------
/*!
\brief Demonstrate a recursive function; a function that calls itself
\author Gareth Edwards

\notes

	The recursion continues until some condition is met.

	To prevent infinite recursion, if...else statement
	(or similar approach) can be used where one branch makes
	the recursive call and other doesn't.

*/

int my_recursion_factorial(int);

VOID application::cpp_2_6_recursion()
{

    int n;

    cout<<"Enter a number to find factorial: ";
    cin >> n;

    cout << "Factorial of " << n <<" = " << my_recursion_factorial(n) << endl;

    return;
}

int my_recursion_factorial(int n)
{
    if (n > 1) 
    {
        return n * my_recursion_factorial( n - 1 );
    }
    else 
    {
        return 1;
    }
}


// -------- pass by value --------
/*!
\brief Demonstrate pass by value
\author Gareth Edwards

\notes

	By default, non-pointer arguments in C++ are passed by value.
	When an argument is passed by value, the argument’s value is
	copied into the value of the corresponding function parameter.

	Using & a copy of the argument is passed to the function, and
	the original argument can not be modified by the function.

*/

VOID my_pass_by_val_test(int);

VOID application::cpp_2_8_pass_by_val()
{

	int var = 5;

	my_pass_by_val_test(var);
	cout << "After pass by value:" << var << endl;


	// ---- wait....
		cin >> var;

	return;
}

VOID my_pass_by_val_test(int var)
{
	var++;
	cout << "Passed by value:" << var << endl;
}


// -------- pass by reference --------
/*!
\brief Demonstrate pass by reference
\author Gareth Edwards

\notes

	When you pass a pointer as parameter you pass by reference.

	When you modify the object in the specified address, you modify
	the object in the caller function.

*/

VOID my_pass_by_ref_test(int *);
VOID my_pass_by_ref_test(int &);

VOID application::cpp_2_7_pass_by_ref()
{

	int var = 5;
	my_pass_by_val_test(var);
	cout << "After pass by value:" << var << endl;

	my_pass_by_ref_test(&var);
	cout << "After pass by * reference: " << var << endl;

	my_pass_by_ref_test(var);
	cout << "After pass by & reference: " << var << endl;


	// ---- wait....
		cin >> var;

	return;
}

VOID my_pass_by_ref_test(int *var)
{
	(*var)++;
	cout << "Passed by * reference:" << *var << endl;
}

VOID my_pass_by_ref_test(int &var)
{
	var++;
	cout << "Passed by & reference:" << var << endl;
}


// -------- return by reference --------
/*!
\brief Demonstrate return by value
\author Gareth Edwards

\notes

	Not only can you pass values by reference to a function
	but you can also return a value by reference.

*/

int & my_ret_by_ref_test_1();
int * my_ret_by_ref_test_2();
int   my_ret_by_ref_val = 6;

VOID application::cpp_2_9_ret_by_ref()
{

	cout << "Before return by reference 1:" << my_ret_by_ref_val << endl;

	my_ret_by_ref_test_1() = 5;
	cout << "After return by reference 1:" << my_ret_by_ref_val << endl;

	int var = *my_ret_by_ref_test_2();
	cout << "After return by reference 2:" << var << endl;


	// ---- wait....
		cin >> var;

	return;
}

int & my_ret_by_ref_test_1()
{
	return my_ret_by_ref_val;
}

int * my_ret_by_ref_test_2()
{
	return &my_ret_by_ref_val;
}
