// TransferDetail.cpp : implementation file
//

#include "stdafx.h"
#include "TransferDetail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransferDetail dialog


CTransferDetail::CTransferDetail(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransferDetail)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM = NULL;
	m_nIdx = -1;
}

CTransferDetail::~CTransferDetail()
{
}


VOID CTransferDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransferDetail)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransferDetail, CDialog)
	//{{AFX_MSG_MAP(CTransferDetail)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransferDetail message handlers

BOOL CTransferDetail::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	//viewer setting 
	m_BookDetailInfoViewer.InitViewer(IDC_gridTRA_DETAIL, this);
	m_BookDetailInfoViewer.SetViewerWidth(20,75);
	
	
	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_표제사항"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("서명"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_발행자사항"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("발행자"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_간행빈도"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("간행빈도"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_ISSN"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISS_저작자사항"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("저작자"),tmp,TRUE);
	

	tmp = m_pDM->GetCellData(_T("SV_권호표제"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("권호표제"),tmp,TRUE);
	

	tmp = m_pDM->GetCellData(_T("SV_권호명"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("권호명"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_부차적권호명"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("부차적권호명"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_간행빈도"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("간행빈도"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_발행일"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("발행일"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SB_입수일"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("입수일"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("UDF_자료실"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("자료실"),tmp,TRUE);

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_가격"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("가격"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_면장수"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("면장수"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_물리적특성"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("물리적특성"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_책크기"),m_nIdx);
	m_BookDetailInfoViewer.SetControlData(_T("책크기"),tmp,TRUE);

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("SV_권호구분"),m_nIdx);

	if(_T("1") == tmp)
		m_BookDetailInfoViewer.SetControlData(_T("권호구분"),_T("일반"),TRUE);
	else if(_T("2") == tmp)	
		m_BookDetailInfoViewer.SetControlData(_T("권호구분"),_T("합병호"),TRUE);

	m_BookDetailInfoViewer.Display();

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CTransferDetail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
