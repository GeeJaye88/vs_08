////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_3_arrays_and_strings.cpp ----------
/*!
\file cpp2_3_arrays_and_strings.cpp
\brief Implementation of the arrays and string examples
\author Gareth Edwards
\note Vanilla C++
*/


////////////////////////////////////////////////////////////////////////////////


// -------- Arrays and String --------

using namespace std;


// -------- arrays --------
/*!
\brief initialise and display an array
\author Gareth Edwards

\notes

	In programming, one of the frequently arising problem is to
	handle numerous data of same type.

	Consider this situation, you are taking a survey of 100 people
	and you have to store their age. To solve this problem in C++,
	you can create an integer array having 100 elements.

	An array is a collection of data that holds fixed number of
	values of same type. For example:

		int age[100];

	Here, the age array can hold maximum of 100 elements of
	integer type.

	The size and type of arrays cannot be changed after its
	declaration.

\notes

	How to declare an array in C++?

	dataType arrayName[arraySize];

\notes

	Elements of an Array and How to access them?

	You can access elements of an array by using indices.

	Suppose you declared an array mark as above.

	The first element is mark[0], second element is mark[1] and so on.

\notes:

	Arrays have 0 as the first index not 1.

	If the size of an array is n, to access the last element, (n-1)
	index is used. In this example, mark[4] is the last element.

	Suppose the starting address of mark[0] is 2120d. Then, the next
	address, a[1], will be 2124d, address of a[2] will be 2128d and
	so on. It's because the size of a float is 4 bytes.

*/
VOID cpp_3_1_arrays()
{

	// ---- local
		int numbers[5] = { 0, 0, 0, 0, 0 };
		int sum = 0;

	// ---- input
		cout << "Enter 5 numbers:";

	// ---- local
		// . storing 5 number entered by user in an array
		// . finding the sum of numbers entered
		for (int i = 0; i < 5; ++i) 
		{
			cin >> numbers[i];
			sum += numbers[i];
		}
    
    cout << "Sum = " << sum << endl;  
}


// -------- display 2d array --------
/*!
\brief initialise and display a 2D array
\author Gareth Edwards
*/
VOID cpp_3_2_2D_array_display()
{

	// ---- init
		int test[3][2] =
		{
			{ 2, -5 },
			{ 4, 0 },
			{ 9, 1 }
		};

	// ---- accessing two dimensional array using nested for loops
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				cout << "test[" << i << "][" << j << "] = " << test[i][j] << endl;
			}
		}

}


// -------- store 2d array --------
/*!
\brief initialise and display a 2D array
\author Gareth Edwards
*/

const int CITY = 2;
const int WEEK = 7;

VOID cpp_3_2_2D_array_store()
{

	int temperature[CITY][WEEK];

    cout << "Enter all temperature for a week of first city and then second city. \n";

    // ---- inserting the values into the temperature array
		for (int i = 0; i < CITY; ++i)
		{
			for(int j = 0; j < WEEK; ++j)
			{
				cout << "City " << i + 1 << ", Day " << j + 1 << " : ";
				cin >> temperature[i][j];
			}
		}

    cout << "\n\nDisplaying Values:\n";

    // ---- accessing the values from the temperature array
		for (int i = 0; i < CITY; ++i)
		{
			for(int j = 0; j < WEEK; ++j)
			{
				cout << "City " << i + 1 << ", Day " << j + 1 << " = " << temperature[i][j] << endl;
			}
		}
}


// -------- 3D array --------
/*!
\brief initialise and display a 3D array
\author Gareth Edwards
*/
VOID cpp_3_2_3D_array()
{

	// ---- this array can store upto 12 elements (2x3x2)
	    int test[2][3][2];

	// ---- input
	    cout << "Enter 12 values: \n";
    
    // ---- inserting the values into the test array using 3 nested for loops.
		for(int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for(int k = 0; k < 2; ++k )
				{
					cin >> test[i][j][k];
				}
			}
		}

    cout<<"\nDisplaying Value stored:"<<endl;

    // ---- displaying the values with proper index.
		for(int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				for(int k = 0; k < 2; ++k)
				{
					cout << "test[" << i << "][" << j << "][" << k << "] = " << test[i][j][k] << endl;
				}
			}
		}
}


// -------- one-dimensional array --------
/*!
\brief passing one-dimensional array to a function
\author Gareth Edwards
*/

VOID my_display_array(int marks[5]);

VOID cpp_3_3_pass_array_to_function()
{
	int marks[5] = { 88, 76, 90, 61, 69 };
	my_display_array(marks);
}

VOID my_display_array(int m[5])
{
	cout << "Displaying marks: " << endl;

	for (int i = 0; i < 5; ++i)
	{
		cout << "Student " << i + 1 << ": " << m[i] << endl;
	}
}


// -------- multi-dimensional array ----------
/*!
\brief passing multi-dimensional array to a function
\author Gareth Edwards
*/

void my_display_multi_array(int n[3][2]);

VOID cpp_3_3_pass_multi_array_to_function()
{
    int num[3][2] = {
        {3, 4},
        {9, 5},
        {7, 1}

    };
	my_display_multi_array(num);
}

void my_display_multi_array(int n[3][2])
{
	cout << "Displaying Values: " << endl;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			cout << n[i][j] << " ";
		}
	}
}


// -------- string --------
/*

\notes

	String is a collection of characters.

	There are two types of strings commonly used in C++ programming language:

	1. Strings that are objects of string class (The Standard C++ Library string class)
	2. C-strings (C-style Strings)

\notes

C-strings

	In C programming, the collection of characters is stored in the form of arrays,
	this is also supported in C++ programming. Hence it's called C-strings.

	C-strings are arrays of type char terminated with null character, that is,
	\0 (ASCII value of null character is 0).

	How to define a C-string?

		char str[] = "C++";
	
	In the above code, str is a string and it holds 4 characters.

	Although, "C++" has 3 character, the null character \0 is added to the end
	of the string automatically.

	Alternative ways of defining a string:

		char str[4] = "C++";

		char str[] = {'C','+','+','\0'};

		char str[4] = {'C','+','+','\0'};

	Like arrays, it is not necessary to use all the space allocated for the string.
	
	For example:

		char str[100] = "C++";

*/


// -------- c string --------
/*!
\brief Display a string entered by user.
\author Gareth Edwards
*/
VOID cpp_3_4_c_string()
{
	char str[100];

	cout << "Enter a string: ";
	cin >> str;
	cout << "You entered: " << str << endl;

	cout << "\nEnter another string: ";
	cin >> str;
	cout << "You entered: " << str << endl;

}

/*

\notes

	Notice that, in the second example only "Programming" is displayed
	instead of "Programming is fun".

	This is because the extraction operator >> works as scanf() in C and
	considers a space " " has a terminating character.

*/


// -------- c string get --------
/*!
\brief Display a string entered by user using get.
\author Gareth Edwards
*/
VOID cpp_3_4_c_string_get()
{
	char str[100];

	cout << "Enter a string: ";
	cin.get(str, 100);
	cout << "You entered: " << str << endl;

}


// -------- str string get --------
/*!
\brief Display a string entered by user using get.
\author Gareth Edwards
*/
VOID cpp_3_4_str_string()
{

	// Declaring a string object
	string str;
	cout << "Enter a string: ";
	getline(cin, str);

	cout << "You entered: " << str << endl;

}


// -------- str string pass to function --------
/*!
\brief Display a string entered by user using get.
\author Gareth Edwards

\notes

	Below, there are two functions my_pass_str() that outputs a string.

	The only difference between the two functions is the parameter.

	The first my_pass_str() function takes char array as a parameter,
	while the second takes string as a parameter.

	This process is known as function overloading.

*/

void my_pass_str(char s[]);
void my_pass_str(string s);

VOID cpp_3_4_pass_str_to_function()
{
	char c_str[100];
	string str;

	cout << "Enter a c string: ";
	cin.get(c_str, 100);

	cout << "Enter another string: ";
	getline(cin, str);

	my_pass_str(c_str);
	my_pass_str(str);
}

void my_pass_str(char s[])
{
	cout << "You entered char array: " << s << endl;
}

void my_pass_str(string s)
{
	cout << "You entered string: " << s << endl;
}

