// StatusChange.cpp : implementation file
//

#include "stdafx.h"
#include "StatusChange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusChange dialog


CStatusChange::CStatusChange(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusChange)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CStatusChange::~CStatusChange()
{
}


VOID CStatusChange::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusChange)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusChange, CDialog)
	//{{AFX_MSG_MAP(CStatusChange)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusChange message handlers

BOOL CStatusChange::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (InitESL_Mgr(_T("SM_CENTER_UPLOAD_CHANGE")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CStatusChange::OnOK() 
{
	CESL_ControlMgr *pParentCM = FindCM(_T("CM_CENTERUPLOAD_01")) ;
	if(pParentCM == NULL) AfxMessageBox (_T("FindCM() Error~!")) ;

    CESL_Grid *pGrid = (CESL_Grid*)pParentCM->FindControl (_T("MainGrid")) ;

	pGrid->SelectMakeList () ;
	

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_CENTERUPLOAD_01")) ;
	if(pDM == NULL) 
	{
       AfxMessageBox (_T("FindDM() Error")) ;
	   return  ;
	}


	CESL_ControlMgr *pCM = FindCM(_T("CM_CENTER_UPLOAD_CHANGE")) ;
	if(pCM == NULL) AfxMessageBox (_T("FindCM() Error~!")) ;

	CString strStatus;
	pCM->GetControlMgrData( _T("상태"), strStatus );

	INT nIdx = pGrid->SelectGetHeadPosition();
	while ( nIdx > 0 )
	{
		CString strRecKey = pDM->GetCellData( _T("REC_KEY"), nIdx );

		CString sWhere;
		if (  strStatus == _T("안올린자료") ) sWhere = _T("CENTER_UPLOAD_STATUS  = '1' ");
		else if (  strStatus == _T("올린자료")) sWhere = _T("CENTER_UPLOAD_STATUS = '0' ");
		else if (  strStatus == _T("안올린자료(확인요망)")) sWhere = _T("CENTER_UPLOAD_STATUS = '2' ");
		else if (  strStatus == _T("안올린자료(MARC에러)") ) sWhere = _T("CENTER_UPLOAD_STATUS = '3' ");
		else if (  strStatus == _T("안올린자료(기타)")) sWhere = _T("CENTER_UPLOAD_STATUS = '4' ");
		else if (  strStatus == _T("임시 DB로 올린자료")) sWhere = _T("CENTER_UPLOAD_STATUS = '5' ");
		else if (  strStatus == _T("반출 후 국중에 제공한 자료")) sWhere = _T("CENTER_UPLOAD_STATUS = '6' ");

		pDM->SetCellData(_T("UPLOAD상태"), strStatus , nIdx );

		CString strQuery;
		strQuery.Format( _T("UPDATE BO_SPECIES_TBL SET %s WHERE REC_KEY = %s; ") , sWhere, strRecKey );

		INT ids = pDM->ExecuteQuery( strQuery, 1 );
		if ( ids < 0 ) 
		{
			AfxMessageBox ( _T("수행중 오류가 났습니다.")) ;
			return;
		}

		pGrid->DisplayLine(nIdx);
		nIdx = pGrid->SelectGetNext();
	}


	AfxMessageBox ( _T("정상적으로 일괄 변경 하였습니다.")) ;
	
	CDialog::OnOK();
	return;
}

HBRUSH CStatusChange::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;

}
