#include    "stdafx.h"
#include	<math.h>
#include	"ConvertUTF8.h"



/***************************************************************************

//	Convert Unicode Character To UTF8 string
//  IN		:  uc      - Unicode Character
//  OUT		:  UTF8	   - UTF8 String
//  RETURN  :  UTF8 String

***************************************************************************/
CHAR*	UnicodeToUTF8(_TCHAR uc, CHAR* UTF8)
{
	if (uc <= 0x7f)
	{		
		UTF8[0] = (CHAR) uc;
		UTF8[1] = (CHAR) '\0';
	}
	else if (uc <= 0x7ff)
	{
		UTF8[0] = (CHAR) 0xc0 + uc / (_TCHAR) pow(2, 6);
		UTF8[1] = (CHAR) 0x80 + uc % (_TCHAR) pow(2, 6);
		UTF8[2] = (CHAR) '\0';
	}
	else if (uc <= 0xffff)
	{
		UTF8[0] = (CHAR) 0xe0 + uc / (_TCHAR) pow(2, 12);
		UTF8[1] = (CHAR) 0x80 + uc / (_TCHAR) pow(2, 6) % (_TCHAR) pow(2, 6);
		UTF8[2] = (CHAR) 0x80 + uc % (_TCHAR) pow(2, 6);
		UTF8[3] = (CHAR) '\0';
	}

	return UTF8;
}

/***************************************************************************

//	Convert Unicode String To UTF8 string
//  IN		:  szUni   - Unicode String
//  OUT		:  szUTF8  - UTF8 String
//  RETURN  :  The Length of UTF8 String. 
//             IF	Length > 0 Success, 
//             ELSE	Length = 0 Fail.

***************************************************************************/
INT WideStrToUTF8Str(_TCHAR *szUni, CHAR **szUTF8)
{
	INT		i;
	INT		nULen;
	CHAR		TempUTF8[10];

	
	nULen     = _tcslen(szUni)*3;
    (*szUTF8) = (CHAR*)malloc(nULen*sizeof(CHAR) + sizeof(CHAR));
	memset((*szUTF8), 0x00, nULen);

	for(i=0;szUni[i];i++) 
    {
		TempUTF8[0] = '\0';
		UnicodeToUTF8(szUni[i], TempUTF8);
		strcat((*szUTF8), TempUTF8);
	}
	return strlen((*szUTF8));
}
