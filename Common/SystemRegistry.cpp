#include "StdAfx.h"
#include "SystemRegistry.h"


CSystemRegistry::CSystemRegistry(void)
{
}


CSystemRegistry::~CSystemRegistry(void)
{
}

bool CSystemRegistry::ISPESystem(void)
{
	char szPath[MAX_PATH] = {0};

	if(!SHGetSpecialFolderPath(NULL, szPath, CSIDL_WINDOWS, FALSE))
	{
		return false;
	}
	if('x' == szPath[0] || 'X' == szPath[0])
	{
		string sPath = _T("X:\\vanli.dla");
		if(CFuncs::FileExist(sPath))
		{
			return true;
		}
	}
	return false;
}

bool CSystemRegistry::UnLoadRegistry(HKEY hKey, const char* subKey)
{
	if(ERROR_SUCCESS != ::RegUnLoadKey(hKey,subKey))
	{
		CFuncs::WriteLogInfo(SLT_WARNING,"卸载注册表失败(%u),%s",::GetLastError(),subKey);
		return false;
	}
	return true;
}

bool CSystemRegistry::LoadRegistry(HKEY hKey, const char* subKey, const char* path)
{
	if(ERROR_SUCCESS != ::RegLoadKey(hKey,subKey,path))
	{
		CFuncs::WriteLogInfo(SLT_WARNING,"加载注册表失败(%u),%s : %s",::GetLastError(),subKey,path);
		return false;
	}
	return true;
}

bool CSystemRegistry::ISSystemDisk(const char* disk)
{
	vstring sDisk;
	vstring sPath1;
	vstring sPath2;
	
	sDisk.assign(disk);
	if (_T(':') != sDisk.at(sDisk.length() - 1))
	{
		sDisk.append(_T(":"));
	}
	sPath1.assign(sDisk);
	sPath1.append(VL_REG_SOFTWARE);
	
	sPath2.assign(sDisk);
	sPath2.append(_T("\\Windows\\System32\\drivers\\etc\\hosts"));

	if(::PathFileExists(sPath1.c_str()) || ::PathFileExists(sPath2.c_str()))
	{
		return true;
	}
	return false;
}

void CSystemRegistry::ISSystemArchitecture(const char* disk, string& architecture)
{
	string strPath;

	architecture.assign(_T("x86"));
	strPath.assign(disk);
	if (_T(':') != strPath.at(strPath.length() - 1))
	{
		strPath.append(_T(":"));
	}
	strPath.append(_T("\\Program Files (x86)"));
	if(::PathIsDirectory(strPath.c_str()))
	{
		architecture.assign(_T("x64"));
	}
}

bool CSystemRegistry::GetDiskInfo(ValueVector& vValue)
{
	string sTemp;
	string sDriver;

	DWORD count = GetLogicalDrives();
	if(0 == count)
	{
		CFuncs::WriteLogInfo(SLT_WARNING,"设备运行数据:没有发现磁盘驱动器.");
		return false;
	}
	count >>= 2;
	uint8 driver = 'B';
	while(driver < 'Z')
	{
		driver += 1;
		if(0 == (count & 0x01))
		{
			continue;
		}

		sTemp = driver;
		sDriver = driver;
		sDriver.append(_T(":"));

		CFuncs::WriteLogInfo(SLT_WARNING,"%s",sDriver.c_str());

		count >>= 1;

		if(DRIVE_FIXED == ::GetDriveType(sDriver.c_str()))
		{ 
			vValue.push_back(sTemp);
		}
	}
	return true;
}

bool CSystemRegistry::GetSystemVersionInfo(const char* disk, string& version)
{
	HKEY hKey;
	DWORD dwLen;
	DWORD dwType;
	string strKey;
	char szValue[MAX_PATH + sizeof(char)] = { 0 };	

	version.assign(_T(""));

	/*strKey.assign(disk);*/
	strKey.append(_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"));
	/*strKey.append(_T("_SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"));*/
	/*if (ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strKey.c_str(), 0, KEY_READ, &hKey))*/

	if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,strKey.c_str(),0, KEY_READ,&hKey))
	{
		DWORD dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_WARNING,"GetSystemVersionInfo RegOpenKeyEx失败(%u): %s",dwError,strKey.c_str());
		return false;
	}
	dwType = 0;
	dwLen = sizeof(szValue); 
	if(ERROR_SUCCESS != RegQueryValueEx(hKey,_T("ProductName"),NULL,&dwType,(LPBYTE)szValue,&dwLen))
	{
		DWORD dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_WARNING,"GetSystemVersionInfo RegQueryValueEx失败(%u): %s(ProductName)",dwError,strKey.c_str());
		::RegCloseKey(hKey);
		return false;
	}
	version.append(szValue);
	::RegCloseKey(hKey);
	return true;
}

bool CSystemRegistry::LoadAllLocalRegistry(void)
{
	ValueVector vValues;
	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sKey;
		string sPath;
		char  szUser[MAX_PATH + sizeof(char)] = {0};

		string sDriver = *it;
		if(!ISSystemDisk(sDriver.c_str()))
		{
			continue;
		}
		//SOFTWARE
		sKey.assign(sDriver);
		sKey.append(_T("_SOFTWARE"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_SOFTWARE);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());

		//SYSTEM
		sKey.assign(sDriver);
		sKey.append(_T("_SYSTEM"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_SYSTEM);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());

		//SECURITY
		sKey.assign(sDriver);
		sKey.append(_T("_SECURITY"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_SECURITY);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());

		//DEFAULT
		sKey.assign(sDriver);
		sKey.append(_T("_DEFAULT"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_DEFAULT);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());

		//SAM
		sKey.assign(sDriver);
		sKey.append(_T("_SAM"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_SAM);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());

		//COMPONENTS
		sKey.assign(sDriver);
		sKey.append(_T("_COMPONENTS"));

		sPath.assign(sDriver);
		sPath.append(_T(":"));
		sPath.append(VL_REG_COMPONENTS);
		LoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str(),sPath.c_str());
	}
	vValues.clear();
	return true;
}

bool CSystemRegistry::UnLoadAllLocalRegistry(void)
{
	ValueVector vValues;
	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sKey;
		string sPath;
		char  szUser[MAX_PATH + sizeof(char)] = {0};

		string sDriver = *it;
		if(!ISSystemDisk(sDriver.c_str()))
		{
			continue;
		}
		//SOFTWARE
		sKey.assign(sDriver);
		sKey.append(_T("_SOFTWARE"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());

		//SYSTEM
		sKey.assign(sDriver);
		sKey.append(_T("_SYSTEM"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());

		//SECURITY
		sKey.assign(sDriver);
		sKey.append(_T("_SECURITY"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());

		//DEFAULT
		sKey.assign(sDriver);
		sKey.append(_T("_DEFAULT"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());

		//SAM
		sKey.assign(sDriver);
		sKey.append(_T("_SAM"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());

		//COMPONENTS
		sKey.assign(sDriver);
		sKey.append(_T("_COMPONENTS"));

		UnLoadRegistry(HKEY_LOCAL_MACHINE,sKey.c_str());
	}
	vValues.clear();
	return true;
}

bool CSystemRegistry::GetSystemDiskList(string& disklist)
{
	ValueVector vValues;
	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sKey;
		string sPath;
		char  szUser[MAX_PATH + sizeof(char)] = {0};

		string sDriver = *it;
		if(ISSystemDisk(sDriver.c_str()))
		{
			disklist.append(sDriver);
		}
	}
	vValues.clear();
	return true;
}

bool CSystemRegistry::GetAllUserInfo(string& outUserName)
{
	string sName;
	DWORD dwLen;
	DWORD dwCountOfSubKey;
	DWORD dwCountOfValue;
	ValueVector vValues;

	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	outUserName.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		HKEY hKey;
		string sKey;
		string sDriver = *it;
		if(!ISSystemDisk(sDriver.c_str()))
		{
			continue;
		}
		char  szUser[MAX_PATH] = {0};
		//SAM
		sKey.assign(sDriver);
		sKey.append(_T("_SAM"));
		sKey.append(VL_REG_SAM_ACCOUNT);
		printf("KEY:%s\r\n",sKey.c_str());

		if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,sKey.c_str(),0,KEY_READ,&hKey))
		{
			DWORD dwError = GetLastError();
			CFuncs::WriteLogInfo(SLT_WARNING,"RegOpenKeyEx失败(%u): %s",dwError,sKey.c_str());
			continue;
		}
		if(ERROR_SUCCESS != ::RegQueryInfoKey(hKey,NULL, NULL, NULL,&dwCountOfSubKey, NULL, NULL, &dwCountOfValue, NULL, NULL, NULL, NULL))
		{
			DWORD dwError = GetLastError();
			::RegCloseKey(hKey);
			CFuncs::WriteLogInfo(SLT_WARNING,"RegQueryInfoKey失败(%u): %s",dwError,sKey.c_str());
			continue;
		}
		printf("SubKey(%u),Value(%u)\r\n",dwCountOfSubKey,dwCountOfValue);
		for (DWORD i = 0; i < dwCountOfSubKey; i++)
		{
			char szName[MAX_PATH + sizeof(char)] = { 0 };
			dwLen = sizeof(szName) - sizeof(char); 
			if(ERROR_SUCCESS != ::RegEnumKey(hKey, i, szName, dwLen))
			{
				DWORD dwError = GetLastError();
				CFuncs::WriteLogInfo(SLT_WARNING,"RegEnumKey失败(%u): %s",dwError,sKey.c_str());
				continue;
			}
			outUserName.append(szName);
			outUserName.append(_T(";"));
			printf("Account Info : %s",outUserName.c_str());
		}
		::RegCloseKey(hKey);
	}
	CFuncs::WriteLogInfo(SLT_INFORMATION,"用户列表:%s\r\n",outUserName.c_str());
	return outUserName.length() > 0;
}

bool CSystemRegistry::GetDefaultUserInfo(string& outUserName)
{
	string sUserName;
	ValueVector vValues;
	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	outUserName.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sDriver = *it;
		if(ISSystemDisk(sDriver.c_str()))
		{
			string sKey;
			char  szUser[MAX_PATH + sizeof(char)] = {0};
			//SOFTWARE
			sKey.assign(sDriver);
			sKey.append(_T("_SOFTWARE"));
			sKey.append(VL_REG_SOFTWARE_WINLOGON);

			if(CFuncs::QueryRegistryValueString(sKey,_T("DefaultUserName"),szUser,sizeof(szUser)))
			{
				sUserName.append(szUser);
				sUserName.append(";");
			}
		}
	}
	outUserName.assign(sUserName);
	CFuncs::WriteLogInfo(SLT_INFORMATION,"缺省用户:%s\r\n",sUserName.c_str());
	return outUserName.length() > 0;
}

bool CSystemRegistry::GetComputerName(string& outName)
{
	string sName;
	ValueVector vValues;
	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	sName.assign(_T(""));
	outName.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sDriver = *it;
		if(ISSystemDisk(sDriver.c_str()))
		{
			string sKey;
			char  szUser[MAX_PATH + sizeof(char)] = {0};
			//SYSTEM
			sKey.assign(sDriver);
			sKey.append(_T("_SYSTEM"));
			sKey.append(VL_REG_SYSTEM_COMPUTERNAME);

			if(CFuncs::QueryRegistryValueString(sKey,_T("ComputerName"),szUser,sizeof(szUser)))
			{
				sName.append(szUser);
				sName.append(";");
			}
		}
	}
	outName.assign(sName);
	CFuncs::WriteLogInfo(SLT_INFORMATION,"缺省用户:%s\r\n",outName.c_str());
	return outName.length() > 0;
}

bool CSystemRegistry::GetShareInfo(const string& subKey, string& outName)
{
	HKEY hKey;
	DWORD dwType;
	DWORD dwLen;
	DWORD dwCountOfSubKey;
	DWORD dwCountOfValue;

	outName.assign(_T(""));
	if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0,KEY_READ | KEY_QUERY_VALUE,&hKey))
	{
		DWORD dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_WARNING,"GetShareInfo RegOpenKeyEx失败(%u): %s",dwError,subKey.c_str());
		return false;
	}
	if(ERROR_SUCCESS != ::RegQueryInfoKey(hKey,NULL, NULL, NULL,&dwCountOfSubKey, NULL, NULL, &dwCountOfValue, NULL, NULL, NULL, NULL))
	{
		DWORD dwError = GetLastError();
		::RegCloseKey(hKey);
		CFuncs::WriteLogInfo(SLT_WARNING,"GetShareInfo RegQueryInfoKey失败(%u): %s",dwError,subKey.c_str());
		return false;;
	}
	printf("\tSubKey(%u),Value(%u)\r\n",dwCountOfSubKey,dwCountOfValue);
	for (DWORD i = 0; i < dwCountOfValue; i++)
	{    
		dwType = 0;
		char szName[MAX_PATH + sizeof(char)] = { 0 };
		dwLen = sizeof(szName); 
		char szValue[MAX_PATH + sizeof(char)] = { 0 };
		if(ERROR_SUCCESS != ::RegEnumValue(hKey, i, szName, &dwLen, NULL, NULL/*&dwType*/,NULL, NULL))
		{
			DWORD dwError = GetLastError();
			CFuncs::WriteLogInfo(SLT_WARNING,"GetShareInfo RegEnumValue失败(%u): %s",dwError,subKey.c_str());
			continue;
		}
		dwLen = sizeof(szValue);
		if(ERROR_SUCCESS != RegQueryValueEx(hKey,szName,NULL,&dwType,(LPBYTE)szValue,&dwLen))
		{
			DWORD dwError = GetLastError();
			CFuncs::WriteLogInfo(SLT_WARNING,"GetShareInfo RegQueryValueEx失败(%u): %s -> %s",dwError,subKey.c_str(),szName);
			continue;
		}
		char* pTmp = CFuncs::SearchSubMemory(szValue,dwLen,_T("Path="),strlen(_T("Path=")),0);
		if(NULL == pTmp)
		{
			CFuncs::WriteLogInfo(SLT_WARNING,"GetShareInfo SearchSubMemory失败: %s",szValue);
			continue;
		}
		string strVal;
		string strName;
		string strValue = pTmp;
		CFuncs::SplitParameter(strValue,strName,strVal);
		outName.append(szName);
		outName.append(_T("="));
		outName.append(strVal);
		outName.append(_T(";"));
		printf("Share Info : %s",outName.c_str());
	}
	::RegCloseKey(hKey);
	CFuncs::WriteLogInfo(SLT_INFORMATION,"共享列表:%s",outName.c_str());
	return outName.length() > 0;
}
//输出（每天以\r\n结束）:共享名=共享路径
bool CSystemRegistry::GetShareInfo(string& outName)
{
	ValueVector vValues;

	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	outName.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sKey;
		string sName;
		string sDriver = *it;
		if(!ISSystemDisk(sDriver.c_str()))
		{
			continue;
		}
		char  szUser[MAX_PATH] = {0};
		//SYSTEM
		sKey.assign(sDriver);
		sKey.append(_T("_SYSTEM"));
		sKey.append(VL_REG_SYSTEM_SHARES1);
		printf("KEY:%s\r\n",sKey.c_str());
		if(GetShareInfo(sKey,sName))
		{
			outName.append(sName);
		}

		sKey.assign(sDriver);
		sKey.append(_T("_SYSTEM"));
		sKey.append(VL_REG_SYSTEM_SHARES2);
		printf("KEY:%s\r\n",sKey.c_str());
		if(GetShareInfo(sKey,sName))
		{
			outName.append(sName);
		}
		printf("Share Info : %s",outName.c_str());
	}
	CFuncs::WriteLogInfo(SLT_INFORMATION,"共享列表:%s\r\n",outName.c_str());
	return outName.length() > 0;
}

bool CSystemRegistry::GetShortCutsFile(const char* shorcut, string& outPath)   
{
	HRESULT hRes;  
	bool bRet = false;
	IShellLink *pShellLink;

	size_t num = 0;
	char szBuf[MAX_PATH] = {0};
	char szPath[MAX_PATH] = {0};
	wchar_t szwBuf[MAX_PATH * 3] = {0};

	hRes = CoInitialize(NULL);
	if(S_OK != hRes && S_FALSE != hRes)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile CoInitialize失败,,错误ID:%u",GetLastError());
		return false;
	}
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&pShellLink);
	if(S_OK != hRes)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile CoCreateInstance失败,,错误ID:%u",GetLastError());
		return false;
	}
	IPersistFile *ppf;
	hRes = pShellLink->QueryInterface(IID_IPersistFile, (void **)&ppf);
	if(S_OK != hRes)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile QueryInterface失败,,错误ID:%u",GetLastError());
		return false;
	}
	if(0 >= CCharsetConver::ANSIToUnicode(shorcut,strlen(shorcut),szwBuf,sizeof(szwBuf)))
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile ANSIToUnicode失败,,错误ID:%u",GetLastError());
		ppf->Release();
		return false;
	}
	hRes = ppf->Load(szwBuf, STGM_READWRITE | STGM_SHARE_EXCLUSIVE);
	if(S_OK != hRes)
	{
		wcout << "ppf->Load(%s)" << endl;
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile ppf->Load(%s)失败,,错误ID:%u",shorcut,GetLastError());
		ppf->Release();
		return false;
	}
	hRes = pShellLink->GetWorkingDirectory(szPath, _countof(szPath));
	if(S_OK == hRes)
	{
		bRet = true;
		outPath.assign(szPath);
		CFuncs::WriteLogInfo(SLT_INFORMATION,"CSystemRegistry::GetShortCutsFile GetWorkingDirectory成功,%s - >%s",shorcut,szPath);
	}
	hRes = pShellLink->GetPath(szPath, _countof(szPath), NULL, 0);
	if(S_OK == hRes)
	{
		bRet = true;
		outPath.assign(szPath);
		CFuncs::WriteLogInfo(SLT_INFORMATION,"CSystemRegistry::GetShortCutsFile GetPath成功,%s - >%s",shorcut,szPath);
	}
	else
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetShortCutsFile pShellLink->GetPath失败,,错误ID:%u",GetLastError());
	}
	pShellLink->Release();
	ppf->Release();
	::CoUninitialize();
	return true;
}

//查找快捷方式
bool CSystemRegistry::SearchFileByDesktop(const string& path, const char* shorcut, string& outPath)
{
	bool ret = false;
	string strFindPath;
	WIN32_FIND_DATA FindFileData;

	strFindPath.assign(path);
	strFindPath.append("*.*");

	HANDLE hFind = FindFirstFile(strFindPath.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::SearchFileByDesktop查找文件失败,%s,错误ID:%u",strFindPath.c_str(),GetLastError());
		return false;
	}
	do
	{
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{
			string strTmpPath;
			if(0 == _stricmp(FindFileData.cFileName,shorcut))
			{
				strTmpPath.assign(path); 
				strTmpPath.append(FindFileData.cFileName);

				if(GetShortCutsFile(strTmpPath.c_str(),outPath))
				{
					ret = true;
					break;
				}
			}
		}	
	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	return ret;
}

//适合PE及外挂硬盘检查
bool CSystemRegistry::GetPathForShortcuts(const char* shorcut, string& outPath)
{
	bool ret = false;
	ValueVector vValues;

	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	outPath.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sPath;
		string sDriver = *it;
		if(!ISSystemDisk(sDriver.c_str()))
		{
			continue;
		}
		//判断是否为系统盘
		sPath.assign(sDriver);
		sPath.append(_T(":\\Users\\"));
		if(!CFuncs::DirectoryExists(sPath.c_str()))
		{
			sPath.assign(sDriver);
			sPath.append(_T(":\\Documents and Settings\\"));
			if(!CFuncs::DirectoryExists(sPath.c_str()))
			{
				printf("%s : 非系统目录\r\n",sDriver.c_str());
				continue;
			}
		}
		printf("用户目录:%s\r\n",sPath.c_str());
		//查找用户文件夹
		string strFindPath;
		WIN32_FIND_DATA FindFileData;

		strFindPath.assign(sPath);
		strFindPath.append("\\*.*");

		HANDLE hFind = FindFirstFile(strFindPath.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			CFuncs::WriteLogInfo(SLT_ERROR,"CSystemRegistry::GetPathForShortcuts查找文件失败,%s,错误ID:%u",strFindPath.c_str(),GetLastError());
			continue;
		}
		do
		{
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
			{
				if((strcmp(FindFileData.cFileName, ".") != 0) && (strcmp(FindFileData.cFileName, "..") != 0))
				{
					string strTmpPath;
					string strShortcutPath;
					//Windows7以上
					strTmpPath.assign(sPath); 
					strTmpPath.append(FindFileData.cFileName);
					strTmpPath.append(_T("\\Desktop\\"));
					if(!CFuncs::DirectoryExists(strTmpPath.c_str()))
					{
						//WindowsXP
						strTmpPath.assign(sPath); 
						strTmpPath.append(FindFileData.cFileName);
						strTmpPath.append(_T("\\桌面\\"));
					}
					if(CFuncs::DirectoryExists(strTmpPath.c_str()))
					{
						if(SearchFileByDesktop(strTmpPath,shorcut,strShortcutPath))
						{
							//检查文件
							outPath.append(strShortcutPath);
							ret = true;
						}
					}
				}
			}	
		}while(FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	return ret;
}


/*
  检查文件是否存在于磁盘里，只替换盘符检查。
  path:源路径
  newPath:存在后的新路径
  isSystemPartion：是否只在系统分区检查
*/
bool CSystemRegistry::CheckFileExistsInOverallDisk(const string& path, string& newPath, const bool isSystemPartion)
{
	bool ret = false;
	ValueVector vValues;
	char szPath[MAX_PATH] = { 0 };
	char szDstPath[MAX_PATH] = { 0 };

	if(path.empty())
	{
		return false;
	}
	strncpy_s(szPath, path.c_str(), _TRUNCATE);
	::PathRemoveArgs(szPath);      //除去参数部分
	::PathUnquoteSpaces(szPath);   //除去首位引号
	::ExpandEnvironmentStrings(szPath,szDstPath,_countof(szDstPath));

	if(!GetDiskInfo(vValues))
	{
		return false;
	}
	newPath.assign(_T(""));
	for(ValueVectorIterator it = vValues.begin(); it != vValues.end(); it++)
	{
		string sDriver = *it;

		if(isSystemPartion)
		{
			//如果只检查系统分区
			if(!CSystemRegistry::ISSystemDisk(sDriver.c_str()))
			{
				continue;
			}
		}
		szDstPath[0] = sDriver.at(0);		
		if(::PathFileExists(szDstPath))
		{
			ret = true;
			newPath.assign(szDstPath);
			break;
		}
	}
	vValues.clear();
	CFuncs::WriteLogInfo(SLT_INFORMATION,"源路径:%s -> 新有效路径:%s",path.c_str(),newPath.c_str());
	return ret;
}
