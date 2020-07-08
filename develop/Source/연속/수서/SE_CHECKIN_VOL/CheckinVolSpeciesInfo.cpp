// CheckinVolSpeciesInfo.cpp : implementation file
//

#include "stdafx.h"
#include "CheckinVolSpeciesInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSpeciesInfo dialog


CCheckinVolSpeciesInfo::CCheckinVolSpeciesInfo(CESL_Mgr* pParent /*=NULL*/)
	: CESL_Mgr(CCheckinVolSpeciesInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCheckinVolSpeciesInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pDM_IDX = NULL;
	m_strRECEIPT_NO = _T("");
	m_strACQ_YEAR = _T("");
	m_nACQ_TYPE = 1; //default
}

CCheckinVolSpeciesInfo::~CCheckinVolSpeciesInfo()
{
}


VOID CCheckinVolSpeciesInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckinVolSpeciesInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckinVolSpeciesInfo, CDialog)
	//{{AFX_MSG_MAP(CCheckinVolSpeciesInfo)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckinVolSpeciesInfo message handlers

BOOL CCheckinVolSpeciesInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	if(InitESL_Mgr(_T("연속_수서_체크인_권단위_종정보"))<0) {
		AfxMessageBox(_T("InitESL_Mgr Error : 연속_수서_체크인_권단위_종정보"));
		return FALSE;
	}
	
	m_pDM_IDX = FindDM(_T("DM_체크인_색인"));
	
	if(!m_pDM_IDX) 	{
		AfxMessageBox(_T("DM Error : 연속_수서_체크인_권단위_종정보"));
		return FALSE;
	}
	
	//viewer setting 
	m_SpeciesInfoViewer.InitViewer(IDC_gridCHECKIN_VOL_SPECIES, this);
	m_SpeciesInfoViewer.SetViewerWidth(20,75);
	
	m_AcqInfoViewer.InitViewer(IDC_gridCHECKIN_VOL_SPECIES_ACQ, this);
	m_AcqInfoViewer.SetViewerWidth(20,75);

	EnableThemeDialogTexture(GetSafeHwnd()); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

INT CCheckinVolSpeciesInfo::ShowSpeciesInfo(INT nIdx)
{

	CString tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_표제사항"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("서명"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_발행자사항"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("발행자"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("UDF_간행빈도"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("간행빈도"),tmp,TRUE);
	
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_간행상태"),nIdx);
	CString strDesc;
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("간행상태코드"),tmp,strDesc);
	m_SpeciesInfoViewer.SetControlData(_T("간행상태"),strDesc,TRUE);
	strDesc = _T("");

	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("ISS_ISSN"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("ISSN"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("SS_제본여부"),nIdx);
	m_SpeciesInfoViewer.SetControlData(_T("제본여부"),tmp,TRUE);
	
	tmp = _T("");
	tmp = m_pDM_IDX->GetCellData(_T("SS_배가위치_코드"),nIdx);
	m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("자료실구분"),tmp,strDesc);
	m_AcqInfoViewer.SetControlData(_T("자료실"),strDesc,TRUE);
	strDesc = _T("");
	
	tmp = _T("");
	if(!m_strACQ_YEAR.IsEmpty())
	{
		tmp.Format(_T("%s - %s"),m_strACQ_YEAR,m_strRECEIPT_NO);
	}
	
	m_AcqInfoViewer.SetControlData(_T("접수번호"),tmp,TRUE);

	CString strSpeciesKey = m_pDM_IDX->GetCellData(_T("ISS_종KEY"),nIdx);
	

	if(1 == m_nACQ_TYPE) ShowPurchaseInfo(strSpeciesKey);
	else if(2 == m_nACQ_TYPE) ShowDonateInfo(strSpeciesKey);


	m_SpeciesInfoViewer.Display();


	return 0;
}


VOID CCheckinVolSpeciesInfo::ShowPurchaseInfo(CString strSpeciesKey)
{
	CESL_DataMgr* pDM_ACQ = FindDM(_T("DM_체크인_권단위_종정보_구입정보"));
	if(!pDM_ACQ || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);
	
	pDM_ACQ->RefreshDataManager(strQuery);

	if(0 == pDM_ACQ->GetRowCount()) return;

	m_pInfo->pCodeMgr->ConvertCodeToDescInDM(pDM_ACQ,_T("구입처관리"),_T("SP_구입처_코드"));

	CString tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_열람우선여부"),0);
	m_AcqInfoViewer.SetControlData(_T("열람우선여부"),tmp,TRUE);
	
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_구입처_코드"),0);
	m_AcqInfoViewer.SetControlData(_T("구입처"),tmp,TRUE);
		
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SP_등록책수"),0);
	m_AcqInfoViewer.SetControlData(_T("등록책수"),tmp,TRUE);	
	
	tmp = _T("");
	CString strStartDate,strEndDate;
	
	strStartDate = pDM_ACQ->GetCellData(_T("SP_구독시작일"),0);
	strEndDate = pDM_ACQ->GetCellData(_T("SP_구독만기일"),0);
	
	if(!strStartDate.IsEmpty() || !strEndDate.IsEmpty())
	{
		tmp = strStartDate + _T(" ~ ") + strEndDate;
	}

	m_AcqInfoViewer.SetControlData(_T("구독기간"),tmp,TRUE);
	

	CString strValue;
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , strValue );
	if ( strValue == _T("Y") )
	{
		CString strDesc;
		tmp = pDM_ACQ->GetCellData(_T("SP_신청학과"),0);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학과"),tmp,strDesc);
		m_AcqInfoViewer.SetControlData(_T("신청학과"),strDesc);
	}

	m_AcqInfoViewer.Display();

}

VOID CCheckinVolSpeciesInfo::ShowDonateInfo(CString strSpeciesKey)
{
	CESL_DataMgr* pDM_ACQ = FindDM(_T("DM_체크인_권단위_종정보_기증정보"));
	if(!pDM_ACQ || strSpeciesKey.IsEmpty()) return;
	
	CString strQuery;
	strQuery.Format(_T("SPECIES_KEY = %s"),strSpeciesKey);

	pDM_ACQ->RefreshDataManager(strQuery);

	if(0 == pDM_ACQ->GetRowCount()) return;
	
	CString tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_열람우선여부"),0);
	m_AcqInfoViewer.SetControlData(_T("열람우선여부"),tmp,TRUE);
			
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_기증자명"),0);
	m_AcqInfoViewer.SetControlData(_T("기증자명"),tmp,TRUE);

	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_등록책수"),0);
	m_AcqInfoViewer.SetControlData(_T("등록책수"),tmp,TRUE);	
	
	tmp = _T("");
	tmp = pDM_ACQ->GetCellData(_T("SD_기증시작일"),0);
	m_AcqInfoViewer.SetControlData(_T("기증시작일"),tmp,TRUE);


	CString strValue;
	GetManageValue( _T("기타") , _T("공통") , _T("육사") , _T("육사") , strValue );
	if ( strValue == _T("Y") )
	{
		CString strDesc;
		tmp = pDM_ACQ->GetCellData(_T("SD_신청학과"),0);
		m_pInfo->pCodeMgr->ConvertCodeToDesc(_T("학과"),tmp,strDesc);
		m_AcqInfoViewer.SetControlData(_T("신청학과"),strDesc);
	}

	m_AcqInfoViewer.Display();

}

VOID CCheckinVolSpeciesInfo::SetReceiptNo(CString strYear, CString strNo)
{
	m_strRECEIPT_NO = strNo;
	m_strACQ_YEAR = strYear;
}

BOOL CCheckinVolSpeciesInfo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CCheckinVolSpeciesInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr=CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}
