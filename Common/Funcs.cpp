#include "stdafx.h"
#include "Funcs.h"

vstring gAppPath;                // 程序路径
vstring gLogPath;                // 日志文件夹路径
vstring gProcName;               // 进程名
vstring gDeviceId;               // 设备编号
vstring gRulePath;				 // 规则路径
vstring gConfigPath;			 // 配置文件路径
vstring gTemporaryPath;          // 临时文件夹路径
vstring gRuleKeywordListXML;     //规则-关键字列表

CFuncs::CFuncs(void)
{
}

CFuncs::~CFuncs(void)
{
}

vstring CFuncs::ParseMac(const uint8* mac)
{
	vstring strMac;
	uint8 tempMac[128];

	memset(tempMac,0,sizeof(tempMac));
	sprintf_s((char *)tempMac,sizeof(tempMac) - 1,"%02x-%02x-%02x-%02x-%02x-%02x",
		mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);

	strMac = (char *)tempMac;
	return strMac;
}

vstring  CFuncs::ParseMD5(const uint8* md5,const uint32 len)
{
	uint8 tempMD5[8];
	vstring strMD5 = "";
	for(uint32 i = 0; i < len; i++)
	{
		memset(tempMD5,0,sizeof(tempMD5));
		sprintf_s((char *)tempMD5,sizeof(tempMD5) - 1,"%02x",md5[i]);

		strMD5.append((char *)tempMD5);
	}
	return strMD5;
}

vstring CFuncs::GetTimeName(void)
{
	time_t temp = GetTimestamp();
	return GetTimeName(temp);
}

vstring CFuncs::GetTimeName(const time_t timestamp)
{
    vstring tempTime;
	struct tm rawtime;
	char  tempValue[32];

	localtime_s(&rawtime,&timestamp);
	memset(tempValue,0,sizeof(tempValue));
	strftime(tempValue, sizeof(tempValue) - 1, "%Y%m%d", &rawtime);
	
	tempTime = tempValue;

	return tempTime;
}

time_t CFuncs::GetTimestamp(void)
{
	time_t temp;

	time(&temp);
	return temp;
}

vstring CFuncs::GetTimeString(void)
{
	vstring tempTime;
	struct tm rawtime;
	char   tempValue[256];
	time_t timestamp = GetTimestamp();

	localtime_s(&rawtime,&timestamp);
	memset(tempValue,0,sizeof(tempValue));
	strftime(tempValue, sizeof(tempValue) - 1, "%Y-%m-%d %H:%M:%S", &rawtime);
	
	tempTime = tempValue;

	return tempTime;
}

vstring CFuncs::GetTimeString(const time_t timestamp)
{
	vstring tempTime;
	struct tm rawtime;
	char   tempValue[256];

	localtime_s(&rawtime,&timestamp);
	memset(tempValue,0,sizeof(tempValue));
	strftime(tempValue, sizeof(tempValue) - 1, "%Y-%m-%d %H:%M:%S", &rawtime);
	
	tempTime = tempValue;

	return tempTime;
}

vstring CFuncs::GetTimeFileName(void)
{
	vstring tempName;
	time_t timestamp;
	struct tm rawtime;
	char   szValue[128];

	timestamp = GetTimestamp();
	localtime_s(&rawtime,&timestamp);
	memset(szValue,0,sizeof(szValue));
	strftime(szValue, sizeof(szValue) - 1, "(%Y%m%d%H%M%S)", &rawtime);

	tempName = szValue;
	return tempName;
}

vstring CFuncs::GetTimeFileName(const char* ext)
{
	vstring tempName;
	time_t timestamp;
	struct tm rawtime;
	char   szValue[128];

	timestamp = GetTimestamp();
	localtime_s(&rawtime,&timestamp);
	memset(szValue,0,sizeof(szValue));
	strftime(szValue, sizeof(szValue) - 1, "(%Y%m%d%H%M%S)", &rawtime);

	tempName = szValue;
	if(ext != NULL)
	{
	    tempName.append(ext);
	}
	return tempName;
}

bool CFuncs::FileExist(const char* path)
{
	if(_access(path,0) == -1)
	{
		//不存在
		return false;
	}
	//文件存在
	return true;
}

bool CFuncs::FileExist(const vstring& path)
{
	if(_access(path.c_str(),0) == -1)
	{
		//不存在
		return false;
	}
	//文件存在
	return true;
}

bool CFuncs::FileExists(const char* path)
{
	DWORD dwType = ::GetFileAttributes(path);
	if (INVALID_FILE_ATTRIBUTES == dwType)
	{
		return false;  //something is wrong with your path!
	}
	if (dwType & FILE_ATTRIBUTE_ARCHIVE)
	{
		return true;   // this is a directory!
	}
	return false;      // this is not a directory!
}

bool CFuncs::DirectoryExists(const char* path)
{
	DWORD dwType = ::GetFileAttributes(path);
	if (INVALID_FILE_ATTRIBUTES == dwType)
	{
		return false;  //something is wrong with your path!
	}
	if (dwType & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;   // this is a directory!
	}
	return false;      // this is not a directory!
}

int	CFuncs::GetSizeFromFile(const char* path)
{
    struct stat statbuf;  
    stat(path, &statbuf);  
    int size = statbuf.st_size;  
  
    return size;  
}

int CFuncs::GetSizeFromFile(const vstring&  path)
{
	return GetSizeFromFile(path.c_str());
}

vstring CFuncs::ParseFileExt(const char* path)
{
	vstring tempExt;
	int8 drive[_MAX_DRIVE];
	int8 dir[_MAX_DIR];
	int8 fname[_MAX_FNAME];
	int8 ext[_MAX_EXT];

	_splitpath_s(path,drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	tempExt = ext;

	return tempExt;
}
vstring CFuncs::ParseFileExt(const vstring& path)
{
	return ParseFileExt(path.c_str());
}

vstring  CFuncs::ParseFileExtEx(const vstring& path)
{
	vstring tempExt;
	int8 drive[_MAX_DRIVE];
	int8 dir[_MAX_DIR];
	int8 fname[_MAX_FNAME];
	int8 ext[_MAX_EXT];

	_splitpath_s(path.c_str(),drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	tempExt = ext;

	if(tempExt.empty())
	{
		return "";
	}
	return tempExt.substr(1,tempExt.length() - 1);
}

vstring CFuncs::ParseFileName(const vstring& path)
{
	vstring tempName;
	int8 drive[_MAX_DRIVE];
	int8 dir[_MAX_DIR];
	int8 fname[_MAX_FNAME];
	int8 ext[_MAX_EXT];

	_splitpath_s(path.c_str(),drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	tempName.assign(fname);

	return tempName;
}

vstring CFuncs::ParseFileNameExt(const vstring& path)
{
	vstring tempName;
	int8 drive[_MAX_DRIVE];
	int8 dir[_MAX_DIR];
	int8 fname[_MAX_FNAME];
	int8 ext[_MAX_EXT];

	_splitpath_s(path.c_str(),drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	tempName.assign(fname);
	tempName.append(ext);

	return tempName;
}

vstring CFuncs::ParseFilePath(const vstring& path)
{
	vstring tempPath;
	int8 drive[_MAX_DRIVE];
	int8 dir[_MAX_DIR];
	int8 fname[_MAX_FNAME];
	int8 ext[_MAX_EXT];

	_splitpath_s(path.c_str(),drive,sizeof(drive),dir,sizeof(dir),fname,sizeof(fname),ext,sizeof(ext));
	tempPath = drive;
	tempPath.append(dir);
	
	return tempPath;
}


bool CFuncs::ParseLogFile(const vstring& path, vstring& fileCreateTime, vstring& fileModifyTime)
{
	if (path.empty())
	{
		return false;
	}
	fileCreateTime.clear();
	fileModifyTime.clear();
    SYSTEMTIME stUTC;
	SYSTEMTIME stLocal;
	char tmpValue[MAX_PATH] = {0};
	WIN32_FILE_ATTRIBUTE_DATA fileAttData;
	memset(&fileAttData, 0, sizeof(fileAttData));

	if (!GetFileAttributesEx( path.c_str(), GetFileExInfoStandard, &fileAttData))
	{
		return false;
	}
	FileTimeToSystemTime(&(fileAttData.ftCreationTime),&stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	sprintf_s(tmpValue,_countof(tmpValue),"%u-%u-%u %u:%u:%u",stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
	fileCreateTime.assign(tmpValue);

	memset(tmpValue,0,sizeof(tmpValue));
	FileTimeToSystemTime(&(fileAttData.ftLastWriteTime),&stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
	sprintf_s(tmpValue,_countof(tmpValue),"%u-%u-%u %u:%u:%u",stLocal.wYear,stLocal.wMonth,stLocal.wDay,stLocal.wHour,stLocal.wMinute,stLocal.wSecond);
	fileModifyTime.assign(tmpValue);

	return true;	
}

vstring CFuncs::SvcFormatMessage(void)
{
	return SvcFormatMessage(GetLastError(),string(""));
}

vstring CFuncs::SvcFormatMessage(const DWORD errorId)
{
	return SvcFormatMessage(errorId,string(""));
}

vstring CFuncs::SvcFormatMessage(const DWORD errorId,const vstring& description)
{
	DWORD ret;
	vstring errorInfo;
	char   tempString[1024];
	char   lpMsgBuf[1024 * 6];

	errorInfo = description;
	memset(tempString, 0, sizeof(tempString));
	sprintf_s(tempString,sizeof(tempString) - 1,"error id : %d",errorId);

	memset(lpMsgBuf,0,sizeof(lpMsgBuf));
	ret = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorId,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /* Default language */
		(LPTSTR)(&lpMsgBuf),
		sizeof(lpMsgBuf) - 1,
		NULL 
		);
	if(!ret)
	{
		errorInfo.append(",");
		errorInfo.append(tempString);
		return errorInfo;
	}
	errorInfo.append(",");
	errorInfo.append(lpMsgBuf);
	errorInfo.append(",");
	errorInfo.append(tempString);
	
	return errorInfo;
}

vstring CFuncs::SvcFormatMessage(const DWORD errorId,const char* format,...)
{
	int ret;
	va_list argptr;
	vstring  description;
	char content[1024 * 20];

	memset(content,0,sizeof(content));
	va_start(argptr, format);
	ret = vsnprintf_s(content, sizeof(content) - 1,_TRUNCATE, format, argptr);
	va_end(argptr);

	if(ret <= 0)
	{
		return "";
	}
	description = content;
	return SvcFormatMessage(errorId,description);
}

bool CFuncs::QueryRegistryValueInteger(const vstring& subKey,const vstring& name,int* outValue)
{
	HKEY  reg; 
	DWORD dwLen;
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0,KEY_READ | KEY_QUERY_VALUE,&reg) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValueInteger RegOpenKeyEx(";
		errorInfo.append(subKey);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		return false;
	}
	*outValue = 0;
	dwType = REG_DWORD;
	dwLen = sizeof(UINT);
	if(RegQueryValueEx(reg,name.c_str(),NULL,&dwType,(LPBYTE)outValue,&dwLen) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValueInteger RegQueryValueEx-name(";
		errorInfo.append(name);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		RegCloseKey(reg);
		return false;
	}
	RegCloseKey(reg);
	return true;
}

bool CFuncs::QueryRegistryValueString(const vstring& subKey,const vstring& name,char* outValue,const int outValueLen)
{
	HKEY  reg; 
	DWORD dwLen;
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0,KEY_READ | KEY_QUERY_VALUE,&reg) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValuevstring RegOpenKeyEx(";
		errorInfo.append(subKey);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		return false;
	}
	dwType = REG_SZ;
	dwLen = outValueLen - 1;
	memset(outValue,0,outValueLen);
	if(RegQueryValueEx(reg,name.c_str(),NULL,&dwType,(LPBYTE)outValue,&dwLen) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValuevstring RegQueryValueEx-name(";
		errorInfo.append(name);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		RegCloseKey(reg);
		return false;
	}
	RegCloseKey(reg);
	return true;
}


bool CFuncs::QueryRegistryValueString(const vstring& subKey, DWORD dwOption, const vstring& name,char* outValue,const int outValueLen)
{
	HKEY  reg = NULL; 
	DWORD dwLen = 0;
	DWORD dwType = 0;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey.c_str(),0, dwOption, &reg) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValuevstring RegOpenKeyEx(";
		errorInfo.append(subKey);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		return false;
	}
	dwType = REG_SZ;
	dwLen = outValueLen - 1;
	memset(outValue,0,outValueLen);
	if(RegQueryValueEx(reg,name.c_str(),NULL,&dwType,(LPBYTE)outValue,&dwLen) != ERROR_SUCCESS)
	{
		vstring errorInfo = "QueryRegistryValuevstring RegQueryValueEx-name(";
		errorInfo.append(name);
		errorInfo.append(")failed");
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),errorInfo));
		RegCloseKey(reg);
		return false;
	}
	RegCloseKey(reg);
	return true;
}

bool CFuncs::SetRegistryValueInteger(const char* subKey,const char* name,const int val)
{
    HKEY  reg; 
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey,0,KEY_ALL_ACCESS,&reg) != ERROR_SUCCESS)
	{
		WriteLogInfo(SLT_ERROR,"RegOpenKeyEx(%s)failed,error id:%d.\n",subKey,GetLastError());
		return false;
	}
	dwType = REG_DWORD;
	if(RegSetValueEx(reg,name,0,dwType,(LPBYTE)(&val),sizeof(uint32)) != ERROR_SUCCESS)
	{
		WriteLogInfo(SLT_ERROR,"RegSetValueEx-name(%s)failed,error id:%d.\n",name,GetLastError());
		RegCloseKey(reg);
		return false;
	}
	RegCloseKey(reg);
	return true;
}

bool CFuncs::SetRegistryValueString(const char* subKey,const char* name,const char* val)
{
    HKEY  reg; 
	DWORD dwType;

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,subKey,0,KEY_ALL_ACCESS,&reg) != ERROR_SUCCESS)
	{
		WriteLogInfo(SLT_ERROR,"RegOpenKeyEx(%s)failed,error id:%d.\n",subKey,GetLastError());
		return false;
	}
	dwType = REG_SZ;
	if(RegSetValueEx(reg,name,0,dwType,(LPBYTE)val,(DWORD)(strlen(val) + 1)) != ERROR_SUCCESS)
	{
		WriteLogInfo(SLT_ERROR,"RegSetValueEx-name(%s)failed,error id:%d.\n",name,GetLastError());
		RegCloseKey(reg);
		return false;
	}
	RegCloseKey(reg);
	return true;
}


void CFuncs::WriteLogInfo(const uint8 type,const vstring& content)
{
	vstring logPath;
	FILE*  fStream;
	time_t timestamp;
	struct tm rawtime;
	char   tempTimestamp[64];
	char   tempFileName[128];

	time(&timestamp);
	localtime_s(&rawtime,&timestamp);
	memset(tempFileName,0,sizeof(tempFileName));
	strftime(tempFileName, sizeof(tempFileName) - 1, "%Y-%m-%d_%H.log", &rawtime);

	logPath = gLogPath;
	logPath.append(tempFileName);

	memset(tempTimestamp,0,sizeof(tempTimestamp));
	strftime(tempTimestamp, sizeof(tempTimestamp) - 1, "%Y-%m-%d %H:%M:%S", &rawtime); 

	vstring errorType = "";
    switch(type)
	{
	case SLT_ERROR:
		errorType = "错误";
		break;
	case SLT_WARNING:
		errorType = "警告";
		break;
	case SLT_INFORMATION:
		errorType = "信息";
		break;
	default:
		errorType = "未知";
		break;
	}

	fopen_s(&fStream,logPath.c_str(),"a+");
	if(fStream == NULL)
	{
		printf("打开日志文件(%s)失败,错误信息: %d\n", logPath.c_str(), GetLastError());
	}
	else
	{
		fprintf(fStream,"[%s] : [%s] ",tempTimestamp,errorType.c_str());
		fprintf(fStream, "%s\r\n", content.c_str());		
		fclose(fStream);	 
		fStream = NULL;
	}
	printf("[%s] : [%s] %s\r\n", tempTimestamp, errorType.c_str(), content.c_str());
}

void CFuncs::WriteLogInfo(const uint8 type,const char *format,...)
{
	int ret;
	vstring tempLog;
	va_list argptr;
	char logBuf[1024 * 20];

	memset(logBuf,0,sizeof(logBuf));
	va_start(argptr, format);
	ret = vsnprintf_s(logBuf, sizeof(logBuf) - 1,_TRUNCATE, format, argptr);
	va_end(argptr);

	if(ret <= 0)
	{
		return;
	}
	tempLog = logBuf;
	WriteLogInfo(type,tempLog);
}

bool CFuncs::CreateBinaryFile(const vstring& path,const char* buffer,const uint32 bufferLen)
{
	if(path.empty() || buffer == NULL || bufferLen <= 0)
	{
		return false;
	}
	FILE* fStream = NULL;
	fopen_s(&fStream,path.c_str(),"wb");
	if(fStream == NULL)
	{
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),"CreateBinaryFile 打开文件(%s)失败",path.c_str()));
		return false;
	}
	size_t ret = fwrite(buffer,bufferLen,1,fStream);
	fclose(fStream);
	fStream = NULL;

	if(ret == 1)
	{
		return true;
	}
	WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),"CreateBinaryFile 写文件(%s)失败",path.c_str()));
	return false;
}

//生成数据GUID
vstring CFuncs::GetGUID(void)
{
	GUID pguid; 
	vstring strGUID;
	char tmpGUID[MAX_PATH] = { 0 };

	strGUID.assign(GetTimeFileName());
	if(S_OK != CoCreateGuid(&pguid))
	{
		return strGUID;
	}
	sprintf_s(tmpGUID,_countof(tmpGUID),"{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		pguid.Data1, pguid.Data2, pguid.Data3, pguid.Data4[0], pguid.Data4[1], pguid.Data4[2], 
		pguid.Data4[3], pguid.Data4[4], pguid.Data4[5], pguid.Data4[6], pguid.Data4[7]
	);
	strGUID.assign(tmpGUID);
	return strGUID;
}

//隐藏执行外部程序
bool CFuncs::ExecuteExternalProgram(const char* path, const char* param, const uint32 timeout)
{
	bool bRet = false;
	if(!CFuncs::FileExist(path))
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"执行外部程序失败,文件不存在,%s\n", path);
		return false;
	}
	vstring strTmpExt = CFuncs::ParseFileExt(path);
	if(strTmpExt.empty())
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"执行外部程序失败,文件没有扩展名,%s\n", path);
		return false;
	}	
	//使用ShellExecuteEx,这样做的原因是防止进程被卡死
	SHELLEXECUTEINFO procInfo;
	memset(&procInfo,0, sizeof(SHELLEXECUTEINFO));
	procInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	procInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	procInfo.lpFile = path;
	procInfo.lpParameters = param;
	procInfo.nShow = SW_HIDE;

	if(!ShellExecuteEx(&procInfo))
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"执行外部程序失败,ShellExecuteEx(%s),错误信息:%u\n", path, GetLastError());
		return false;
	}
	if (procInfo.hProcess == NULL)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"执行外部程序成功，但hProcess为NULL");
		return false;
	}
	//检查解压执行超时
	DWORD dwRet = WaitForSingleObject(procInfo.hProcess, timeout * 1000);
	switch(dwRet)
	{
	case WAIT_ABANDONED:
		CFuncs::WriteLogInfo(SLT_ERROR,"等待执行外部程序返回值:WAIT_ABANDONED,%s",path);
		break;
	case WAIT_OBJECT_0:
		bRet = true;
		break;
	case WAIT_TIMEOUT:
		CFuncs::WriteLogInfo(SLT_ERROR,"等待执行外部程序超时(%u)返回值:WAIT_TIMEOUT,%s",timeout,path);
		break;
	case WAIT_FAILED:
		CFuncs::WriteLogInfo(SLT_ERROR,"等待执行外部程序失败,返回值:WAIT_FAILED,%s",path);
		break;
	default:
		CFuncs::WriteLogInfo(SLT_ERROR,"等待执行外部程序返回未知值,返回值:%u,%s",dwRet,path);
		break;
	}
    BOOL bTemp = TerminateProcess(procInfo.hProcess,0);
	CloseHandle(procInfo.hProcess);
	if(!bTemp)
	{
		bRet = false;
	}
	return bRet;
}

//删除文件夹及其文件
int CFuncs::DeleteDirectory(const char* path)
{
	char szParam[MAX_PATH] = { 0 };
	if(!FileExist(path))
	{
		return -1;
	}
	sprintf_s(szParam,sizeof(szParam) - 1,"cmd /c rd \"%s\" /s /q",path);
	return WinExec(szParam,SW_HIDE);
}

//删除文件夹及其文件
int CFuncs::DeleteDirectory(const vstring& path)
{
	char szParam[MAX_PATH] = { 0 };
	if(!FileExist(path))
	{
		return -1;
	}
	sprintf_s(szParam,sizeof(szParam) - 1,"cmd /c rd \"%s\" /s /q",path.c_str());
	return WinExec(szParam,SW_HIDE);
}

//拆分参数(如:a=b)
bool CFuncs::SplitParameter(const vstring& param, vstring& key, vstring& value)
{
	key = "";
	value = "";
	if(param.length() < 1)
	{
		return false;
	}
	size_t pos = param.find("=",0);
	if(pos == string::npos)
	{
		key = param;
		return false;
	}
	key = param.substr(0,pos);
	value = param.substr(pos + 1);
	return true;
}

bool CFuncs::RollbackLog(const vstring& path,const uint16 saveDays)
{
	vstring logPath = path;
	logPath.append("log\\");

	vstring findPath = logPath;
	findPath.append("*.*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"开始执行回滚日志文件失败，%s\n",logPath.c_str());
		return false;
	}

	time_t currentTime = CFuncs::GetTimestamp();
	CFuncs::WriteLogInfo(SLT_INFORMATION,"开始执行回滚日志文件，%s\n",logPath.c_str());
	do
	{
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
		{			
			//将文件的FileTime转换为time_t
            ULARGE_INTEGER tmpUI; 
            tmpUI.LowPart  = FindFileData.ftLastWriteTime.dwLowDateTime; 
            tmpUI.HighPart = FindFileData.ftLastWriteTime.dwHighDateTime; 
            //int64 tmpValue = FindFileData.ftLastWriteTime.dwHighDateTime << 32 + FindFileData.ftLastWriteTime.dwLowDateTime; 
            time_t fileTime = ((LONGLONG)(tmpUI.QuadPart - 116444736000000000) / 10000000); 
            
			uint64 tmpBalance = currentTime - fileTime;
			uint64 tmpSaveDays = saveDays * 24 * 60 * 60;
			if(tmpBalance >= tmpSaveDays)
			{
				vstring tmpPath = logPath;
				tmpPath.append(FindFileData.cFileName);
				if(!DeleteFile(tmpPath.c_str()))
				{
					CFuncs::WriteLogInfo(SLT_ERROR,"删除日志文件失败(%s),Error Code:%u.\n",tmpPath.c_str(),GetLastError());
				}
			}
		}	
	}while(FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
	return true;
}


char* CFuncs::SearchSubMemory(const char* data, const size_t data_len, const char* substr, const size_t substr_len, const size_t offset)
{
	if(NULL == data || data_len < 1 || NULL == substr || substr_len < 1)
	{
		return NULL;
	}
	size_t num = 0;
	const char* pTmp = data; 
	for(size_t i = 0; i < data_len; i++ )
	{
		if(substr_len > data_len - num)
		{
			return NULL;
		}
		if(memcmp(pTmp, substr, substr_len) == 0)
		{
			return (char *)pTmp;
		}
		num++;
		pTmp++;
		if(num == data_len - offset)
		{
			int rrr = 1;
		}
	}
	return NULL;
}


char* CFuncs::SearchSubMemory(const char* data, const size_t data_len, const char* substr, const size_t substr_len, const size_t offset, size_t& index)
{
	if(NULL == data || data_len < 1 || NULL == substr || substr_len < 1)
	{
		return NULL;
	}
	size_t num = 0;
	const char* pTmp = data; 
	for(size_t i = 0; i < data_len; i++ )
	{
		if(substr_len > data_len - num)
		{
			return NULL;
		}
		if(memcmp(pTmp, substr, substr_len) == 0)
		{
			index = num;
			return (char *)pTmp;
		}
		num++;
		pTmp++;
		if(num == data_len - offset)
		{
			int rrr = 1;
		}
	}
	return NULL;
}


//全部转换小写
vstring CFuncs::UpperToLower(const vstring& src)
{
	vstring result = src;
	for(int i = 0; i < static_cast<int>(result.size()); i++)
	{
		if((result[i] >= 'A') && (result[i] <= 'Z'))
		{
			result[i] = result[i] - 'A' + 'a';
		}
	}
	return result;
}

vstring CFuncs::Trim(const vstring& str)
{
	vstring str1 = str;
	vstring::size_type pos = str1.find_last_not_of(' ');
	if(pos != string::npos)
	{
		str1.erase(pos + 1);
		pos = str1.find_first_not_of(' ');
		if(pos != string::npos) 
		{
			str1.erase(0, pos);
		}
	}
	else 
	{
		str1.erase(str1.begin(), str1.end());
	}
	return str;
}
/*
bool CFuncs::GetSystemVersion(vstring& version)
{
	bool ret = false;
	SYSTEM_INFO sysInfo;                //用SYSTEM_INFO结构判断64位AMD处理器  
	OSVERSIONINFOEX osvi;
	vstring osName;

	version.assign("");
	GetSystemInfo(&sysInfo);
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if(!GetVersionEx((OSVERSIONINFO *)(&osvi)))
	{
		return ret;
	}
	osName.assign("");
	//下面根据版本信息判断操作系统名称
	switch(osvi.dwMajorVersion) //判断主版本号
	{                       
	case 4:
		switch(osvi.dwMinorVersion)
		{                //判断次版本号
		case 0:
			if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
			{
				osName.assign("Microsoft Windows NT 4.0");  //1996年7月发布
			}
			else if(osvi.dwPlatformId==VER_PLATFORM_WIN32_WINDOWS)
			{
				osName.assign("Microsoft Windows 95");
			}
			break;
		case 10:
			osName.assign("Microsoft Windows 98");
			break;
		case 90:
			osName.assign("Microsoft Windows ME");
			break;
		}
		break;
	case 5:
		switch(osvi.dwMinorVersion)
		{               //再比较dwMinorVersion的值
		case 0:
			osName.assign("Microsoft Windows 2000");    //1999年12月发布
			break;
		case 1:
			osName.assign("Microsoft Windows XP");      //2001年8月发布
			break;
		case 2:
			if(osvi.wProductType == VER_NT_WORKSTATION && sysInfo.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
				osName.assign("Microsoft Windows XP Professional x64 Edition");
			//else if(GetSystemMetrics(SM_SERVERR2)==0)
			//	osName.assign("Microsoft Windows Server 2003");   //2003年3月发布
			//else if(GetSystemMetrics(SM_SERVERR2)!=0)
			//	osName.assign("Microsoft Windows Server 2003 R2");
			break;
		}
		break;
	case 6:
		switch(osvi.dwMinorVersion)
		{
		case 0:
			if(osvi.wProductType == VER_NT_WORKSTATION)
				osName.assign("Microsoft Windows Vista");
			else
				osName.assign("Microsoft Windows Server 2008");   //服务器版本
			break;
		case 1:
			if(osvi.wProductType == VER_NT_WORKSTATION)
				osName.assign("Microsoft Windows 7");
			else
				osName.assign("Microsoft Windows Server 2008 R2");
			break;
		case 2:
			if(osvi.wProductType == VER_NT_WORKSTATION)
				osName.assign("Microsoft Windows 8");
			else
				osName.assign("Microsoft Windows Server 2012");
			break;
		case 3:
			if(osvi.wProductType == VER_NT_WORKSTATION)
				osName.assign("Microsoft Windows 8.1");
			else
				osName.assign("Microsoft Windows Server 2012 R2");
			break;
		}
		break;
	case 10:
		switch(osvi.dwMinorVersion)
		{
		case 0:
			if(osvi.wProductType == VER_NT_WORKSTATION)
				osName.assign("Microsoft Windows 10");
			else
				osName.assign("Microsoft Windows Server 2016 Preview");   
			break;
		}
	default:
		osName.assign("未知操作系统");
		break;
	}
	osName.append(" ");
	osName.append(osvi.szCSDVersion);
	osName.append(" ");
	switch(sysInfo.wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_INTEL:
		osName.append("Intel");
		break;
	case PROCESSOR_ARCHITECTURE_MIPS:
		osName.append("MIPS");
		break;
	case PROCESSOR_ARCHITECTURE_ALPHA:
		osName.append("Alpha");
		break;
	case PROCESSOR_ARCHITECTURE_PPC:
		osName.append("PPC");
		break;
	case PROCESSOR_ARCHITECTURE_SHX:
		osName.append("SHX");
		break;
	case PROCESSOR_ARCHITECTURE_ARM:
		osName.append("ARM");
		break;
	case PROCESSOR_ARCHITECTURE_IA64:
		osName.append("IA64");
		break;
	case PROCESSOR_ARCHITECTURE_ALPHA64:
		osName.append("Alpha64");
		break;
	case PROCESSOR_ARCHITECTURE_MSIL:
		osName.append("MSIL");
		break;
	case PROCESSOR_ARCHITECTURE_AMD64:
		osName.append("AMD64");
		break;
	case PROCESSOR_ARCHITECTURE_IA32_ON_WIN64:
		osName.append("IA32 On WIN64");
		break;
	}
	version.assign(osName);
	WriteLogInfo(SLT_INFORMATION,"%s\n",osName.c_str());
	return true;
}
*/
//Jan 25 17:07:11 2016 转换为 2016-01-25 17:07:11
bool CFuncs::WeekOffConversion(const vstring& timeStr, vstring& outTime)
{
	outTime.assign("");
	if(timeStr.empty())
	{
		return false;
	}
	char monCh[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	for(int i = 0; i < 12; i++)
	{
		size_t iSearch = timeStr.find(monCh[i]);
		if(string::npos != iSearch)
		{	
			size_t length = timeStr.length();
			if(length < 6)
			{
				return false;
			}
			vstring yeahStr = timeStr.substr(length - 4, length);

			vstring remainStr = timeStr.substr(0, length - 5);

			char ch[11] = {0};
			errno_t ret = _itoa_s(i+1, ch, 10, 10);
			if( 0 != ret)
			{
				return false;
			}
			vstring monStr = string(ch);
			if(iSearch + sizeof(monCh[i]) > remainStr.length())
			{
				return false;
			}
			vstring tmpStr = remainStr.substr(iSearch + sizeof(monCh[i]), remainStr.length());

			tmpStr = yeahStr.append("-") + monStr.append("-") + tmpStr;
			outTime = tmpStr;
			return true;
		}
	}
	return false;
}

vstring CFuncs::DeleteSpaceKey(const vstring& value)
{
	char str[1024 * 2];
	memset(str,0,sizeof(str));
	for(size_t i = 0, j = 0; i < value.length(); i++)
	{
		if(j >= sizeof(str) - 1)
		{
			break;
		}
		if(' ' != value.at(i))
		{
			str[j] = value.at(i);
			j++;
		}
	}
	string str1 = string(str);
	return str1;
}

vstring CFuncs::DeleteSpaceKey(const char* value, const size_t len)
{
	char str[1024 * 2];
	memset(str,0,sizeof(str));
	for(size_t i = 0, j = 0; i < len; i++)
	{
		if(j >= sizeof(str) - 1)
		{
			break;
		}
		if(' ' != value[i])
		{
			str[j] = value[i];
			j++;
		}
	}
	string str1 = string(str);
	return str1;
}

bool CFuncs::Init(void)
{
	char szPath[MAX_PATH] = { 0 };

	GetModuleFileName(NULL,szPath,_countof(szPath) - sizeof(char));
	return Init(szPath);
}

bool CFuncs::Init(const char* path)
{
	char drive[_MAX_DRIVE] = { 0 };
	char dir[_MAX_DIR] = { 0 };
	char fname[_MAX_FNAME] = { 0 };
	char ext[_MAX_EXT] = { 0 };
	
	gAppPath.clear();
	gLogPath.clear();
	gProcName.clear();
	gDeviceId.clear();

	_splitpath_s(path,drive,_countof(drive),dir,_countof(dir),fname,_countof(fname),ext,_countof(ext));

	gAppPath = drive;
	gAppPath.append(dir);

	gProcName = fname;
	gProcName.append(ext);

	gLogPath = gAppPath;
	gLogPath.append("Log\\");
	CreateDirectory(gLogPath.c_str(),NULL);

	gRulePath = gAppPath;
	gRulePath.append("Rule\\");
	CreateDirectory(gRulePath.c_str(), NULL);

	gTemporaryPath = gAppPath;
	gTemporaryPath.append("Temporary\\");
	CreateDirectory(gTemporaryPath.c_str(), NULL);

	gConfigPath = gAppPath;
	gConfigPath.append("Config\\");
	CreateDirectory(gConfigPath.c_str(), NULL);

	gRuleKeywordListXML = gRulePath;     //规则-关键字列表
	gRuleKeywordListXML.append(DEFAULT_RULE_KEYWORD_LIST);

	return true;
}

FILE* CFuncs::AppendBinaryFile(const string& path,const char* buffer,const size_t bufferLen)
{
	if(path.empty() || buffer == NULL || bufferLen <= 0)
	{
		return NULL;
	}
	FILE* fStream = NULL;
	fopen_s(&fStream,path.c_str(),"ab");
	if(fStream == NULL)
	{
		WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),"AppendBinaryFile1 打开文件(%s)失败",path.c_str()));
		return NULL;
	}
	size_t ret = fwrite(buffer,bufferLen,1,fStream);
	if(ret == 1)
	{
		return fStream;
	}
	WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),"AppendBinaryFile1 写文件(%s)失败",path.c_str()));
	fclose(fStream);
	fStream = NULL;
	return NULL;
}

bool CFuncs::AppendBinaryFile(FILE* stream, const char* buffer,const size_t bufferLen)
{
	if(NULL == stream || buffer == NULL || bufferLen <= 0)
	{
		return false;
	}
	size_t ret = fwrite(buffer,bufferLen,1,stream);
	if(ret == 1)
	{
		return true;
	}
	WriteLogInfo(SLT_ERROR,SvcFormatMessage(GetLastError(),"AppendBinaryFile2 写文件失败"));
	fclose(stream);
	stream = NULL;
	return false;
}

void CFuncs::AppendBinaryFile(FILE* stream)
{
	if(NULL != stream)
	{
		fclose(stream);
		stream = NULL;
	}
}
void CFuncs::Destroy()
{

}
