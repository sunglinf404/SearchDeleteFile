#pragma once
#include "../Common/Funcs.h"
#include "../Mime/Base64.h"
#include <vector>
#include "DeleteStruct.h"
#define  SECTOR_SIZE 512
#define  FILE_SECTOR_SIZE	1024
class CFileRecovery
{
private:
	HANDLE hDrive;
private:	
	bool WriteLargeFile(char *WriteBuff,char *filename,const char *fileaddr,UCHAR CuNum);

	bool  ReadSQData(HANDLE hDevice, UCHAR* Buffer, DWORD SIZE, DWORD64 addr, DWORD *BackBytesCount);

	bool WriteLitteFile(char *filename,const char* h80data);

	bool ReadCharData(HANDLE hDevice, char* Buffer, DWORD SIZE, DWORD64 addr, DWORD *BackBytesCount);

public:
	CFileRecovery(void);
	~CFileRecovery(void);
public:
	bool FileRecovery(const char* fileName, const char* fileAddr, const char* filePath, const char* patition);


};

