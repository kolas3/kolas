// SeSpeciesDelete.cpp : implementation file
//

#include "stdafx.h"
#include "SeSpeciesDelete.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesDelete dialog


CSeSpeciesDelete::CSeSpeciesDelete(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(IDD, pParent)
{
	//{{AFX_DATA_INIT(CSeSpeciesDelete)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDM = NULL;
	m_strSpeciesKey = _T("");
}
CSeSpeciesDelete::~CSeSpeciesDelete()
{
}

VOID CSeSpeciesDelete::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSeSpeciesDelete)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSeSpeciesDelete, CDialog)
	//{{AFX_MSG_MAP(CSeSpeciesDelete)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSeSpeciesDelete message handlers

BOOL CSeSpeciesDelete::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("연속_종삭제"))<0) {
		AfxMessageBox( _T("InitESL_Mgr Error : 연속_종삭제") );
		return FALSE;
	}
	
	m_pDM = FindDM(_T("DM_연속_종삭제"));
	

	if(!m_pDM) {
		AfxMessageBox( _T("DM Error : 연속_종삭제") );
		return FALSE;
	}	

	//viewer setting 
	m_SpeciesInfoViewer.InitViewer(IDC_gridAPI_SPECIES_DELETE, this);
	m_SpeciesInfoViewer.SetViewerWidth(30,68);
	
	if(m_strSpeciesKey.IsEmpty())
	{
		AfxMessageBox(_T("종KEY를 찾을 수 없습니다!"));
		EndDialog(0);
	}

	
	if(ShowSpeciesInfo()<0)
	{
		AfxMessageBox(_T("종 정보를 찾을 수 없습니다!"));
		EndDialog(0);
	}

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


INT CSeSpeciesDelete::ShowSpeciesInfo()
{
	
	CString strWhere;
	strWhere.Format(_T("S.REC_KEY = %s AND S.REC_KEY = I.REC_KEY"),m_strSpeciesKey);
	
	m_pDM->RefreshDataManager(strWhere);

	if(0 == m_pDM->GetRowCount()) return -1;

	
	CString tmp = _T("");
	tmp = m_pDM->GetCellData(_T("제어번호"),0);
	m_SpeciesInfoViewer.SetControlData(_T("제어번호"),tmp,TRUE);



	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("서명"),0);
	m_SpeciesInfoViewer.SetControlData(_T("서명"),tmp,TRUE);

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("발행자"),0);
	m_SpeciesInfoViewer.SetControlData(_T("발행자"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("ISSN"),0);
	m_SpeciesInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("간행빈도"),0);
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행빈도코드"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("간행빈도"),strDesc,TRUE);
	strDesc = _T("");


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("간행상태"),0);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행상태코드"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("간행상태"),strDesc,TRUE);
	strDesc = _T("");

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("입력일"),0);
	m_SpeciesInfoViewer.SetControlData(_T("입력일"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("권수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("권수"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("구입권수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("구입권수"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("구입책수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("구입책수"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("기증권수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("기증권수"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("기증책수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("기증책수"),tmp,TRUE);
	

	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("기사수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("기사수"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("부록수"),0);
	m_SpeciesInfoViewer.SetControlData(_T("부록수"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("최초입수권호"),0);
	m_SpeciesInfoViewer.SetControlData(_T("최초입수권호"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("최종입수권호"),0);
	m_SpeciesInfoViewer.SetControlData(_T("최종입수권호"),tmp,TRUE);

	
	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("최초책입수일"),0);
	m_SpeciesInfoViewer.SetControlData(_T("최초책입수일"),tmp,TRUE);


	tmp = _T("");
	tmp = m_pDM->GetCellData(_T("최종책입수일"),0);
	m_SpeciesInfoViewer.SetControlData(_T("최종책입수일"),tmp,TRUE);



	m_SpeciesInfoViewer.Display();
	

	return 0;
}

HBRUSH CSeSpeciesDelete::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
