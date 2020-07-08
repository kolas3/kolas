// BO_CAT_PROC_0325.cpp : implementation file
//

#include "stdafx.h"
//#include _T("bo_cat_proc.h")
#include "BO_CAT_PROC_0325.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0325 dialog


CBO_CAT_PROC_0325::CBO_CAT_PROC_0325(CESL_Mgr* pParent, CString sDm, CMarc &m_marc /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//pDM = FindDM(_T("DM_BO_CAT_PROC_0000"));
	m_pInfo->pMarcMgr->Encoding(&m_marc, strStreamMarc);

	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
			UpdateData(FALSE);
		}	
	}

	//{{AFX_DATA_INIT(CBO_CAT_PROC_0325)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBO_CAT_PROC_0325::~CBO_CAT_PROC_0325()
{
}

VOID CBO_CAT_PROC_0325::DoDataExchange(CDataExchange* pDX)
{
EFS_BEGIN

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBO_CAT_PROC_0325)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP

EFS_END
}


BEGIN_MESSAGE_MAP(CBO_CAT_PROC_0325, CDialog)
	//{{AFX_MSG_MAP(CBO_CAT_PROC_0325)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBO_CAT_PROC_0325 message handlers

BOOL CBO_CAT_PROC_0325::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
EFS_BEGIN

	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::Create(IDD, pParentWnd);

EFS_END
return FALSE;

}
 
BOOL CBO_CAT_PROC_0325::OnInitDialog() 
{
EFS_BEGIN

	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("BO_CAT_PROC_0325")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	//decoding
	CString strTagCode, strErrMsg, strHelpMsg;
	
	if(m_pInfo->pMarcMgr->Decoding(strStreamMarc, &m_marc))
	{
		POSITION pos;
		pos = m_pInfo->pMarcMgr->GetHeadPositionErrMsg();
		while (pos)
		{
			m_pInfo->pMarcMgr->GetNextErrMsg(strTagCode, strErrMsg, strHelpMsg, pos);
//			m_strOutput += ( strErrMsg + strHelpMsg + _T("\r\n"));
			UpdateData(FALSE);
		}	
	}

	CString sItemData;		
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@0-5"), sItemData);

	INT iIntCh = 0;
	if ( sItemData != _T("") ) {
		for ( INT i = 0 ; i < sItemData.GetLength() ; i ++ ) {
			CString sCh = sItemData.Mid( i, 1 );
			//TCHAR ch = sCh.GetBuffer(0);
			TCHAR ch = sCh.GetAt(0);
			if(!((ch >= '0')&&(ch <= '9'))) {			
				iIntCh = 1;
				break;
			}
		}
		if ( iIntCh == 0 ) {
			SetControlData(_T("CM_BO_CAT_PROC_0325"), _T("입력일자"), sItemData);
		}
		else{
			SetControlData(_T("CM_BO_CAT_PROC_0325"), _T("입력일자"), _T(""));
		}
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0325"), _T("날짜") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0325"), _T("입력일자") , sDate );
	}
	/*
	CString sItemData;
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@0-5"), sItemData);	
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("입력날짜"), sItemData);
	}
	else {
		//sItemData = _T("");
		GetControlData( _T("CM_BO_CAT_PROC_0321"), _T("날짜") , sItemData );
		CString sDate;
		sDate.Format( _T("%s%s%s"), sItemData.Mid(2,2), sItemData.Mid(5,2), sItemData.Right(2) );
		SetControlData( _T("CM_BO_CAT_PROC_0321"), _T("입력날짜") , sDate );
	}
*/
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("유형"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("발행년유형"), sItemData);
	}

	sItemData = _T("");
	/*
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("발행년1"), sItemData);
	}
	*/
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("발행년"), sItemData);
	if ( sItemData != _T("") ) {
		for ( INT i = 0 ; i < sItemData.GetLength() ; i ++ ) {
			CString sCh = sItemData.Mid( i, 1 );
			//TCHAR ch = sCh.GetBuffer(0);
			TCHAR ch = sCh.GetAt(0);
			if(!((ch >= '0')&&(ch <= '9'))) {			
				iIntCh = 1;
				break;
			}
		}
		if ( iIntCh == 0 ) {
			SetControlData(_T("CM_BO_CAT_PROC_0325"), _T("발행년1"), sItemData);
		}
		else{
			SetControlData(_T("CM_BO_CAT_PROC_0325"), _T("발행년1"), _T(""));
		}
		//SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("발행년1"), sItemData);
	}
	else {
		CTime time = CTime::GetCurrentTime();
		m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@7-10"), sItemData);
		//m_pInfo->pMarcMgr->GetItem(&m_marc, _T("발행년"), sItemData);
		if (sItemData.IsEmpty())
			sItemData = time.Format(_T("%Y"));
		SetControlData(_T("CM_BO_CAT_PROC_0320_01"), _T("발행년1"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@11-14"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("발행년2"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@15-17"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("발행국부호"), sItemData);
	}

	/*
	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("한국대학부호"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("한국대학부호"), sItemData);
	}
	*/

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@26-27"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("한국대학부호"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@28"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("수정"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@32"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("목록전거"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@35-37"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("언어구분"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@38-39"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("정부기관부호"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@22"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("이용수준"), sItemData);
	}

	sItemData = _T("");
	m_pInfo->pMarcMgr->GetItem(&m_marc, _T("008@25"), sItemData);
	if ( sItemData != _T("") ) {
		SetControlData(_T("CM_BO_CAT_PROC_0321"), _T("컴퓨터파일유형"), sItemData);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE

EFS_END
return FALSE;

}

HBRUSH CBO_CAT_PROC_0325::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	return hbr; 
}