#include "StdAfx.h"
#include "UrlConver.h"


CUrlConver::CUrlConver(void)
{
}


CUrlConver::~CUrlConver(void)
{
}


char CUrlConver::CharToInt(char ch)
{
	try
	{
		if(ch >= '0' && ch <= '9')
		{
			return (char)(ch - '0');
		}
		if(ch >= 'a' && ch <= 'f')
		{
			return (char)(ch - 'a' + 10);
		}
		if(ch >= 'A' && ch <= 'F')
		{
			return (char)(ch - 'A' + 10);
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"CharToInt异常!");
	}
	return -1;
}

short CUrlConver::HexCharToShort(char c) 
{
	try
	{
		if ( '0' <= c && c <= '9' ) 
		{
			return short(c - '0');
		} 
		else if ( 'a' <= c && c <= 'f' ) 
		{
			return ( short(c - 'a') + 10 );
		} 
		else if ( 'A' <= c && c <= 'F' ) 
		{
			return ( short(c - 'A') + 10 );
		} 
		else 
		{
			return -1;
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"HexCharToShort异常!");
	}
	
}

//字符转换整数
short CUrlConver::HexCharToInteger(const char src)
{
	if(src >= '0' && src <= '9')
	{
		return (short)(src - '0');
	}
	if(src >= 'a' && src <= 'f')
	{
		return (short)(src - 'a' + 10);
	}
	if(src >= 'A' && src <= 'F')
	{
		return (short)(src - 'A' + 10);
	}
	return -1;
}

char CUrlConver::StrToBin(char* str)
{
	try
	{
		char tempWord[2];
		char chn;

		tempWord[0] = CharToInt(str[0]);                         
		tempWord[1] = CharToInt(str[1]);                         

		chn = (tempWord[0] << 4) | tempWord[1];                  

		return chn;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"StrToBin异常!");
	}
	return 0;
}

uint32 CUrlConver::HexToInt(const uint8* data ,uint32 len)
{
	uint32 k = 0;
	uint32 num = 0;
	for (uint32 i = 0; i < len;i++)
	{
		if ( '0' <= data[i] && data[i] <= '9' ) 
		{
			k = data[i] - '0';
		} 
		else if ( 'a' <= data[i] && data[i] <= 'f' ) 
		{
			k = data[i] - 'a' + 10 ;
		} 
		else if ( 'A' <= data[i] && data[i] <= 'F' ) 
		{
			k = data[i] - 'A' + 10 ;
		} 
		else 
		{
			break;
		}
		num = num * 16 + k ;	
	}
	return num;
}


void CUrlConver::GB2312ToUnicode(WCHAR* pOut,char* gbBuffer)
{
	try
	{
		::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,gbBuffer,2,pOut,1);
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"GB2312ToUnicode异常!");
	}
}



void CUrlConver::UTF8ToUnicode(WCHAR* pOut, char* pText)
{
	try
	{
		char* uchar = (char *)pOut;

		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UTF8ToUnicode异常!");
	}
}

void CUrlConver::UnicodeToUTF8(char* pOut, WCHAR* pText)
{
	try
	{
		// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
		char* pchar = (char *)pText;

		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
		pOut[2] = (0x80 | (pchar[0] & 0x3F));
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UnicodeToUTF8异常!");
	}
}
void CUrlConver::UnicodeToGB2312(char* pOut, WCHAR uData)
{
	try
	{
		WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(WCHAR),NULL,NULL);
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UnicodeToGB2312异常!");
	}
}

//UTF8 转 GB2312
void CUrlConver::UTF8ToGB2312(string& pOut, char* pText, uint32 pLen, char* pBuffer, const uint32 bufLen)
{
	pOut.clear();
	try
	{
		char buf[4];
		//char* rst = new char[pLen + (pLen >> 2) + 2];
		
		if ((pLen + (pLen >> 2) + 2) > bufLen)
		{
			CFuncs::WriteLogInfo(SLT_ERROR,"UTF8ToGB2312, (pLen + (pLen >> 2) + 2) > %u",bufLen);
			return;
		}
		memset(buf,0,4);
		memset(pBuffer,0,pLen + (pLen >> 2) + 2);

		int i =0;
		int j = 0;

		while((uint32)i < pLen)
		{
			if(*(pText + i) >= 0)
			{

				pBuffer[j++] = pText[i++];
			}
			else                 
			{
				WCHAR Wtemp;
				UTF8ToUnicode(&Wtemp,pText + i);
				UnicodeToGB2312(buf,Wtemp);

				unsigned short int tmp = 0;
				tmp = pBuffer[j] = buf[0];
				tmp = pBuffer[j + 1] = buf[1];
				tmp = pBuffer[j + 2] = buf[2];

				i += 3;    
				j += 2;   
			}
		}
		pBuffer[j] = '\0';
		pOut = pBuffer; 

		//delete []rst;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UTF8ToGB2312异常!");
	}
}

//GB2312 转为 UTF8
void CUrlConver::GB2312ToUTF8(string& pOut,char* pText, uint32 pLen)
{
	pOut.clear();
	try
	{
		char buf[4];
		memset(buf,0,4);

		int i = 0;
		while((uint32)i < pLen)
		{
			//如果是英文直接复制就可以
			if( pText[i] >= 0)
			{
				char asciistr[2] = {0};
				asciistr[0] = (pText[i++]);
				pOut.append(asciistr);
			}
			else
			{
				WCHAR pbuffer;
				GB2312ToUnicode(&pbuffer,pText + i);

				UnicodeToUTF8(buf,&pbuffer);

				pOut.append(buf);

				i += 2;
			}
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"GB2312ToUTF8异常!");
	}
	
}

//把str编码为网页中的 GB2312 url encode ,英文不变，汉字双字节  如%3D%AE%88
string CUrlConver::UrlGB2312(const char* str)
{
	try
	{
		string dd;
		size_t len = strlen(str);
		for (size_t i = 0; i < len;i++)
		{
			if(isalnum((BYTE)str[i]))
			{
				char tempbuff[2];
				sprintf_s(tempbuff,"%c",str[i]);
				dd.append(tempbuff);
			}
			else if (isspace((BYTE)str[i]))
			{
				dd.append("+");
			}
			else
			{
				char tempbuff[4];
				sprintf_s(tempbuff,"%%%X%X",((BYTE *)str)[i] >> 4,((BYTE *)str)[i] % 16);
				dd.append(tempbuff);
			}
		}
		return dd;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UrlGB2312异常!");
	}
	return string("");
}

//把url GB2312解码
string CUrlConver::UrlGB2312Decoder(const string& str)
{
	try
	{
		char tmp[2];
		string output="";
		int i = 0;
		int idx = 0;
		int len = (int)str.length();

		while(i < len)
		{
			if(str[i] == '%' && ((i + 3) < len))
			{
				tmp[0] = str[i+1];
				tmp[1] = str[i+2];
				output += StrToBin(tmp);
				i = i + 3;
			}
			else if(str[i] == '+')
			{
				output += ' ';
				i++;
			}
			else
			{
				output += str[i];
				i++;
			}
		}
		return output;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UrlGB2312Decoder异常!");
	}
	return string("");
}

//把str编码为网页中的 UTF-8 url encode ,英文不变，汉字三字节  如%3D%AE%88
string CUrlConver::UrlUTF8(const char* str)
{
	try
	{
		string tt;
		string dd;
		
		//这句需要修改，暂时不用
		//GB2312ToUTF8(tt,(char *)str,(int)strlen(str));

		size_t len = tt.length();
		for (size_t i=0;i<len;i++)
		{
			if(isalnum((BYTE)tt.at(i)))
			{
				char tempbuff[2]={0};
				sprintf_s(tempbuff,"%c",(BYTE)tt.at(i));
				dd.append(tempbuff);
			}
			else if (isspace((BYTE)tt.at(i)))
			{
				dd.append("+");
			}
			else
			{
				char tempbuff[4];
				sprintf_s(tempbuff,"%%%X%X",((BYTE)tt.at(i)) >>4,((BYTE)tt.at(i)) %16);
				dd.append(tempbuff);
			}

		}
		return dd;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UrlUTF8异常!");
	}
	return string("");
}

//把url utf8解码
string CUrlConver::UrlUTF8Decoder(const string& str, char* buf, const uint32 len)
{
	string output = "";
	try
	{
		string temp = UrlGB2312Decoder(str);

		UTF8ToGB2312(output,(char *)temp.data(),(uint32)strlen(temp.data()),buf,len);
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UrlUTF8Decode异常!");
	}
	return output;
}

string CUrlConver::UrlDecoder(string &str)
{
	int num = 0;
	char c0 = 0;
	char c1 = 0;
	string result = "";
	try
	{
		for ( unsigned int i = 0; i < str.length(); i++ ) 
		{
			char c = str[i];
			if ( c != '%' ) 
			{
				result += c;
			} 
			else 
			{
				if (i + 2 < str.length())
				{
					c1 = str[++i];
					c0 = str[++i];
					num = 0;
					num += HexCharToShort(c1) * 16 + HexCharToShort(c0);
					result += char(num);
				}
				else
				{
					break;
				}
			}
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"UrlDecoder(string)异常!");
	}
	return result;
}

bool CUrlConver::UrlDecoder(const char* src, const uint32 srclen, char* dst, const uint32 dstlen)
{
	char ch0 = 0;
	char ch1 = 0;
	int number = 0;
	size_t dLen = 0;
	char *sData = NULL;
	char *dData = NULL;
	if (src == NULL || srclen == 0 || dst == NULL || dstlen == 0)
	{
		return false;
	}
	sData = (char *)src;
	dData = (char *)dst;
	while (sData != NULL && dData != NULL)
	{
		if(*sData != '%')
		{
			*dData = *sData;
		} 
		else 
		{
			if((unsigned int)(sData + 2 - src) >= srclen)
			{
				break;
			}
			ch1 = *(++sData);
			ch0 = *(++sData);
			number = HexCharToInteger(ch1) * 16 + HexCharToInteger(ch0);
			*dData = char(number);
		}
		if (((unsigned int)(sData + 1 - src) >= srclen) || ((unsigned int)(dData + 1 - dst) >= (dstlen - 1)))
		{
			break;
		}
		dData++;
		sData++;
	}
	*(++dData) = '\0';
	return true;
}
	
//HtmlUnicode解码(\uxxxx格式)
bool CUrlConver::HtmlUnicodeDecorder(const char* srcData, const unsigned int srcLen, char* desData,const unsigned int desLen)
{
	char tempBuf[5];
	wchar_t sNumber; 
	char pchAnsi[3];  
	int iChangeNumber = 0;
	unsigned int dLen = 0 ;

	if (srcData == NULL || srcLen == 0 || desData == NULL || desLen == 0)
	{
		return false;
	}

	for (size_t i = 0 ;i < srcLen; i++)
	{
		if ((srcData[i] == '\\') && ((i + 1) < srcLen) && (srcData[i+1] == 'u'))
		{
			i += 2;
			if ( (i+4) < srcLen)
			{
				memset(tempBuf,0,5);
				memcpy(tempBuf,&srcData[i],4);
				sNumber = (wchar_t)strtoul( tempBuf,NULL,16);
				iChangeNumber = WideCharToMultiByte( CP_ACP, 0, &sNumber, 1,pchAnsi, 3,NULL,NULL);
				if ( iChangeNumber != 0)
				{
					if ( (dLen + iChangeNumber) >= (desLen - 1) )
					{
						break;
					}
					memcpy(desData + dLen,pchAnsi,iChangeNumber);
					dLen += iChangeNumber;
				}
				i += 3;
			}
		}
		else
		{
			if ( (dLen + 1) >= (desLen - 1) )
			{
				break;
			}
			desData[dLen] = srcData[i];
			dLen++;
		}
	}
	desData[dLen] = '\0';
	return true;
}

wstring CUrlConver::StringToWstring(const string &str)
{
	_bstr_t t;
	wstring outstr;
	try
	{
		t = str.c_str();
		wchar_t *pWChar = (wchar_t *)t;
		outstr = t;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"StringToWstring异常!");
	}
	return outstr;
}

string  CUrlConver::WstringToString(const wstring& wstr)
{
	_bstr_t t;
	string outstr="";
	try
	{
		t = wstr.c_str();
		char* pChar = (char *)t;
		outstr = t;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"WstringToString异常!");
	}
	return outstr;
}

bool CUrlConver::IsHtmlUnicode(string &strValue)
{
	if (strValue.empty())
	{
		return false;
	}
	if (strValue.find(UC_FLAG) != string::npos)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//\uxxxx格式Unicode
void  CUrlConver::DecodeHtmlUnicode( string& content)
{
	try
	{
		const int UC_CODE_LEN = 4;
		size_t iCurrPos = 0;

		while ( (iCurrPos = content.find( UC_FLAG, iCurrPos)) != string::npos)
		{
			string strAnsi = DecodeUnicodeNumber( content.substr( iCurrPos + strlen(UC_FLAG),UC_CODE_LEN));
			if ( strAnsi.length() != 0)
			{
				content.replace( iCurrPos, strlen(UC_FLAG)+UC_CODE_LEN, strAnsi);
			}
			++iCurrPos;
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"DecodeHtmlUnicode异常!");
	}
}

string  CUrlConver::DecodeUnicodeNumber( const string& strNumber)
{
	try
	{
		wchar_t sNumber = (wchar_t)strtoul( strNumber.c_str(),NULL,16);
		char pchAnsi[3];   
		int iChangeNumber = WideCharToMultiByte( CP_ACP, 0, &sNumber, 1,pchAnsi, 3,NULL,NULL);
		if ( iChangeNumber == 0)
		{
			return "";
		}
		else
		{
			pchAnsi[iChangeNumber] = '\0'; 
			return pchAnsi;
		}
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"DecodeUnicodeNumber异常!");
	}	
	return "";
}

bool CUrlConver::IsTextUTF8(char* str,size_t length)
{
	try
	{
		if (str==NULL || length == 0)
		{
			return false;
		}
		size_t i;
		DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
		UCHAR chr;
		bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
		for(i=0;i<length;i++)
		{
			chr= *(str+i);
			if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			{
				bAllAscii= false;
			}
			if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if(chr>=0x80)
				{
					if(chr>=0xFC&&chr<=0xFD)
						nBytes=6;
					else if(chr>=0xF8)
						nBytes=5;
					else if(chr>=0xF0)
						nBytes=4;
					else if(chr>=0xE0)
						nBytes=3;
					else if(chr>=0xC0)
						nBytes=2;
					else
					{
						return false;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if( (chr&0xC0) != 0x80 )
				{
					return false;
				}
				nBytes--;
			}
		}
		if( nBytes > 0 ) //违返规则
		{
			return false;
		}
		if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
		{
			return false;
		}
		return true;
	}
	catch (...)
	{
		CFuncs::WriteLogInfo(SLT_ERROR,"IsTextUTF8异常!");
	}
} 

bool CUrlConver::IsUrlCode(const string& str)
{
	size_t count = 0;
	for (size_t i = 0; i < str.length();i++ )
	{
		if ( str[i] != '%' )
		{
			count++;
		}
	}
	if (count > (str.length() / 4) )
	{
		return true;
	}
	return false;
}

//%uxxxx解码
bool CUrlConver::ReplaseCode( char* srcData, const unsigned int srcLen)
{
	if (srcData == NULL || srcLen == 0)
	{
		return false;
	}
	for (size_t i=0; i < (srcLen - 1) ;i++)
	{
		if ((srcData[i] == '%') && (srcData[i+1] == 'u'))
		{
			srcData[i] = '\\';
		}
	}
	return true;
}

//搜索+换空格
bool CUrlConver::ReplaseCode2( char* srcData, const unsigned int srcLen)
{
	if (srcData == NULL || srcLen == 0)
	{
		return false;
	}
	for (size_t i=0; i < srcLen ;i++)
	{
		if ((srcData[i] == '+'))
		{
			srcData[i] = ' ';
		}
	}
	return true;
}


//附件名特殊字符处理,\ / : * ? " < > | 
bool CUrlConver::ClearSpecialChar( char* srcData, const unsigned int srcLen )
{
	if (srcData == NULL || srcLen == 0)
	{
		return false;
	}
	size_t j = 0;
	for (size_t i = 0; i < srcLen ; i++)
	{
		if ((srcData[i] == '\\' || srcData[i] == '/' || srcData[i] == ':' || 
			 srcData[i] == '*'  || srcData[i] == '?' || srcData[i] == '\"' || 
			 srcData[i] == '<'  || srcData[i] == '>' || srcData[i] == '|'))
		{
			
		}
		else
		{
			srcData[j] = srcData[i];
			j++;
		}
	}
	srcData[j] = '\0';
	return true;
}

//处理网页转移字符
bool CUrlConver::ClearTransferredChar(char *srcData, const unsigned int srcLen)
{
	if (srcData == NULL || srcLen == 0)
	{
		return false;
	}

	size_t j = 0;
	for (size_t i = 0; i < srcLen; i++)
	{
		if (srcData[i] == '\\')
		{
			switch(srcData[i+1])
			{
			case 'a'://响铃
				srcData[j] = '\a';
				i++;j++;
				break;
			case 'b'://退格
				srcData[j] = '\b';
				i++;j++;
				break;
			case 'f'://换页
				srcData[j] = '\f';
				i++;j++;
				break;
			case 'n'://换行
				srcData[j] = '\n';
				i++;j++;
				break;
			case 'r'://回车
				srcData[j] = '\r';
				i++;j++;
				break;
			case 't'://水平制表
				srcData[j] = '\t';
				i++;j++;
				break;
			case 'v'://垂直制表
				srcData[j] = '\v';
				i++;j++;
				break;
			case '\\'://反斜杠
				srcData[j] = '\\';
				i++;j++;
				break;
			case '?'://问号字符
				srcData[j] = '\?';
				i++;j++;
				break;
			case '\''://单引号字符
				srcData[j] = '\'';
				i++;j++;
				break;
			case '\"'://双引号字符
				srcData[j] = '\"';
				i++;j++;
				break;
			default:
				break;
			}
		}
		else
		{
			srcData[j] = srcData[i];
			j++;
		}
	}
	srcData[j] = '\0';
	return true;
}

//全部转换小写
string CUrlConver::UpperToLower(const string& src)
{
	string result = src;
	for(int i = 0; i < static_cast<int>(result.size()); i++)
	{
		if((result[i] >= 'A') && (result[i] <= 'Z'))
		{
			result[i] = result[i] - 'A' + 'a';
		}
	}
	return result;
}

void CUrlConver::UpperToLower(char* src ,const unsigned int srcLen)
{
	if(NULL == src || 1 > srcLen)
	{
		return;
	}
	for(unsigned int i = 0; i < srcLen; i++)
	{
		if((src[i] >= 'A') && (src[i] <= 'Z'))
		{
			src[i] = src[i] - 'A' + 'a';
		}
	}
}

//全部转换大写
string CUrlConver::LowerToUpper(const string& src)
{
	string result = src;
	for(int i = 0; i < static_cast<int>(result.size()); i++)
	{
		if((result[i] >= 'a') && (result[i] <= 'z'))
		{
			result[i] = result[i] - 'a' + 'A';
		}
	}
	return result;
}

void CUrlConver::LowerToUpper(char* src ,const unsigned int srcLen)
{
	if(NULL == src || 1 > srcLen)
	{
		return;
	}
	for(unsigned int i = 0; i < srcLen; i++)
	{
		if((src[i] >= 'a') && (src[i] <= 'z'))
		{
			src[i] = src[i] - 'a' + 'A';
		}
	}
}