// ESL_MBCSFileIO.cpp: implementation of the CESL_MBCSFileIO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESL_MBCSFileIO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CESL_MBCSFileIO::CESL_MBCSFileIO()
{
	m_fd = NULL;
}

CESL_MBCSFileIO::~CESL_MBCSFileIO()
{
	Close();
}

INT CESL_MBCSFileIO::Open(CString filename)
{
EFS_BEGIN

	Close();
	m_fd = _tfopen(filename, _T("rb"));
	if (!m_fd) return -1;
	return 0;

EFS_END
return -1;

}

INT CESL_MBCSFileIO::Close(void)
{
EFS_BEGIN

	if (m_fd) {
		fclose(m_fd);
		m_fd = NULL;
	}
	return 0;

EFS_END
return -1;

}

INT CESL_MBCSFileIO::GetFileLine(char* pLine, INT* pLineLen, INT nLineSize)
{
EFS_BEGIN

	char * r = fgets(pLine, nLineSize, m_fd);
	if (r == NULL) return _TEOF;
	pLine[strlen(pLine)-1] = '\0';
	*pLineLen = strlen(pLine);
	return *pLineLen;

EFS_END
return -1;

}
