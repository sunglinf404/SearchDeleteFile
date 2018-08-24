#pragma once

#include <windows.h>

#define DEFAULT_DATA_HEADER_VERSION      (1) 
#define DEFAULT_DATA_HEADER_MAGICNUMBER	 (0xCD91E1F0)

#define MAX_LOG_LEN               (1024 * 3)

#define MAX_IP_LEN                (32)
#define MAX_MAC_LEN               (6)

#define MAX_PROTO_DEVICE_LEN            (20 + 1)
#define MAX_PROTO_GUID_LEN              (64 + 1)
#define MAX_PROTO_PATH_LEN              (512 + 1)
#define MAX_PROTO_USER_LEN              (32 + 1)
#define MAX_PROTO_NAME_LEN              (64 + 1)
#define MAX_PROTO_DESCRIPTION_LEN       (256 + 1)
#define MAX_PROTO_MACHINE_CODE_LEN      (64)
#define MAX_PROTO_DATETIME_LEN          (20 + 1)
#define MAX_PROTO_HOSTNAME_LEN          (64 + 1)
#define MAX_PROTO_VERSION_LEN           (128 + 1)
#define MAX_PROTO_TEXT_LEN              (64 + 1)
#define MAX_PROTO_MEMO_LEN              (128 + 1)
#define MAX_PROTO_CONTENT_LEN           (1024 + 1)
#define MAX_PROTO_TYPE_LEN              (16 + 1)
#define MAX_PROTO_STATUS_LEN            (12 + 1)
#define MAX_PROTO_URL_LEN               (1024 + 1)
#define MAX_PROTO_SUMMARY_LEN           (2048 + 1)
#define MAX_PROTO_TITLE_LEN             (128 + 1)

//数据来源
typedef enum _CommonDataSource_
{
	CDS_MIN = 0,

	CDS_UNKOWN = CDS_MIN,    //未知
	CDS_PC,                  //电脑
	CDS_MOBILE,              //移动设备
	CDS_DISK,                //磁盘/硬盘

	CDS_MAX
}CommonDataSource;

//检查来源
typedef enum _CommonCheckSource_
{
	CCS_MIN = 0,

	CCS_UNKOWN = CCS_MIN,    //未知
	CCS_GENERAL,             //常规
	CCS_DEEP,                //深度

	CCS_MAX
}CommonCheckSource;

//文件来源
typedef enum _CommonFileSource_
{
	CFS_MIN = 0,

	CFS_UNKOWN = CFS_MIN,    //未知
	CFS_GENERAL,             //常规
	CFS_RECOVERY,            //恢复

	CFS_MAX
}CommonFileSource;

//检查类型
typedef enum _CommonCheckType_
{
	CCT_MIN = 0,

	CCT_UNKOWN = CCT_MIN,    //未知
	CCT_PC,                  //常规
	CCT_DISK,                //磁盘
	CCT_MOBILE,              //手机
	CCT_VM,                  //虚拟机
	CCT_CLONE,               //克隆文件

	CCT_MAX
}CommonCheckType;

//上网类型(1:浏览器 2：下载软件 3：云盘 4：邮件  5:即时通讯)
typedef enum _CommonInternetType_
{
	CIT_MIN = 0,

	CIT_UNKOWN = CIT_MIN,    //未知
	CIT_BROWSER,             //浏览器
	CIT_DOWNLOADER,          //下载软件
	CIT_CLOUD,               //云盘
	CIT_EMAIL,               //邮件
	CIT_IM,                  //即时通讯

	CIT_MAX
}CommonInternetType;

//磁盘加密类型
typedef enum _CommonEncryptType_
{
	CET_MIN = 0,

	CET_NORMAL = CET_MIN,  //未加密
	CET_BITLOCKER,         //bitlocker
	CET_TURECRYPT,         //TrueCrypt
	CET_OTHER,             //其他

	DET_MAX
}CommonEncryptType;

//操作系统类型
typedef enum _CommonOSType_
{
	COS_MIN = 0,

	COS_UNKNOW = COS_MIN,  //未知
	COS_ENTITY,            //实体机
	COS_VIRTUAL,           //虚拟机
	COS_CLONE,             //克隆机

	COS_MAX
}CommonOSType;

//根据硬盘判断操作系统版本
typedef enum _DiskOSVersion_
{
	DOV_MIN = 0,

	DOV_UNKNOW = DOV_MIN,
	DOV_WIN_XP,
	DOV_WIN_SERVER_2003,
	DOV_WIN_7,
	DOV_WIN_8,
	DOV_WIN_10,
	DOV_WIN_SERVER_2008,
	DOV_WIN_SERVER_2012,
	DOV_MAX
}DiskOSVersion;

//数据类型
typedef enum _CommonDataType_
{
	CDT_MIN = 0,
	
	CDT_HOST_INFO,                          //主机信息
	CDT_ACCOUNT_INFO,                       //账号信息
	CDT_NETWORK_INFO,                       //网络信息
	CDT_DISK_INFO,                          //磁盘信息
	CDT_DISKPARTION_INFO,                   //磁盘分区信息
	CDT_SHARE_INFO,                         //文件共享信息
	CDT_SYSTEM_PATCH,                       //系统补丁
	CDT_OS_INSTALLATION,                    //操作系统安装信息
	CDT_ANTIVIRUS_INFO,                     //病毒防护软件信息
	CDT_BM_SYSTEM,                          //保密业务系统
	CDT_WIREKESS_DEVICE,                    //无线设备检查
	CDT_VIRTUAL_MACHINE,                    //虚拟机检查
	CDT_ACCOUNT_STRATEGY,                   //账户安全策略
	CDT_ACCOUNT_SETTINGS,                   //账户安全设置
	CDT_AUDIT_STRATEGY,                     //安全审计策略
	CDT_SERVICES_INFO,                      //服务信息
	CDT_PORTS_INFO,                         //端口信息
	CDT_ACCOUNT_PERMISSION,                 //账户权限配置
	CDT_USB_RECORD,                         //USB记录检查
	CDT_INTERNET_CHECK,                     //上网记录检查
	CDT_EMAIL_RECORD,						//邮件记录检查
	CDT_FILE_CHECK,                         //文件检查
	CDT_EMAIL_CHECK,                        //邮件检查
	
	CDT_MAX
}CommonDataType;

#pragma pack(push, 1)

//数据公共头信息
typedef struct _CommonHeader_
{
	unsigned int         HeaderSize;                  //数据头(CommonHeader)长度
	unsigned int         Version;                     //版本
	unsigned char        TaskID[MAX_PROTO_GUID_LEN];  //任务ID
	unsigned int         MagicNumber;                 //MAGICNUMBER
	unsigned int         DataSource;                  //数据来源  CommonDataSource
	unsigned int         DataType;	                  //数据类型  CommonDataType
	unsigned int         DataLen;                     //数据区总长度,除头信息
}CommonHeader;


//主机信息
typedef struct _DHHostInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型(1：PC 2：硬盘  3：手机 4：虚拟机  5：克隆文件)
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径（对应检查类型匹配的路径，用于区分数据）
	unsigned char OSVersion[MAX_PROTO_VERSION_LEN];          //操作系统版本
	unsigned char InstallTime[MAX_PROTO_DATETIME_LEN];       //安装时间
	unsigned char HostName[MAX_PROTO_HOSTNAME_LEN];          //主机名
	unsigned int  OSType;                                    //类型   CommonOSType
	unsigned char GroupName[MAX_PROTO_HOSTNAME_LEN];         //工作组
	unsigned char Description[MAX_PROTO_DESCRIPTION_LEN];	 //描述
} DHHostInfo;

//账户信息
typedef struct _DHAccountInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char CurrentUser[MAX_PROTO_USER_LEN];           //当前账户
	unsigned char UserDescription[MAX_PROTO_DESCRIPTION_LEN];//账户描述信息
	unsigned char UserPermissions[MAX_PROTO_TYPE_LEN];		 //用户权限 禁用or启用
	unsigned char UserType[MAX_PROTO_NAME_LEN];				 //用户类型
} DHAccountInfo;

//网络信息
typedef struct _DHNetworkInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char AdapterName[MAX_PROTO_NAME_LEN];           //适配器名称
	unsigned char IP[MAX_IP_LEN];                            //IP地址
	unsigned char MAC[MAX_MAC_LEN];                          //MAC地址
} DHNetworkInfo;

//硬盘信息
typedef struct _DHDiskInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char GUID[MAX_PROTO_GUID_LEN];                  //GUID 用于与分区信息关联
	unsigned char Company[MAX_PROTO_MEMO_LEN];               //厂商
	unsigned char Product[MAX_PROTO_MEMO_LEN];               //型号
	unsigned char SN[MAX_PROTO_MEMO_LEN];                    //序列号
	unsigned __int64  TotalSpace;                            //总容量
	unsigned short Partition;                                //分区数量
	unsigned int  BootTimes;                                 //启动次数
	unsigned char Using[MAX_PROTO_DATETIME_LEN];             //启动时间
	unsigned char Status[MAX_PROTO_STATUS_LEN];              //状态
	unsigned short isSlave;                                   //主从盘 0: 主盘 1：从盘
	unsigned short isVirtual;                                 //虚拟磁盘 0：否 1：是
} DHDiskInfo;

//硬盘分区信息
typedef struct _DHDiskPartitionInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char GUID[MAX_PROTO_GUID_LEN];                  //GUID 用于与磁盘关联
	unsigned int  IsHide;                                    //是否为隐藏,0：正常，1隐藏
	unsigned int  EncryptType;                               //加密 类型，详见类型：CommonEncryptType
	unsigned __int64  TotalSpace;                            //总容量
	unsigned __int64  FreeSpace;                             //卷标 剩余空间
	unsigned char   LogicalName[MAX_PROTO_DEVICE_LEN];       //卷标 盘符如C:
	unsigned char   VolumeSerialNumber[MAX_PROTO_NAME_LEN];  //卷标 序列号
} DHDiskPartitionInfo;

//共享信息
typedef struct _DHShareInfo
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //共享名称
	unsigned char Path[MAX_PROTO_PATH_LEN];                  //文件路径
	unsigned char Description[MAX_PROTO_DESCRIPTION_LEN];    //描述
	unsigned char Authority[MAX_PROTO_CONTENT_LEN];          //权限
	unsigned char Deduction[MAX_PROTO_CONTENT_LEN];          //违规提示信息
} DHShareInfo;

//系统补丁
typedef struct _DHSystemPatch
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char ID[MAX_PROTO_USER_LEN];                    //补丁ID
	unsigned char Description[MAX_PROTO_DESCRIPTION_LEN];    //补丁描述
	unsigned char VulDescription[MAX_PROTO_DESCRIPTION_LEN]; //漏洞描述
	unsigned char Date[MAX_PROTO_DATETIME_LEN];              //补丁时间
	unsigned char Deduction[MAX_PROTO_CONTENT_LEN];          //违规提示信息
} DHSystemPatch;

//操作系统
typedef struct _DHOSInstallation
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //操作系统名称
	unsigned char Version[MAX_PROTO_VERSION_LEN];            //操作系统版本
	unsigned char InstallPath[MAX_PROTO_PATH_LEN];           //安装路径
	unsigned char InstallDate[MAX_PROTO_DATETIME_LEN];       //安装时间
	unsigned char Deduction[MAX_PROTO_CONTENT_LEN];          //违规提示信息
} DHOSInstallation;

//安全防护软件
typedef struct _DHAntivirus
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //名称
	unsigned char Version[MAX_PROTO_VERSION_LEN];            //版本
	unsigned char UpdateDate[MAX_PROTO_DATETIME_LEN];        //更新时间
	unsigned char Deduction[MAX_PROTO_CONTENT_LEN];          //违规提示信息
} DHAntivirus;

//保密业务系统
typedef struct _DHBMSystem
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //名称
}DHBMSysteme;

//无线通信设备
typedef struct _DHWirelessDevice
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //名称
	unsigned char Company[MAX_PROTO_MEMO_LEN];               //厂商
	unsigned char Description[MAX_PROTO_DESCRIPTION_LEN];    //描述
	unsigned char Type;                                      //类型
	unsigned char Deduction[MAX_PROTO_NAME_LEN];             //违规提示信息
} DHWirelessDevice;

//虚拟机安装
typedef struct _DHVirtualMachine
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Name[MAX_PROTO_NAME_LEN];                  //名称
	unsigned char Version[MAX_PROTO_VERSION_LEN];            //软件版本
	unsigned char Company[MAX_PROTO_MEMO_LEN];               //软件公司
	unsigned char InstallPath[MAX_PROTO_PATH_LEN];           //安装路径
	unsigned char InstallDate[MAX_PROTO_DATETIME_LEN];       //安装时间
	unsigned char Deduction[MAX_PROTO_NAME_LEN];             //违规提示信息
} DHVirtualMachine;

//账户安全策略
typedef struct _DHAccountStrategy
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char PasswordComplex[MAX_PROTO_DATETIME_LEN];   //密码必须符合复杂性要求
	unsigned char MinPasswordLength[MAX_PROTO_USER_LEN];     //密码长度最小值
	unsigned short MaxValidityPeriod;                        //密码最长使用期限
	unsigned short MinValidityPeriod;                        //密码最短使用期限
	unsigned char PasswordHistory[256];                      //强制密码历史
	unsigned char LockTime[MAX_PROTO_DATETIME_LEN];          //账户锁定时间
	unsigned int  RestCounter;                               //复位账户锁定计数器
	unsigned int  LockThreshold;                             //账户锁定阈值
	unsigned char GuestStatus[MAX_PROTO_NAME_LEN];           //Guest账户状态
	unsigned char AutoLogon[MAX_PROTO_NAME_LEN];             //自动登陆设置
	unsigned char Deduction[MAX_PROTO_NAME_LEN];             //违规提示信息
} DHAccountStrategy;

//账户安全设置
typedef struct _DHAccountSetting
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char UserName[MAX_PROTO_USER_LEN];              //账户名
	unsigned char Password[MAX_PROTO_USER_LEN];              //密码
	unsigned char PasswordPrompt[MAX_PROTO_PATH_LEN];        //密码设置情况
	unsigned short Length;                                   //密码长度
	unsigned char Complex[MAX_PROTO_TEXT_LEN];               //密码复杂度
	unsigned char Deduction[MAX_PROTO_NAME_LEN];             //违规提示信息
} DHAccountSetting;

//安全审计策略
typedef struct _DHAuditStrategy
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Modify[MAX_PROTO_TEXT_LEN];                //审查策略更改
	unsigned char Login[MAX_PROTO_TEXT_LEN];                 //审核登录事件
	unsigned char Object[MAX_PROTO_TEXT_LEN];                //审核对象访问
	unsigned char Process[MAX_PROTO_TEXT_LEN];               //审核过程跟踪
	unsigned char AD[MAX_PROTO_TEXT_LEN];                    //审核目录服务访问
	unsigned char SpecialUse[MAX_PROTO_TEXT_LEN];            //审核权限使用
	unsigned char Event[MAX_PROTO_TEXT_LEN];                 //审核系统事件
	unsigned char LoginOther[MAX_PROTO_TEXT_LEN];            //审核账户登录事件
	unsigned char Account[MAX_PROTO_USER_LEN];               //审核账户管理
	unsigned char Deduction[MAX_PROTO_TEXT_LEN];             //违规提示信息
} DHAuditStrategy;

//服务情况
typedef struct _DHServices
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char ServiceShowName[MAX_PROTO_NAME_LEN];       //服务显示名称
	unsigned char ServiceName[MAX_PROTO_NAME_LEN];           //服务名称
	unsigned int  PID;                                       //进程ID
	unsigned char PName[MAX_PROTO_NAME_LEN];                 //进程名称
	unsigned char Description[256];                          //服务描述
	unsigned char Path[MAX_PROTO_PATH_LEN];                  //程序路径
	unsigned char Status[MAX_PROTO_STATUS_LEN];              //状态
	unsigned int  Signature;                                 //签名情况 0 表示没有签名  1 表示签名了
	unsigned char AutoRun[MAX_PROTO_STATUS_LEN];             //启动设置
	unsigned char Deduction[MAX_PROTO_TEXT_LEN];             //违规提示信息
} DHServices;

//端口情况
typedef struct _DHPorts
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned short Port;                                     //端口号
	unsigned char Protocol[MAX_PROTO_STATUS_LEN];            //协议
	unsigned char LocalIP[MAX_IP_LEN];                       //本地IP地址
	unsigned char RemoteIP[MAX_IP_LEN];                      //对端IP地址
	unsigned char Status[MAX_PROTO_STATUS_LEN];              //状态
	unsigned int  PID;                                       //进程ID 
	unsigned char PName[MAX_PROTO_NAME_LEN];                 //进程名称
	unsigned char Deduction[MAX_PROTO_TEXT_LEN];             //违规提示信息
} DHPorts;

//账户权限配置
typedef struct _DHAccountPermission
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char UserName[MAX_PROTO_NAME_LEN];              //账户名
	unsigned char PermissionPrompt[MAX_PROTO_TEXT_LEN];      //账户权限设置情况
	unsigned char Deduction[MAX_PROTO_TEXT_LEN];             //违规提示信息
	unsigned char Description[MAX_PROTO_DESCRIPTION_LEN];    //描述
} DHAccountPermission;

//USB记录
typedef struct _DHUSBRecord
{
	CommonHeader   Header;
    
	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型 CommonCheckType
	unsigned int  CheckSource;                               //检查来源 CommonCheckSource
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char Type[MAX_PROTO_TYPE_LEN];                  //设备类型
	unsigned char DeviceName[MAX_PROTO_NAME_LEN];            //设备名称
	unsigned char SN[MAX_PROTO_MEMO_LEN];                    //序列号
	unsigned char VID[MAX_PROTO_MEMO_LEN];
	unsigned char PID[MAX_PROTO_MEMO_LEN];
	unsigned char FirstDate[MAX_PROTO_DATETIME_LEN];      	 //首次使用时间	
	unsigned char LastDate[MAX_PROTO_DATETIME_LEN];      	 //末次使用时间
	unsigned char Deduction[MAX_PROTO_TEXT_LEN];             //违规提示信息
}DHUSBRecord;


//上网记录常规
typedef struct _DHInternetRecord
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型 CommonCheckType
	unsigned int  CheckSource;                               //检查来源 CommonCheckSource
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned int  InternetType;                              //上网类型 CommonInternetType
	unsigned char Date[MAX_PROTO_DATETIME_LEN];       	     //上网时间
	unsigned char AppName[MAX_PROTO_NAME_LEN];		         //软件名称
	unsigned char AppProcess[MAX_PROTO_NAME_LEN];            //程序名
	unsigned char Domain[MAX_PROTO_PATH_LEN];      	         //域名	
	unsigned char Action[MAX_PROTO_STATUS_LEN];      	     //动作	
	unsigned char Address[MAX_PROTO_URL_LEN];      	         //网址
	unsigned char User[MAX_PROTO_USER_LEN];      	         //操作用户
	unsigned char Deduction[MAX_PROTO_MEMO_LEN];             //违规提示信息
	unsigned char Title[MAX_PROTO_TITLE_LEN];                //网页名称
} DHInternetRecord;

//邮件记录
typedef struct _DHEMailRecord
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型 CommonCheckType
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char AppName[MAX_PROTO_NAME_LEN];               //所属软件 Foxmail,Outlook等
	unsigned char ProcName[MAX_PROTO_NAME_LEN];              //软件进程名
	unsigned char BoxType[MAX_PROTO_NAME_LEN];               //所属邮箱,如：收件箱，发件箱等
	unsigned char FileName[MAX_PROTO_NAME_LEN];	             //文件名
	unsigned char Sender[MAX_PROTO_DATETIME_LEN];            //发件人
	unsigned char Receiver[MAX_PROTO_HOSTNAME_LEN];          //收件人
	unsigned char Subject[MAX_PROTO_MEMO_LEN];           //主题	
	unsigned char ActionTime[MAX_PROTO_DATETIME_LEN];        //发生时间	
	unsigned char Deduction[MAX_PROTO_MEMO_LEN];             //违规提示信息
} DHEMailRecord;


//文件检查常规
typedef struct _DHFileCheck
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型 CommonCheckType
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char FileName[MAX_PROTO_PATH_LEN];              //文件名
	unsigned int  FileSource;                                //文件来源 CommonFileSource
	unsigned char FilePath[MAX_PROTO_PATH_LEN];	             //文件路径
	unsigned long long  FileSize;                            //文件大小
	unsigned char FileMD5[MAX_PROTO_NAME_LEN];               //文件MD5
	unsigned char FileModifyTime[MAX_PROTO_DATETIME_LEN];    //文件修改时间	
	unsigned char FileCreateTime[MAX_PROTO_DATETIME_LEN];    //文件创建时间	
	unsigned char OwnerApp[MAX_PROTO_NAME_LEN];      	     //所属应用
	unsigned char Deduction[MAX_PROTO_MEMO_LEN];             //违规提示信息
	unsigned char Keywords[MAX_PROTO_TEXT_LEN];      	     //命中关键字 关键字间分号分隔
	unsigned char PageCounts[MAX_PROTO_TEXT_LEN];            //页码 多个页码分号分隔
	unsigned char Summary[MAX_PROTO_SUMMARY_LEN];            //文件摘要多个关键字摘要 	
	unsigned char SecretLevel[MAX_PROTO_DEVICE_LEN];      	 //文件密级 秘密，机密，绝密
	unsigned long HitsFileSize;								 //命中关键字或者密级文件的大小
} DHFileCheck;

//邮件检查
typedef struct _DHEMailCheck
{
	CommonHeader   Header;

	unsigned char Timestamp[MAX_PROTO_DATETIME_LEN];         //检查时间
	unsigned int  CheckType;                                 //检查类型 CommonCheckType
	unsigned char CheckPath[MAX_PROTO_PATH_LEN];             //检查路径
	unsigned char AppName[MAX_PROTO_NAME_LEN];               //所属软件 Foxmail,Outlook等
	unsigned char BoxType[MAX_PROTO_NAME_LEN];               //所属邮箱,如：收件箱，发件箱等
	unsigned char FileName[MAX_PROTO_NAME_LEN];	             //文件名
	unsigned char Sender[MAX_PROTO_DATETIME_LEN];            //发件人
	unsigned char Receiver[MAX_PROTO_HOSTNAME_LEN];          //收件人
	unsigned char Subject[MAX_PROTO_MEMO_LEN];               //主题	
	unsigned char ActionTime[MAX_PROTO_DATETIME_LEN];        //发生时间	
	unsigned char Deduction[MAX_PROTO_MEMO_LEN];             //违规提示信息
	unsigned char Keywords[MAX_PROTO_TEXT_LEN];      	     //命中关键字 关键字间分号分隔
	unsigned char PageCounts[MAX_PROTO_TEXT_LEN];            //页码 多个页码分号分隔
	unsigned char Summary[MAX_PROTO_SUMMARY_LEN];            //文件摘要多个关键字摘要
	unsigned char SecretLevel[MAX_PROTO_DEVICE_LEN];      	 //文件密级 秘密，机密，绝密
	unsigned long HitsFileSize;								 //命中关键字或者密级文件的大小
} DHEMailCheck;

#pragma pack(pop)