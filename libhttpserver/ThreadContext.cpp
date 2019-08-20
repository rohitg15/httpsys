#include "ThreadContext.h"


ThreadContext::ThreadContext()
	:
	m_reqType(HttpRequestType::HTTP_GET)
{}

void ThreadContext::UpdateRequestType(
	enum HttpRequestType reqType
) 
{
	m_reqType = reqType;
}



void ThreadContext::RegisterSendResponseCallback(
	HttpSendResponseCallback f
)
{
	m_sendHttpResponse = f;
}


void ThreadContext::Execute()
{
	switch (m_reqType)
	{
		case HttpRequestType::HTTP_GET:
		{
			m_sendHttpResponse();
			break;
		}
		default:
		{
			break;
		}
	}
}