// SIReportReadBroker.cpp: implementation of the CSIReportReadBroker class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SIReportReadBroker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

#include "efs.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSIReportReadBroker::CSIReportReadBroker()
{
	BrokerFileName=_T("rpt_tmp\\broker.tmp");
	MODE = 0;
	PK = _T("");
	RPTALIAS = _T("");
}

CSIReportReadBroker::~CSIReportReadBroker()
{

}

INT CSIReportReadBroker::Load(CString filename/*=_T("")*/)
{
EFS_BEGIN

	TCHAR szbuf[2048];
	CString strBuf;

	MODE = 0;
	PK = _T("");
	RPTALIAS = _T("");

	FILE *fd = _tfopen(BrokerFileName, _T("rb"));
	if (fd == NULL) return -1;		// 파일을 열수 없음

	//++++++++++++++++++++++++++++++++++++++++++
	//Unicode일땐 2byte건너띄기
	FILE* fpUni = fd;

	if( 0xFF == fgetc(fpUni) ) 
	{
		if( 0xFE != fgetc(fpUni) )
		{
			fseek(fpUni, 0, SEEK_SET);
		}
	}
	else
	{
		fseek(fpUni, 0, SEEK_SET);
	}
	//------------------------------------------
	
	if (_fgetts(szbuf, 2047, fd)) {	// PK를 읽자
		szbuf[_tcsclen(szbuf)-1] = '\0';
		strBuf.Format(_T("%s"), szbuf);
		strBuf.TrimLeft();			strBuf.TrimRight();
		PK = strBuf;	
	} else {
		fclose(fd);
		return -2;					// PK를 파일에서 읽지 못함
	}
	
	if (_fgetts(szbuf, 2047, fd)) {	// SI Report Alias를 읽자
		szbuf[_tcsclen(szbuf)-1] = '\0';
		strBuf.Format(_T("%s"), szbuf);
		strBuf.TrimLeft();			strBuf.TrimRight();
		RPTALIAS = strBuf;	
	} else {
		fclose(fd);
		return -3;					// RPT ALIAS를 파일에서 읽지 못함
	}

	if (_fgetts(szbuf, 2047, fd)) {	// MODIFY MODE를 읽자
		szbuf[_tcsclen(szbuf)-1] = '\0';
		strBuf.Format(_T("%s"), szbuf);
		strBuf.TrimLeft();			strBuf.TrimRight();
		if (strBuf.GetLength() > 0) {
			MODE = _ttoi(strBuf);
		}
	} else {
		fclose(fd);
		return -3;					// MODIFY MODE를 파일에서 읽지 못함
	}

	fclose(fd);
	return 0;

EFS_END
return -1;

}

INT CSIReportReadBroker::Save(CString filename/*=_T("")*/)
{
EFS_BEGIN

	FILE *fd = _tfopen(BrokerFileName, _T("wb"));
	if (fd == NULL) return -1;		// 파일을 열수 없음

		/*UNCHANGE*/fputc(0xFF, fd);
		/*UNCHANGE*/fputc(0xFE, fd);
		
	_ftprintf(fd, _T("%s\r\n"), PK);
	_ftprintf(fd, _T("%s\r\n"), RPTALIAS);
	_ftprintf(fd, _T("%d\r\n"), MODE);

	fclose(fd);
	return 0;

EFS_END
return -1;

}

INT CSIReportReadBroker::SetData(CString PK, CString RPTALIAS, CString strMODE)
{
EFS_BEGIN

	this->PK = PK;
	this->RPTALIAS = RPTALIAS;
	MODE = _ttoi(strMODE);
	return 0;

EFS_END
return -1;

}

INT CSIReportReadBroker::SetData(CString PK, CString RPTALIAS, INT nMODE)
{
EFS_BEGIN

	this->PK = PK;
	this->RPTALIAS = RPTALIAS;
	MODE = nMODE;
	return 0;

EFS_END
return -1;

}
