// SE_CAT_REG_VIEW.cpp : implementation file
//

#include "stdafx.h"
#include "SE_CAT_REG_VIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_VIEW dialog


CSE_CAT_REG_VIEW::CSE_CAT_REG_VIEW(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSE_CAT_REG_VIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSE_CAT_REG_VIEW::~CSE_CAT_REG_VIEW()
{
}

BOOL CSE_CAT_REG_VIEW::Create(CWnd* pParentWnd)
{

	return CDialog::Create(IDD, pParentWnd);

	
}

VOID CSE_CAT_REG_VIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSE_CAT_REG_VIEW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSE_CAT_REG_VIEW, CDialog)
	//{{AFX_MSG_MAP(CSE_CAT_REG_VIEW)
	ON_BN_CLICKED(IDC_bPREV, OnbPREV)
	ON_BN_CLICKED(IDC_bNEXT, OnbNEXT)
	ON_BN_CLICKED(IDC_bSaveNo, OnbSaveNo)
	ON_BN_CLICKED(IDC_BINDBOOK_LIST, OnBindbookList)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSE_CAT_REG_VIEW message handlers

BOOL CSE_CAT_REG_VIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (InitESL_Mgr(_T("SM_연속_정리_등록_상세보기")) < 0)
	{
		AfxMessageBox(_T("InitESL_Mgr Error"));
		return FALSE;
	}

	m_nCount=0;
	CESL_Grid *pGrid;
	pGrid = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return FALSE;
	}

	m_nCount=pDM->GetRowCount();
	
	if(m_nCount==0)
	{
		ESLAfxMessageBox(_T(" 자료가 없습니다."));
		CDialog::EndDialog(0);
		return FALSE;
	}


	INT idx;
	idx=pGrid->GetRow() - 1;

	if (idx < 0)
	{
		idx = 0;
	}

	m_INDEX=idx;

	// 제본 책 여부
	CString strBookType;

	strBookType=pDM->GetCellData(_T("책종류"),m_INDEX);

	if(strBookType==_T("B"))
	{
		GetDlgItem(IDC_BINDBOOK_LIST)->ShowWindow(TRUE);
	}

	m_Viewer.InitViewer(IDC_VIEWGRID, this);
	m_Viewer.SetViewerWidth(20, 75);

	MakeData(m_INDEX);

	CESL_ControlMgr *pCM = FindCM(_T("CM_연속_정리_등록_상세보기"));
	if ( NULL == pCM ) {
		AfxMessageBox ( _T("CM_연속_정리_등록_상세보기") );
		return FALSE;
	}

	pCM->AllControlDisplay(m_INDEX);
	
	m_Viewer.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CSE_CAT_REG_VIEW::MakeData(INT nINDEX)
{

	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	m_Viewer.SetControlData(_T("권  호"), pDM->GetCellData(_T("BB_편제명부록명"),nINDEX) );
	m_Viewer.SetControlData(_T("부차적권호"), pDM->GetCellData(_T("부차적권호"),nINDEX) );
	m_Viewer.SetControlData(_T("발행일"), pDM->GetCellData(_T("발행일"),nINDEX) );
	m_Viewer.SetControlData(_T("매체구분"),pDM->GetCellData(_T("BB_매체구분_코드"),nINDEX) );
	m_Viewer.SetControlData(_T("가격"), pDM->GetCellData(_T("가격"),nINDEX) );
	m_Viewer.SetControlData(_T("크기"), pDM->GetCellData(_T("크기"),nINDEX) );

	CESL_Grid *pGrid = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));
	if (pGrid) pGrid->SetReverse(nINDEX);

}

VOID CSE_CAT_REG_VIEW::OnbPREV() 
{
	// TODO: Add your control notification handler code here
	if( m_INDEX == 0)
	{
		ESLAfxMessageBox(_T("처음 자료 입니다."));
		return;
	}

	m_INDEX--;

	MakeData(m_INDEX);

	CESL_ControlMgr *pCM = FindCM(_T("CM_연속_정리_등록_상세보기"));
	if ( NULL == pCM ) {
		AfxMessageBox ( _T("CM_연속_정리_등록_상세보기") );
		return ;
	}

	pCM->AllControlDisplay(m_INDEX);
	
	m_Viewer.Display();
	
}

VOID CSE_CAT_REG_VIEW::OnbNEXT() 
{
	// TODO: Add your control notification handler code here
	if( m_INDEX == m_nCount-1)
	{
		ESLAfxMessageBox(_T("마지막 자료 입니다."));
		return;
	}

	m_INDEX++;

	MakeData(m_INDEX);

	CESL_ControlMgr *pCM = FindCM(_T("CM_연속_정리_등록_상세보기"));
	if ( NULL == pCM ) {
		AfxMessageBox ( _T("CM_연속_정리_등록_상세보기") );
		return ;
	}

	pCM->AllControlDisplay(m_INDEX);
	
	m_Viewer.Display();		
}

VOID CSE_CAT_REG_VIEW::OnbSaveNo() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CString /*strTITLE,*/strVOL_TITLE,strOTHER_VOL_TITLE,strPUBLISH_DATE,strPRICE,strRecKey;
	// GetControlData( _T("CM_연속_정리_등록_상세보기"), _T("서명"), strTITLE );
	GetControlData( _T("CM_연속_정리_등록_상세보기"), _T("권호명"), strVOL_TITLE );
	GetControlData( _T("CM_연속_정리_등록_상세보기"), _T("부차적권호명"), strOTHER_VOL_TITLE );
	GetControlData( _T("CM_연속_정리_등록_상세보기"), _T("발행일"), strPUBLISH_DATE );
	GetControlData( _T("CM_연속_정리_등록_상세보기"), _T("가격"), strPRICE );
	
	strRecKey = pDM->GetCellData(_T("권호키"),m_INDEX);

	// 040315_HSS 서명은 바꾸지 않는다. AddFrame으로 변경
	INT ids = pDM->StartFrame();

	CString strQuery;
	strQuery.Format(_T("update SE_VOL_TBL set VOL_TITLE='%s', OTHER_VOL_TITLE='%s'")
					_T(", PUBLISH_DATE='%s', PRICE='%s' where rec_key=%s;")
					, strVOL_TITLE, strOTHER_VOL_TITLE
					, strPUBLISH_DATE, strPRICE, strRecKey);
	ids = pDM->AddFrame( strQuery );
	if (ids<0){
		AfxMessageBox ( _T("AddFrame Error!") );
		return ;
	}
	
	pDM->SendFrame();
	pDM->EndFrame();

	//pDM->SetCellData(_T("IBS_본표제"), strTITLE,m_INDEX);
	pDM->SetCellData(_T("BB_편제명부록명") ,strVOL_TITLE,m_INDEX);
	pDM->SetCellData(_T("부차적권호명"), strOTHER_VOL_TITLE,m_INDEX);
	pDM->SetCellData(_T("발행일"), strPUBLISH_DATE,m_INDEX);
	pDM->SetCellData(_T("가격"), strPRICE,m_INDEX);

	CESL_Grid *pGrid;
	pGrid = ( CESL_Grid * )FindControl(_T("CM_연속_정리_등록_메인"), _T("MAIN_GRID"));

	AfxMessageBox(_T("저장되었습니다"));
	
	pGrid->DisplayLine(m_INDEX);
}

VOID CSE_CAT_REG_VIEW::OnBindbookList() 
{
	// TODO: Add your control notification handler code here
	CESL_DataMgr *pDM = FindDM(_T("DM_연속_정리_등록_메인"));
	if ( NULL == pDM ) {
		AfxMessageBox ( _T("DM_연속_정리_등록_메인") );
		return ;
	}

	CSE_REG_BIND_LIST Dlg;
	Dlg.m_RecKey = pDM->GetCellData(_T("BB_책KEY"),m_INDEX);

	Dlg.DoModal();
}

HBRUSH CSE_CAT_REG_VIEW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
