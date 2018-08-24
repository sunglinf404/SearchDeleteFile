#include "StdAfx.h"
#include "FileRecovery.h"


CFileRecovery::CFileRecovery(void)
{
	hDrive=NULL;
}


CFileRecovery::~CFileRecovery(void)
{
	if(hDrive != NULL && hDrive != INVALID_HANDLE_VALUE)
	{
		(void)CloseHandle(hDrive);	
		hDrive=NULL;
	}
	
}
bool  CFileRecovery::ReadSQData(HANDLE hDevice, UCHAR* Buffer, DWORD SIZE, DWORD64 addr, DWORD *BackBytesCount)
{
	LARGE_INTEGER LiAddr = {NULL};	
	LiAddr.QuadPart = addr;
	DWORD dwError = 0;

	BOOL bRet = SetFilePointerEx(hDevice, LiAddr, NULL,FILE_BEGIN);
	if(!bRet)
	{

		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("ReadSQData::SetFilePointerEx失败!,\
										   错误返回码: dwError = %d"), dwError);
		return false;	
	}
	bRet = ReadFile(hDevice, Buffer, SIZE, BackBytesCount, NULL);
	if(!bRet)
	{
		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("ReadSQData::ReadFile失败!,\
										   错误返回码: dwError = %d"), dwError);					
		return false;	
	}

	return true;
}
bool  CFileRecovery::ReadCharData(HANDLE hDevice, char* Buffer, DWORD SIZE, DWORD64 addr, DWORD *BackBytesCount)
{
	LARGE_INTEGER LiAddr = {NULL};	
	LiAddr.QuadPart = addr;
	DWORD dwError = 0;

	BOOL bRet = SetFilePointerEx(hDevice, LiAddr, NULL,FILE_BEGIN);
	if(!bRet)
	{

		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("ReadSQData::SetFilePointerEx失败!,\
										   错误返回码: dwError = %d"), dwError);
		return false;	
	}
	bRet = ReadFile(hDevice, Buffer, SIZE, BackBytesCount, NULL);
	if(!bRet)
	{
		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("ReadSQData::ReadFile失败!,\
										   错误返回码: dwError = %d"), dwError);					
		return false;	
	}

	return true;
}
bool CFileRecovery::WriteLargeFile(char *WriteBuff,char *filename,const char *fileaddr,UCHAR CuNum)
{
	if(fileaddr == NULL)
	{
		return false;
	}
	vector<LONG64>H80Addr;
	vector<DWORD>H80Len;
	UCHAR* pAddr = (UCHAR*)(fileaddr + 9);
	/*UCHAR *H80datarun = (UCHAR*)fileaddr[8];*/
	DWORD h80offset = 0;//跳过标识符FileAddr:
	DWORD TotalLen = NULL;
	BOOL Ret = FALSE;
	DWORD BackBytesCount = NULL;

	DWORD addrLen = strlen((char*)fileaddr);
	UCHAR* addrBuff = (UCHAR*)malloc(addrLen - 8);
	if (NULL == addrBuff)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("CFileRecovery::WriteLargeFile malloc失败!"));
		return false;
	}
	memset(addrBuff, 0, addrLen - 8);
	if(CBase64::Decode(pAddr, addrLen - 9, addrBuff, addrLen - 8) < 0)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("CFileRecovery::WriteLargeFile CBase64::Decode解码失败!"));
		return false;
	}
	DWORD64 filerealSize = NULL;
	RtlCopyMemory(&filerealSize, &addrBuff[0], 8);
	
	UCHAR *H80datarun = &addrBuff[8];
	
	while(h80offset < (addrLen - 17))
	{
		if (H80datarun[h80offset] > 0 && H80datarun[h80offset] < 0x50)
		{
			UCHAR adres_fig = H80datarun[h80offset] >> 4;
			UCHAR len_fig = H80datarun[h80offset] & 0xf;

			DWORD H80_datarun_len = NULL;
			for(UCHAR w = len_fig; w > 0; w--)
			{
				H80_datarun_len = H80_datarun_len | (H80datarun[h80offset+w] << (8*(w-1)));
				
			}
			H80Len.push_back(H80_datarun_len);
			LONG64 H80_datarun = NULL;
			for (int w = adres_fig; w > 0; w--)
			{
				H80_datarun = H80_datarun | (H80datarun[h80offset+w+len_fig] << (8*(w-1)));
				
			}

			if (H80datarun[h80offset+adres_fig+len_fig] > 127)
			{
				if (adres_fig == 3)
				{
					H80_datarun = ~(H80_datarun^0xffffff);
				}
				if (adres_fig == 2)
				{
					H80_datarun = ~(H80_datarun^0xffff);					
				}

			} 

			if (H80Addr.size() > 0)
			{
				H80Addr.push_back((H80Addr[(H80Addr.size()-1)] + H80_datarun));
			}else
			{
				H80Addr.push_back(H80_datarun);
			}

			h80offset = h80offset + adres_fig + len_fig + 1;
		}else
		{
			break;
		}
	}
	if (NULL == H80Addr.size())
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("WriteLargeFile::H80Addr为空!"));
		return false;
	}else if(H80Addr.size() != H80Len.size())
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("WriteLargeFile::地址与长度数目不一致!"));
		return false;
	}
	for (DWORD H80Num = NULL; H80Num < H80Len.size(); H80Num++)
	{
		TotalLen += H80Len[H80Num] * CuNum;
	}
	if ((TotalLen * SECTOR_SIZE) < filerealSize)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("WriteLargeFile::实际长度大于了分配长度!"));
		return false;
	}
	if (TotalLen < 8000)//扇区
	{
		memset(WriteBuff, 0, 8000 * SECTOR_SIZE);
		DWORD WriteIndex = NULL;
		for (DWORD H80Num = NULL; H80Num < H80Addr.size(); H80Num++)
		{
			Ret = ReadCharData(hDrive, &WriteBuff[WriteIndex], H80Len[H80Num] * CuNum * SECTOR_SIZE,
				H80Addr[H80Num] * CuNum * SECTOR_SIZE,
				&BackBytesCount);		
			if(!Ret)
			{			
				CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:ReadSQData失败,错误是%d", GetLastError());

				return false;	
			}
			WriteIndex += (H80Len[H80Num] * CuNum * SECTOR_SIZE);
		}
		DWORD nNumberOfBytesWritten = NULL;
		HANDLE hFile_recov = ::CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile_recov != INVALID_HANDLE_VALUE)
		{

			Ret = ::WriteFile(hFile_recov, WriteBuff, (DWORD)filerealSize, &nNumberOfBytesWritten, NULL);
			if(!Ret)
			{	
				CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
				(void)CloseHandle(hFile_recov);
				hFile_recov = NULL;
				return false;
			}
			(void)CloseHandle(hFile_recov);
			hFile_recov = NULL;

		}
		else
		{
			CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:CreateFileW失败,错误是%d", GetLastError());		
			return false;
		}
	}
	else
	{
		HANDLE hFile_recov = ::CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
		if (hFile_recov == INVALID_HANDLE_VALUE)
		{
			CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:CreateFileW失败,错误是%d", GetLastError());
			return false;
		}
		LARGE_INTEGER WriteIndex = {NULL};
		
		for (DWORD H80Num = NULL; H80Num < H80Addr.size(); H80Num++)
		{
			if ((H80Len[H80Num] * CuNum) < 8000)
			{
				memset(WriteBuff, 0, 8000*512);
				Ret = ReadCharData(hDrive,&WriteBuff[0],H80Len[H80Num] * CuNum * SECTOR_SIZE,
					H80Addr[H80Num] * CuNum * SECTOR_SIZE,
					&BackBytesCount);		
				if(!Ret)
				{			
					CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:ReadSQData失败,错误是%d", GetLastError());
					(void)CloseHandle(hFile_recov);
					hFile_recov = NULL;
					return false;	
				}
				Ret = SetFilePointerEx(hFile_recov,
					WriteIndex,
					NULL,
					FILE_BEGIN);
				if(!Ret)
				{
					CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:SetFilePointerEx失败,错误是%d", GetLastError());
					(void)CloseHandle(hFile_recov);
					hFile_recov = NULL;
					return false;	
				}
				DWORD nNumberOfBytesWritten = 0;
				if (((WriteIndex.QuadPart) + (H80Len[H80Num] * CuNum * SECTOR_SIZE)) > filerealSize)
				{
					Ret = ::WriteFile(hFile_recov, WriteBuff, (DWORD)(filerealSize - (WriteIndex.QuadPart)) , &nNumberOfBytesWritten, NULL);
					if(!Ret)
					{	
						CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
						(void)CloseHandle(hFile_recov);
						hFile_recov = NULL;
						return false;
					}
					break;
				} 
				else
				{
					Ret = ::WriteFile(hFile_recov, WriteBuff, H80Len[H80Num] * CuNum * SECTOR_SIZE , &nNumberOfBytesWritten, NULL);
					if(!Ret)
					{	
						CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
						(void)CloseHandle(hFile_recov);
						hFile_recov = NULL;
						return false;
					}
				}
				
				(WriteIndex.QuadPart) += (H80Len[H80Num] * CuNum * SECTOR_SIZE);
			} 
			else
			{
				DWORD H80DataOffset = NULL;//用于h80地址偏移
				DWORD nNumberOfBytesWritten = NULL;
				for(DWORD n = 0; n <= (H80Len[H80Num] * CuNum) / 8000; n++)
				{
					memset(WriteBuff, 0, 8000 * 512);
					if (n == (H80Len[H80Num] * CuNum)/8000)//剩余的 或此地址数据小于 8000 为0时
					{
						Ret = ReadCharData(hDrive,&WriteBuff[0],(H80Len[H80Num] * CuNum)* SECTOR_SIZE - H80DataOffset,
							H80Addr[H80Num] * CuNum * SECTOR_SIZE + H80DataOffset,
							&BackBytesCount);		
						if(!Ret)
						{			
							CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:ReadSQData失败,错误是%d", GetLastError());
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return false;	
						}
						Ret = SetFilePointerEx(hFile_recov,
							WriteIndex,
							NULL,
							FILE_BEGIN);
						if(!Ret)
						{
							CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:SetFilePointerEx失败,错误是%d", GetLastError());
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return false;	
						}
						if (((WriteIndex.QuadPart) + ((H80Len[H80Num] * CuNum) * SECTOR_SIZE - H80DataOffset)) > filerealSize)
						{
							Ret = ::WriteFile(hFile_recov, WriteBuff, (DWORD)(filerealSize - (WriteIndex.QuadPart)) , &nNumberOfBytesWritten, NULL);
							if(!Ret)
							{	
								CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
								(void)CloseHandle(hFile_recov);
								hFile_recov = NULL;
								return false;
							}
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return true;
						} 
						else
						{
							Ret = ::WriteFile(hFile_recov, WriteBuff, (H80Len[H80Num] * CuNum) * SECTOR_SIZE - H80DataOffset , &nNumberOfBytesWritten, NULL);
							if(!Ret)
							{	
								CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
								(void)CloseHandle(hFile_recov);
								hFile_recov = NULL;
								return false;
							}
						}
						
						(WriteIndex.QuadPart) += ((H80Len[H80Num] * CuNum) * SECTOR_SIZE - H80DataOffset);
					} 
					else
					{
						Ret = ReadCharData(hDrive,&WriteBuff[0],8000 * SECTOR_SIZE,
							H80Addr[H80Num] * CuNum * SECTOR_SIZE + H80DataOffset,
							&BackBytesCount);		
						if(!Ret)
						{			
							CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:ReadSQData失败,错误是%d", GetLastError());
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return false;	
						}
						H80DataOffset += (8000 * SECTOR_SIZE);
						Ret = SetFilePointerEx(hFile_recov,
							WriteIndex,
							NULL,
							FILE_BEGIN);
						if(!Ret)
						{
							CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:SetFilePointerEx失败,错误是%d", GetLastError());
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return false;	
						}
						if (((WriteIndex.QuadPart) + (8000 * SECTOR_SIZE)) > filerealSize)
						{
							Ret = ::WriteFile(hFile_recov, WriteBuff, (DWORD)(filerealSize - (WriteIndex.QuadPart)) , &nNumberOfBytesWritten, NULL);
							if(!Ret)
							{	
								CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
								(void)CloseHandle(hFile_recov);
								hFile_recov = NULL;
								return false;
							}
							(void)CloseHandle(hFile_recov);
							hFile_recov = NULL;
							return true;
						} 
						else
						{
							Ret = ::WriteFile(hFile_recov, WriteBuff, 8000 * SECTOR_SIZE , &nNumberOfBytesWritten, NULL);
							if(!Ret)
							{	
								CFuncs::WriteLogInfo(SLT_ERROR, "WriteLargeFile:WriteFile失败,错误是%d", GetLastError());
								(void)CloseHandle(hFile_recov);
								hFile_recov = NULL;
								return false;
							}
						}
						
						(WriteIndex.QuadPart) += (8000 * SECTOR_SIZE);
					}
				}
			}
		}
		(void)CloseHandle(hFile_recov);
		hFile_recov = NULL;
	}
	return true;
}
bool CFileRecovery::WriteLitteFile(char *filename,const char* h80data)
{
	BOOL Ret = FALSE;
	DWORD nNumberOfBytesWritten=NULL;
	HANDLE hFile_recov = ::CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, NULL, NULL);
	if (hFile_recov == INVALID_HANDLE_VALUE)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, "WriteLitteFile:CreateFileW失败,错误是%d", GetLastError());
		return false;
	}

	int dataLen = strlen((char*)h80data);
	UCHAR *dataBuff = (UCHAR*)malloc(dataLen - 8);
	if (NULL == dataBuff)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("CFileRecovery::WriteLitteFile失败!"));
		CloseHandle(hFile_recov);
		return false;
	}
	memset(dataBuff, 0, dataLen - 8);
	if(CBase64::Decode((unsigned char*)h80data + 9, dataLen - 9, dataBuff,  dataLen - 8) < 0)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("CFileRecovery::WriteLitteFile CBase64::Decode解码失败!"));
		CloseHandle(hFile_recov);
		return false;
	}
	/*string strData = string((char*)H80Tem, (ATTriBase[i_attr]->TWOATTRIBUTEHEAD.P_head._PN_AttrBody_Length)*2);*/

	/*Ret = ::WriteFile(hFile_recov, &h80data[9],(strlen(h80data)-9) , &nNumberOfBytesWritten, NULL);*/
	Ret = ::WriteFile(hFile_recov, dataBuff, dataLen - 8, &nNumberOfBytesWritten, NULL);
	if(!Ret)
	{	
		CFuncs::WriteLogInfo(SLT_ERROR, "WriteLitteFile:WriteFile失败,错误是%d", GetLastError());
		CloseHandle(hFile_recov);
		return false;
	}
	CloseHandle(hFile_recov);
	hFile_recov=NULL;
	return true;
}
bool CFileRecovery::FileRecovery(const char* fileName, const char* fileAddr, const char* filePath,const char* patition)
{
	DWORD dwError = NULL;
	char disk = patition[0];
	char szVol[7] = { '\\', '\\', '.', '\\', disk, ':', 0};
	 hDrive = CreateFile(
		szVol, 
		GENERIC_READ , 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);
	if (hDrive == INVALID_HANDLE_VALUE)
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("FileRecorvyFuction::CreateFile:获取磁盘句柄失败!"));
		return false;
	}
	bool bRet = false;
	DWORD BackBytesCount = 0;

	UCHAR *NTFSBuff = (UCHAR*)malloc(FILE_SECTOR_SIZE+SECTOR_SIZE);
	if (NULL == NTFSBuff)
	{
		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("WriteFileBuffer:malloc:分配NTFSBuff内存失败!,\
										   错误返回码: dwError = %d"), dwError);
		return false;
	}
	memset(NTFSBuff,0,FILE_SECTOR_SIZE);

	bRet=ReadSQData(hDrive, NTFSBuff, SECTOR_SIZE, 0, &BackBytesCount);		
	if(!bRet)
	{	
		CFuncs::WriteLogInfo(SLT_ERROR, "GetMFTAddr::ReadSQData:读取主机NTFS第一扇区失败!,获取MFT起始地址失败!");
		free(NTFSBuff);
		NTFSBuff=NULL;
		return false;	
	}
	LNTFS_TABLES first_ntfs_dbr = (LNTFS_TABLES)&NTFSBuff[0];
	UCHAR HostPatitionCuNum = first_ntfs_dbr->_Single_Cu_Num;

	free(NTFSBuff);
	NTFSBuff=NULL;

	DWORD NameSize = sizeof(wchar_t)*(strlen(fileName) + strlen(filePath) + 1);
	//wchar_t * NameBuf = new wchar_t[NameSize];
	char *NameBuf = (char*) malloc(NameSize);
	if (NULL == NameBuf)
	{
		dwError = GetLastError();
		CFuncs::WriteLogInfo(SLT_ERROR, _T("FileRecorvyFuction:malloc:分配NameBuf内存失败!,\
										   错误返回码: dwError = %d"), dwError);
		return false;
	}
	memset(NameBuf, 0, NameSize);

	//DWORD dBufSize = MultiByteToWideChar(CP_ACP, 0, filePath, strlen(filePath), NameBuf, NameSize);
	for(DWORD i = 0 ; i < strlen(filePath) ; i ++)
	{
		NameBuf[i] = filePath[i];
	}
	for(DWORD i = 0 ; i < strlen(fileName) ; i ++)
	{
		NameBuf[strlen(filePath) + i] = fileName[i];
		//RtlCopyMemory(&, &,);
	}
	//if (fileAddr[0] == 70 && fileAddr[1] == 105 && fileAddr[2] == 108 && fileAddr[3] == 101 && fileAddr[4] == 0x64
	//	&& fileAddr[5] == 0x64&& fileAddr[6] == 0x72 && fileAddr[7] == 0x3a)//FileAddr: 地址，大文件
	if(0 == strncmp(fileAddr, "FileAddr:", 8))
	{
		CFuncs::WriteLogInfo(SLT_INFORMATION, "FileRecorvyFuction:地址，此文件为大文件!");
		char *WriteFileBuffer = (char*)malloc(4096*1001);
		if (NULL == WriteFileBuffer)
		{
			dwError = GetLastError();
			CFuncs::WriteLogInfo(SLT_ERROR, _T("WriteFileBuffer:malloc:分配WriteFileBuffer内存失败!,\
											   错误返回码: dwError = %d"), dwError);
			free(NameBuf);
			NameBuf = NULL;
			return false;
		}
		memset(WriteFileBuffer, 0, 4096*1000);
		if(!WriteLargeFile(WriteFileBuffer, NameBuf, fileAddr, HostPatitionCuNum))
		{
			CFuncs::WriteLogInfo(SLT_ERROR, _T("FileRecorvyFuction::恢复大文件失败!"));
			free(WriteFileBuffer);
			WriteFileBuffer=NULL;
			free(NameBuf);
			NameBuf = NULL;
			return false;
		}else
		{
			CFuncs::WriteLogInfo(SLT_INFORMATION, "FileRecorvyFuction:恢复大文件成功!");		
		}
		free(WriteFileBuffer);
		WriteFileBuffer=NULL;
	} 
	else if (fileAddr[0] == 0x41 && fileAddr[1] == 0x64 && fileAddr[2] == 0x64 && fileAddr[3] == 0x72 && fileAddr[4] == 0x44 && fileAddr[5] == 0x61
		 && fileAddr[6] == 0x74 && fileAddr[7] == 0x61 && fileAddr[8] == 0x3a)//AddrData: 数据，小文件
	{
		CFuncs::WriteLogInfo(SLT_INFORMATION, "FileRecorvyFuction:数据，此文件为小文件!");
		if(!WriteLitteFile(NameBuf, fileAddr))
		{
			CFuncs::WriteLogInfo(SLT_ERROR, _T("FileRecorvyFuction::恢复小文件失败!"));
			 free(NameBuf);
			NameBuf = NULL;
			return false;
		}
		else
		{
			CFuncs::WriteLogInfo(SLT_INFORMATION, "FileRecorvyFuction:恢复小文件成功!");
		}
	}
	else
	{
		CFuncs::WriteLogInfo(SLT_ERROR, _T("FileRecorvyFuction::标识符错误或者地址与数据为0!"));
		free(NameBuf);
		NameBuf = NULL;
		return false;
	}

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	
	free(NameBuf);
	NameBuf = NULL;
	return true;
}
