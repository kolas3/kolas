// BasicPrintMgr.cpp: implementation of the CBasicPrintMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicPrintMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 생성자
CBasicPrintMgr::CBasicPrintMgr(CESL_Mgr * pParentMgr)
{
	m_pParentMgr = pParentMgr;
	m_nPrintType = -1;

	m_MapPrintData.RemoveAll();	
	
	m_pPrintDM = NULL;
	m_pSIReportManager = new CSIReportManager(pParentMgr);
}
// 소멸자
CBasicPrintMgr::~CBasicPrintMgr()
{
	if( m_pSIReportManager )
	{
		delete m_pSIReportManager;
		m_pSIReportManager = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 멤버변수에 매핑된 데이터를 모두 지운다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
VOID CBasicPrintMgr::InitPrintData()
{
	CString strKey, strValue;
	POSITION pos = m_MapPrintData.GetStartPosition();
	while( pos != NULL )
	{
		m_MapPrintData.GetNextAssoc( pos, strKey, strValue);
		m_MapPrintData.RemoveKey(strKey);
	}

	m_MapPrintData.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - SIReport클래스를 설정하고 출력용DM을 이용하여 출력용 페이지를 만든다.
// [ 미니스펙 ]
// 1. 출력하기 위한 구성요소를 체크한다.(SIReport클래스, 출력용DM, 접속정보)
// 2. 멤버변수로 매핑되어있는 값을 SIReport클래스에 설정한다.
// 3. SIReport클래스에 접속정보를 설정한다.
// 4. DB로부터 해당 파일명의 SIReport정보를 가져온다.
// 5. 출력용 DM을 SIReport클래스에 설정한다.
// 6. 출력용 페이지를 만든다.
//
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::MakeXMLPageFile()
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids;
	CString strKey, strValue;
	POSITION pos;

	//================================================================
	// 1. 출력하기 위한 구성요소를 체크한다.(SIReport클래스, 출력용DM, 접속정보)
	//================================================================
	if( CheckValidation() == FALSE )	return -1;	
	
	//================================================================
	// 2. 멤버변수로 매핑되어있는 값을 SIReport클래스에 설정한다.
	//================================================================
	pos = m_MapPrintData.GetStartPosition();
	while( pos != NULL )
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	//================================================================
	// 3. SIReport클래스에 접속정보를 설정한다.
	//================================================================
	m_pSIReportManager->SetCONNECTION_INFO(m_strConnectionInfo);

	//================================================================
	// 4. DB로부터 해당 파일명의 SIReport정보를 가져온다.
	//================================================================
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if( ids < 0 )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return 0;
	}
	
	//m_pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	
	//================================================================
	// 5. 출력용 DM을 SIReport클래스에 설정한다.
	//================================================================
	m_pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);	

	//================================================================
	// 6. 출력용 페이지를 만든다.
	//================================================================
	m_pSIReportManager->MAKEPAGE_DrawWorkForm(-1);

	return 0;
}

INT CBasicPrintMgr::Print2()
{
	return m_pSIReportManager->Print2();
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 임시로 저장된 rpt파일들을 모두 지운다.
// [ 미니스펙 ] - 생략
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::InitRptFile()
{
	INT i;
	CString strTmp;
	CFile fd;
	
	for( i=1; i<99999; i++ )
	{
		strTmp.Format(_T("rpt_tmp\\%s%04d.rpt"), m_pSIReportManager->m_strPageHeader, i);
		if( fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false )		break;

		fd.Close();
		DeleteFile(strTmp);
	}

	for( i=1; i<99999; i++ )
	{
		strTmp.Format(_T("rpt_tmp\\%s%04d.xml"), m_pSIReportManager->m_strPageHeader, i);
		if( fd.Open(strTmp, CFile::modeRead | CFile::typeBinary) == false )		break;

		fd.Close();
		DeleteFile(strTmp);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - SIReport클래스를 설정하여 출력한다.
// [ 미니스펙 ]
// 1. 출력하기 위한 구성요소를 체크한다.(SIReport클래스, 출력용DM, 접속정보)
// 2. 멤버변수로 매핑되어있는 값을 SIReport클래스에 설정한다.
// 3. SIReport클래스에 접속정보를 설정한다.
// 4. DB로부터 해당 파일명의 SIReport정보를 가져온다.
// 5. 출력용 DM을 SIReport클래스에 설정한다.
// 6. 출력타입이 연속용지출력이라면 SIReport클래스에 해당내용을 설정해야한다.
// 7. SIReport클레스에 구성된 내용으로 출력한다.
//
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::Print()
{
	//================================================================
	// 변수정의
	//================================================================
	INT ids;	
	CString strKey, strValue;
	POSITION pos;
	
	//================================================================
	// 1. 출력하기 위한 구성요소를 체크한다.(SIReport클래스, 출력용DM, 접속정보)
	//================================================================
	if( CheckValidation() == FALSE )	return -1;
	
	//================================================================
	// 2. 멤버변수로 매핑되어있는 값을 SIReport클래스에 설정한다.
	//================================================================
	pos = m_MapPrintData.GetStartPosition();
	while(pos != NULL)
	{
		m_MapPrintData.GetNextAssoc(pos, strKey, strValue);
		m_pSIReportManager->SetTempVariable(strKey, strValue);
	}
	
	//================================================================
	// 3. SIReport클래스에 접속정보를 설정한다.
	//================================================================
	m_pSIReportManager->SetCONNECTION_INFO(m_strConnectionInfo);

	//================================================================
	// 4. DB로부터 해당 파일명의 SIReport정보를 가져온다.
	//================================================================
	ids = m_pSIReportManager->GetSIReportFileFromDB(m_strSIReportFileName);
	if (ids < 0)
	{
		m_pParentMgr->ESLAfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return -1;
	}
	
	//================================================================
	// 5. 출력용 DM을 SIReport클래스에 설정한다.
	//================================================================
	m_pSIReportManager->SetDataMgr(0, m_pPrintDM, 0);
	

	//================================================================
	// 6. 출력타입이 연속용지출력이라면 SIReport클래스에 해당내용을 설정해야한다.
	//================================================================
	if( m_nPrintType == SERIES_PAPER )
	{
		m_pSIReportManager->m_nUserPaperSize = -9999;
		m_pSIReportManager->m_nOrientation = DMORIENT_PORTRAIT;
		m_pSIReportManager->m_nResolution = 1728; 
	}
	
	//================================================================
	// 7. SIReport클레스에 구성된 내용으로 출력한다.
	//================================================================

	ids = m_pSIReportManager->Print();
	
	return ids;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 출력하기 위한 구성요소를 체크한다.(SIReport클래스, 출력용DM, 접속정보)
// [ 미니스펙 ]
// 1. SIReport관련 클래스가 설정되지 않았다면 에러리턴한다.
// 2. 출력용 DM이 설정되지 않았다면 에러리턴한다.
// 3. 접속정보가 설정되지 않았다면 에러리턴한다.
// 4. 출력용 DM에 데이터가 없다면 에러리턴한다.
//
//////////////////////////////////////////////////////////////////////
BOOL CBasicPrintMgr::CheckValidation()
{
	//================================================================	
	// 1. SIReport관련 클래스가 설정되지 않았다면 에러리턴한다.
	//================================================================
	if( m_pSIReportManager == NULL )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("Report Manager를 생성하지 못하였습니다."));
		return FALSE;
	}

	//================================================================	
	// 2. 출력용 DM이 설정되지 않았다면 에러리턴한다.
	//================================================================
	if( m_pPrintDM == NULL )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("출력용 DM을 설정하지 않았습니다."));
		return FALSE;
	}

	//================================================================	
	// 3. 접속정보가 설정되지 않았다면 에러리턴한다.
	//================================================================
	if( m_strConnectionInfo.IsEmpty() == TRUE )
	{
		// 출력용 DM에도 접속정보가 없다면 에러리턴한다.
		m_strConnectionInfo = m_pPrintDM->CONNECTION_INFO;
		if( m_strConnectionInfo.IsEmpty() == TRUE ) 
		{
			m_pParentMgr->ESLAfxMessageBox(_T("DB연결 정보를 설정하지 않았습니다."));
			return FALSE;
		}
	}

	//================================================================	
	// 4. 출력용 DM에 데이터가 없다면 에러리턴한다.
	//================================================================
	if( m_pPrintDM->GetRowCount() == 0 )
	{
		m_pParentMgr->ESLAfxMessageBox(_T("출력할 데이터가 없습니다."));
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 온 키와 값을 멤버변수에 매핑한다.
//////////////////////////////////////////////////////////////////////
INT CBasicPrintMgr::AddPrintData(CString strKey, CString strValue)
{
	if( strKey.IsEmpty() == TRUE )		return -1;
	
	m_MapPrintData.SetAt(strKey, strValue);

	return 0;
}

//////////////////////////////////////////////////////////////////////
// [ 함수설명 ]
// - 인자로 온 키값에 해당하는 값을 매핑된 변수에서 찾아 리턴한다.
//////////////////////////////////////////////////////////////////////
CString CBasicPrintMgr::GetPrintData(CString strKey)
{
	if( strKey.IsEmpty() == TRUE )		return _T("");
	
	CString strValue;
	m_MapPrintData.Lookup(strKey, strValue);

	return strValue;
}

VOID CBasicPrintMgr::DeleteTempPages(VOID)
{
	m_pSIReportManager->DeleteTempPageFiles();
}

//2019.07.17 JOB.2019.0059 ADD BY KYJ : 로그 기록시 사유 추가
VOID CBasicPrintMgr::SetPrivacyPrintLogParm(BOOL bFlag, CString strKey, CString strID, CString strTitle, CString strManageCode, CString strLogMsg, INT nAccessLevel)
{
	m_pSIReportManager->SetPrivacyPrintLogParm(bFlag,strKey,strID,strTitle,strManageCode,strLogMsg,nAccessLevel);
}