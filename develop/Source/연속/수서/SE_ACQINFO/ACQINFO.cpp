// ACQINFO.cpp : implementation file
//

#include "stdafx.h"
#include "ACQINFO.h"
#include "AcqInfoSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SENDQUERY WM_USER+937

/////////////////////////////////////////////////////////////////////////////
// CACQINFO dialog


CACQINFO::CACQINFO(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CACQINFO)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDlg = NULL;
	m_nACQ_TYPE=1; // default
	m_pDM = NULL;
	m_pGrid = NULL;
	m_nCurrentIdx = -1;
	m_pMgr = pParent;
}

CACQINFO::~CACQINFO()
{
	//search dialog delete
	if(m_pDlg)
	{
		delete m_pDlg;
		m_pDlg = NULL;		
	}

}

BOOL CACQINFO::Create(CWnd *pParentWnd)
{
	return CDialog::Create(IDD, pParentWnd);
}

void CACQINFO::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACQINFO)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACQINFO, CDialog)
	//{{AFX_MSG_MAP(CACQINFO)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(SENDQUERY, OnSendQuery)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACQINFO message handlers


VOID CACQINFO::DoSearch()
{
	if(m_pDlg)
	{
		m_pDlg->ShowWindow(TRUE);
		m_pDlg->UpdateWindow();
		m_pDlg->CenterWindow();
		m_pDlg->SetFocusOnTitle();
	}
}

BOOL CACQINFO::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if( InitESL_Mgr( _T("연속_수서_입수자료관리") ) < 0 )
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_수서_입수자료관리") );
	
	m_pGrid = ( CESL_Grid * )FindControl( _T("CM_입수자료관리_검색"), _T("그리드") );
	if( !m_pGrid )
		return -1;

	m_pDM = FindDM(_T("DM_입수자료관리_검색"));

	AllControlDisplay(_T("CM_입수자료관리_검색"), -1);

	//search dialog
	if(!m_pDlg)
	{
		m_pDlg = new CAcqInfoSearch(this);
		m_pDlg->m_nACQ_TYPE = m_nACQ_TYPE;
		m_pDlg->pMain = this->pMain;
		m_pDlg->pParentMgr = (CESL_Mgr*)this;
		m_pDlg->m_pInfo = this->m_pInfo;
		m_pDlg->Create(this);
		m_pDlg->ShowWindow(FALSE);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CACQINFO::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	ResizeControl();
	
}

void CACQINFO::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	ResizeControl();	
}

VOID CACQINFO::OnSendQuery(WPARAM w,LPARAM l)
{
	if(2 == m_nACQ_TYPE) m_pDM->TBL_NAME = _T("IDX_SE_TBL I, SE_VOL_TBL V, SE_ACQ_TYPE_VOL_TBL A, SE_DONATE_MANAGE_TBL D");

	m_pDM->RefreshDataManager(m_pDlg->m_strQuery);
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행빈도코드"),_T("UDF_간행빈도"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("간행상태코드"),_T("UDF_간행상태"));
	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(m_pDM,_T("자료실구분"),_T("UDF_자료실"));
	
	INT nRowCnt;
	nRowCnt = m_pDM->GetRowCount();

	CString strACQ_CODE;

	for(INT i=0;i<nRowCnt;i++)
	{
		strACQ_CODE = _T("");
		strACQ_CODE = m_pDM->GetCellData(_T("수입구분"),i);
				
		if(_T("1") == strACQ_CODE )
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("구입"),i);
		}
		else if(_T("2") == strACQ_CODE)
		{
			m_pDM->SetCellData(_T("UDF_수입구분"),_T("기증"),i);
		}		
		
	}

	m_pGrid->Display();
	m_nCurrentIdx = -1;

	if(0 == nRowCnt)
	{
		CString str;
		str.Format(_T("검색 결과가 없습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
		AfxMessageBox(str);
	}
	else
	{
		CString str;
		str.Format(_T(" %d 건의 자료가 검색되었습니다."),nRowCnt);
		static_cast<CView*>(pMain)->GetParentFrame()->SendMessage(WM_USER+2004, NULL, (long)str.GetBuffer(0));
	}
	
	if( nRowCnt > 0 )
		m_pDlg->ShowWindow(SW_HIDE);
}

VOID CACQINFO::ResizeControl(VOID)
{
	if (GetSafeHwnd() == NULL) return;
	CWnd *pWnd = NULL;

	CRect rcDlg, rcControl;
	GetClientRect(rcDlg);

	pWnd = GetDlgItem(IDC_gridACQINFO);
	if (pWnd) {
		rcControl = rcDlg;
		rcControl.top = rcControl.top;
		
		pWnd->MoveWindow(rcControl);//수정안함
	}
	return;
}

VOID CACQINFO::PrintGridData()
{
	INT nRowCnt = m_pDM->GetRowCount();

	if(0 == nRowCnt)
	{
		AfxMessageBox(_T("출력할 자료가 없습니다!"));
		return;
	}

	//본서명,발행자를 마크에서 가지고 온다
	CMarc Marc;
	CString strStreamMarc = _T("");
	CString strTitle,strPublisher;
	m_pInfo->pMarcMgr->SetMarcGroup(_T("연속"));


	for(INT i=0;i<nRowCnt;i++)
	{
		strStreamMarc = m_pDM->GetCellData(_T("MARC"),i);
		
		m_pInfo->pMarcMgr->Decoding(strStreamMarc,&Marc);
		
		strTitle = _T("");
		strPublisher = _T("");
		
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("245$a"),strTitle);
		m_pInfo->pMarcMgr->GetItem(&Marc,_T("260$b"),strPublisher);
		
		m_pDM->SetCellData(_T("UDF_본서명"),strTitle,i);
		m_pDM->SetCellData(_T("UDF_발행자"),strPublisher,i);
	}
	
	
	//print
	CSIReportManager *pSIReportManager = new CSIReportManager(this);
	

	INT ids;
	pSIReportManager->SetCONNECTION_INFO(m_pDM->CONNECTION_INFO);
	ids = pSIReportManager->GetSIReportFileFromDB(_T("연속_수서_입수자료관리"));
	
	if (ids < 0) {
		AfxMessageBox(_T("DB로 부터 SI Report 정보를 가져오지 못하였습니다."));
		return ;
	}
	
	pSIReportManager->m_pWorkForm->paper = PAPER_A4_LANDSCAPE;
	pSIReportManager->SetDataMgr(0, m_pDM, 0);
	
	ids = pSIReportManager->Print();

	if (pSIReportManager) {
		delete pSIReportManager;
		pSIReportManager = NULL;
	}

}
HBRUSH CACQINFO::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
