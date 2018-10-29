#ifndef _CALLBACKS_H_
#define _CALLBACKS_H_

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <http.h>
#include <stdio.h>
#include <memory>
#include <vector>

#pragma comment(lib, "httpapi.lib")

//class ICallable
//{
//public:
//	virtual DWORD operator() () = 0;
//};

class HttpSendResponseCallback 
{
public:
	HttpSendResponseCallback();

	HttpSendResponseCallback(HttpSendResponseCallback& other);

	HttpSendResponseCallback& operator=(HttpSendResponseCallback& other);

	HttpSendResponseCallback(
		HANDLE hReqQueue,
		std::vector<char> buf,
		USHORT statusCode,
		char* pReason,
		char* pEntityString
	);

	 DWORD operator() ();

private:
	HANDLE m_hReqQueue;
	std::vector<char> m_buf;
	USHORT m_statusCode;
	char* m_pReason;
	char* m_pEntityString;
};

#endif		//	_CALLBACKS_H_