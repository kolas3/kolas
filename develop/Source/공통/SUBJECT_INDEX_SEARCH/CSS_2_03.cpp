// CSS_2_03.cpp : implementation file
//

#include "stdafx.h"
#include "CSS_2_03.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_03 dialog
// 2005.01.25 확장개선 사업에서는 요청은 제외
/////////////////////////////////////////////////////////////////////////////

CCSS_2_03::CCSS_2_03(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CCSS_2_03)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CCSS_2_03::~CCSS_2_03()
{
	
}


VOID CCSS_2_03::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSS_2_03)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCSS_2_03, CDialog)
	//{{AFX_MSG_MAP(CCSS_2_03)
	ON_BN_CLICKED(IDC_BTN_SEARCH, OnBtnSearch)
	ON_BN_CLICKED(IDC_BTN_REQUEST, OnBtnRequest)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSS_2_03 message handlers

VOID CCSS_2_03::OnBtnSearch() 
{

	/*
	//검색 
	EDBM_DATA_ARRAY_TYPE RequestOut;
	CString sSQL;
    CString sTerm;
    //===========================================================================
    // 1. SQL구성
    //===========================================================================
    // Table: TERMREQUEST
    // Output: RequestOut
    // [00]REQUESTTERM   :
    // [01]REQUESTTYPE   :
    // [02]STATUS        :
    GetDlgItemText(IDC_EDT_TERM, sTerm);
    sSQL.Format(_T("select REQUESTTERM,REQUESTTYPE,STATUS ")
                _T("from TERMREQUEST ")
                _T("where REQUESTID is not null "));

    if(sTerm.GetLength()) sSQL += _T("and REQUESTTERM LIKE '") + sTerm + _T("%' ");

    //===========================================================================
    // 2. SQL실행
    //===========================================================================
    INT ids;
	ids = GetQueryData(_T("thesaurus"), _T("eco"), _T("eco"), sSQL.GetBuffer(0), 1, -1, &RequestOut);
    if(RequestOut.nRowCount == 0) {
        AfxMessageBox(_T("검색결과가 없습니다."));
        return;
    }

    //===========================================================================
    // 3. SQL 실행결과 디스플에이
    //===========================================================================
    CF1Book *pVcf = (CF1Book*)GetDlgItem(IDC_VCF_REQUEST);
    INT i, j;
    pVcf->SetMaxRow(RequestOut.nRowCount);
    for(i = 0; i < RequestOut.nRowCount; i++) {
        for(j = 0; j < RequestOut.nColumnCount; j++)
            pVcf->SetTextRC(i + 1, j + 1, RequestOut.ColumnData[j].db_data[i].stringData);
    }
    pVcf->SetActiveCell(1, 1);
    pVcf->ShowActiveCell();
	*/

	
}

VOID CCSS_2_03::OnBtnRequest() 
{
	//요청 
	
}

BOOL CCSS_2_03::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetDlgItemText(IDC_EDT_TERM, m_sTerm);

	EnableThemeDialogTexture(GetSafeHwnd());
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CCSS_2_03::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
