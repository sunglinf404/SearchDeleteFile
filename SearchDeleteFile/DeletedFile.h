#pragma once

# include <stdlib.h> 
#include <vector>
#include "../Common/Funcs.h"
#include "../Mime/UrlConver.h"
#include <windows.h>
#include <winioctl.h>
#include <stdio.h>
#include<iostream>
#include <malloc.h>
#include "DeleteStruct.h"
#include <string>
#include "../Mime/Base64.h"

#define  SECTOR_SIZE		512
#define  FILE_SECTOR_SIZE	1024
#define  FIRST_MBR_RANCE	40960

class CDeletedFile
{
private:
	HANDLE m_ParentDevice;
private:
	bool  GetHostNtfsStartAddr(HANDLE hDevice,vector<DWORD64>& start_sq, UCHAR *MbrBuffer);

	bool  ReadSQData(HANDLE hDevice, UCHAR* Buffer, DWORD SIZE, DWORD64 addr, DWORD *BackBytesCount);

	bool  GetMbrStartAddr(HANDLE hDevice, vector<DWORD64>& start_sq, UCHAR *MbrBuffer, DWORD64 *LiAddr,DWORD *PtitionIdetifi);

	bool  GetPititionName(vector<char>&chkdsk,vector<DWORD64>&dwDiskNumber);

	bool  GetMFTAddr(HANDLE hDevice,DWORD64 start_sq,vector<LONG64>& HostStarMFTAddr,vector<DWORD64>& HostStarMFTLen
		,UCHAR *HostPatitionCuNum,UCHAR *PatitionBuffer);

	bool  GetH80DataRunAddr(HANDLE Drive,UCHAR *CacenBuff,DWORD64 Start_NTFS_Addr,LONG64 Start_MftCU,UCHAR CU_NUM,DWORD Refernum
		,DWORD *H20FileRefer,UCHAR *H20Buff,DWORD fNameNum,const char* fName, vector<DWORD> fNamelen
		,string& fileList,DWORD *ParentMft);

	bool  GetH20FileReferH80Addr(HANDLE Drive,UCHAR *CacenBuff,DWORD64 StartNtfsAddr,LONG64 StartMftAddr,string& fileList);

	bool  UnicodeToZifu(UCHAR *Source_Unico,string& fileList, DWORD Size);

	bool GetFileNameAndPath(HANDLE P_Driver, DWORD64 StartNTFSaddr, vector<LONG64> StartMFTaddr, vector<DWORD64> StartMFTaddrLen, UCHAR CuNumber
		,DWORD ParentMFT,UCHAR *CacheBuffer,string& fileLists,const char* chkdk);

	bool FileTimeConver(UCHAR* szFileTime, string& strTime);

public:
	CDeletedFile(void);
	~CDeletedFile(void);

public:
	bool WINAPI SearchDeletedFile(const char* partion, const char* fileType,string& fileLists);

};

