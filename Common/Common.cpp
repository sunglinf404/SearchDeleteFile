#include "StdAfx.h"
#include "Common.h"


CCommon::CCommon(void)
{
}


CCommon::~CCommon(void)
{
}


bool CCommon::Is64BitOS(void)
{
	SYSTEM_INFO systemInfo = { 0 };
	GetNativeSystemInfo(&systemInfo);
	if( systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
		|| systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
	{
		return true;
	}
	return false;
	//BOOL b64System = FALSE;
	//PFIsWow64Procss funcIsWow64Process = NULL;

	//funcIsWow64Process = (PFIsWow64Procss)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");
	//if (NULL != funcIsWow64Process)
	//{
	//	funcIsWow64Process(GetCurrentProcess(), &b64System);
	//}
	//if (TRUE == b64System)
	//{
	//	return true;
	//}
	//return false;
}


vstring CCommon::TimestampToTime(DWORD timestamp)
{
	char szTime[32] = { 0 };
	time_t timeSt = timestamp;
	struct tm srtTm;

	memset(&srtTm, 0, sizeof(tm));
	gmtime_s(&srtTm, &timeSt);
	/*strftime(szTime, 20, "%Y-%m-%d %H:%M:%S", &srtTm);*/
	sprintf_s(szTime, _countof(szTime), "%04d年%02d月%02d日 %02d:%02d:%02d", srtTm.tm_year + 1900, srtTm.tm_mon + 1, srtTm.tm_mday,
		srtTm.tm_hour, srtTm.tm_min, srtTm.tm_sec);
	vstring strTime = vstring(szTime);
	return strTime;
}

vstring CCommon::TimestampToTime(const char* szTimestamp)
{
	char szTime[32] = { 0 };
	time_t timeSt = _atoi64(szTimestamp);
	struct tm srtTm;

	memset(&srtTm, 0, sizeof(tm));
	gmtime_s(&srtTm, &timeSt);
	/*strftime(szTime, 20, "%Y-%m-%d %H:%M:%S", &srtTm);*/
	sprintf_s(szTime, _countof(szTime), "%04d-%02d-%02d %02d:%02d:%02d", srtTm.tm_year + 1900, srtTm.tm_mon + 1, srtTm.tm_mday,
		srtTm.tm_hour, srtTm.tm_min, srtTm.tm_sec);
	vstring strTime = vstring(szTime);
	return strTime;
}

bool CCommon::Is64BitOSForPE(vstring& strSystemDisk)
{
	if(strSystemDisk.empty())
	{
		CFuncs::WriteLogInfo(SLT_ERROR, "CCommon::Is64BitOSForPE 传入系统盘的盘符参数错误： %s", strSystemDisk.c_str());
		return false;
	}
	vstring strDisk = strSystemDisk;
	if(':' != strDisk[strDisk.length() - 1])
	{
		strDisk.append(":");
	}
	strDisk.append("\\Windows\\SysWOW64");
	if(CFuncs::DirectoryExists(strDisk.c_str()))
	{
		return true;
	}
	return false;	
}

bool CCommon::GetDeviceIDValue(vstring& strInfo, vstring& strValue)
{
	if(strInfo.empty())
	{
		return false;
	}
	size_t beginPos = strInfo.find(SegMentation_DeviceId, 0);
	if(string::npos == beginPos)
	{
		return false;
	}
	beginPos += SegMentation_DeviceId.length();
	size_t endPos = strInfo.find(SegMentation, beginPos);
	if(string::npos == endPos)
	{
		return false;
	}
	strValue = strInfo.substr(beginPos, endPos - beginPos);
	return true;
}

bool CCommon::GetSystemUsers(SysDiskMap& mSystemUsers)
{
	vstring sTemp;
	vstring sDriver;
	vstring strUserPath;
	size_t osVersion = 0;

	DWORD count = GetLogicalDrives();
	if (0 == count)
	{
		CFuncs::WriteLogInfo(SLT_WARNING, "设备运行数据:没有发现磁盘驱动器.");
		return false;
	}
	count >>= 2;
	uint8 driver = 'B';
	while ((count & 0x01) && driver < 'Z')
	{
		driver += 1;

		sTemp = driver;
		sDriver = driver;
		sDriver.append(_T(":"));
		count >>= 1;
		if (DRIVE_FIXED == ::GetDriveType(sDriver.c_str()))
		{
			if (CSystemRegistry::ISSystemDisk(sDriver.c_str()))
			{
				if (!GetOSVersionByDisk(sDriver, strUserPath, osVersion))
				{
					CFuncs::WriteLogInfo(SLT_ERROR, "GetOSVersionByDisk 获取系统磁盘(%s)的系统版本失败!", sDriver.c_str());
					continue;
				}
				if (_T('\\') != strUserPath.at(strUserPath.length() - 1))
				{
					strUserPath.append(_T("\\"));
				}
				vstring tempPath = strUserPath + vstring(_T("*.*"));
				WIN32_FIND_DATA winFindData;
				memset(&winFindData, 0, sizeof(winFindData));
				HANDLE hFind = FindFirstFile(tempPath.c_str(), &winFindData);
				if (INVALID_HANDLE_VALUE == hFind)
				{
					return false;
				}
				do
				{
					if ('.' == winFindData.cFileName[0] || '..' == winFindData.cFileName[0]) 
					{
						continue;
					}
					if (winFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						vstring strUserDir = strUserPath + vstring(winFindData.cFileName);
						mSystemUsers.insert(pair<vstring, size_t>(strUserDir, osVersion));
					}
				} while (NULL != FindNextFile(hFind, &winFindData));
				FindClose(hFind);
			}
		}
	}
	return true;
}


bool CCommon::GetOSVersionByDisk(const vstring& strDisk, vstring& strUserPath, size_t& osVersion)
{
	vstring tempDisk = strDisk;
	strUserPath.clear();
	if (tempDisk.empty())
	{
		return false;
	}
	if (_T('\\') != tempDisk.at(tempDisk.length() - 1))
	{
		tempDisk.append(_T("\\"));
	}
	vstring strWin98_1 = tempDisk + vstring("WINDOWS\\Cookies");
	vstring strWin98_2 = tempDisk + vstring("WINDOWS\\History");
	vstring strWindows = tempDisk + vstring("Documents and Settings");
	vstring strWin7 = tempDisk + vstring("Users");
	vstring strWinXp = tempDisk + vstring("WINDOWS");
	vstring strWinNT = tempDisk + vstring("Winnt");

	if (PathIsDirectory(strWin98_1.c_str()) && PathIsDirectory(strWin98_2.c_str()))
	{
		osVersion = DOV_UNKNOW;
		return true;
	}
	else if (PathIsDirectory(strWindows.c_str()) && PathIsDirectory(strWin7.c_str()))
	{
	/*	if (IS64BitSystem())
		{
			if (PathIsDirectory(strDisk+"Documents and Settings") & PathIsDirectory(strDisk+"Users") &  PathIsDirectory(strDisk+"WINDOWS"))
			{
			return OS_WINXP;
			}
		}*/
		strUserPath = strWin7;
		osVersion = DOV_WIN_7;
		return true;
	}
	else if (PathIsDirectory(strWindows.c_str()) & PathIsDirectory(strWinXp.c_str()))
	{
		strUserPath = strWindows;
		osVersion = DOV_WIN_XP;
		return true;
	}
	else if (PathIsDirectory(strWindows.c_str()) & PathIsDirectory(strWinNT.c_str()))
	{
		strUserPath = strWindows;
		osVersion = DOV_UNKNOW;
		return true;
	}
	return false;
}

time_t CCommon::FileTimeToTime_t(FILETIME  fTime)
{
	ULARGE_INTEGER  ulInteger;
	memset(&ulInteger, 0, sizeof(ULARGE_INTEGER));
	ulInteger.LowPart = fTime.dwLowDateTime;
	ulInteger.HighPart = fTime.dwHighDateTime;

	return ((LONGLONG)(ulInteger.QuadPart - 116444736000000000) / 10000000);
}


char* CCommon::ANSIToUTF8(LPSTR srcCode)
{
	wchar_t* strUnicode = ANSIToUnicode(srcCode);
	int srcCodeLen = WideCharToMultiByte(CP_UTF8, NULL, strUnicode, wcslen(strUnicode), NULL, 0, NULL, NULL);
	char* result = new char[srcCodeLen + 1];
	WideCharToMultiByte(CP_UTF8, NULL, strUnicode, wcslen(strUnicode), result, srcCodeLen, NULL, NULL);//Unicode转换为ANSI   
	result[srcCodeLen] = '\0';
	delete strUnicode;
	return result;
}

wchar_t*  CCommon::ANSIToUnicode(LPSTR srcCode)
{
	char* sContent = srcCode;
	int wcsLen = ::MultiByteToWideChar(CP_ACP, NULL, sContent, strlen(sContent), NULL, 0);
	wchar_t* wszString = new wchar_t[wcsLen + 1];
	::MultiByteToWideChar(CP_ACP, NULL, sContent, strlen(sContent), wszString, wcsLen);
	wszString[wcsLen] = '\0';

	return wszString;
}


bool CCommon::GetInstallAppLocation(const vstring& appName, vstring& appInstallLocation)
{
	HKEY hKey = NULL;
	int  ret = 0;
	DWORD dwOptions = 0;
	vstring tempKey;
	if (CSystemRegistry::ISPESystem())
	{
		ValueVector vValues;
		if (!CSystemRegistry::GetDiskInfo(vValues))
		{
			vValues.clear();
			return false;
		}
		for (ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
		{
			vstring strDriver = *it;
			if (!CSystemRegistry::ISSystemDisk(strDriver.c_str()))
			{
				continue;
			}
			tempKey = vstring(strDriver);
			tempKey.append("_SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
		}
	    vValues.clear();
	}
	else
	{
		tempKey.assign("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
	}
	if (CCommon::Is64BitOS())
	{
		//操作系统是64位,安装的软件可能是32位
		dwOptions = KEY_READ | KEY_WOW64_32KEY;
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, tempKey.c_str(), 0, dwOptions, &hKey);
		if (ERROR_SUCCESS == ret)
		{
			if (FindInstallApp(hKey, appName, appInstallLocation, dwOptions))
			{
				RegCloseKey(hKey);
				return true;
			}
			RegCloseKey(hKey);
		}

		//读取64位系统上64位软件安装的注册表信息(软件有可能是64位)
		dwOptions = KEY_READ | KEY_WOW64_64KEY;
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, tempKey.c_str(), 0, dwOptions, &hKey);
		if (ERROR_SUCCESS != ret)
		{
			CFuncs::WriteLogInfo(SLT_ERROR, "");
			return false;

		}
		if (FindInstallApp(hKey, appName, appInstallLocation, dwOptions))
		{
			RegCloseKey(hKey);
			return true;
		}
	}
	else
	{
		//操作系统是32位
		dwOptions = KEY_READ;
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, tempKey.c_str(), 0, dwOptions, &hKey);
		if (ERROR_SUCCESS != ret)
		{
			CFuncs::WriteLogInfo(SLT_INFORMATION, "RegOpenKeyEx(%d)打开注册表：%s失败", ret, tempKey.c_str());
			return false;
		}
		CFuncs::WriteLogInfo(SLT_INFORMATION, "RegOpenKeyEx(%d)打开注册表：%s", ret, tempKey.c_str());
		if (FindInstallApp(hKey, appName,appInstallLocation, dwOptions))
		{
			RegCloseKey(hKey);
			return true;
		}
	}
	RegCloseKey(hKey);
	return false;
}

bool CCommon::FindInstallApp(HKEY hKey, const vstring& appName, vstring& appInstallLocation, DWORD& dwOptions)
{
	int ret = 0;
	DWORD subKeyCount = 0;          //子键数量
	DWORD subKeyNameMaxLen = 0;     //子健键名字的最大长度
	DWORD keyValueCount = 0;        //键值数量
	DWORD keyValueNameMaxLen = 0;   //键值名字的最大长度
	DWORD keyValueDataMaxLen = 0;   //键值数据的最大长度

	//获取子键信息
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, &subKeyCount, &subKeyNameMaxLen, NULL,
		&keyValueCount, &keyValueNameMaxLen, &keyValueDataMaxLen, NULL, NULL);
	if (ERROR_SUCCESS != ret)
	{
		return false;
	}

	DWORD dwNameLen = 0;
	char szSubKeyName[MAX_KEY_LENGTH] = {0};
	for (int i = 0; i < (int)subKeyCount; i++)      
	{
		dwNameLen = subKeyNameMaxLen + 1;
		memset(szSubKeyName, 0, sizeof(szSubKeyName));
		ret = RegEnumKeyEx(hKey, i, szSubKeyName, &dwNameLen, NULL, NULL, NULL, NULL);
		if (ERROR_SUCCESS != ret)
		{
			continue;
		}
		if (GetInstallAppInfo(hKey, szSubKeyName, appName, appInstallLocation, dwOptions))
		{
			return true;
		}
	}
	return false;
}

bool CCommon::GetInstallAppInfo(HKEY hKey, char* szSubKeyName, const vstring& appName, vstring& appInstallLocation, DWORD& dwOptions)
{
	int ret = 0;
	DWORD dataSize = 1024;
	char szValues[BUFFSIZE] = { 0 };
	HKEY hInstallAppKey = NULL;
	vstring strName;
	
	ret = RegOpenKeyEx(hKey, szSubKeyName, 0, dwOptions, &hInstallAppKey);
	if (ERROR_SUCCESS != ret)
	{
		return false;
	}
	ret = RegQueryValueEx(hInstallAppKey, "DisplayName", NULL, NULL, (LPBYTE)szValues, &dataSize);
	if (ERROR_SUCCESS != ret)
	{
		RegCloseKey(hInstallAppKey);
		return false;
	}
	strName.assign(szValues);

	if (string::npos != strName.find(appName))
	{
		dataSize = 1024;
		//查找软件安装的路径
		ret = RegQueryValueEx(hInstallAppKey, "InstallLocation", NULL, NULL, (LPBYTE)szValues, &dataSize);
		if (ERROR_SUCCESS == ret)
		{
			appInstallLocation = vstring(szValues);
			RegCloseKey(hInstallAppKey);
			return true;
		}
		ret = RegQueryValueEx(hInstallAppKey, "UninstallString", NULL, NULL, (LPBYTE)szValues, &dataSize);
		if (ERROR_SUCCESS == ret)
		{
			appInstallLocation = vstring(szValues);
			RegCloseKey(hInstallAppKey);
			return true;
		}
	}
	RegCloseKey(hInstallAppKey);
	return false;
}

bool CCommon::UnicodeToANSI(wchar_t* srcCode, vstring &sText)
{
	if (NULL == srcCode)
	{
		return false;
	}
	sText.clear();
	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, srcCode, wcslen(srcCode), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, srcCode, wcslen(srcCode), szAnsi, ansiLen, NULL, NULL);
	memset(szAnsi, 0, ansiLen);
	sText = szAnsi;
	delete[] szAnsi;
	return true;
}

bool CCommon::GetSystemPath(vstring& strSystemReg, vstring& systemPath)
{
	DWORD dwOption = 0;
	DWORD dwError = 0;

	if (CCommon::Is64BitOS())
	{
		dwOption = KEY_READ | KEY_WOW64_64KEY;
	}
	else
	{
		dwOption = KEY_READ;
	}

	DWORD dwRet = 0;
	HKEY hKey = NULL;
	char szBuff[MAX_PATH] = { 0 };
	DWORD buffLen = sizeof(szBuff);
	DWORD dataType = REG_SZ;
	DWORD dwDate = 0;

	dwRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSystemReg.c_str(), 0, dwOption, &hKey);
	if (ERROR_SUCCESS != dwRet)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, "CCommon::GetSystemPath RegOpenKeyEx 打开注册表:%s失败, 错误返回码: %d", 
			strSystemReg.c_str(), dwRet);
		return false;
	}
	dwRet = RegQueryValueEx(hKey, (LPTSTR)("SystemRoot"), NULL, &dataType, (BYTE *)szBuff, &buffLen);
	if (ERROR_SUCCESS == dwRet)
	{
		systemPath = vstring(szBuff);
		uint8 szDisk = strSystemReg[0];
		vstring strDisk;
		if(szDisk > 'C')
		{
			szDisk = szDisk - 1;
			strDisk = szDisk;
			systemPath.replace(0, 1, strDisk.c_str());
		}	
	}
	else
	{
		CFuncs::WriteLogInfo(SLT_ERROR, "CCommon::GetSystemPath RegQueryValueEx 打开注册表%s查询键值:%s失败",
			strSystemReg.c_str(), "SystemRoot");
		RegCloseKey(hKey);
		return false;
	}
	RegCloseKey(hKey);
	return true;
}


bool CCommon::GetSystemPath(vstring& systemPath)
{
	char szPath[MAX_PATH] = {0}; 
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS, NULL, SHGFP_TYPE_CURRENT, szPath))) 
	{  
		systemPath = vstring(szPath);
		return true;
	}  
	return false;
}