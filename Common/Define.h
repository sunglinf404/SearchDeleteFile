#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <string>

using namespace std;

typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef __int64             int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64    uint64;

//内容挖掘文件最大长度
#define MAX_MINING_FILE_CONTENT_LEN   (1024 * 1024 * 50)

#ifdef UNICODE
typedef std::wstring  vstring;
#else
typedef std::string   vstring;
#endif

//#ifndef TIXML_USE_STL
//    #define TIXML_USE_STL
//#endif

//日志类型
typedef enum _SystemLogType_
{
	SLT_MIN = 0,

	SLT_UNKOWN = SLT_MIN,   //未知
	SLT_INFORMATION,        //信息
	SLT_WARNING,            //警告
    SLT_ERROR,              //错误

	SLT_MAX
}SystemLogType;

//返回值类型
typedef enum _ReturnValueType_
{
	RVT_MIN = 0,

	RVT_TRUE = RVT_MIN,   //成功
	RVT_MAGIC,            //magic错误
	RVT_FALSE,            //失败
    RVT_BUFFER,           //json分配的Buffer太小

	RVT_MAX
}ReturnValueType;
//关键字
#define DEFAULT_RULE_KEYWORD_LIST         ("keyword.rule")
