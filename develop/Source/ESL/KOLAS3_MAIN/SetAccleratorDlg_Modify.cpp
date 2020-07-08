// SetAccleratorDlg_Modify.cpp : implementation file
//

#include "stdafx.h"
#include "KOLAS3_MAIN.h"
#include "SetAccleratorDlg_Modify.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetAccleratorDlg_Modify dialog


CSetAccleratorDlg_Modify::CSetAccleratorDlg_Modify(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAccleratorDlg_Modify::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetAccleratorDlg_Modify)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strACC = _T("");
    m_pParent = (CSetAcceleratorDlg*)pParent  ;

}


VOID CSetAccleratorDlg_Modify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetAccleratorDlg_Modify)
	DDX_Control(pDX, IDC_cKEY, m_cACC_KEY);
	DDX_Control(pDX, IDC_bSYSKEY, m_cACC_SYSKEY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetAccleratorDlg_Modify, CDialog)
	//{{AFX_MSG_MAP(CSetAccleratorDlg_Modify)
	ON_BN_CLICKED(IDC_bAPPLY, OnbAPPLY)
	ON_BN_CLICKED(IDC_bCANCEL, OnbCANCEL)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bMODIFY, OnbMODIFY)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetAccleratorDlg_Modify message handlers

BOOL CSetAccleratorDlg_Modify::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	m_cACC_SYSKEY.ResetContent();
	m_cACC_SYSKEY.AddString(_T("단축키 없음"));
	m_cACC_SYSKEY.AddString(_T("CTRL"));
	m_cACC_SYSKEY.AddString(_T("ALT"));
	m_cACC_SYSKEY.SetCurSel(0);

	CString str;
	m_cACC_KEY.ResetContent();
	for (INT i = 'A'; i <= 'Z'; i++) {
		str.Format(_T("%c"), i);
		m_cACC_KEY.AddString(str);
	}
	m_cACC_KEY.SetCurSel(0);

	//==============================================================
	//2004-03-23 [이학중 수정 ]
	INT ids ;
	ids = GetCurrentData(0);
	
	//==============================================================
/*
	
	// 단축키 설정
	m_strACC.TrimLeft();			m_strACC.TrimRight();
	m_strACC.MakeUpper();

	CString strSYSKEY, strKEY;
	INT nCutPos = m_strACC.Find(_T("+"), 0);
	if (nCutPos == -1) {
		m_cACC_KEY.SelectString(0, _T("단축키 없음"));
	} else {
		strSYSKEY = m_strACC.Left(nCutPos);
		strSYSKEY.TrimLeft();			strSYSKEY.TrimRight();
		strSYSKEY.MakeUpper();
		strKEY = m_strACC.Mid(nCutPos+1);
		strKEY.TrimLeft();				strKEY.TrimRight();
		strKEY.MakeUpper();

		m_cACC_SYSKEY.SelectString(0, strSYSKEY);
		m_cACC_KEY.SelectString(0, strKEY);
	}
*/

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSetAccleratorDlg_Modify::OnbAPPLY() 
{
	CString strSYSKEY, strKEY;
	m_cACC_SYSKEY.GetWindowText(strSYSKEY);
	strSYSKEY.TrimLeft();		strSYSKEY.TrimRight();		strSYSKEY.MakeUpper();
	m_cACC_KEY.GetWindowText(strKEY);
	strKEY.TrimLeft();		strKEY.TrimRight();		strKEY.MakeUpper();
	if (strSYSKEY.Find(_T("없음")) == -1) {
		m_strACC.Format(_T("%s+%s"), strSYSKEY, strKEY);
	} else {
		m_strACC = _T("");
	}
	
	if (m_strACC == _T("CTRL+C") || m_strACC == _T("CTRL+V") || m_strACC == _T("CTRL+X") || m_strACC == _T("CTRL+Z")) {
		AfxMessageBox(_T("CTRL+C, V, X, Z 는 사용자 정의 단축키로 사용할 수 없습니다."));
		return;
	}

	SetDefaultData();

	CDialog::OnOK();
}

VOID CSetAccleratorDlg_Modify::OnbCANCEL() 
{
	CDialog::OnCancel();	
}

VOID CSetAccleratorDlg_Modify::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	GetCurrentData (1);
}

VOID CSetAccleratorDlg_Modify::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	GetCurrentData (-1);
}

VOID CSetAccleratorDlg_Modify::OnbMODIFY() 
{
	// TODO: Add your control notification handler code here
	CString strSYSKEY, strKEY;
	m_cACC_SYSKEY.GetWindowText(strSYSKEY);
	strSYSKEY.TrimLeft();		strSYSKEY.TrimRight();		strSYSKEY.MakeUpper();
	m_cACC_KEY.GetWindowText(strKEY);
	strKEY.TrimLeft();		strKEY.TrimRight();		strKEY.MakeUpper();
	if (strSYSKEY.Find(_T("없음")) == -1) {
		m_strACC.Format(_T("%s+%s"), strSYSKEY, strKEY);
	} else {
		m_strACC = _T("");
	}
	
	if (m_strACC == _T("CTRL+C") || m_strACC == _T("CTRL+V") || m_strACC == _T("CTRL+X") || m_strACC == _T("CTRL+Z")) {
		AfxMessageBox(_T("CTRL+C, V, X, Z 는 사용자 정의 단축키로 사용할 수 없습니다."));
		return;
	}
   	SetDefaultData(); 	
}

INT CSetAccleratorDlg_Modify::GetCurrentData(INT ndir)
{

   
	if (m_pParent->m_arrRowIdxList.GetSize() == 0) return-1;
	
	INT nRow ;
	
	
	if(ndir == 0)
	{   
		nRow = m_pParent->m_pGrid->GetRow()-1;
		m_nIdx = nRow ;
	}
	if(ndir == -1)
	{
		if(m_nIdx == 0)
		{
			AfxMessageBox(_T("처음 자료 입니다"));
			return -1;
		}
		nRow = m_nIdx-1 ;
		m_nIdx = nRow ;
	}
  
	if(ndir == 1)
	{
		if(m_nIdx == m_pParent->m_arrRowIdxList.GetSize()-1)
		{
			AfxMessageBox(_T("마지막 자료 입니다"));
			return -1;
		}
		nRow = m_nIdx+1 ;
		m_nIdx = nRow ;
	}
 
	
	INT nDataIdx = m_pParent->m_arrRowIdxList.GetAt(nRow);
	POSITION pos = m_pParent->m_pListToolBarItem->FindIndex(nDataIdx);
	if (pos == NULL) return-1;
	m_pToolBarItem = (CToolBarItem *)m_pParent->m_pListToolBarItem->GetAt(pos);;
	if (m_pToolBarItem == NULL) return-1;
	
	
	m_strACC = m_pToolBarItem->m_strAcceclerator;
	
	// 단축키 설정
	m_strACC.TrimLeft();			m_strACC.TrimRight();
	m_strACC.MakeUpper();
	
	CString strSYSKEY, strKEY;
	INT nCutPos = m_strACC.Find(_T("+"), 0);
	if (nCutPos == -1) {
		//2004-03-23 [이학중]
		//m_cACC_KEY.SelectString(0, _T("단축키 없음"));
		m_cACC_SYSKEY.SelectString(0, _T("단축키 없음"));
	} else {
		strSYSKEY = m_strACC.Left(nCutPos);
		strSYSKEY.TrimLeft();			strSYSKEY.TrimRight();
		strSYSKEY.MakeUpper();
		strKEY = m_strACC.Mid(nCutPos+1);
		strKEY.TrimLeft();				strKEY.TrimRight();
		strKEY.MakeUpper();
		
		m_cACC_SYSKEY.SelectString(0, strSYSKEY);
		m_cACC_KEY.SelectString(0, strKEY);
	}

	m_pParent->m_pGrid->SetReverse (nRow);


//	UpdateData(FALSE);

	return 0 ;
}

VOID CSetAccleratorDlg_Modify::SetDefaultData()
{

	m_pToolBarItem->m_strAcceclerator = m_strACC;
	
	CESL_DataMgr dm;
	dm.SetCONNECTION_INFO(m_pParent->CONNECTION_INFO);
	
	CString strQuery;
	//++2009.01.14 UPDATE BY CJY {{++	
	strQuery.Format(
		_T("UPDATE ESL_NEWTOOLBAR_TBL SET ACCECLERATOR='%s' WHERE PK=%s;"), 
		m_strACC, m_pToolBarItem->m_strPK);
//DEL 	strQuery.Format(_T("UPDATE ESL_TOOLBAR_TBL SET ACCECLERATOR='%s' WHERE PK=%s;"), m_strACC, m_pToolBarItem->m_strPK);
	//--2009.01.14 UPDATE BY CJY --}}
	dm.StartFrame();
	dm.AddFrame(strQuery);
	INT ids = dm.SendFrame();
	dm.EndFrame();
	
	if (ids == 0) {
		m_pToolBarItem->m_strAcceclerator = m_strACC;
		m_pParent->m_pGrid->SetTextMatrix(m_nIdx+1, 3, m_pToolBarItem->m_strAcceclerator);
		//AfxMessageBox(_T("수정되었습니다."));
	} else {
		AfxMessageBox(_T("수정작업중 에러가 발생하였습니다."));
	}

}
