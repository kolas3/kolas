// CHash.cpp : implementation file
//



#include "stdafx.h"
#include "CHash.h"



#if SUPPORT_CRC32
	#include "crc32.h" 
#endif
#if SUPPORT_GOSTHASH 
	#include "gosthash.h" 
#endif
#if SUPPORT_MD2 
	#include "md2.h" 
#endif
#if SUPPORT_MD4 
	#include "md4.h" 
#endif
#if SUPPORT_MD5 
	#include "md5.h" 
#endif
#if SUPPORT_SHA1
	#include "sha1.h" 
#endif
#if SUPPORT_SHA2
	#include "sha2.h" 
#endif



/////////////////////////////////////////////////////////////////////////////
// CHash class



CHash::CHash()
{
	// Set some variables for defaultness
	hashAlgo = SHA1;
	hashFile = "";
	hashOperation = 1;
	hashString = "";
	sha2Strength = 256;
	tempHash.Empty();
}
CHash::~CHash()
{
}


// Do/Retrieve the hash
//
//
CString CHash::DoHash()
{
	CString retHash = "";

	tempHash.Empty();

	// See what hash was selected and initiate it

	// If CRC32 don't bother calling a seperate function, do it here
	if (hashAlgo == CRC32)
	{
#if SUPPORT_CRC32
		DWORD crc32r;
		CCrc32Static crc32;

		if (hashOperation == STRING_HASH)
		{
			crc32.StringCrc32((LPCSTR)hashString, crc32r);
		}
		if (hashOperation == FILE_HASH)
		{
			crc32.FileCrc32Streams((LPCSTR)hashFile, crc32r);
		}

		retHash.Format(_T("%08x"), crc32r);
#endif
	}

	if (hashAlgo == GOSTHASH)
		retHash = GOSTHash();

	if (hashAlgo == MD2)
		retHash = MD2Hash();

	if (hashAlgo == MD4)
		retHash = MD4Hash();

	if (hashAlgo == MD5)
		retHash = MD5Hash();

	if (hashAlgo == SHA1)
		retHash = SHA1Hash();

	if (hashAlgo == SHA2)
		retHash = SHA2Hash();

	// Do the styling
	if (hashFormatting == LOWERCASE_SPACES || hashFormatting == UPPERCASE_SPACES)
	{
		// Add spacing
		bool isChar = false;
		int hashLen = retHash.GetLength();

		for (int i = 0; i < hashLen; i++)
		{
			if (!isChar)
			{
				tempHash += retHash.GetAt(i);
				isChar = true;
			}
			else
			{
				tempHash += retHash.GetAt(i);
				tempHash += " ";
				isChar = false;
			}
		}

		retHash = tempHash;
	}

	if (hashFormatting == UPPERCASE_NOSPACES || hashFormatting == UPPERCASE_SPACES)
	{
		retHash.MakeUpper();
	}

	return retHash;
}
// End DoHash ///////////////////////////////////////////////////



// Get the current algorithm
//
//
int CHash::GetHashAlgorithm()
{
	return hashAlgo;
}
// End GetHashAlgorithm /////////////////////////////////////////



// Get the current operation
//
//
int CHash::GetHashOperation()
{
	return hashOperation;
}
// End GetHashOperation /////////////////////////////////////////



// Get the input string
//
//
CString CHash::GetHashString()
{
	return hashString;
}
// End GetHashString ////////////////////////////////////////////



// Get the hash file
//
//
CString CHash::GetHashFile()
{
	return hashFile;
}
// End GetHashFile //////////////////////////////////////////////



// Get the hash format
//
//
int CHash::GetHashFormat()
{
	return hashFormatting;
}
// End GetHashFormat ////////////////////////////////////////////



// Get the strength of the SHA-2 hash (256, 384 or 512)
//
//
int CHash::GetSHA2Strength()
{
	return sha2Strength;
}
// End GetSHA2Strength //////////////////////////////////////////



// Hash with GOST-Hash
//
//
CString CHash::GOSTHash()
{
#if SUPPORT_GOSTHASH
	// Common variables
	FILE *fileToHash = NULL;
	unsigned char fileBuf[SIZE_OF_BUFFER];
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];

	// GOST-Hash object to work with
	GostHashCtx m_gost;

	// Check what operation we're doing
	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, reset the object, give it the text, and finalise it
		gosthash_reset(&m_gost);
		gosthash_update(&m_gost, reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength());
		gosthash_final(&m_gost, tempOut);
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";
		gosthash_reset(&m_gost);

		fileToHash = fopen(hashFile, "rb");
	
		do
		{
			lenRead = fread(fileBuf, 1, SIZE_OF_BUFFER, fileToHash);
			if(lenRead != 0)
			{
				gosthash_update(&m_gost, fileBuf, lenRead);
			}
		} while (lenRead == SIZE_OF_BUFFER);

		fclose(fileToHash); fileToHash = NULL;

		gosthash_final(&m_gost, tempOut);
	}

	for (int i = 0 ; i < 32 ; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp , 16);

		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}
		tempHash += tmp;
	}

	delete[] tempOut;

	outHash = tempHash;
#endif
	return outHash;
}
// End GOSTHash /////////////////////////////////////////////////



// Hash with MD2
//
//
CString CHash::MD2Hash()
{
#if SUPPORT_MD2
	// Common variables
	FILE *fileToHash = NULL;
	unsigned char fileBuf[SIZE_OF_BUFFER];
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];

	// MD2 object to work with
	CMD2 m_md2;

	// Check what operation we're doing
	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, open the object, give it the text, and finalise it
		m_md2.Init();
		m_md2.Update(reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength());
		m_md2.TruncatedFinal(tempOut, 16);
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";
		m_md2.Init();

		fileToHash = fopen(hashFile, "rb");
	
		do
		{
			lenRead = fread(fileBuf, 1, SIZE_OF_BUFFER, fileToHash);
			if(lenRead != 0)
			{
				m_md2.Update(fileBuf, lenRead);
			}
		} while (lenRead == SIZE_OF_BUFFER);

		fclose(fileToHash); fileToHash = NULL;

		m_md2.TruncatedFinal(tempOut, 16);
	}

	for (int i = 0 ; i < 16 ; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp, 16);
		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}
		tempHash += tmp;	
	}

	delete[] tempOut;

	outHash = tempHash;
#endif
	return outHash;
}
// End MD2Hash //////////////////////////////////////////////////



// Hash with MD4
//
//
CString CHash::MD4Hash()
{
#if SUPPORT_MD4
	// Common variables
	FILE *fileToHash = NULL;
	unsigned char fileBuf[SIZE_OF_BUFFER];
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];

	// MD4 object to work with
	MD4_CTX m_md4;

	// Check what operation we're doing
	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, open the object, give it the text, and finalise it
		MD4Init(&m_md4);
		MD4Update(&m_md4, reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength());
		MD4Final(tempOut, &m_md4);
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";
		MD4Init(&m_md4);

		fileToHash = fopen(hashFile, "rb");
	
		do
		{
			lenRead = fread(fileBuf, 1, SIZE_OF_BUFFER, fileToHash);
			if(lenRead != 0)
			{
				MD4Update(&m_md4, fileBuf, lenRead);
			}
		} while (lenRead == SIZE_OF_BUFFER);

		fclose(fileToHash); fileToHash = NULL;

		MD4Final(tempOut, &m_md4);
	}

	for (int i = 0 ; i < 16 ; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp, 16);
		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}

		tempHash += tmp;	
	}

	delete[] tempOut;

	outHash = tempHash;
#endif
	return outHash;
}
// End MD4Hash //////////////////////////////////////////////////



// Hash with MD5
//
//
CString CHash::MD5Hash()
{
#if SUPPORT_MD5
	// Common variables
	FILE *fileToHash = NULL;
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];
	CString tempHash = "";

	// MD5 object to work with
	MD5_CTX m_md5;

	// Check what operation we're doing
	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, open the object, give it the text, and finalise it
		MD5Init(&m_md5);
		MD5Update(&m_md5, reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength());
		MD5Final(&m_md5);

		// Get each chunk of the hash and append it to the output
		for ( int i = 0 ; i < 16 ; i++ )
		{
			char tmp[3];
			_itoa(m_md5.digest[i], tmp , 16);

			if (strlen(tmp) == 1)
			{
				tmp[1] = tmp[0];
				tmp[0] = '0';
				tmp[2] = '\0';
			}
			tempHash += tmp;
		}
		// Copy the temporary hash to the final buffer
		outHash = tempHash;
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";

		if (md5file(hashFile.GetBuffer(MAX_PATH), 0, &m_md5))
		{
			for (int i = 0; i < 16; i++)
			{
				char tmp[3];
				_itoa(m_md5.digest[i], tmp, 16);
				if (strlen(tmp) == 1)
				{
					tmp[1] = tmp[0];
					tmp[0] = '0';
					tmp[2] = '\0';
				}
				tempHash += tmp;		
			}
		}
		outHash = tempHash;
	}

	delete[] tempOut;

#endif
	return outHash;
}
// End MD5Hash //////////////////////////////////////////////////



// Hash with SHA-1
//
//
CString CHash::SHA1Hash()
{
#if SUPPORT_SHA1
	// Common variables
	FILE *fileToHash = NULL;
	unsigned char fileBuf[SIZE_OF_BUFFER];
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];

	// SHA-1 object
	sha1_ctx m_sha1;

	// Check what operation we're doing
	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, open the object, give it the text, and finalise it
		sha1_begin(&m_sha1);
		sha1_hash(reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength(), &m_sha1);
		sha1_end(tempOut, &m_sha1);
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";
		sha1_begin(&m_sha1);

		fileToHash = fopen(hashFile, "rb");
	
		do
		{
			lenRead = fread(fileBuf, 1, SIZE_OF_BUFFER, fileToHash);
			if(lenRead != 0)
			{
				sha1_hash(fileBuf, lenRead, &m_sha1);
			}
		} while (lenRead == SIZE_OF_BUFFER);

		fclose(fileToHash); fileToHash = NULL;

		sha1_end(tempOut, &m_sha1);
	}

	for (int i = 0 ; i < 20 ; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp, 16);
		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}
		tempHash += tmp;	

	}

	delete[] tempOut;

	outHash = tempHash;
#endif
	return outHash;
}
// End SHA1Hash /////////////////////////////////////////////////



// Hash with SHA-2
//
//
CString CHash::SHA2Hash()
{
#if SUPPORT_SHA2
	// Common variables
	FILE *fileToHash = NULL;
	unsigned char fileBuf[SIZE_OF_BUFFER];
	unsigned long lenRead = 0;

	// The outputted hash
	CString outHash;

	// Temporary working buffers
	unsigned char* tempOut = new unsigned char[256];

	sha2_ctx sha;

	// Work out hash length
	int hashLength = sha2Strength / 8;

	if (hashOperation == STRING_HASH)
	{
		// Hashing a string, open the object, give it the text, and finalise it
		sha2_begin(sha2Strength, &sha);
		sha2_hash(reinterpret_cast<unsigned char *>(hashString.GetBuffer(hashString.GetLength())), hashString.GetLength(), &sha);
		sha2_end(tempOut, &sha);
	}

	// Hash a file
	if (hashOperation == FILE_HASH)
	{
		if (hashFile.IsEmpty()) return "";
		sha2_begin(sha2Strength, &sha);

		fileToHash = fopen(hashFile, "rb");
	
		do
		{
			lenRead = fread(fileBuf, 1, SIZE_OF_BUFFER, fileToHash);
			if(lenRead != 0)
			{
				sha2_hash(fileBuf, lenRead, &sha);
			}
		} while (lenRead == SIZE_OF_BUFFER);

		fclose(fileToHash); fileToHash = NULL;

		sha2_end(tempOut, &sha);
	}

	for (int i = 0; i < hashLength; i++)
	{
		char tmp[3];
		_itoa(tempOut[i], tmp, 16);
		if (strlen(tmp) == 1)
		{
			tmp[1] = tmp[0];
			tmp[0] = '0';
			tmp[2] = '\0';
		}
		tempHash += tmp;		
	}

	delete[] tempOut;

	outHash = tempHash;
#endif
	return outHash;
}
// End SHA2Hash /////////////////////////////////////////////////



// Set the hash algorithm
//
//
void CHash::SetHashAlgorithm(int Algo)
{
	hashAlgo = Algo;
}
// End SetHashAlgorithm /////////////////////////////////////////



// Set the formatting style of the output hash
//
//
void CHash::SetHashFormat(int Style)
{
	if (Style == 1 || Style == 2 || Style == 3 || Style == 4)
	{
		hashFormatting = Style;
	}
	else
	{
		hashFormatting = 1;
	}
}
// End SetHashFormat ////////////////////////////////////////////



// Set the hash operation
// Available operations:
//
// STRING_HASH
// FILE_HASH
//
//
void CHash::SetHashOperation(int Style)
{
	// Check the style is legit
	if (Style == 1 || Style == 2)
	{
		hashOperation = Style;
	}
	else
	{
		hashOperation = 1;
	}
}
// End SetHashOperation /////////////////////////////////////////



// Set the hash file
//
//
void CHash::SetHashFile(LPCSTR File)
{
	hashFile = File;
}
// End SetHashFile //////////////////////////////////////////////



// Set the SHA-2 hash strength
//
// 256, 384 or 512
//
//
void CHash::SetSHA2Strength(int Strength)
{
	// Check the strength is legit
	if (Strength == 256 || Strength == 384 || Strength == 512)
	{
		sha2Strength = Strength;
	}
	else
	{
		sha2Strength = 256;
	}
}
// End SetSHA2Strength///////////////////////////////////////////



// Set the input string
//
//
void CHash::SetHashString(LPCSTR Hash)
{
	hashString.Empty();
	hashString = Hash;
}
// End SetHashString ////////////////////////////////////////////