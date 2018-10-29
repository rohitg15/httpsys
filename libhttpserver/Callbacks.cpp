#include "Callbacks.h"
#include "HttpDispatchTable.h"


HttpSendResponseCallback::HttpSendResponseCallback()
	: m_hReqQueue(NULL),
	  m_statusCode(0),
	  m_pReason(NULL),
	  m_pEntityString(NULL)
{

}

HttpSendResponseCallback::HttpSendResponseCallback(HttpSendResponseCallback& other)
{
	m_hReqQueue = other.m_hReqQueue;
	m_buf = other.m_buf;
	m_statusCode = other.m_statusCode;
	// this might lead to a Race
	m_pReason = other.m_pReason;
	m_pEntityString = other.m_pEntityString;
}

HttpSendResponseCallback& HttpSendResponseCallback::operator=(HttpSendResponseCallback& other)
{
	if (this != &other)
	{
		m_hReqQueue = other.m_hReqQueue;
		m_buf = other.m_buf;
		m_statusCode = other.m_statusCode;
		// this might lead to a Race
		m_pReason = other.m_pReason;
		m_pEntityString = other.m_pEntityString;
	}
	return *this;
}

HttpSendResponseCallback::HttpSendResponseCallback(
	HANDLE hReqQueue,
	std::vector<char> buf,
	USHORT statusCode,
	char* pReason,
	char* pEntityString
) : m_hReqQueue(hReqQueue),
	m_buf(buf),
	m_statusCode(statusCode),
	m_pReason(pReason),
	m_pEntityString(pEntityString)
{

}

DWORD HttpSendResponseCallback::operator()()
{
	return HttpDispatchTable::SendHttpResponse(m_hReqQueue, m_buf, m_statusCode, m_pReason, m_pEntityString);
}