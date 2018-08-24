#pragma once

#include "Windows.h"
#include <string>

class CBase64
{
public:
	CBase64(void);
	~CBase64(void);

	
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
	static int Encode(const unsigned char *src, size_t slen, unsigned char *dst, size_t dlen);

	static std::string Encode(const unsigned char * str,int bytes); 

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
	static int Decode(const unsigned char *src, size_t slen, unsigned char *dst, size_t dlen);
};

