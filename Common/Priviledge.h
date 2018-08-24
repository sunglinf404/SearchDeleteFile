#pragma once

#include "Funcs.h"

#define  SYS_MUTEX_INSTANCE_FAILED     (0)  //创建互斥量失败
#define  SYS_MUTEX_INSTANCE_SUCCESSED  (1)  //创建互斥量成功
#define  SYS_MUTEX_INSTANCE_EXISTS     (2)  //互斥量已经创建过(存在)

class CPriviledge
{
private:
	static bool SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
public:
	CPriviledge(void);
	~CPriviledge(void);
public:
	static bool RaisePriviledage(void);
	static int  CreateGlobalMutex(const char* name, const char* description, HANDLE& outHandle);
};
