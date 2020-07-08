// ShelfSearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SHELF_LOC_REGISTER.h"
#include "ShelfSearchDlg.h"
#include "SearchTabDlg.h"
#include "..\..\단행\열람\장서관리\\Loc_CommonAPI\LocCommonAPI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShelfSearchDlg dialog


CShelfSearchDlg::CShelfSearchDlg(CESL_Mgr* pParent /*=NULL*/)
: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CShelfSearchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	this->pParent = pParent;
}

CShelfSearchDlg::~CShelfSearchDlg()
{
	
}

void CShelfSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShelfSearchDlg)
	DDX_Control(pDX, IDC_STATIC_MC, m_ctlManageCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CShelfSearchDlg, CDialog)
	//{{AFX_MSG_MAP(CShelfSearchDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_btnSearch, OnbtnSearch)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShelfSearchDlg message handlers

HBRUSH CShelfSearchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CShelfSearchDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();	
	
	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CShelfSearchDlg::OnbtnSearch() 
{
 	INT ids;

	CString		strTitle;			// 서명
	CString		strAuthor;			// 저자	
	CString		strPublisher;		// 발행자
	CString		strShelfNum;		// 서가번호

	( (CEdit*) GetDlgItem (IDC_edtTITLE) )->GetWindowText(strTitle);
	( (CEdit*) GetDlgItem (IDC_edtAUTHOR) )->GetWindowText(strAuthor);
	( (CEdit*) GetDlgItem (IDC_edtPUBLISHER) )->GetWindowText(strPublisher);
	( (CEdit*) GetDlgItem (IDC_edtSHELF_NUM) )->GetWindowText(strShelfNum);

	if(strTitle.IsEmpty() && strAuthor.IsEmpty() && strPublisher.IsEmpty() && strShelfNum.IsEmpty())
	{
		AfxMessageBox(_T("검색 정보가 부족합니다."));
		return;
	}


 	ids  = DoSearch();

	if(ids==5) 
	{
		AfxMessageBox(_T("검색 결과가 없습니다."));
		return ;
	}

 	if ( ids ) 
 	{
 		AfxMessageBox(_T("검색중 오류가 발생했습니다."));
 	}
	
	CSearchTabDlg searchTabDlg(this);
	if(IDOK == searchTabDlg.DoModal())
	{
	}
}

INT CShelfSearchDlg::DoSearch()
{
	CESL_DataMgr *pDM_BO = FindDM(_T("DM_SHELF_LOC_REG_BO"));
	CESL_DataMgr *pDM_SE = FindDM(_T("DM_SHELF_LOC_REG_SE"));


	if ( pDM_BO == NULL || pDM_SE ==NULL ) return -100;

	CString		strCondition;
	CString		strTitle;			// 서명
	CString		strAuthor;			// 저자	
	CString		strPublisher;		// 발행자
	CString		strShelfNum;		// 서가번호
	CString		strShelfRow;		// 서가행번호
	CString		strOption;			
	INT			ids=-1;
	INT			ids1=-1;

	// validation check
	
	// query make
	strCondition = _T("");
	( (CEdit*) GetDlgItem (IDC_edtTITLE) )->GetWindowText(strTitle);
	if ( !strTitle.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, BO_IDX_TITLE, '%s*') "), strTitle);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtAUTHOR) )->GetWindowText(strAuthor);
	if ( !strAuthor.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, BO_IDX_AUTHOR, '%s*') "), strAuthor);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtPUBLISHER) )->GetWindowText(strPublisher);
	if ( !strPublisher.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, BO_IDX_PUBLISHER, '%s*') "), strPublisher);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtSHELF_NUM) )->GetWindowText(strShelfNum);
	if ( !strShelfNum.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("SHELF_NUM='%s'"), strShelfNum);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtSHELF_ROW) )->GetWindowText(strShelfRow);
	if ( !strShelfRow.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("SHELF_ROW='%s'"), strShelfRow);		
		strCondition += strOption;
	}
	strOption.Format(_T("AND IB.MANAGE_CODE=UDF_MANAGE_CODE"), strTitle);		
	strCondition += strOption;

	// search
	ids  = pDM_BO->RefreshDataManager(strCondition);
	CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_BO"));


	strCondition = _T("");
	strOption = _T("");
	( (CEdit*) GetDlgItem (IDC_edtTITLE) )->GetWindowText(strTitle);
	if ( !strTitle.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, SE_IDX_TITLE, '%s*') "), strTitle);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtAUTHOR) )->GetWindowText(strAuthor);
	if ( !strAuthor.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, SE_IDX_AUTHOR, '%s*') "), strAuthor);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtPUBLISHER) )->GetWindowText(strPublisher);
	if ( !strPublisher.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("&ECOSEARCH(IB.REC_KEY, SE_IDX_PUBLISHER, '%s*') "), strPublisher);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtSHELF_NUM) )->GetWindowText(strShelfNum);
	if ( !strShelfNum.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("SHELF_NUM='%s'"), strShelfNum);		
		strCondition += strOption;
	}
	( (CEdit*) GetDlgItem (IDC_edtSHELF_ROW) )->GetWindowText(strShelfRow);
	if ( !strShelfRow.IsEmpty() )  
	{
		if ( !strCondition.IsEmpty() ) strCondition += _T(" AND ");
		strOption.Format(_T("SHELF_ROW='%s'"), strShelfRow);		
		strCondition += strOption;
	}
	strOption.Format(_T("AND IB.MANAGE_CODE=UDF_MANAGE_CODE"), strTitle);		
	strCondition += strOption;

	ids1  = pDM_SE->RefreshDataManager(strCondition);
	CLocCommonAPI::MakeShelfCode(this,_T("DM_SHELF_LOC_REG_SE"));

	if( pDM_SE->GetRowCount()==0 && pDM_BO->GetRowCount()==0 ) return 5;

	if ( ids <0 || ids1 <0) return ids;

	return 0;
}

BOOL CShelfSearchDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_KEYDOWN )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			OnbtnSearch();
			return TRUE;
		}
	}
		
	return CDialog::PreTranslateMessage(pMsg);
}

void CShelfSearchDlg::SetManageCode()
{
	m_ctlManageCode.SetWindowText(this->m_strManageCode);
}

void CShelfSearchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
//	((CSHELF_LOC_REGISTERDlg*)pParent)->DlgQuit();
//	CDialog::OnCancel();
}
