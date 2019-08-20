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

#pragma comment(lib, "httpapi.lib")
#include <string>
#include <vector>
#include "threadpool.h"
#include "HttpServer.h"


bool KILL_SERVER = false;

BOOL WINAPI ConsoleHandler(DWORD signal)
{
	if (signal == CTRL_C_EVENT)
	{
		KILL_SERVER = true;
		std::cout << "Received CTRL-C, safely terminating";
		return TRUE;
	}
	return FALSE;
}


int main(int argc, char *argv[])
{

	std::unique_ptr<HttpServer, std::function<void(HttpServer*)>> pServer(new HttpServer, [](HttpServer* p)
	{
		p->Cleanup();
	});
	std::wstring url = L"http://127.0.0.1:8080/test0";
	pServer->AddUrl(url);
	std::wcout << L"http.SYS listening for incoming requests at : " << url << std::endl;
	while (!KILL_SERVER)
	{
		try
		{
			pServer->HandleRequests(1024);
		}
		catch (const std::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}


    return 0;
}
