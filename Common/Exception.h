#pragma once

#include <exception>
#include "Define.h"

class CException : public exception
{
private:
	vstring		m_asMsg;
	int			m_iCode;
public:
	CException(const vstring& msg, const int code);
	~CException(void);

	void PutMsg(const vstring& msg);
    int  GetCode(void);
	virtual const char* GetMessage(void) const throw();
};
