// SE_CAT_GROUP_INSERT.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_GROUP_INSERT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GROUP_INSERT dialog


CSE_CAT_GROUP_INSERT::CSE_CAT_GROUP_INSERT(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_GROUP_INSERT)
	m_strYear = _T("");
	//}}AFX_DATA_INIT
}

CSE_CAT_GROUP_INSERT::~CSE_CAT_GROUP_INSERT()
{
}

BOOL CSE_CAT_GROUP_INSERT::Create(CWnd* pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

VOID CSE_CAT_GROUP_INSERT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_GROUP_INSERT)
	DDX_Text(pDX, IDC_GROUP_YEAR, m_strYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_GROUP_INSERT, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_GROUP_INSERT)
	ON_BN_CLICKED(IDC_GROUP_SEARCH, OnGroupSearch)
	ON_BN_CLICKED(IDC_bSaveNo, OnbSaveNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_GROUP_INSERT message handlers

BOOL CSE_CAT_GROUP_INSERT::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	if (InitESL_Mgr(_T("SM_단행_정리_목록_원부")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}


		
	DisplayData();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CSE_CAT_GROUP_INSERT::DisplayData()
{
	CESL_DataMgr *pDM;

	pDM = FindDM(_T("DM_단행_정리_목록_원부"));

	CTime mTime = pDM->GetDBTime();
	m_strYear.Format(_T("%d"), mTime.GetYear());

	UpdateData(FALSE);

	OnGroupSearch();

	return 0;
}

BEGIN_EVENTSINK_MAP(CSE_CAT_GROUP_INSERT, CDialog)
    //{{AFX_EVENTSINK_MAP(CSE_CAT_GROUP_INSERT)
	ON_EVENT(CSE_CAT_GROUP_INSERT, IDC_GROUPGRID, -600 /* Click */, OnClickGroupgrid, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

VOID CSE_CAT_GROUP_INSERT::OnClickGroupgrid() 
{
	// TODO: Add your control notification handler code here
	CESL_Grid *pGrid;

	pGrid=( CESL_Grid * )FindControl(_T("CM_단행_정리_목록_원부"), _T("GRID"));
		
		
	INT row = pGrid->GetRow();
	
	if (row == 0) return;
	if (row > 0) {
		pGrid->SetReverse(row-1);
		m_GROUP_NO = pGrid->GetAt(row-1,0);
		m_GROUP_REC_KEY = pGrid->GetAt(row-1,4);
	}
	
}

VOID CSE_CAT_GROUP_INSERT::OnGroupSearch() 
{
	// TODO: Add your control notification handler code here
	CString strQry,tmpQry;
	INT ids;

	CESL_DataMgr *pDM;

	pDM = FindDM(_T("DM_단행_정리_목록_원부"));

	UpdateData(TRUE);

	m_strYear.TrimLeft();
	m_strYear.TrimRight();


	if( m_strYear != _T("") )
	{
		tmpQry.Format(_T(" ACCESSION_REC_MAKE_YEAR = '%s' "), m_strYear);
		strQry = strQry + tmpQry;
	}	

	ids = pDM->RefreshDataManager(strQry);
	if (ids < 0 )
	{
		AfxMessageBox(_T("RefreshDataManager 를 확인해 주세요"));
		return;
	}

	CESL_ControlMgr *pCM;
	pCM = FindCM(_T("CM_단행_정리_목록_원부"));

	CESL_Grid *pGrid;
	pGrid=( CESL_Grid * )FindControl(_T("CM_단행_정리_목록_원부"), _T("GRID"));

	pGrid->Display();

//	pCM->AllControlDisplay();

}

VOID CSE_CAT_GROUP_INSERT::OnbSaveNo() 
{
	// TODO: Add your control notification handler code here

	m_GROUP_REC_KEY.TrimLeft();
	m_GROUP_REC_KEY.TrimRight();

	if( m_GROUP_REC_KEY == _T("") )
	{
		AfxMessageBox(_T("원부를 선택하여 주십시요!"));	
		return ;
	}

	SetControlData(_T("CM_단행_정리_누락등록번호_부여"), _T("원부번호"), m_GROUP_NO);

	CDialog::OnOK();
}

