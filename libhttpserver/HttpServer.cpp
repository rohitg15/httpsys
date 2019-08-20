#include "HttpServer.h"
#include "ThreadContext.h"
#include <iostream>
#include <functional>
#include <memory>
#include <Utils.h>

HttpServer::HttpServer(
	uint32_t numThreads
)
	: m_hReqQueue(NULL),
	  m_pThreadPool(std::make_unique<httpsys::ThreadPool<ThreadContext>>(numThreads))
{
	std::cout << "Initialized HttpServer" << std::endl;
	HTTPAPI_VERSION apiVersion = HTTPAPI_VERSION_1;
	ULONG ret = HttpInitialize(
		apiVersion,
		HTTP_INITIALIZE_SERVER,
		NULL
	);
	if (ret != NO_ERROR)
	{
		std::cout << "Error: " << ret << std::endl;
		throw std::runtime_error("Error in HttpInitialize");
	}

	ret = HttpCreateHttpHandle(
		&m_hReqQueue,
		0
	);
	if (ret != NO_ERROR)
	{
		std::cout << "Error: " << ret << std::endl;
		throw std::runtime_error("Error in HttpInitialize");
	}
}

void HttpServer::AddUrl(
	const std::wstring& url
)
{
	ULONG ret = HttpAddUrl(
		m_hReqQueue,
		url.c_str(),
		NULL
	);
	if (ret != NO_ERROR)
	{
		std::cout << "Error: " << ret << std::endl;
		throw std::runtime_error("Error in HttpAddUrl");
	}
	std::wcout << L"Added url : " << url << L"\n";
	m_urls.emplace_back(url);
}


void HttpServer::HandleRequests(
	unsigned long payloadSize
)
{
	ULONG requestBufLen = sizeof(HTTP_REQUEST) + payloadSize;
	HTTP_REQUEST_ID requestId;
	HTTP_SET_NULL_ID(&requestId);
	std::vector<char> requestBuf(requestBufLen, 0x00);
	bool receivedMoreData = false;
	do
	{
		ULONG result = 0;
		ULONG bytesRead = 0;
		PHTTP_REQUEST pHttpRequest = reinterpret_cast<PHTTP_REQUEST>(requestBuf.data());
		receivedMoreData = false;

		RtlZeroMemory(pHttpRequest, requestBufLen);

		result = HttpReceiveHttpRequest(
			m_hReqQueue,
			requestId,
			0,
			pHttpRequest,
			requestBufLen,
			&bytesRead,
			NULL
		);

		if (NO_ERROR == result)
		{
			if (NO_ERROR != (result = DispatchRequest(requestBuf)))
			{
				std::cout << " Error: " << result << std::endl;
				break;
			}
		}
		else if (ERROR_MORE_DATA == result)
		{
			// input buffer too small to handle incoming data
			ErrorUtils::PrintCSBackupAPIErrorMessage(result);
			requestBuf.resize(bytesRead, 0x0);
			receivedMoreData = true;
		}
		else if (ERROR_CONNECTION_INVALID == result && !HTTP_IS_NULL_ID(&requestId))
		{
			// TCP connection corrupted by peer
			ErrorUtils::PrintCSBackupAPIErrorMessage(result);
			HTTP_SET_NULL_ID(&requestId);
		}
		else
		{
			ErrorUtils::PrintCSBackupAPIErrorMessage(result);
			break;
		}
	} while (receivedMoreData);
}

ULONG HttpServer::DispatchRequest(
	std::vector<char>& buf,
	ULONG bytesRead
)
{
	ULONG result = 0;
	PHTTP_REQUEST pHttpRequest = reinterpret_cast<PHTTP_REQUEST>(buf.data());
	switch (pHttpRequest->Verb)
	{
		case HttpVerbGET:
		{
			std::wcout << L"Got http GET for " << pHttpRequest->CookedUrl.pFullUrl << std::endl;
			HttpSendResponseCallback cb(m_hReqQueue, buf, 200, "OK", "Server Hello!");
			ThreadContext thCtx{ HttpRequestType::HTTP_GET, cb };
			m_pThreadPool->AddJob(thCtx);
			break;
		}
		case HttpVerbPOST:
		{
			std::wcout << L"Got http POST for " << pHttpRequest->CookedUrl.pFullUrl << std::endl;

			break;
		}
		default:
		{
			break;
		}
	}
	return result;
}

void HttpServer::Cleanup()
{
	m_pThreadPool->KillThreads();
	for (auto const& url : m_urls)
	{
		HttpRemoveUrl(m_hReqQueue, url.c_str());
	}
	if (m_hReqQueue)
	{
		CloseHandle(m_hReqQueue);
	}
	HttpTerminate(HTTP_INITIALIZE_SERVER, NULL);
	std::cout << "Cleanup finished!" << std::endl;
}
