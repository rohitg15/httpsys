#ifndef _UTILS_H_
#define _UTILS_H_

#include <Windows.h>
#include <cstdio>

class ErrorUtils
{
public:

	static void PrintCSBackupAPIErrorMessage(DWORD dwErr)
	{

		WCHAR   wszMsgBuff[512];  // Buffer for text.

		DWORD   dwChars;  // Number of chars returned.

		// Try to get the message from the system errors.
		dwChars = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			wszMsgBuff,
			512,
			NULL);

		if (0 == dwChars)
		{
			// The error code did not exist in the system errors.
			// Try Ntdsbmsg.dll for the error code.

			HINSTANCE hInst;

			// Load the library.
			hInst = LoadLibrary(L"Ntdsbmsg.dll");
			if (NULL == hInst)
			{
				printf("cannot load Ntdsbmsg.dll\n");
				exit(1);  // Could 'return' instead of 'exit'.
			}

			// Try getting message text from ntdsbmsg.
			dwChars = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				hInst,
				dwErr,
				0,
				wszMsgBuff,
				512,
				NULL);

			// Free the library.
			FreeLibrary(hInst);

		}

		// Display the error message, or generic text if not found.
		printf("Error value: %d Message: %ws\n",
			dwErr,
			dwChars ? wszMsgBuff : L"Error message not found.");

	}
};

#endif	//	_UTILS_H_