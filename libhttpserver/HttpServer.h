#pragma once
#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

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

#pragma comment(lib, "httpapi.lib")
#include <string>
#include <vector>
#include "HttpDispatchTable.h"
#include "threadpool.h"
#include "ThreadContext.h"
#include <memory>



class HttpServer
{
public:
	HttpServer(
		uint32_t numThreads = 10
	);

	HttpServer(HttpServer&) = delete;

	HttpServer& operator=(HttpServer&) = delete;

	void AddUrl(
		const std::wstring& url
	);

	void Cleanup();

	void HandleRequests(
		unsigned long payloadSize
	);

private:


	ULONG DispatchRequest(
		std::vector<char>& buf,
		ULONG bytesRead = 0
	);

	std::vector<std::wstring> m_urls;
	/*HttpDispatchTable m_httpDispatchTbl;*/
	std::unique_ptr<filesafe::ThreadPool<ThreadContext>> m_pThreadPool;
	HANDLE m_hReqQueue;

};


#endif		//	_HTTP_SERVER_H_
