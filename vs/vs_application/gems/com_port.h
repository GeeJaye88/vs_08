
// ---------- SetupComPort ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
/*
INT application::SetupComPort()
{

	int i, n,
	cport_nr = 0,        // /dev/ttyS0 (COM1 on windows)
	bdrate = 115200;     // 9600 baud

	unsigned char buf[4096];

	char mode[] = { '8', 'N', '1', 0 };

	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
	printf("Can not open comport\n");

	return(0);
	}

	while (1)
	{
	n = RS232_PollComport(cport_nr, buf, 4095);

	if (n > 0)
	{
	buf[n] = 0;   // always put a "null" at the end of a string!

	for (i = 0; i < n; i++)
	{
	if (buf[i] < 32)  // replace unreadable control-codes by dots
	{
	buf[i] = '.';
	}
	}

	printf("received %i bytes: %s\n", n, (char *)buf);
	}

	Sleep(100);

	}


	// Done
	return VS_OK;
}
*/

/*

BOOL  success  = FALSE;
DWORD baudrate = 9600;
BYTE  byteSize = 8;
BYTE  stopBits = ONESTOPBIT;
BYTE  parity   = NOPARITY;

// Open serial port
HANDLE serialHandle;

serialHandle = CreateFileA("\\\\.\\COM1",
GENERIC_READ, // | GENERIC_WRITE,
0,
NULL,
OPEN_EXISTING,
FILE_ATTRIBUTE_NORMAL,
NULL
);

// Do some basic settings
DCB serialParams = { 0 };
serialParams.DCBlength = sizeof(serialParams);

success = GetCommState(serialHandle, &serialParams);
serialParams.BaudRate = baudrate;
serialParams.ByteSize = byteSize;
serialParams.StopBits = stopBits;
serialParams.Parity   = parity;
success = SetCommState(serialHandle, &serialParams);

// Set timeouts
COMMTIMEOUTS timeout = { 0 };
timeout.ReadIntervalTimeout = 50;
timeout.ReadTotalTimeoutConstant = 50;
timeout.ReadTotalTimeoutMultiplier = 50;
timeout.WriteTotalTimeoutConstant = 50;
timeout.WriteTotalTimeoutMultiplier = 10;

success = SetCommTimeouts(serialHandle, &timeout);

*/
