#pragma once

#include "../Common/Funcs.h"
#include "../Include/DataConst.h"
#include "../Common/SystemRegistry.h"
#include "../Include/DataHeader.h"
#include "../Common/Priviledge.h"
 
const vstring  SegMentation_DeviceId  =   ("DeviceID=\"");
const vstring  SegMentation           =   ("\"");

const vstring  strSystemReg = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";

const vstring  Cache_Action           =   ("view");
const vstring  Download_Action		  =   ("download");
const vstring  Upload_Action		  =   ("upload");
const vstring  Err_Action			  =   ("loginError");
const vstring  Login_Action			  =   ("login");
const vstring  Logout_Action		  =   ("logout");
const vstring  IM_Action			  =   ("im");
const vstring  Baidu_Browser		  =   ("baidu");
const vstring  Baidu_Record			  =	  ("history.db");


#define		BUFFSIZE			1024
#define		MAX_KEY_LENGTH	    255


//typedef bool(WINAPI* PFIsWow64Procss)(HANDLE handle, PBOOL pbool);

//解密遨游浏览器上网记录文件,解密后是sqlite
//src		: 文件路径
//dst		：解密后文件路径
//返回值	：成功true，失败 false  数据库时间类型是:time_t

typedef bool (*PFDecryptMaxthon)(const char* src, const char* dst);

//解密DreamMail DB文件,解密后是sqlite
//src		: 文件路径
//dst		：解密后文件路径
//返回值	：成功TRUE，失败FALSE
typedef bool (*PFDecryptDreamMailDB)(const char* src, const char* dst);


typedef bool (*PFCallbackInternetRecord)(int cdsType, const char* checkPath, const char* szTime, const char* szDomain, 
	 const char* szAction, const char* szTitle, const char* name, const char* process);

typedef bool (*PFCallbackEMailRecord)(int cdsType, const char* checkPath, const char* boxType, const char* sender, 
	 const char* receiver, const char* subject, const char* time, const char* name, const char* process);

class CCommon
{
public:
	CCommon(void);  
	~CCommon(void);
public:
	static vstring TimestampToTime(DWORD timestamp);

	static vstring TimestampToTime(const char* szTimestamp);

	static bool Is64BitOS(void);

	static bool Is64BitOSForPE(vstring& strSystemDisk);

	static bool GetDeviceIDValue(vstring& strInfo, vstring& strValue);

	static bool GetSystemUsers(SysDiskMap& mSystemUsers);

	static bool GetOSVersionByDisk(const vstring& strDisk, vstring& strUserPath, size_t& osVersion);

	static time_t FileTimeToTime_t(FILETIME fTime);

	static char* ANSIToUTF8(LPSTR srcCode);

	static wchar_t*  ANSIToUnicode(LPSTR srcCode);

	static bool UnicodeToANSI(wchar_t* srcCode, vstring &sText);

	static bool GetInstallAppLocation(const vstring& appName,vstring& appInstallLocation);

	static bool FindInstallApp(HKEY hKey, const vstring& appName, vstring& appInstallLocation, DWORD& dwOptions);

	static bool GetInstallAppInfo(HKEY hKey, char* szSubKeyName, const vstring& appName, vstring& appInstallLocation, DWORD& dwOptions);

	static bool GetSystemPath(vstring& strSystemReg, vstring& systemPath);

	static bool GetSystemPath(vstring& systemPath);


};

