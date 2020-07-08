/*****************************************************************************
Module :     URLEncode.cpp
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: CPP URL Encoder
*****************************************************************************/
#define _CRTDBG_MAP_ALLOC

#include "stdafx.h"
#include <math.h>
#include <malloc.h>
#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>
#include <WININET.H>

#include "URLEncode.h"

#define MAX_BUFFER_SIZE 4096
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#include <stdlib.h>
#include <crtdbg.h>
// HEX Values array
TCHAR hexVals[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
// UNSAFE String
CString CURLEncode::csUnsafeString= _T("\"<>%\\^[]`+$,@:;/!#?=&");

// PURPOSE OF THIS FUNCTION IS TO CONVERT A GIVEN CHAR TO URL HEX FORM
CString CURLEncode::convert(TCHAR val) 
{
	CString csRet;
	csRet += _T("%");
	csRet += decToHex(val, 16);	
	return  csRet;
}

// THIS IS A HELPER FUNCTION.
// PURPOSE OF THIS FUNCTION IS TO GENERATE A HEX REPRESENTATION OF GIVEN CHARACTER
CString CURLEncode::decToHex(TCHAR num, int radix)
{	
	int temp=0;	
	CString csTmp;
	int num_char;
	num_char = (int) num;
	
	// ISO-8859-1 
	// IF THE IF LOOP IS COMMENTED, THE CODE WILL FAIL TO GENERATE A 
	// PROPER URL ENCODE FOR THE CHARACTERS WHOSE RANGE IN 127-255(DECIMAL)
	if (num_char < 0)
		num_char = 256 + num_char;

	while (num_char >= radix)
    {
		temp = num_char % radix;
		num_char = (int)floor(num_char / radix);
		csTmp = hexVals[temp];
    }
	
	csTmp += hexVals[num_char];

	if(csTmp.GetLength() < 2)
	{
		csTmp += '0';
	}

	CString strdecToHex(csTmp);
	// Reverse the String
	strdecToHex.MakeReverse(); 
	
	return strdecToHex;
}

// PURPOSE OF THIS FUNCTION IS TO CHECK TO SEE IF A CHAR IS URL UNSAFE.
// TRUE = UNSAFE, FALSE = SAFE
bool CURLEncode::isUnsafe(TCHAR compareChar)
{
	bool bcharfound = false;
	TCHAR tmpsafeChar;
	int m_strLen = 0;
	
	m_strLen = csUnsafeString.GetLength();
	for(int ichar_pos = 0; ichar_pos < m_strLen ;ichar_pos++)
	{
		tmpsafeChar = csUnsafeString.GetAt(ichar_pos); 
		if(tmpsafeChar == compareChar)
		{ 
			bcharfound = true;
			break;
		} 
	}
	
	int char_ascii_value = 0;
	//char_ascii_value = __toascii(compareChar);
	char_ascii_value = (int) compareChar;

	if(bcharfound == false &&  char_ascii_value > 32 && char_ascii_value < 123)
	{
		return false;
	}
	// found no unsafe chars, return false		
	else
	{
		return true;
	}
	
	return true;
}
// PURPOSE OF THIS FUNCTION IS TO CONVERT A STRING 
// TO URL ENCODE FORM.
CString CURLEncode::URLEncode(CString pcsEncode)
{	
	int ichar_pos;
	CString csEncode;
	CString csEncoded;	
	int m_length;

	csEncode = pcsEncode;
	m_length = csEncode.GetLength();
	
	for(ichar_pos = 0; ichar_pos < m_length; ichar_pos++)
	{
		TCHAR ch = csEncode.GetAt(ichar_pos);
		if (ch < ' ') 
		{
			ch = ch;
		}		
		if(!isUnsafe(ch))
		{
			// Safe Character				
			csEncoded += CString(ch);
		}
		else
		{
			// get Hex Value of the Character
			csEncoded += convert(ch);
		}
	}
	

	return csEncoded;

}