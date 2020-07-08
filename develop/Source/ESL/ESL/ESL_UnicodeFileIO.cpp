// ESL_UnicodeFileIO.cpp: implementation of the CESL_UnicodeFileIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_UnicodeFileIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include <io.h>
#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_UnicodeFileIO::CESL_UnicodeFileIO()
{	
	m_fd = NULL;
	m_filetype = -1;
}

CESL_UnicodeFileIO::~CESL_UnicodeFileIO()
{
	Close();
}

INT CESL_UnicodeFileIO::Open(CString filename)
{
EFS_BEGIN

	Close();
	m_fd = _tfopen(filename, _T("rb"));
	if (!m_fd) return -1;
	m_filetype = GetFileType();
	return 0;

EFS_END
return -1;

}

INT CESL_UnicodeFileIO::Close(void)
{
EFS_BEGIN

	if (m_fd) {
		fclose(m_fd);
		m_fd = NULL;
	} 
	if (m_filetype > 0) m_filetype = -1;
	return 0;

EFS_END
return -1;

}

/* 파일 유형을 리턴한다. */
INT CESL_UnicodeFileIO::GetFileType(void)
{
EFS_BEGIN

	if (!m_fd) return -1;
    INT   nFileType;
    long  lFilePos;
    unsigned char c1, c2;
 
    /* 파일에서 현재 위치를 기억 */
    lFilePos = ftell(m_fd);
    /* 파일 처음으로 이동 */
    fseek(m_fd, 0, SEEK_SET);
    /* 파일 처음으로 2바이트를 읽어 파일의 유형을 파악 */
    c1 = fgetc(m_fd);	
    c2 = fgetc(m_fd);
    if(c1 == 0xff && c2 == 0xfe)        nFileType = 2;	//FILE_UNIL;  /* UNICODE Little Endian */
    else if(c1 == 0xfe && c2 == 0xff)   nFileType = 1;	//FILE_UNIB;  /* UNICODE Big Endian    */
    else                                nFileType = 0;	//FILE_MBCS;  /* MBCS                  */
    /* 기억했던 파일 위치로 이동 */
    fseek(m_fd, lFilePos, SEEK_SET);
 
    return nFileType;

EFS_END
return -1;

}
 
/* 파일에서 한줄을 읽는다 */
/* 파일 형식(UNICODE Big Ed.,UNICODE Little Ed.)에 상관없이 읽어준다. */
INT  CESL_UnicodeFileIO::GetFileLine(unsigned short* pLine, INT* pLineLen, INT nLineSize)
{
EFS_BEGIN

	INT   idx;
	INT   ids;
	CHAR *pMBCSLine = NULL;        /* 파일이 MBCS이면 여기에 기록    */
	unsigned short *pUNILine = NULL;        /* 파일이 UNICODE이면 여기에 기록 */
	INT   nReturn = 0;
	unsigned char c1, c2;
	_TINT ch;

	pLine[0] = _T('\0');
	idx=0;
	*pLineLen = 0;
 
	if(m_filetype < 0) return _TEOF;
	/* UNICODE 형식이면서 0번부터 Read하려고 하면 */
	if(m_filetype && ftell(m_fd) < 2) 
		fseek(m_fd, 2, SEEK_SET);
 
    /* 파일이 UNICODE 형식일 경우 */
    if(m_filetype == 2/*FILE_UNIL*/ || m_filetype == 1/*FILE_UNIB*/)
    {
		pUNILine = (unsigned short*)malloc(nLineSize * sizeof(unsigned short));
		while(1) {
			ids = fread( &c1, sizeof( char ), 1, m_fd );
			ids = fread( &c2, sizeof( char ), 1, m_fd );

			if (ids == EOF) ids = 0;
			if(m_filetype == 2/*FILE_UNIL*/)
				ch = (c2 << 8) + c1;    /* UNICODE Little Endian */
			else
				ch = (c1 << 8) + c2;    /* UNICODE Big Endian    */
			TCHAR tt = ch;
			if(ch == _T('\r')) continue;
			if(ch == _T('\n') || idx == nLineSize - 1)
			{
				nReturn = 0;
				break;
			}
			if (ids <= 0) 
			{
				if(idx == 0) nReturn = _TEOF;
					else
					nReturn = 0;
				break;
			}
			TCHAR t = ch;
			pUNILine[idx] = ch;
			idx += ids;
		}
		pUNILine[idx] = 0;
    }
 
    /* 데몬이 UNICODE를 사용하는 경우 */
    if(m_filetype == 2/*FILE_UNIL*/ || m_filetype == 1/*FILE_UNIB*/) {
		wcscpy(pLine, pUNILine);
		free(pUNILine);
    }
    /* 데몬이 MBCS를 사용하는 경우 */
    else
    {
        /* TODO: NT에서만 가능하다, UNIX용도 찾아야 함!!!*/
		// KOL.UDF.022
		if(&pMBCSLine != NULL){
			ids = strlen(pMBCSLine);
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, 
								pMBCSLine, strlen(pMBCSLine) + 1,
								pLine, nLineSize / sizeof(TCHAR));
			free(pMBCSLine);
		}
    }
	/* 읽은 길이 */
	*pLineLen = wcslen(pLine);
	return nReturn;

EFS_END
return -1;

}
