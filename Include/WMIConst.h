#pragma once

#include <tchar.h>
#include <map>  
#include <vector>
#include <string>
#include "..\common\define.h"

#define WMI_NAMESPACE_CIMV2               ("root\\CIMV2")
#define WMI_SQL_DISKDRIVE                 ("select * from Win32_DiskDrive")
#define WMI_SQL_DISKPARTITION             ("select * from Win32_DiskPartition")
#define WMI_SQL_LOGICALDISK               ("select * from Win32_LogicalDisk")
#define WMI_SQL_LOGICALDISKTOPARTITION    ("select * from Win32_LogicalDiskToPartition")

#define WMI_SQL_BASEBOARD                 ("select * from Win32_BaseBoard")
#define WMI_SQL_SHARE                     ("select * from Win32_Share")
#define WMI_SQL_GROUP                     ("select * from Win32_Group")

#define WMI_NAMESPACE_CIMV2_SECURITY      ("root\\CIMV2\\Security\\MicrosoftVolumeEncryption")
#define WMI_SQL_ENCRYPTABLEVOLUME         ("select * from Win32_EncryptableVolume")

#define WMI_SQL_PHYSICALMEMORY			  ("select * from  Win32_PhysicalMemory")
#define WMI_SQL_NETWORKADAPTER			  ("select * from  Win32_NetworkAdapter")
#define WMI_SQL_GROUPUSER				  ("select * from  Win32_GroupUser")

#define WMI_NAMESPACE_WMI				  ("ROOT\\WMI")
#define	WMI_SQL_MSSTORAGEDRIVER_ATAPISMARTDATA	  ("select * from MSStorageDriver_ATAPISmartData")


#define LEAST_STRING_LENGTH     (8)
#define MIN_STRING_LENGTH       (64)
#define DEFAULT_STRING_LENGTH   (128)
#define MAX_STRING_LENGTH       (256)

#pragma pack(push, 1)

typedef struct _WmiDiskDriveTime_
{
	uint32 BootTimes;                          //重启次数
	uint32 UsingTime;						   //使用时间（小时）
}WmiDiskDriveTime;

//硬盘信息 DiskDrive
typedef struct _WmiDiskDrive_
{
	char   Caption[DEFAULT_STRING_LENGTH];     //型号
	uint32 Index;                              //索引号从0开始
	char   CheckPath[DEFAULT_STRING_LENGTH];   //新增加 CheckPath 匹配磁盘分区
	char   SerialNumber[MAX_STRING_LENGTH];    //序号
	uint32 Signature;                          //识别标识
	uint64 Size;                               //大小
	char   Status[MIN_STRING_LENGTH];          //状态
	char   SystemName[DEFAULT_STRING_LENGTH];  //操作系统名字
	char   Name[MIN_STRING_LENGTH];            //名称
	uint32 Partitions;                         //分区数量
	char   Manufacturer[MIN_STRING_LENGTH];    //厂商
	uint32 BootTimes;                          //重启次数
	uint32 UsingTime;						   //使用时间（小时）

	//以下为自定义字段,不属于该类型标准字段.
	uint32 IsSlave;                            //是否为从盘,0：主盘，1：从盘
	uint32 IsVirtual;                          //是否为虚拟磁盘,0：否，1：是
	uint32 UseStatus;                          //使用状态：0：在用，1：曾用
	char   Description[MIN_STRING_LENGTH];     //描述
}WmiDiskDrive;

//硬盘分区信息 DiskPartition
typedef struct _WmiDiskPartition_
{
	char   DeviceID[MIN_STRING_LENGTH];    //ID
	uint32 DiskIndex;                      //硬盘序号
	uint32 Index;                          //分区序号
	uint64 Size;                           //总大小
	char   SystemName[MIN_STRING_LENGTH];  //操作系统名字
	char   Type[MIN_STRING_LENGTH];        //类型

	//以下为自定义字段,不属于该类型标准字段.
	uint32 IsHide;                                     //是否为隐藏,0：正常，1隐藏

	uint32 IsVirtual;                                  //磁盘 是否为虚拟磁盘,0：否，1：是

	uint32 EncryptType;                                //加密 类型，详见类型：DiskEncryptType

	uint64 FreeSpace;                                  //卷标 剩余空间
	char   LogicalName[LEAST_STRING_LENGTH];           //卷标 盘符如C:
	char   VolumeSerialNumber[DEFAULT_STRING_LENGTH];  //卷标 序列号
	char   Description[MIN_STRING_LENGTH];             //描述
}WmiDiskPartition;

////逻辑磁盘信息 LogicalDisk
//typedef struct _WmiLogicalDisk_
//{
//	char   DeviceID[LEAST_STRING_LENGTH];              //名称 卷标名(盘符)
//	uint64 Size;                                       //大小
//	uint64 FreeSpace;                                  //剩余空间
//	char   SystemName[MIN_STRING_LENGTH];              //操作系统名字
//	uint32 DriveType;                                  //磁盘类型
//	uint32 MediaType;                                  //媒介类型
//	char   FileSystem[LEAST_STRING_LENGTH];            //文件系统
//	char   VolumeSerialNumber[DEFAULT_STRING_LENGTH];  //分区序列号
//}WmiLogicalDisk;

//逻辑磁盘信息 LogicalDisk
typedef struct _WmiLogicalDisk_
{
	char   DeviceID[LEAST_STRING_LENGTH];              //名称 卷标名(盘符)
	uint64 Size;                                       //大小
	uint64 FreeSpace;                                  //剩余空间
	char   SystemName[MIN_STRING_LENGTH];              //操作系统名字
	uint32 DriveType;                                  //磁盘类型
	uint32 MediaType;                                  //媒介类型
	char   FileSystem[LEAST_STRING_LENGTH];            //文件系统
	char   VolumeSerialNumber[DEFAULT_STRING_LENGTH];  //分区序列号

    uint32 IsHide;                                     //是否为隐藏,0：正常，1隐藏

	uint32 EncryptType;                                //加密 类型，详见类型：DiskEncryptType
}WmiLogicalDisk;

// LogicalDiskToPartition
typedef struct _WmiLogicalDiskToPartition_
{
	char Antecedent[MAX_STRING_LENGTH];     //磁盘分区索引信息
	char Dependent[MAX_STRING_LENGTH];      //逻辑磁盘分区
}WmiLogicalDiskToPartition;	

//分区加密 EncryptableVolume
typedef struct _WmiEncryptableVolum_
{
	char   DeviceID[DEFAULT_STRING_LENGTH];     //设备ID
	char   DriveLetter[LEAST_STRING_LENGTH];    //分区号如：C
	uint32 ProtectionStatus;                    //加密状态
}WmiEncryptableVolum;

//共享信息 
typedef struct _WmiShareInfo_
{
	char   Caption[MAX_PATH];      //标题
	char   Description[MAX_PATH];  //描述
	char   Name[MAX_PATH];         //名称
	char   Path[MAX_PATH];         //路径
	char   Status[MAX_PATH];       //状态
}WmiShareInfo;

//共享信息 
typedef struct _WmiGroupInfo_
{
	char   Caption[MAX_PATH];      //标题
	char   Description[MAX_PATH];  //描述
	char   Name[MAX_PATH];         //名称
	char   Status[MAX_PATH];       //状态
}WmiGroupInfo;

#pragma pack(pop)

typedef std::vector<WmiDiskDriveTime> WmiDiskDriveTimeVector;
typedef WmiDiskDriveTimeVector::iterator WmiDiskDriveTimeVectorIterator;
//相关结构定义
typedef std::vector<WmiDiskDrive> WmiDiskDriveVector;
typedef WmiDiskDriveVector::iterator WmiDiskDriveVectorIterator;

typedef std::vector< WmiDiskPartition> WmiDiskPartitionVector;
typedef WmiDiskPartitionVector::iterator WmiDiskPartitionVectorIterator;

typedef std::vector<WmiLogicalDisk> WmiLogicalDiskVector;
typedef WmiLogicalDiskVector::iterator WmiLogicalDiskVectorIterator;

typedef std::vector<WmiLogicalDiskToPartition> WmiLogicalDiskToPartitionVector;
typedef WmiLogicalDiskToPartitionVector::iterator WmiLogicalDiskToPartitionVectorIterator;

typedef std::vector<WmiEncryptableVolum> WmiEncryptableVolumVector;
typedef WmiEncryptableVolumVector::iterator WmiEncryptableVolumVectorIterator;

typedef std::vector<WmiGroupInfo> WmiGroupInfoVector;
typedef WmiGroupInfoVector::iterator WmiGroupInfoVectorIterator;

typedef std::vector<std::string> WmiVector;
typedef WmiVector::iterator WmiVectorIterator;

typedef std::vector<std::string> WmiPropertyVector;
typedef WmiPropertyVector::iterator WmiPropertyVectorIterator;

typedef std::vector<WmiShareInfo> WmiShareInfoVector;
typedef WmiShareInfoVector::iterator WmiShareInfoVectorIterator;