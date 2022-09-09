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


// -------- libray function --------
/*!
\brief using libray function
\author Gareth Edwards
*/
VOID application::cpp_2_1_function_lib()
{

	// ---- local
		using namespace std;


	// ---- input
	    double number, squareRoot;
		cout << "Enter a number: ";
		cin >> number;


    // ---- sqrt() is a library function to calculate square root
	  squareRoot = sqrt(number);
	  cout << "Square root of " << number << " = " << squareRoot << endl;;

	return;
}


// -------- user function defintion --------
/*!
\brief User defined function
\author Gareth Edwards
*/

int add(int, int);

VOID application::cpp_2_2_function_user()
{

	// ---- local
		using namespace std;


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

	// ---- do something
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

	// ---- local
		using namespace std;

	// ---- no argument is passed to prime()
		prime1();

	return;
}


void prime1() // Note: Return type of function is void because value is not returned.
{

	// ---- local
		using namespace std;
		int num, i, flag = 0;


	// ---- input
		cout << "Enter a positive integer enter to check: ";
		cin >> num;


	// ---- do it...
		for(i = 2; i <= num/2; ++i)
		{
			if(num % i == 0)
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
		using namespace std;
		int num, i, flag = 0;


    // ---- no argument is passed to prime2()
		num = prime2();
		for (i = 2; i <= num/2; ++i)
		{
			if (num%i == 0)
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
		using namespace std;
		int n;

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
		using namespace std;
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

	// ---- local
		using namespace std;

		
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
		using namespace std;
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

