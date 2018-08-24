#include "stdafx.h"
#include "Priviledge.h"

CPriviledge::CPriviledge(void)
{

}

CPriviledge::~CPriviledge(void)
{

}

/************************************************* 
Function:       SetPrivilege
Description:    设置权限
Calls:          系统函数
Called By:       
Input:          
Return:         
*************************************************/ 
bool CPriviledge::SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	LUID luid;
	DWORD dwRet = 0;
    TOKEN_PRIVILEGES tp;
   
    if ( !::LookupPrivilegeValue (NULL, lpszPrivilege, &luid ) ) 
    {
		CFuncs::WriteLogInfo(SLT_ERROR,"SetPrivilege LookupPrivilegeValue failed,Error info : %d",CFuncs::SvcFormatMessage().c_str());
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;

	// enable or disable privilege
    tp.Privileges[0].Attributes = bEnablePrivilege?SE_PRIVILEGE_ENABLED:0;

    if ( !::AdjustTokenPrivileges (hToken, FALSE, &tp, sizeof (TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES) NULL, (PDWORD) NULL) ) 
    {
		CFuncs::WriteLogInfo(SLT_ERROR,"SetPrivilege AdjustTokenPrivileges failed,error info : %s",
			CFuncs::SvcFormatMessage().c_str());
        return false;
    } 
	dwRet = ::GetLastError();
    if (ERROR_SUCCESS != dwRet)
    {
		CFuncs::WriteLogInfo(SLT_ERROR,"SetPrivilege AdjustTokenPrivileges not success,error info : %s",
			CFuncs::SvcFormatMessage(dwRet).c_str());
        return false;
    }

    return true;
}

/************************************************* 
Function:       RaisePrivillage
Description:    提权
Calls:          系统函数
Called By:       
Input:          
Return:         
*************************************************/ 
bool CPriviledge::RaisePriviledage(void)
{
	bool ret = false;
    HANDLE htok = NULL;
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &htok))
    {
		CFuncs::WriteLogInfo(SLT_ERROR,"OpenProcessToken(GetCurrentProcess()) failed,Error info : %d",
			CFuncs::SvcFormatMessage().c_str());
        return false;
    }
    ret = SetPrivilege (htok, SE_BACKUP_NAME, TRUE) && SetPrivilege (htok, SE_RESTORE_NAME, TRUE) && 
		SetPrivilege (htok, SE_TAKE_OWNERSHIP_NAME, TRUE) && SetPrivilege (htok, SE_DEBUG_NAME, TRUE) && 
		SetPrivilege (htok, SE_DEBUG_NAME, TRUE) && SetPrivilege (htok, SE_CREATE_GLOBAL_NAME, TRUE); 

    CloseHandle(htok);
    return ret;
}

int CPriviledge::CreateGlobalMutex(const char* name, const char* description, HANDLE& outHandle)
{
	HANDLE  hMutex;
	SECURITY_ATTRIBUTES SecAttr;  
	SECURITY_DESCRIPTOR SecDesc;  

	if(!RaisePriviledage())
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"%s, 系统初始化失败,提升系统权限失败.",description);
		return SYS_MUTEX_INSTANCE_FAILED;
	}

	SecAttr.nLength = sizeof(SecAttr);  
	SecAttr.bInheritHandle = FALSE;  
	SecAttr.lpSecurityDescriptor = &SecDesc;  

	InitializeSecurityDescriptor(&SecDesc, SECURITY_DESCRIPTOR_REVISION);  
	SetSecurityDescriptorDacl(&SecDesc, TRUE, 0, FALSE);  
  
	hMutex = ::CreateMutex(&SecAttr, FALSE, name);
	DWORD dwRet = ::GetLastError();
	if (dwRet == ERROR_ALREADY_EXISTS) 
	{
		CFuncs::WriteLogInfo(SLT_WARNING,"----%s已经运行,只能运行一个实例程序.----",description);
		return SYS_MUTEX_INSTANCE_EXISTS;
	}
	else if(NULL == hMutex)
	{
		CFuncs::WriteLogInfo(SLT_WARNING,"%s,初始化互斥量失败,错误信息:%s",description,CFuncs::SvcFormatMessage(dwRet).c_str());
		return SYS_MUTEX_INSTANCE_FAILED;
	}
	outHandle = hMutex;
	CFuncs::WriteLogInfo(SLT_INFORMATION,"%s : hMutex = 0x%I64x",description, hMutex);
	return SYS_MUTEX_INSTANCE_SUCCESSED;
}
