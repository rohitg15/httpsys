#include "HttpDispatchTable.h"
#include <iostream>

HttpDispatchTable::HttpDispatchTable()
{
	std::cout << "Initialized HttpDispatchTable" << std::endl;
}

DWORD HttpDispatchTable::SendHttpResponse(
	HANDLE hReqQueue,
	std::vector<char>& buf,
	USHORT statusCode,
	char* pReason,
	char* pEntityString
)
{
	HTTP_RESPONSE response;
	HTTP_DATA_CHUNK dataChunk;
	DWORD bytesSent = 0;
	auto pHttpRequest = reinterpret_cast<PHTTP_REQUEST>(buf.data());

	// Initialize response object
	RtlZeroMemory(&response, sizeof(response));
	response.StatusCode = statusCode;
	response.pReason = pReason;
	response.ReasonLength = static_cast<USHORT>(strlen(pReason));

	// add headers
	std::string contentTypeHeader = "text/html";
	response.Headers.KnownHeaders[HttpHeaderContentType].pRawValue = contentTypeHeader.c_str();
	response.Headers.KnownHeaders[HttpHeaderContentType].RawValueLength = contentTypeHeader.size();

	if (pEntityString)
	{
		dataChunk.DataChunkType = HttpDataChunkFromMemory;
		dataChunk.FromMemory.pBuffer = pEntityString;
		dataChunk.FromMemory.BufferLength = static_cast<ULONG>(strlen(pEntityString));
		response.EntityChunkCount = 1;
		response.pEntityChunks = &dataChunk;
	}

	auto result = HttpSendHttpResponse(
		hReqQueue,
		pHttpRequest->RequestId,
		0,
		&response,
		NULL,
		&bytesSent,
		NULL,
		0,
		NULL,
		NULL
	);
	if (result != NO_ERROR)
	{
		std::cout << "HttpSendHttpResponse failed " << result << std::endl;
	}
	return result;
}
