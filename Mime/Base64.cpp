#include "StdAfx.h"
#include "Base64.h"


CBase64::CBase64(void)
{

}


CBase64::~CBase64(void)
{
}


/**
* \brief          Encode a buffer into base64 format
*
* \param dst      destination buffer
* \param dlen     size of the buffer
* \param src      source buffer
* \param slen     amount of data to be encoded
*
* \return         than 0 if successful, or -1.
*                 *dlen is always updated to reflect the amount
*                 of data that has (or would have) been written.
*
* \note           Call this function with *dlen = 0 to obtain the
*                 required buffer size in *dlen
*/

int CBase64::Encode(const unsigned char *src, size_t slen, unsigned char *dst, size_t dlen)
{
	//±àÂë±í
    const char base64_enc_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	size_t i = 0;
	size_t n = 0;
	size_t dstlen = 0;
	int C1 = 0; 
	int C2 = 0;
	int C3 = 0;
	unsigned char *p = NULL;

	dstlen = dlen;
	if( slen == 0 )
	{
		return 0;
	}

    n = (slen << 3) / 6;

    switch( (slen << 3) - (n * 6) )
    {
        case  2: n += 3; break;
        case  4: n += 2; break;
        default: break;
    }

    if( dstlen < n + 1 )
    {
        dstlen = n + 1;
        return -1;
    }

    n = (slen / 3) * 3;

    for( i = 0, p = dst; i < n; i += 3 )
    {
        C1 = *src++;
        C2 = *src++;
        C3 = *src++;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
        *p++ = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
        *p++ = base64_enc_map[C3 & 0x3F];
    }

    if( i < slen )
    {
        C1 = *src++;
        C2 = ((i + 1) < slen) ? *src++ : 0;

        *p++ = base64_enc_map[(C1 >> 2) & 0x3F];
        *p++ = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];

        if( (i + 1) < slen )
             *p++ = base64_enc_map[((C2 & 15) << 2) & 0x3F];
        else *p++ = '=';

        *p++ = '=';
    }

    dstlen = p - dst;
    *p = 0;

    return (int)dstlen;
}


std::string CBase64::Encode(const unsigned char * str,int bytes) 
{
	const char base64_enc_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int num = 0,bin = 0;
	std::string _encode_result;
	const unsigned char * current;
	current = str;
	while(bytes > 2) {
		_encode_result += base64_enc_map[current[0] >> 2];
		_encode_result += base64_enc_map[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		_encode_result += base64_enc_map[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		_encode_result += base64_enc_map[current[2] & 0x3f];

		current += 3;
		bytes -= 3;
	}
	if(bytes > 0)
	{
		_encode_result += base64_enc_map[current[0] >> 2];
		if(bytes%3 == 1) {
			_encode_result += base64_enc_map[(current[0] & 0x03) << 4];
			_encode_result += "==";
		} else if(bytes%3 == 2) {
			_encode_result += base64_enc_map[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			_encode_result += base64_enc_map[(current[1] & 0x0f) << 2];
			_encode_result += "=";
		}
	}
	return _encode_result;
}
	/**
	* \brief          Decode a base64-formatted buffer
	*
	* \param dst      destination buffer
	* \param dlen     size of the buffer
	* \param src      source buffer
	* \param slen     amount of data to be decoded
	*
	* \return         than 0 if successful, -1 if the input data is not
	*                 correct. *dlen is always updated to reflect the amount
	*                 of data that has (or would have) been written.
	*
	* \note           Call this function with *dlen = 0 to obtain the
	*                 required buffer size in *dlen
	*/
int CBase64::Decode(const unsigned char *src, size_t slen, unsigned char *dst, size_t dlen)
{
	static const unsigned char base64_dec_map[128] =
	{
		127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
		127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
		127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
		127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
		127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
		54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
		127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
		5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
		15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
		25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
		29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
		39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
		49,  50,  51, 127, 127, 127, 127, 127
	};

	size_t i, n;
    size_t j, x;
	size_t dstlen = dlen;
    unsigned char *p;

    for( i = j = n = 0; i < slen; i++ )
    {
        if( ( slen - i ) >= 2 &&
            src[i] == '\r' && src[i + 1] == '\n' )
            continue;

        if( src[i] == '\n' )
            continue;

        if( src[i] == '=' && ++j > 2 )
            return( -1 );

        if( src[i] > 127 || base64_dec_map[src[i]] == 127 )
            return( -1 );

        if( base64_dec_map[src[i]] < 64 && j != 0 )
            return( -1 );

        n++;
    }

    if( n == 0 )
        return( 0 );

    n = ((n * 6) + 7) >> 3;

    if( dstlen < n )
    {
        dstlen = n;
        return( -1 );
    }

   for( j = 3, n = x = 0, p = dst; i > 0; i--, src++ )
   {
        if( *src == '\r' || *src == '\n' )
            continue;

        j -= ( base64_dec_map[*src] == 64 );
        x  = (x << 6) | ( base64_dec_map[*src] & 0x3F );

        if( ++n == 4 )
        {
            n = 0;
            if( j > 0 ) *p++ = (unsigned char)( x >> 16 );
            if( j > 1 ) *p++ = (unsigned char)( x >>  8 );
            if( j > 2 ) *p++ = (unsigned char)( x       );
        }
    }

    dstlen = p - dst;

    return (int)dstlen;
}