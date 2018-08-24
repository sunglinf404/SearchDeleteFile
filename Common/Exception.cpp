#include "stdafx.h"
#include "Exception.h"

CException::CException(const vstring& msg, const int code):exception(msg.c_str())
{
	PutMsg( msg);
	m_iCode = code;
}

CException::~CException(void)
{

}

void CException::PutMsg(const vstring& msg)
{
	m_asMsg = msg;
}

int  CException::GetCode(void)
{
	return m_iCode;
}


const char* CException::GetMessage(void) const throw()
{
	return m_asMsg.c_str();
}
