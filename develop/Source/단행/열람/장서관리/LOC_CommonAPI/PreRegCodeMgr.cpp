// PreRegCodeMgr.cpp : implementation file
//

#include "stdafx.h"
#include "PreRegCodeMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreRegCodeMgr dialog


CPreRegCodeMgr::CPreRegCodeMgr(CESL_Mgr* pParent /*=NULL*/, INT nMode /*=0*/, CString strManageCode/*=_T("")*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreRegCodeMgr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nMode = nMode;
	m_pParent = pParent;

	CString strFunc = _T("CPreRegCodeMgr");
	INT ids = InitESL_Mgr(_T("SM_PRE_REG_CODE_MGR"));
	if(0 > ids){ERROR_MSG_RETURN_VOID2(-1000, strFunc);}

	CString strDM = _T("DM_MNG_PRE_REG_NO");
	m_pDM = FindDM(strDM);
	if(NULL == m_pDM){ERROR_MSG_RETURN_VOID2(-1000, strFunc);}

	CString strMngQuery;
	if(0 == m_nMode)
	{
		CKolas2up_MobileApi MobileApi;
		MobileApi.SetMode(m_pInfo->MODE);
		MobileApi.SetDM(m_pDM);
		ids = MobileApi.RefreshDataManager(_T(""));
		if(0 > ids){ERROR_MSG_RETURN_VOID2(-1000, strFunc);}

		CESL_DataMgr TmpDM;
		TmpDM.SetCONNECTION_INFO(_T(""));
		strMngQuery = 
		_T("SELECT VALUE_1 FROM ESL_MANAGE_TBL ")
		_T("WHERE GROUP_2='열람' AND GROUP_3='공통' AND CLASS_ALIAS='등록구분' ")
		_T("AND VALUE_NAME='도서관별 등록구분 중복여부' AND MANAGE_CODE=UDF_MANAGE_CODE");
		TmpDM.GetOneValueQuery(strMngQuery, m_sIsPreRegCodeFirst);
	}
	else
	{

	}

	if(m_sIsPreRegCodeFirst.IsEmpty())
	{
		m_sIsPreRegCodeFirst = _T("N");
	}
}

CPreRegCodeMgr::~CPreRegCodeMgr()
{

}
VOID CPreRegCodeMgr::DoDataExchange(CDataExchange* pDX)
{
	EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreRegCodeMgr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

	EFS_END
}


BEGIN_MESSAGE_MAP(CPreRegCodeMgr, CDialog)
	//{{AFX_MSG_MAP(CPreRegCodeMgr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

INT CPreRegCodeMgr::IsInPreRegNo( CString sRegNo , CString sManageCode, CString &sChangeRegNo )
{
	EFS_BEGIN

	INT ids;

	sRegNo.TrimLeft(); sRegNo.TrimRight();
	sRegNo.MakeUpper();

	sRegNo.Replace( _T(" ") , _T("") );
	sChangeRegNo = sRegNo;

	INT nRegCnt = m_pDM->GetRowCount();

	CString sRegManageCode;
	CString sPreRegNoLength;
	CString sPreRegCode;
	CString sNewRegCode;
	INT nPreRegCodeLength;

	CString sChangeRegCode;
	CString sChangeRegNoNotInCode;

	bool bIsEqual=FALSE;

	CString sInputRegCode , sOutputRegCode;

	INT nRegCodeLength;
	nRegCodeLength = m_pParent->m_pInfo->nREG_CODE_LENGTH;
	
	if(_T("N") == m_sIsPreRegCodeFirst)
	{
		if( sRegNo.GetLength() > nRegCodeLength )
		{
			sInputRegCode.Empty(); sOutputRegCode.Empty();
			sInputRegCode = sRegNo.Mid(0,nRegCodeLength);
			m_pParent->m_pInfo->pCodeMgr->ConvertCodeToDesc( _T("등록구분") , sInputRegCode , sOutputRegCode );
			if( !sOutputRegCode.IsEmpty() ) return 0;
		}
	}
	
	for( INT i = 0 ; i < nRegCnt ; i++ )
	{
		ids = m_pDM->GetCellData( _T("이전등록구분") , i , sPreRegCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsInPreRegNo") );
		ids = m_pDM->GetCellData( _T("자리수") , i , sPreRegNoLength );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsInPreRegNo") );
		ids = m_pDM->GetCellData( _T("현재등록구분") , i , sNewRegCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsInPreRegNo") );
		ids = m_pDM->GetCellData( _T("관리구분") , i , sRegManageCode );
		if( ids < 0 ) ERROR_MSG_RETURN_ERROR_CODE2( -1 , _T("IsInPreRegNo") );	

		if(sManageCode != sRegManageCode)
		{
			continue;
		}

		if( sRegNo.GetLength() == _ttoi(sPreRegNoLength) )
		{
			nPreRegCodeLength = sPreRegCode.GetLength();

			if( sRegNo.GetLength() < nPreRegCodeLength ) continue;			

			sChangeRegNoNotInCode = sRegNo.Mid( nPreRegCodeLength );
			
			if( nPreRegCodeLength > 0 )
			{
				sChangeRegCode = sRegNo.Mid(0,nPreRegCodeLength);				
				if( sChangeRegCode.Compare(sPreRegCode) != 0 )
					continue;
			}
			else
			{
				if( sRegNo.GetAt(0) < '0' ||  (sRegNo.GetAt(0) > '9') )
					continue;
			}

			sChangeRegNo = sNewRegCode+sChangeRegNoNotInCode;
			bIsEqual = TRUE;
		}
	
	}
	if( bIsEqual ) return 1;

	return 0;

	EFS_END
	return -1;
}
INT CPreRegCodeMgr::IsInPreRegNo( CString sRegNo , CString &sChangeRegNo )
{
	DWORD dwPID = ::GetCurrentProcessId();
	CString strCHMfileName;
  	strCHMfileName.Format(_T("..\\CFG\\EFS\\user_info(%u).log"), dwPID);
	
	CStdioFile fd;
	if (TRUE == fd.Open(strCHMfileName, CFile::modeRead|CFile::typeBinary)) 
	{
		TCHAR cUni;
		fd.Read( &cUni, sizeof(TCHAR) );
		if( 0xFEFF != cUni )
		{
			fd.SeekToBegin();
		}
		CString strLine;
		fd.ReadString(strLine);
		strLine.TrimLeft();			strLine.TrimRight();
		if (strLine.GetLength() > 0)
		{
			strCHMfileName = strLine;
		}
		fd.Close();

		CString strManageCode=strLine;
		return IsInPreRegNo(sRegNo, strManageCode, sChangeRegNo);
	}	

	return -1000;
}
