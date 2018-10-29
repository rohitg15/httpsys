#ifndef _THREAD_CONTEXT_H_
#define _THREAD_CONTEXT_H_

#include <vector>
#include <functional>

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
#include "Callbacks.h"


enum class HttpRequestType
{
	HTTP_GET,
	HTTP_POST,
	HTTP_HEAD,
	HTTP_PUT,
	HTTP_DELETE
};

class ThreadContext
{

public:
	typedef std::function<DWORD(HANDLE, PHTTP_REQUEST, USHORT, char*, char*)> cb_t;
	
	ThreadContext();

	void UpdateRequestType(
		enum HttpRequestType reqType
	);

	void RegisterSendResponseCallback(
		HttpSendResponseCallback f
	);


	void Execute();

private:
	enum HttpRequestType m_reqType;
	HttpSendResponseCallback m_sendHttpResponse;
};

#endif	//	_THREAD_CONTEXT_H_
