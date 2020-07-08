// SE_preGROUP_MNG.cpp : implementation file
//

#include "stdafx.h"
#include "SE_preGROUP_MNG.h"
#include "SE_CAT_MAKE_preGROUP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_preGROUP_MNG dialog


CSE_preGROUP_MNG::CSE_preGROUP_MNG(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_preGROUP_MNG)
	m_Year = _T("");
	//}}AFX_DATA_INIT
}
CSE_preGROUP_MNG::~CSE_preGROUP_MNG()
{
}

BOOL CSE_preGROUP_MNG::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_preGROUP_MNG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_preGROUP_MNG)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_Year);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_preGROUP_MNG, CDialog)
	//{{AFX_MSG_MAP(CSE_preGROUP_MNG)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_NewGroup, OnNewGroup)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_preGROUP_MNG message handlers

BOOL CSE_preGROUP_MNG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_楷加_沥府_啊盔何包府")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}
		
	DisplayData();	

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_preGROUP_MNG::DisplayData()
{
	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_楷加_沥府_啊盔何包府"));

	// 040304_HSS 荐沥 LocalTime栏肺 Select +++++++++++++++
	//CTime mTime = pDM->GetDBTime();
	CTime mTime = CTime::GetCurrentTime();
	// ++++++++++++++++++++++++++++++++++++++++++++++++++++
	m_Year.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return ;

}

VOID CSE_preGROUP_MNG::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;

	CESL_DataMgr *pDM;
	pDM = FindDM(_T("DM_楷加_沥府_啊盔何包府"));

	UpdateData(TRUE);

	m_Year.TrimLeft();
	m_Year.TrimRight();
	
	if( m_Year != _T("") )
	{
		tmpQry.Format(_T(" ACCESSION_REC_MAKE_YEAR = %s "), m_Year);
		strQry = strQry + tmpQry;
	}	

	pDM->RefreshDataManager(strQry);

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_楷加_沥府_啊盔何包府"));

	pCM->AllControlDisplay();
	
}

VOID CSE_preGROUP_MNG::OnNewGroup() 
{
	// TODO: Add your control notification handler code here
	CSE_CAT_MAKE_preGROUP Dlg(this);
	Dlg.DoModal();

	DisplayData();
	
}

BEGIN_EVENTSINK_MAP(CSE_preGROUP_MNG, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_preGROUP_MNG)
	ON_EVENT(CSE_preGROUP_MNG, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_preGROUP_MNG::OnClickGroupgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid=( CESL_Grid * )FindControl(_T("CM_楷加_沥府_啊盔何包府"), _T("GRID"));;

	INT row = pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		pGrid->SetReverse(row-1);
		m_GROUP_NO = pGrid->GetAt(row-1,0);
		m_GROUP_REC_KEY = pGrid->GetAt(row-1,4);
	}		
}

HBRUSH CSE_preGROUP_MNG::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}
