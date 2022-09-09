////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_1_flow_control.cpp ----------
/*!
\file cpp2_1_flow_control.cpp
\brief Implementation of the flow control examples
\author Gareth Edwards
\note Vanilla C++
*/


////////////////////////////////////////////////////////////////////////////////


// -------- Flow_Control --------


// -------- C++ if, etc... --------
/*!
\brief if...
\author Gareth Edwards
*/
VOID application::cpp_1_1_if_original()
{
	using namespace std;

	int number;
	cout << "Enter an integer: ";
	cin >> number;

	// ---- checks if the number is positive
		if (number > 0)
		{
			cout << "You entered a positive integer: " << number << endl;
		}

	cout << "This statement is always executed.";
}


// -------- if, etc... --------
/*!
\brief if, if...else and Nested if...else
\author Gareth Edwards
*/
VOID application::cpp_1_2_if()
{

	using namespace std;

	// ---- local
		float number;


	// ---- enter
		cout << "Enter an number: ";
		cin >> number;


	// ---- identify integer
		BOOL isInt = FALSE;
		if ( number == 0 )
		{
			isInt = TRUE;
		}
		else
		{
			FLOAT test_number = number < 0 ? -number : number;
			if ((test_number / (FLOAT)((INT)test_number)) == test_number)
			{
				isInt = TRUE;
			}
		}


	// ---- report back to base
		if ( isInt )
		{
			if (number > 0)
			{
				cout << "You entered a positive integer: " << number << endl;
			}
			else if (number < 0)
			{
				cout << "You entered a negative integer: " << number << endl;
			}
			else
			{
				cout << "You entered zero: 0" << endl;
			}
		}
		else
		{
			cout << "You entered a non-integer: " << number << " !!!" << endl;
		}

}


// -------- for statements --------
/*!
\brief Loops are used in programming to repeat a specific block of code.

\note There are three type of loops in C++ programming:

	for loop
	while loop
	do...while loop

*/
VOID application::cpp_1_3_for()
{
	
	using namespace std;

	int n, factorial = 1;

	cout << "Enter a positive integer: ";
	cin >> n;

	for (int i = 1; i <= n; ++i) {
		factorial *= i;   // factorial = factorial * i;
	}

	cout << "Factorial of " << n << " = " << factorial;
}


// -------- loop statements --------
/*!
\brief Loops are used in programming to repeat a
	specific block of code.
*/
VOID application::cpp_1_4_loop()
{
	// ---- local
		using namespace std;
		int input_number;


	// ---- must be initialised before each loop
		int factorial;
		int i;


	// ---- INPUT
		cout << "Enter a positive integer: ";
		cin >> input_number;
		cout << endl;


	// ---- MULTI - SWITCH
		BOOL ok = FALSE;
		int c[4] = { 1, 2, 3, 4 };
		for (int i = 0; i < 4; i++)
		{
			if (input_number == c[i])
			{
				cout << "Input number " << input_number << " is OK " << endl << endl;
				ok = TRUE;
			}
		}
		if (!ok)
		{
			cout << "Input number " << input_number << " is NOT OK !!!" << endl << endl;
			return;
		}


	// ---- FOR
		factorial = 1;
		for ( i = 1; i <= input_number; ++i )
		{
			factorial *= i;
			cout << "- " << factorial << endl;
		}
		cout << "For - factorial of " << input_number << " = " << factorial << endl << endl;


	// ---- WHILE
		factorial = 1;
		i = 1;
		while ( i <= input_number )
		{
			factorial *= i;
			cout << "- " << factorial << endl;
			i++;
		}
		cout << "While - factorial of " << input_number << " = " << factorial << endl << endl;


	// ---- DO
		factorial = 1;
		i = 1;
		do {
			factorial *= i;
			cout << "- " << factorial << endl;
			i++;
		} while (i <= input_number);
		cout << "Do - factorial of " << input_number << " = " << factorial << endl << endl;


	// ---- SWITCH
		switch ( input_number )
		{
			case 1:
			case 2:
			case 3:
			case 4:
				cout << "Input number " << input_number << " is less than 5 " << endl << endl;
				break;
			default:
				cout << "Input number " << input_number << " is greater than 4 " << endl << endl;
				break;
		}

}


// -------- break statements --------
/*!

\brief Add all number entered by user until user enters 0
\note The test expression is always true.
*/
VOID application::cpp_1_5_break()
{

	// ---- local
		using namespace std;
		float number, sum = 0.0;

    // ---- test expression is always true
		while (true)
		{
			cout << "Enter a number: ";
			cin >> number;
        
			if (number != 0.0)
			{
				sum += number;
			}
			else
			{
				// terminates the loop if number equals 0.0
				break;
			}

		}

    cout << "Sum = " << sum << endl;
}


// -------- continue statements --------
/*!

\brief Display integer from 1 to 10 except 6 and 9.
\note When i is 6 or 9, execution of statement cout << i << "\t";
	is skipped inside the loop using continue; statement.
*/
VOID application::cpp_1_6_continue()
{

	// ---- local
		using namespace std;

	// ---- local
		for (int i = 1; i <= 10; ++i)
		{
			if ( i == 6 || i == 9)
			{
				continue;
			}
			cout << i << "\t" << endl;
		}
}



// -------- continue statements --------
/*!

\brief The goto statement is to be avoided...
\note The goto statement is used for altering the
	normal sequence of program execution by transferring
	control to some other part of the program.
*/
VOID application::cpp_1_7_goto()
{

	// ---- local
	using namespace std;
    float num, average, sum = 0.0;
    int i, n;

    cout << "Maximum number of inputs: ";
    cin >> n;

    for(i = 1; i <= n; ++i)
    {
        cout << "Enter n" << i << ": " << endl;
        cin >> num;
        
        if(num < 0.0)
        {
           // Control of the program move to jump:
            goto jump;
        } 
        sum += num;
    }
    
	// ---- argh....
	jump:
		average = sum / (i - 1);
		cout << "\nAverage = " << average << endl;

    return;
}



