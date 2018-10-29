#ifndef _HTTP_DISPATCH_TABLE_H_
#define _HTTP_DISPATCH_TABLE_H_
#pragma once
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



class HttpDispatchTable
{

public:
	HttpDispatchTable();

	HttpDispatchTable(HttpDispatchTable&) = delete;

	HttpDispatchTable& operator=(HttpDispatchTable&) = delete;

	static DWORD SendHttpResponse(
		HANDLE hReqQueue,
		std::vector<char>& buf,
		USHORT statusCode,
		char* pReason,
		char* pEntityString
	);

	
private:

};

#endif		//	_HTTP_DISPATCH_TABLE_H_