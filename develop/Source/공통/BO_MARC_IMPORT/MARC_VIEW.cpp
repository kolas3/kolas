// MARC_VIEW.cpp : implementation file
//

#include "stdafx.h"
#include "MARC_VIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMARC_VIEW dialog


CMARC_VIEW::CMARC_VIEW(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CMARC_VIEW)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	AfxInitRichEdit();
	m_pParentGrid = NULL;
	m_pParentDM = NULL;
	m_nIdx = -1;
	m_nFirstIdx = -1;
	m_nLastIdx = -1;
}


CMARC_VIEW::~CMARC_VIEW()
{
}

VOID CMARC_VIEW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMARC_VIEW)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMARC_VIEW, CDialog)
	//{{AFX_MSG_MAP(CMARC_VIEW)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_btnMarcCLOSE, OnbtnMarcCLOSE)
	ON_BN_CLICKED(IDC_btnNEXT, OnbtnNEXT)
	ON_BN_CLICKED(IDC_btnPREV, OnbtnPREV)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMARC_VIEW message handlers

VOID CMARC_VIEW::OnSize(UINT nType, INT cx, INT cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	RECT rect, r;
	GetClientRect(&rect);
	CWnd *pWnd = NULL;
    	
	pWnd = GetDlgItem(IDC_edtSPECIES_MARC_EDITOR);
	r = rect;
	r.bottom = r.bottom - 27;
	if (pWnd) pWnd->MoveWindow(&r, true);
     
    INT mid = (rect.right+rect.left)/2;  

	pWnd = GetDlgItem(IDC_btnMarcCLOSE);
	r = rect;
	r.left = mid - 42;
	r.right = mid + 42;
	r.top = r.bottom -24;
	r.bottom = r.bottom;
	if (pWnd) pWnd->MoveWindow(&r, true); 
}

BOOL CMARC_VIEW::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//-------------- // 황정영 수정: 선택한 마크만 볼 수 있도록 //---------// 
	if(m_pParentDM == NULL || m_pParentGrid == NULL)
	{
		AfxMessageBox(_T("초기화 실패"));
		return FALSE;
	}
	
	// TODO: Add extra initialization here
	INT idx = m_pParentGrid->SelectMakeList();
	if( idx<0 )
	{
		AfxMessageBox(_T("선정된 항목이 없습니다."));
		return FALSE;
	}

	m_nIdx = m_pParentGrid->SelectGetHeadPosition();
	if( m_nIdx<0 ) 
	{
		AfxMessageBox(_T("선정된 항목이 없습니다."));
		//EndDialog(0);
		return FALSE;
	}
	
	SetFirstLastIdx();
	//--------// 요기까지 // ------//

	INT ids = Display(0);
	if(ids < 0)
	{
		EndDialog(IDCANCEL);
		return FALSE;
	}

	EnableThemeDialogTexture(GetSafeHwnd());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

VOID CMARC_VIEW::OnbtnMarcCLOSE() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

VOID CMARC_VIEW::OnbtnNEXT() 
{
	// TODO: Add your control notification handler code here
	Display(1);
}

VOID CMARC_VIEW::OnbtnPREV() 
{
	// TODO: Add your control notification handler code here
	Display(-1);
}

INT CMARC_VIEW::Display(INT nIdx)
{
//-------------- // 황정영 수정: 선택한 마크만 볼 수 있도록 //---------// 
/*	if(m_pParentGrid == NULL || m_pParentDM == NULL) return -1;
	switch(nIdx) 
	{
	case 0:
		SetFirstLastIdx();
		m_nIdx = m_pParentGrid->GetIdx();
		if(m_nIdx < 0) m_nIdx = 0;
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		if(m_nIdx < m_nLastIdx) m_nIdx++;
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		if(m_nIdx > m_nFirstIdx) m_nIdx--;
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return -1;
	}
*/
	switch(nIdx) 
	{
	case 0:
		m_nIdx = m_pParentGrid->SelectGetHeadPosition();
		if(m_nIdx < 0) m_nIdx = 0;
		ShowNextPrevButton(m_nIdx);
		break;
	case 1:
		m_nIdx = m_pParentGrid->SelectGetNext();
		ShowNextPrevButton(m_nIdx);
		break;
	case -1:
		m_nIdx = m_pParentGrid->SelectGetPrev();
		ShowNextPrevButton(m_nIdx);
		break;
	default:
		return -1;
	}
	//---------// 요기까지 //-----------//
	
	CString tmpREC_KEY, strMarcStream;
	tmpREC_KEY = m_pParentDM->GetCellData(_T("IBS_종KEY"), m_nIdx);
	CString strQuery;
	strQuery.Format(_T("SELECT MARC FROM BO_SPECIES_TBL WHERE REC_KEY = %s"), tmpREC_KEY);
	INT ids = m_pParentDM->GetOneValueQuery(strQuery, strMarcStream); 
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("마크정보를 추출할 수 없습니다."));
		return -1;
	}
	if(strMarcStream.IsEmpty())
	{
		ESLAfxMessageBox(_T("선정한 자료에 마크정보가 없습니다."));
		return -1;
	}
	ids = m_pInfo->pMarcMgr->Decoding(strMarcStream, &m_Marc);	
	if(ids < 0)
	{
		ESLAfxMessageBox(_T("마크구조체를 생성할 수 없습니다."));
		return -1;
	}
	
	m_marcEditor.SubclassDlgItem(IDC_edtSPECIES_MARC_EDITOR, -1, this);
	m_marcEditor.SetMarcMgr(m_pInfo->pMarcMgr);
	m_marcEditor.InitUserAlias();
	m_marcEditor.InitLimitTag();
	m_marcEditor.Init(m_pInfo->pMarcMgr, &m_Marc);
	m_marcEditor.Display();
	m_pParentGrid->SetReverse(m_nIdx);
	return 0;
}

VOID CMARC_VIEW::SetFirstLastIdx()
{
//	m_nFirstIdx = 0;
//	m_nLastIdx = m_pParentDM->GetRowCount()-1;
	m_nFirstIdx = m_pParentGrid->SelectGetHeadPosition();
	m_nLastIdx = m_pParentGrid->SelectGetTailPosition();
}

VOID CMARC_VIEW::ShowNextPrevButton(INT nIdx)
{
	GetDlgItem(IDC_btnNEXT)->EnableWindow(TRUE);
	GetDlgItem(IDC_btnPREV)->EnableWindow(TRUE);
	
	if(m_nFirstIdx >= nIdx || nIdx < 0)
		GetDlgItem(IDC_btnPREV)->EnableWindow(FALSE);
	if(m_nLastIdx <= nIdx || nIdx < 0)
		GetDlgItem(IDC_btnNEXT)->EnableWindow(FALSE);
}

HBRUSH CMARC_VIEW::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{	
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
