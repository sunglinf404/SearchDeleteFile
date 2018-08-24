#pragma once

#include "../Common/Funcs.h"
#include <iostream>
#include <comutil.h>
#include "../Common/Exception.h"

#pragma comment(lib, "comsuppw.lib")

#define UC_FLAG ("\\u")

class CUrlConver
{
public:
	CUrlConver(void);
	~CUrlConver(void);
public:
	static void      GB2312ToUnicode(wchar_t* pOut,char* gbBuffer);
	static void      UTF8ToUnicode(wchar_t* pOut,char *pText);
	static void      UnicodeToUTF8(char* pOut,wchar_t* pText);
	static void      UnicodeToGB2312(char* pOut,wchar_t uData);

	static void      GB2312ToUTF8(string& pOut, char* pText, uint32 pLen); //gb2312 转 utf8
	static void      UTF8ToGB2312(string& pOut, char* pText, uint32 pLen, char* pBuffer, const uint32 bufLen); //utf8 转 gb2312

	static char      CharToInt(char ch);
	static char      StrToBin(char* str);
	static short     HexCharToShort(char c);
	static string    DecodeUnicodeNumber(const string& strNumber);
public:
	//十六进制字符转换为int
	static uint32 HexToInt(const uint8* data ,uint32 len);

	static string UrlGB2312(const char* str);                       //urlgb2312编码
	static string UrlGB2312Decoder(const string& str);              //urlgb2312解码

	static string UrlUTF8(const char* str);                         //urlutf8 编码
	static string UrlUTF8Decoder(const string& str, char* buf, const uint32 len); //urlutf8解码

	static string UrlDecoder(string &str);                          //url解码
	static bool   UrlDecoder(const char* src, const uint32 srclen, char* dst, const uint32 dstlen);
	
	static wstring StringToWstring(const string& str);              //转换为宽字符
	static string  WstringToString(const wstring &wstr);            //转换为窄字符

	static void  DecodeHtmlUnicode( string& content);               //html_unicode,\uxxxx格式

	static bool IsUrlCode(const string& str);
	static bool IsTextUTF8(char* str,size_t length);                
	static bool IsHtmlUnicode(string &strValue);

	//////////////////////////////////////////////////////////////////////////
	//全部转换小写
	static string  UpperToLower(const string& src);
	static void    UpperToLower(char* src ,const unsigned int srcLen);

	//全部转换大写
	static string  LowerToUpper(const string& src);
	static void    LowerToUpper(char* src ,const unsigned int srcLen);

	//字符转换整数
	static short HexCharToInteger(const char src);
	//URL解码
	static bool  URLDecorder(const char* srcData, const unsigned int srcLen,char* desData,const unsigned int desLen);
	//utf-8转换unicode
	static bool  UTF8ToUnicode(const char* srcData, const unsigned int srcLen,wchar_t* desData,const unsigned int desLen);
	//GB2312解码
	static bool  GB2312Decorder(const char* srcData, const unsigned int srcLen,char* desData, unsigned int &desLen);
	//HtmlUnicode解码(\uxxxx格式)
	static bool  HtmlUnicodeDecorder(const char* srcData, const unsigned int srcLen, char* desData,const unsigned int desLen);


	//UTF-8解码(不用,大部分Unicode,不能转换为ansi)
	static bool  UTF8Decorder(const char* srcData, const unsigned int srcLen,char* desData, unsigned int &desLen);
	//unicode解码
	static bool  UnicodeDecorder(const char* srcData, const unsigned int srcLen,char* desData, unsigned int &desLen,const bool bNetFlag);

	//%uxxxx转换为\uxxxx
	static bool ReplaseCode(char* srcData, const unsigned int srcLen);
	//搜索+换空格
	static bool ReplaseCode2(char* srcData, const unsigned int srcLen);

	//附件名特殊字符处理,\ / : * ? " < > | 
	static bool ClearSpecialChar( char* srcData, const unsigned int srcLen );

	//处理网页转移字符,\/,\"等,除了\r,\n,\t其他全部删除\符号 
	static bool ClearTransferredChar(char *srcData, const unsigned int srcLen);
};

